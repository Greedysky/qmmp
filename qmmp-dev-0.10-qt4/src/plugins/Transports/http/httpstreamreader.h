/***************************************************************************
 *   Copyright (C) 2006-2016 by Ilya Kotov                                 *
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
#ifndef HTTPSTREAMREADER_H
#define HTTPSTREAMREADER_H

#include <QObject>
#include <QIODevice>
#include <QUrl>
#include <QMutex>
#include <QString>
#include <QHash>
#include <QThread>
#include <curl/curl.h>
#ifdef WITH_ENCA
#include <enca.h>
#endif

class QTextCodec;
class QFileInfo;
class DownloadThread;
class HTTPInputSource;

/*! @internal
 *   @author Ilya Kotov <forkotov02@hotmail.ru>
 */
struct HttpStreamData
{
    char *buf;
    long buf_fill;
    QString content_type;
    bool aborted;
    QHash <QString, QByteArray> header;
    bool icy_meta_data;
    int icy_metaint;
};

/*! @internal
 *   @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class HttpStreamReader : public QIODevice
{
    Q_OBJECT
public:
    HttpStreamReader(const QString &url, HTTPInputSource *parent);

    virtual ~HttpStreamReader();

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
    QString contentType();
    void downloadFile();
    QMutex *mutex();
    HttpStreamData *stream();
    void checkBuffer();
    void run();

signals:
    void ready();
    void error();

protected:
    qint64 readData(char*, qint64);
    qint64 writeData(const char*, qint64);

private:
    void abort();
    qint64 readBuffer(char* data, qint64 maxlen);
    void readICYMetaData();
    void parseICYMetaData(char *data, qint64 size);
    void sendStreamInfo(QTextCodec *codec);
    CURL *m_handle;
    QMutex m_mutex;
    HttpStreamData m_stream;
    QString m_url;
    QString m_userAgent;
    int m_metacount;
    QString m_title;
    bool m_ready;
    bool m_meta_sent;
    long m_buffer_size;
    QTextCodec *m_codec;
    DownloadThread *m_thread;
    HTTPInputSource *m_parent;
#ifdef WITH_ENCA
    EncaAnalyser m_analyser;
    QTextCodec *m_prevCodec;
#endif
};

class DownloadThread : public QThread
{
    Q_OBJECT
public:
    DownloadThread(HttpStreamReader *parent);
    virtual ~DownloadThread ();

private:
    virtual void run();
    HttpStreamReader *m_parent;
};

#endif
