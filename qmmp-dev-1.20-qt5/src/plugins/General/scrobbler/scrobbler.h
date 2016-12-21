/***************************************************************************
 *   Copyright (C) 2010-2016 by Ilya Kotov                                 *
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
#ifndef SCROBBLER_H
#define SCROBBLER_H

#include <QMap>
#include <QObject>
#include <qmmp/qmmp.h>
#include "scrobblercache.h"

class QNetworkAccessManager;
class QNetworkReply;
class QIODevice;
class QTime;
class SoundCore;


/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class ScrobblerResponse
{
public:
    QString status;
    QString token;
    QString code;
    QString error;
    QString key;
    QString name;
    QString subscriber;

    void parse(QIODevice *device);
};

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class Scrobbler : public QObject
{
    Q_OBJECT
public:
    Scrobbler(const QString &scrobblerUrl, const QString &name,QObject *parent = 0);
    ~Scrobbler();

private slots:
    void setState(Qmmp::State state);
    void updateMetaData();
    void processResponse(QNetworkReply *reply);
    void setupProxy();
    void submit();

private:
    enum { MIN_SONG_LENGTH = 30 };

    void sendNotification(const SongInfo &info);
    SongInfo m_song;
    QList <SongInfo> m_cachedSongs;
    QByteArray m_ua;
    int m_submitedSongs;
    QString m_session;
    QNetworkAccessManager *m_http;
    SoundCore *m_core;
    QNetworkReply *m_submitReply, *m_notificationReply;
    QTime *m_time;
    ScrobblerCache *m_cache;
    QString m_scrobblerUrl, m_name;
    Qmmp::State m_previousState;
    int m_elapsed;
};

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class ScrobblerAuth : public QObject
{
    Q_OBJECT
public:
    explicit ScrobblerAuth(const QString &scrobblerUrl, const QString &authUrl,
                           const QString &name, QObject *parent = 0);
    void getToken();
    void getSession();
    void checkSession(const QString &session);
    QString session() const;

    enum ErrorType
    {
        NO_ERROR = 0,
        NETWORK_ERROR,
        LASTFM_ERROR
    };

signals:
    void tokenRequestFinished(int error);
    void sessionRequestFinished(int error);
    void checkSessionFinished(int error);

private slots:
    void processResponse(QNetworkReply *reply);

private:
    QString m_token, m_session;
    QByteArray m_ua;
    QNetworkAccessManager *m_http;
    QNetworkReply *m_getTokenReply, *m_getSessionReply, *m_checkSessionReply;
    QString m_scrobblerUrl, m_authUrl, m_name;
};

#endif //SCROBBLER_H
