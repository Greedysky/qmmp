/***************************************************************************
 *   Copyright (C) 2013-2015 by Ilya Kotov                                 *
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

#ifndef PLAYLISTCONTAINER_P_H
#define PLAYLISTCONTAINER_P_H

#include <QList>
#include "playlistitem.h"
#include "playlisttrack.h"
#include "playlistgroup.h"


/*! @internal
 * @brief Base class of playlist containers.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 * @author Vladimir Kuznetsov <vovanec@gmail.com>
 */
class PlayListContainer
{
public:
    PlayListContainer(){}
    virtual ~PlayListContainer(){}

    virtual void addTrack(PlayListTrack *track);
    virtual void addTracks(QList<PlayListTrack *> tracks) = 0;
    virtual void insertTrack(int index, PlayListTrack *track) = 0;
    virtual void replaceTracks(QList<PlayListTrack *> tracks) = 0;
    virtual QList<PlayListGroup *> groups() const = 0;
    virtual QList<PlayListItem *> items() const = 0;
    virtual QList<PlayListTrack *> tracks() const = 0;
    virtual int count() const = 0;
    virtual int trackCount() const = 0;
    virtual QList<PlayListItem *> mid(int pos, int count) const = 0;
    virtual bool isEmpty() const = 0;
    virtual bool isSelected(int index) const = 0;
    virtual void setSelected(int index, bool selected) = 0;
    virtual void clearSelection() = 0;
    virtual int indexOf(PlayListItem *item) const = 0;
    virtual PlayListItem *item(int index) const = 0;
    virtual PlayListTrack *track(int index) const = 0;
    virtual PlayListGroup *group(int index) const = 0;
    virtual bool contains(PlayListItem *item) const = 0;
    virtual int indexOfTrack(int index) const = 0;
    virtual PlayListTrack *findTrack(int number) const = 0;
    virtual void removeTrack(PlayListTrack *track) = 0;
    virtual void removeTracks(QList<PlayListTrack *> tracks) = 0;
    virtual bool move(QList<int> indexes, int from, int to) = 0;
    virtual QList<PlayListTrack *> takeAllTracks() = 0;
    virtual void clear() = 0;

    virtual void reverseList() = 0;
    virtual void randomizeList() = 0;

protected:
    void swapTrackNumbers(QList<PlayListItem *> *container, int index1, int index2);
};

#endif // PLAYLISTCONTAINER_P_H
