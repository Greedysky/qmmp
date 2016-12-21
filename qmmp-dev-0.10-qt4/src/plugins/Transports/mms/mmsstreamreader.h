/***************************************************************************
 *   Copyright (C) 2006-2012 by Ilya Kotov                                 *
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
#ifndef MMSSTREAMREADER_H
#define MMSSTREAMREADER_H

#include <QObject>
#include <QIODevice>
#include <QUrl>
#include <QMutex>
#include <QThread>
#define this var
#include <libmms/mmsx.h>
#undef this

class QFileInfo;
class DownloadThread;
class MMSInputSource;

/*! @internal
 *   @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class MMSStreamReader : public QIODevice
{
    Q_OBJECT
public:
    MMSStreamReader(const QString &url, MMSInputSource *parent);

    ~MMSStreamReader();

    /**
     *  QIODevice API
     */
    bool atEnd () const;
    qint64 bytesAvailable () const;
    qint64 bytesToWrite () const;
    void close ();
    bool isSequential () const;
    bool open (OpenMode mode);
    bool seek (qint64 pos);
    /**
     *  helper functions
     */
    void downloadFile();


    QMutex *mutex();
    void abort();
    void checkBuffer();
    void run();

signals:
    void ready();
    void error();

private:
    virtual qint64 readData(char*, qint64);
    virtual qint64 writeData(const char*, qint64);

    QMutex m_mutex;
    QString m_url;
    mmsx_t *m_handle;
    bool m_aborted;
    qint64 m_buffer_size, m_prebuf_size;
    char *m_buffer;
    qint64 m_buffer_at;
    bool m_ready;
    DownloadThread *m_thread;
    MMSInputSource *m_parent;
};

class DownloadThread : public QThread
{
    Q_OBJECT
public:
    DownloadThread(MMSStreamReader *parent);
    virtual ~DownloadThread ();
    static void usleep(unsigned long usecs)
        {
            QThread::usleep(usecs);
        }

private:
    virtual void run();
    MMSStreamReader *m_parent;
};

#endif
