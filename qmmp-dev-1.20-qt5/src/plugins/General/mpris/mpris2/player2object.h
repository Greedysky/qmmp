/***************************************************************************
 *   Copyright (C) 2010-2015 by Ilya Kotov                                 *
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
#ifndef PLAYER2OBJECT_H
#define PLAYER2OBJECT_H

#include <QDBusAbstractAdaptor>
#include <QVariantMap>
#include <QDBusObjectPath>
#include <QMap>
#include <QUuid>
#include <qmmp/qmmp.h>

class SoundCore;
class MediaPlayer;
class PlayListManager;
class PlayListTrack;
class PlayListModel;
class QmmpUiSettings;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class Player2Object : public QDBusAbstractAdaptor
{
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")
Q_PROPERTY(bool CanControl READ canControl)
Q_PROPERTY(bool CanGoNext READ canGoNext)
Q_PROPERTY(bool CanGoPrevious READ canGoPrevious)
Q_PROPERTY(bool CanPause READ canPause)
Q_PROPERTY(bool CanPlay READ canPlay)
Q_PROPERTY(bool CanSeek READ canSeek)
Q_PROPERTY(QString LoopStatus READ loopStatus WRITE setLoopStatus)
Q_PROPERTY(double MaximumRate READ maximumRate)
Q_PROPERTY(QVariantMap Metadata READ metadata)
Q_PROPERTY(double MinimumRate READ minimumRate)
Q_PROPERTY(QString PlaybackStatus READ playbackStatus)
Q_PROPERTY(qlonglong Position READ position)
Q_PROPERTY(double Rate READ rate WRITE setRate)
Q_PROPERTY(bool Shuffle READ shuffle WRITE setShuffle)
Q_PROPERTY(double Volume READ volume WRITE setVolume)

public:
    Player2Object(QObject *parent = 0);
    virtual ~Player2Object();
    bool canControl() const;
    bool canGoNext() const;
    bool canGoPrevious() const;
    bool canPause() const;
    bool canPlay() const;
    bool canSeek() const;
    QString loopStatus() const;
    void setLoopStatus(const QString &value);
    double maximumRate() const;
    QVariantMap metadata() const;
    double minimumRate() const;
    QString playbackStatus() const;
    qlonglong position() const;
    double rate() const;
    void setRate(double value);
    bool shuffle() const;
    void setShuffle(bool value);
    double volume() const;
    void setVolume(double value);

public slots:
    void Next();
    void OpenUri(const QString &in0);
    void Pause();
    void Play();
    void PlayPause();
    void Previous();
    void Seek(qlonglong Offset);
    void SetPosition(const QDBusObjectPath &TrackId, qlonglong Position);
    void Stop();
signals:
    void Seeked(qlonglong Position);

private slots:
    void emitPropertiesChanged();
    void updateId();
    void checkState(Qmmp::State state);
    void checkSeeking(qint64 elapsed);
    void playTrack(PlayListTrack *item);
    void disconnectPl();
    void setModel(PlayListModel *selected, PlayListModel *previous);


private:
    void syncProperties();
    SoundCore *m_core;
    MediaPlayer *m_player;
    PlayListManager *m_pl_manager;
    QmmpUiSettings *m_ui_settings;
    QMap<QString, QVariant> m_props;
    QDBusObjectPath m_trackID;
    PlayListTrack *m_prev_track;
    qint64 m_previous_pos;

};

#endif
