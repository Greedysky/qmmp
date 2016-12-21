/***************************************************************************
 *   Copyright (C) 2014-2015 by Ilya Kotov                                 *
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

#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusPendingCallWatcher>
#include <QDBusConnectionInterface>
#include <QVariantList>
#include <QDateTime>
#include <QCoreApplication>
#include <qmmpui/mediaplayer.h>
#include <qmmp/soundcore.h>
#include "mediakeys.h"

MediaKeys::MediaKeys(QObject *parent) :
    QObject(parent)
{
    m_isRegistered = false;
    m_interface = 0;
    if(!QDBusConnection::sessionBus().interface()->isServiceRegistered("org.gnome.SettingsDaemon"))
    {
        qWarning("MediaKeys: gnome settings daemon is not running");
        return;
    }

    m_interface = new QDBusInterface("org.gnome.SettingsDaemon",
                                     "/org/gnome/SettingsDaemon/MediaKeys",
                                     "org.gnome.SettingsDaemon.MediaKeys",
                                     QDBusConnection::sessionBus(), this);

    QDBusPendingReply<> reply = grabMediaPlayerKeys(QCoreApplication::applicationName(),
                                                    QDateTime::currentDateTime().toTime_t());

    QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), SLOT(onRegisterFinished(QDBusPendingCallWatcher*)));
}

MediaKeys::~MediaKeys()
{
    if(m_isRegistered && m_interface)
    {
        QDBusPendingReply<> reply = releaseMediaPlayerKeys(QCoreApplication::applicationName());
        reply.waitForFinished();
        qWarning("MediaKeys: unregistered");
    }
}

QDBusPendingReply<> MediaKeys::grabMediaPlayerKeys(const QString &application, uint time)
{
    QVariantList argumentList;
    argumentList << QVariant::fromValue(application) << QVariant::fromValue(time);
    return m_interface->asyncCallWithArgumentList(QLatin1String("GrabMediaPlayerKeys"), argumentList);
}

QDBusPendingReply<> MediaKeys::releaseMediaPlayerKeys(const QString &application)
{
    QVariantList argumentList;
    argumentList << QVariant::fromValue(application);
    return m_interface->asyncCallWithArgumentList(QLatin1String("ReleaseMediaPlayerKeys"), argumentList);
}

void MediaKeys::onRegisterFinished(QDBusPendingCallWatcher *watcher)
{
    QDBusMessage reply = watcher->reply();
    watcher->deleteLater();

    if (reply.type() == QDBusMessage::ErrorMessage)
    {
        qWarning("MediaKeys: unable to grab media keys: [%s] - %s",
                 qPrintable(reply.errorName()), qPrintable(reply.errorMessage()));
        return;
    }
    m_interface->connection().connect("org.gnome.SettingsDaemon",
                                      "/org/gnome/SettingsDaemon/MediaKeys",
                                      "org.gnome.SettingsDaemon.MediaKeys",
                                      "MediaPlayerKeyPressed", this,
                                      SLOT(onKeyPressed(const QString&, const QString&)));
    m_isRegistered = true;
    qDebug("MediaKeys: registered");

}

void MediaKeys::onKeyPressed(const QString &in0, const QString &in1)
{
    if(in0 != QCoreApplication::applicationName())
        return;
    MediaPlayer *player = MediaPlayer::instance();
    SoundCore *core = SoundCore::instance();
    qDebug("MediaKeys: [%s] pressed", qPrintable(in1));
    if(in1 == "Play")
    {
        if (core->state() == Qmmp::Stopped)
            player->play();
        else if (core->state() != Qmmp::FatalError)
            core->pause();
    }
    else if(in1 == "Pause")
        core->pause();
    else if(in1 == "Stop")
        player->stop();
    else if(in1 == "Previous")
        player->previous();
    else if(in1 == "Next")
        player->next();
    else
        qWarning("MediaKeys: unknown media key pressed");
}
