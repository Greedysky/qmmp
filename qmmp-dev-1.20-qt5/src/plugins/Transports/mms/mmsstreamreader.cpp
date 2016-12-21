/***************************************************************************
 *   Copyright (C) 2006-2013 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#include <QApplication>
#include <QSettings>
#include <stdint.h>
#include <stdlib.h>
#include <qmmp/qmmp.h>
#include <qmmp/statehandler.h>
#include <qmmp/inputsource.h>
#include "mmsinputsource.h"
#include "mmsstreamreader.h"

MMSStreamReader::MMSStreamReader(const QString &url, MMSInputSource *parent) : QIODevice(parent)
{
    m_parent = parent;
    m_url = url;
    m_handle = 0;
    m_aborted = false;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_prebuf_size = settings.value("MMS/buffer_size",384).toInt() * 1024;
    m_buffer_size = m_prebuf_size;
    m_buffer = (char *)malloc(m_buffer_size);
    m_ready = false;
    m_buffer_at = 0;
    m_thread = new DownloadThread(this);
}

MMSStreamReader::~MMSStreamReader()
{
    qDebug("%s", Q_FUNC_INFO);
    abort();
    free(m_buffer);
    m_buffer = 0;
    m_buffer_at = 0;
    m_buffer_size = 0;
}

bool MMSStreamReader::atEnd () const
{
    return false;
}

qint64 MMSStreamReader::bytesToWrite () const
{
    return -1;
}

void MMSStreamReader::close ()
{
    abort();
    QIODevice::close();
}

bool MMSStreamReader::isSequential () const
{
    return true;
}

bool MMSStreamReader::open (OpenMode mode)
{
    if (mode != QIODevice::ReadOnly)
        return false;
    QIODevice::open(mode);
    return m_ready;
}

bool MMSStreamReader::seek (qint64 pos)
{
    Q_UNUSED(pos);
    return false;
}

qint64 MMSStreamReader::writeData(const char*, qint64)
{
    return -1;
}

qint64 MMSStreamReader::readData(char* data, qint64 maxlen)
{
    m_mutex.lock();
    qint64 len = qMin<qint64>(m_buffer_at, maxlen);
    memmove(data, m_buffer, len);
    m_buffer_at -= len;
    memmove(m_buffer, m_buffer + len, m_buffer_at);
    m_mutex.unlock();
    return len;
}

void MMSStreamReader::downloadFile()
{
     m_thread->start();
}

QMutex *MMSStreamReader::mutex()
{
    return &m_mutex;
}

void MMSStreamReader::abort()
{
    m_mutex.lock();
    if (m_aborted)
    {
        m_mutex.unlock();
        return;
    }
    m_aborted = true;
    m_mutex.unlock();
    if(m_thread->isRunning())
        m_thread->wait();
    m_ready = false;
    if (m_handle)
        mmsx_close(m_handle);
    m_handle = 0;
}

qint64 MMSStreamReader::bytesAvailable() const
{
    return QIODevice::bytesAvailable() + m_buffer_at;
}

void MMSStreamReader::run()
{
    int to_read = 1024;
    char prebuf[to_read];
    m_handle = mmsx_connect (0, 0, m_url.toLocal8Bit().constData(), 128 * 1024);
    if(!m_handle)
    {
        qWarning("MMSStreamReader: connection failed");
        setErrorString("connection failed");
        emit error();
        QIODevice::close();
        return;
    }
    m_mutex.lock();
    if(m_aborted)
    {
        m_mutex.unlock();
        qDebug("MMSStreamReader: aborted");
        return;
    }
    m_mutex.unlock();
    qint64 len = 0;
    forever
    {
        m_mutex.lock();

        if(m_buffer_at + to_read > m_buffer_size)
        {
            m_buffer_size = m_buffer_at + to_read;
            m_buffer = (char *)realloc(m_buffer, m_buffer_size);
        }
        m_mutex.unlock();
        len = mmsx_read (0, m_handle, prebuf, to_read);
        m_mutex.lock();
        if(len < 0)
        {
            m_mutex.unlock();
            qWarning("MMSStreamReader: mms thread funished with code %lld (%s)", len, strerror(len));
            if(!m_aborted && !m_ready)
            {
                setErrorString(strerror(len));
                emit error();
            }
            break;
        }
        memcpy(m_buffer + m_buffer_at, prebuf, len);
        m_buffer_at += len;
        if(!m_ready)
            checkBuffer();
        m_mutex.unlock();
        if(m_aborted)
            break;
        DownloadThread::usleep(5000);
    }
    QIODevice::close();
}

void MMSStreamReader::checkBuffer()
{
    if (m_buffer_at > m_prebuf_size && !m_ready)
    {
        m_ready = true;
        qDebug("MMSStreamReader: ready");
        QMap<Qmmp::MetaData, QString> metaData;
        metaData.insert(Qmmp::URL, m_url);
        m_parent->addMetaData(metaData);
        emit ready();
    }
    else if (!m_ready)
    {
        StateHandler::instance()->dispatchBuffer(100 * m_buffer_at / m_buffer_size);
        qApp->processEvents();
    }
}

DownloadThread::DownloadThread(MMSStreamReader *parent) : QThread(parent)
{
    m_parent = parent;
}

DownloadThread::~DownloadThread (){}

void DownloadThread::run()
{
    m_parent->run();
}
