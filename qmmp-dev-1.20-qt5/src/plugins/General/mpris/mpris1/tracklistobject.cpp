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

#include <QFile>
#include <QUrl>

#include <qmmpui/playlistmanager.h>
#include <qmmpui/mediaplayer.h>
#include <qmmpui/playlistitem.h>
#include <qmmpui/qmmpuisettings.h>
#include "tracklistobject.h"

TrackListObject::TrackListObject(QObject *parent) : QObject(parent)
{
    m_player = MediaPlayer::instance();
    m_ui_settings = QmmpUiSettings::instance();
    m_pl_manager = m_player->playListManager();
    m_model = m_pl_manager->currentPlayList();
    connect (m_model, SIGNAL(listChanged(int)), SLOT(updateTrackList(int)));
    connect (m_pl_manager, SIGNAL(currentPlayListChanged(PlayListModel*,PlayListModel*)),
             SLOT(switchPlayList(PlayListModel*,PlayListModel*)));
    m_prev_count = 0;
}

TrackListObject::~TrackListObject()
{
}

int TrackListObject::AddTrack(const QString &in0, bool in1)
{
    QString path = in0;
    if(in0.startsWith("file://"))
    {
        path = QUrl(in0).toLocalFile ();
        if(!QFile::exists(path))
            return 1; //error
    }
    if(in1)
    {
        m_pl_manager->selectPlayList(m_model);
        m_player->stop();
        connect(m_model, SIGNAL(trackAdded(PlayListTrack*)), SLOT(playTrack(PlayListTrack*)));
    }
    m_model->add(path);
    return 0;
}

void TrackListObject::DelTrack(int in0)
{
    m_model->removeTrack(in0);
}

int TrackListObject::GetCurrentTrack()
{
    return m_model->currentIndex();
}

int TrackListObject::GetLength()
{
    return m_model->trackCount();
}

QVariantMap TrackListObject::GetMetadata(int in0)
{
    QVariantMap map;
    PlayListTrack *track = m_model->findTrack(in0);

    if (track)
    {
        if (track->url().contains("://"))
            map.insert("location", track->url());
        else
            map.insert("location", "file://" + track->url());
        map.insert("title", track->value(Qmmp::TITLE));
        map.insert("artist", track->value(Qmmp::ARTIST));
        map.insert("albumartist", track->value(Qmmp::ALBUMARTIST));
        map.insert("album", track->value(Qmmp::ALBUM));
        map.insert("tracknumber", track->value(Qmmp::TRACK));
        map.insert("time", (quint32)track->length());
        map.insert("mtime", (quint32)track->length() * 1000);
        map.insert("genre", track->value(Qmmp::GENRE));
        map.insert("comment", track->value(Qmmp::COMMENT));
        map.insert("year", track->value(Qmmp::YEAR).toUInt());
    }
    return map;
}

void TrackListObject::SetLoop(bool in0)
{
    m_ui_settings->setRepeatableList(in0);
}

void TrackListObject::SetRandom(bool in0)
{
    m_ui_settings->setShuffle(in0);
}

void TrackListObject::playTrack(PlayListTrack *track)
{
    m_model->setCurrent(track);
    m_player->play();
    disconnect(m_model,SIGNAL(trackAdded(PlayListTrack*)), this, SLOT(playTrack(PlayListTrack*)));
}

void  TrackListObject::updateTrackList(int flags)
{
    if(flags & PlayListModel::STRUCTURE)
        emit TrackListChange(m_model->trackCount());
}

void TrackListObject::switchPlayList(PlayListModel *cur, PlayListModel *prev)
{
    m_model = cur;
    connect (m_model, SIGNAL(listChanged(int)), SLOT(updateTrackList(int)));
    if(prev)
        disconnect(prev,0,this,0);
    updateTrackList(PlayListModel::STRUCTURE);
}
