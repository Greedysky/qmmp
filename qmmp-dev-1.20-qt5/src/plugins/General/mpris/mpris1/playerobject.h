/***************************************************************************
 *   Copyright (C) 2008-2013 by Ilya Kotov                                 *
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
#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include <QObject>
#include <QVariantMap>

class SoundCore;
class MediaPlayer;
class PlayListManager;
class QmmpUiSettings;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/

struct PlayerStatus
{
    int state;  // 0 = Playing, 1 = Paused, 2 = Stopped.
    int random; // 0 = Playing linearly, 1 = Playing randomly.
    int repeat; // 0 = Go to the next element once the current has finished playing, 1 = Repeat the current element
    int repeatPlayList; // 0 = Stop playing once the last element has been played, 1 = Never give up playing
};

Q_DECLARE_METATYPE(PlayerStatus)

class PlayerObject : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.MediaPlayer")

public:
    PlayerObject(QObject *parent = 0);

    ~PlayerObject();


    enum PlayerCaps
    {

        NONE                  = 0,
        CAN_GO_NEXT           = 1 << 0,
        CAN_GO_PREV           = 1 << 1,
        CAN_PAUSE             = 1 << 2,
        CAN_PLAY              = 1 << 3,
        CAN_SEEK              = 1 << 4,
        CAN_PROVIDE_METADATA  = 1 << 5,
        CAN_HAS_TRACKLIST     = 1 << 6
    };


public slots:
    void Next();
    void Prev();
    void Pause();
    void Stop();
    void Play();
    void Repeat(bool in0);
    PlayerStatus GetStatus();
    QVariantMap GetMetadata();
    int GetCaps();
    void VolumeSet(int in0);
    int VolumeGet();
    void PositionSet(int in0);
    int PositionGet();

signals:
    void CapsChange(int);
    void TrackChange(QVariantMap);
    void StatusChange(PlayerStatus);

private slots:
    void updateCaps();
    void updateTrack();
    void updateStatus();

private:
    SoundCore *m_core;
    MediaPlayer *m_player;
    PlayListManager *m_pl_manager;
    QmmpUiSettings *m_ui_settings;

};

#endif
