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

#ifndef GROUPEDCONTAINER_P_H
#define GROUPEDCONTAINER_P_H

#include "playlistcontainer_p.h"

/*! @internal
 * @brief Playlist container with groups support.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class GroupedContainer : public PlayListContainer
{
public:
    GroupedContainer();

    virtual ~GroupedContainer();

    void addTrack(PlayListTrack *track);
    void addTracks(QList<PlayListTrack *> tracks);
    void insertTrack(int index, PlayListTrack *track);
    void replaceTracks(QList<PlayListTrack *> tracks);
    QList<PlayListGroup *> groups() const;
    QList<PlayListTrack *> tracks() const;
    QList<PlayListItem *> items() const;
    int count() const;
    int trackCount() const;
    QList<PlayListItem *> mid(int pos, int count) const;
    bool isEmpty() const;
    bool isSelected(int index) const;
    void setSelected(int index, bool selected);
    void clearSelection();
    int indexOf(PlayListItem *item) const;
    PlayListItem *item(int index) const;
    PlayListTrack *track(int index) const;
    PlayListGroup *group(int index) const;
    bool contains(PlayListItem *item) const;
    int indexOfTrack(int index) const;
    PlayListTrack *findTrack(int number) const;
    void removeTrack(PlayListTrack *track);
    void removeTracks(QList<PlayListTrack *> tracks);
    bool move(QList<int> indexes, int from, int to);
    QList<PlayListTrack *> takeAllTracks();
    void clear();

    void reverseList();
    void randomizeList();

private:
    void updateCache() const;
    QList<PlayListGroup *> m_groups;

    mutable QList<PlayListItem *> m_items;
    mutable bool m_update;

};

#endif // GROUPEDCONTAINER_P_H
