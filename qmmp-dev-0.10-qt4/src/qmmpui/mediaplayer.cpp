/***************************************************************************
 *   Copyright (C) 2008-2015 by Ilya Kotov                                 *
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
#include <QString>
#include <QTranslator>
#include <QLocale>
#include "playlistitem.h"
#include "qmmpuisettings.h"
#include "mediaplayer.h"

MediaPlayer *MediaPlayer::m_instance = 0;

MediaPlayer::MediaPlayer(QObject *parent)
        : QObject(parent)
{
    if(m_instance)
        qFatal("StateHandler: only one instance is allowed");
    m_instance = this;
    m_pl_manager = 0;
    m_core = 0;
    m_skips = 0;
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/libqmmpui_") + locale);
    qApp->installTranslator(translator);
    m_core = new SoundCore(this);
    m_settings = new QmmpUiSettings(this);
    m_pl_manager = new PlayListManager(this);
    connect(m_core, SIGNAL(nextTrackRequest()), SLOT(updateNextUrl()));
    connect(m_core, SIGNAL(finished()), SLOT(playNext()));
    connect(m_core, SIGNAL(stateChanged(Qmmp::State)), SLOT(processState(Qmmp::State)));
    connect(m_core, SIGNAL(metaDataChanged()),SLOT(updateMetaData()));
}

MediaPlayer::~MediaPlayer()
{
    m_instance = 0;
}

MediaPlayer* MediaPlayer::instance()
{
    return m_instance;
}

PlayListManager *MediaPlayer::playListManager()
{
    return m_pl_manager;
}

void MediaPlayer::play(qint64 offset)
{
    m_pl_manager->currentPlayList()->doCurrentVisibleRequest();
    if (m_core->state() == Qmmp::Paused)
    {
        m_core->pause();
        return;
    }

    if (m_pl_manager->currentPlayList()->count() == 0)
        return;

    QString s = m_pl_manager->currentPlayList()->currentTrack()->url();
    if (s.isEmpty())
    {
        m_nextUrl.clear();
        return;
    }
    if(m_nextUrl == s)
    {
        m_nextUrl.clear();
        return;
    }
    m_core->play(s, false, offset);
}

void MediaPlayer::stop()
{
    m_core->stop();
    m_nextUrl.clear();
    m_skips = 0;
}

void MediaPlayer::next()
{
    bool playNext = m_core->state() != Qmmp::Stopped;
    stop();
    if (m_pl_manager->currentPlayList()->next() && playNext)
        play();
}

void MediaPlayer::previous()
{
    bool playNext = m_core->state() != Qmmp::Stopped;
    stop();
    if (m_pl_manager->currentPlayList()->previous() && playNext)
        play();
}

void MediaPlayer::playNext()
{
    if(m_settings->isRepeatableTrack())
    {
        play();
        return;
    }
    if(m_settings->isNoPlayListAdvance())
    {
        stop();
        return;
    }
    if (!m_pl_manager->currentPlayList()->next())
    {
        stop();
        return;
    }
    play();
}

void MediaPlayer::updateNextUrl()
{
    m_nextUrl.clear();
    PlayListTrack *track = 0;
    if(m_settings->isRepeatableTrack())
        track = m_pl_manager->currentPlayList()->currentTrack();
    else if(!m_settings->isNoPlayListAdvance())
        track = m_pl_manager->currentPlayList()->nextTrack();

    if(track)
    {
        bool ok = m_core->play(track->url(), true);
        if(ok)
        {
            m_nextUrl = track->url();
            qDebug("MediaPlayer: next track state: received");
        }
        else
            qDebug("MediaPlayer: next track state: error");
    }
    else
        qDebug("MediaPlayer: next track state: unknown");

}

void MediaPlayer::processState(Qmmp::State state)
{
    switch ((int) state)
    {
    case Qmmp::NormalError:
        m_core->stop();
        m_nextUrl.clear();
        if (m_skips <= m_pl_manager->currentPlayList()->count())
        {
            m_skips++;
            playNext();
        }
        break;
    case Qmmp::FatalError:
        m_core->stop();
        m_nextUrl.clear();
        break;
    case Qmmp::Playing:
        m_skips = 0;
    default:
        ;
    }
}

void MediaPlayer::updateMetaData()
{
    qDebug("===== metadata ======");
    qDebug("ARTIST = %s", qPrintable(m_core->metaData(Qmmp::ARTIST)));
    qDebug("TITLE = %s", qPrintable(m_core->metaData(Qmmp::TITLE)));
    qDebug("ALBUMARTIST = %s", qPrintable(m_core->metaData(Qmmp::ALBUMARTIST)));
    qDebug("ALBUM = %s", qPrintable(m_core->metaData(Qmmp::ALBUM)));
    qDebug("COMMENT = %s", qPrintable(m_core->metaData(Qmmp::COMMENT)));
    qDebug("GENRE = %s", qPrintable(m_core->metaData(Qmmp::GENRE)));
    qDebug("YEAR = %s", qPrintable(m_core->metaData(Qmmp::YEAR)));
    qDebug("TRACK = %s", qPrintable(m_core->metaData(Qmmp::TRACK)));
    qDebug("== end of metadata ==");

    PlayListModel *pl = m_pl_manager->currentPlayList();
    if (pl->currentTrack() && pl->currentTrack()->url() == m_core->metaData().value(Qmmp::URL))
    {
        pl->currentTrack()->updateMetaData(m_core->metaData());
        pl->updateMetaData();
    }
}
