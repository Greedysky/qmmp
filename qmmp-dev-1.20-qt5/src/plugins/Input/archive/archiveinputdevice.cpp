/***************************************************************************
 *   Copyright (C) 2016 by Ilya Kotov                                      *
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

#include <QRegExp>
#include "archiveinputdevice.h"

ArchiveInputDevice::ArchiveInputDevice(const QString &url, QObject *parent)  : QIODevice(parent)
{
    m_archive = 0;
    m_entry = 0;
    QString filePath = url.section("#", -1);
    QString archivePath = url;
    archivePath.remove(QRegExp("^.+://"));
    archivePath.remove(QRegExp("#.+$"));

    m_archive = archive_read_new();
    archive_read_support_filter_all(m_archive);
    archive_read_support_format_all(m_archive);

    int r = archive_read_open_filename(m_archive, archivePath.toLocal8Bit().constData(), 10240);
    if (r != ARCHIVE_OK)
    {
        qWarning("ArchiveInputDevice: unable to open file '%s', libarchive error: %s",
                 qPrintable(archivePath), archive_error_string(m_archive));
        return;
    }

    while (archive_read_next_header(m_archive, &m_entry) == ARCHIVE_OK)
    {
        QString pathName = QString::fromLocal8Bit(archive_entry_pathname(m_entry));
        if(!pathName.startsWith("/"))
            pathName.prepend("/");

        if(archive_entry_filetype(m_entry) == AE_IFREG && filePath == pathName)
        {
            open(QIODevice::ReadOnly);
            m_buffer.open(QBuffer::ReadWrite);
            break;
        }
        archive_read_data_skip(m_archive);
    }
    m_close_libarchive = true;
}

ArchiveInputDevice::ArchiveInputDevice(archive *a, archive_entry *e, QObject *parent) : QIODevice(parent)
{
    m_archive = a;
    m_entry = e;
    open(QIODevice::ReadOnly);
    m_buffer.open(QBuffer::ReadWrite);
    m_close_libarchive = false;
}

ArchiveInputDevice::~ArchiveInputDevice()
{
    if(m_close_libarchive && m_archive)
    {
        archive_read_close(m_archive);
        archive_read_free(m_archive);
        m_archive = 0;
    }
}

bool ArchiveInputDevice::seek(qint64 pos)
{
    QIODevice::seek(pos);

    if(pos > m_buffer.size())
    {
        char tmp[1024];
        qint64 delta = pos - m_buffer.size();
        while (delta > 0)
        {
            qint64 r = qMin(qint64(sizeof(tmp)), delta);
            r = archive_read_data(m_archive, tmp, r);
            if(r > 0)
            {
                m_buffer.buffer().append(tmp, r);
                delta -= r;
                continue;
            }
            else if(r < 0)
            {
                qWarning("ArchiveInputDevice: seeking failed; libarchive error: %s", archive_error_string(m_archive));
            }
            return false;
        }

    }

    return m_buffer.seek(pos);
}

qint64 ArchiveInputDevice::size() const
{
    return archive_entry_size(m_entry);
}

qint64 ArchiveInputDevice::readData(char *data, qint64 maxSize)
{
    if(m_buffer.pos() + maxSize > m_buffer.size())
    {
        qint64 l = m_buffer.pos() + maxSize - m_buffer.size();
        char tmp[l];
        int r = archive_read_data(m_archive, tmp, l);
        if(r > 0)
            m_buffer.buffer().append(tmp, r);
        else if(r < 0)
            qWarning("ArchiveInputDevice: reading failed; libarchive error: %s", archive_error_string(m_archive));
    }
    return m_buffer.read(data, maxSize);
}

qint64 ArchiveInputDevice::writeData(const char *, qint64)
{
    return -1;
}
