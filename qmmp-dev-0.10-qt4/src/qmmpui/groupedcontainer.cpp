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

#include "playlistmodel.h"
#include "groupedcontainer_p.h"

GroupedContainer::GroupedContainer()
{
    m_update = true;
}

GroupedContainer::~GroupedContainer()
{
    clear();
}

void GroupedContainer::addTrack(PlayListTrack *track)
{
    int firstIndex = 0, lastIndex = 0;
    //insert if possible
    for(int i = 0; i < m_groups.count(); ++i)
    {
        if(i == 0)
        {
           firstIndex = 0;
           lastIndex = m_groups[i]->count();
        }
        else
        {
            firstIndex = lastIndex + 1;
            lastIndex = firstIndex + m_groups[i]->count();
        }

        if(track->groupName() == m_groups[i]->formattedTitle())
        {
            m_groups[i]->trackList.append(track);
            m_items.insert(lastIndex + 1, track);
            m_update = true;
            return;
        }
    }
    PlayListGroup *group = new PlayListGroup(track->groupName());
    group->trackList.append(track);
    m_groups.append(group);
    m_items.append(group);
    m_items.append(track);
    track->setTrackIndex(trackCount() - 1);
}

void GroupedContainer::addTracks(QList<PlayListTrack *> tracks)
{
    bool found = false;

    for(int i = 0; i < tracks.count(); ++i)
    {
        found = false;
        for(int j = m_groups.count() - 1; j >= 0; --j)
        {
            if(m_groups.at(j)->formattedTitle() == tracks.at(i)->groupName())
            {
                found = true;
                m_groups.at(j)->trackList.append(tracks[i]);
                break;
            }
        }

        if(found)
            continue;

        m_groups << new PlayListGroup(tracks.at(i)->groupName());
        m_groups.last()->trackList.append(tracks.at(i));
    }
    m_update = true;
}

void GroupedContainer::insertTrack(int index, PlayListTrack *track)
{
    int firstIndex = 0, lastIndex = 0;
    //insert if possible
    for(int i = 0; i < m_groups.count(); ++i)
    {
        if(i == 0)
        {
           firstIndex = 0;
           lastIndex = m_groups[i]->count();
        }
        else
        {
            firstIndex = lastIndex + 1;
            lastIndex = firstIndex + m_groups[i]->count();
        }

        if(track->groupName() == m_groups[i]->formattedTitle() &&
                index > firstIndex && index <= lastIndex + 1)
        {
            m_groups[i]->trackList.insert(index - firstIndex - 1, track);
            m_update = true;
            return;
        }
    }
    //just add otherwise
    addTrack(track);
}

void GroupedContainer::replaceTracks(QList<PlayListTrack *> tracks)
{
    foreach (PlayListGroup *g, m_groups)
    {
        g->trackList.clear();
    }
    clear();
    addTracks(tracks);
}

QList<PlayListGroup *> GroupedContainer::groups() const
{
    return m_groups;
}

QList<PlayListTrack *> GroupedContainer::tracks() const
{
    QList<PlayListTrack *> trackList;
    for(int i = 0; i < m_groups.count(); ++i)
    {
        trackList.append(m_groups[i]->trackList);
    }
    return trackList;
}

QList<PlayListItem *> GroupedContainer::items() const
{
    updateCache();
    return m_items;
}

int GroupedContainer::count() const
{
    updateCache();
    return m_items.count();
}

int GroupedContainer::trackCount() const
{
    updateCache();
    return m_items.count() - m_groups.count();
}

QList<PlayListItem *> GroupedContainer::mid(int pos, int count) const
{
    updateCache();
    return m_items.mid(pos, count);
}

bool GroupedContainer::isEmpty() const
{
    return m_groups.isEmpty();
}

bool GroupedContainer::isSelected(int index) const
{
    updateCache();
    if (0 <= index && index < m_items.count())
        return m_items.at(index)->isSelected();
    return false;
}

void GroupedContainer::setSelected(int index, bool selected)
{
    updateCache();
    if (0 <= index && index < m_items.count())// && !m_items.at(index)->isGroup())
        m_items.at(index)->setSelected(selected);
}

void GroupedContainer::clearSelection()
{
    updateCache();
    foreach (PlayListItem *item, m_items)
    {
        item->setSelected(false);
    }
}

int GroupedContainer::indexOf(PlayListItem *item) const
{
    updateCache();
    return m_items.indexOf(item);
}

PlayListItem *GroupedContainer::item(int index) const
{
    updateCache();
    if(index >= count() || index < 0)
    {
        qWarning("GroupedContainer: index is out of range");
        return 0;
    }
    return m_items.at(index);
}

PlayListTrack *GroupedContainer::track(int index) const
{
    updateCache();
    PlayListItem *i = item(index);
    if(!i || i->isGroup())
        return 0;
    return dynamic_cast<PlayListTrack *> (i);
}

PlayListGroup *GroupedContainer::group(int index) const
{
    PlayListItem *i = item(index);
    if(i && i->isGroup())
        return dynamic_cast<PlayListGroup *> (i);
    return 0;
}

bool GroupedContainer::contains(PlayListItem *item) const
{
    updateCache();
    return m_items.contains(item);
}

int GroupedContainer::indexOfTrack(int index) const
{
    updateCache();
    if(index >= count() || index < 0)
    {
        qWarning("GroupedContainer: index is out of range");
        return -1;
    }
    return m_items.at(index)->trackIndex();
}

PlayListTrack *GroupedContainer::findTrack(int number) const
{
    int firstNumber = 0;
    foreach (PlayListGroup *group, m_groups)
    {
        if(number >= firstNumber && number < firstNumber + group->count())
        {
            return group->trackList.at(number - firstNumber);
        }
        firstNumber += group->count();
    }
    return 0;
}

void GroupedContainer::removeTrack(PlayListTrack *track)
{
    foreach(PlayListGroup *group, m_groups)
    {
        if(group->contains(track))
        {
            group->trackList.removeAll(track);
            m_items.removeAll(track);
            if(group->isEmpty())
            {
                m_groups.removeAll(group);
                m_items.removeAll(group);
                delete group;
            }
            return;
        }
    }
}

void GroupedContainer::removeTracks(QList<PlayListTrack *> tracks)
{
    foreach(PlayListTrack *t, tracks)
        removeTrack(t);
}

bool GroupedContainer::move(QList<int> indexes, int from, int to)
{
    updateCache();
    PlayListGroup *group = 0;
    int firstIndex = 0, lastIndex = 0;

    for(int i = 0; i < m_groups.count(); ++i)
    {
        if(i == 0)
        {
           firstIndex = 0;
           lastIndex = m_groups[i]->count();
        }
        else
        {
            firstIndex = lastIndex + 1;
            lastIndex = firstIndex + m_groups[i]->count();
        }

        if(from > firstIndex && from <= lastIndex && to > firstIndex && to <= lastIndex)
        {
            group = m_groups.at(i);
            break;
        }
    }

    if(!group)
        return false;

    foreach (int i, indexes)
    {
        if(i <= firstIndex || i > lastIndex)
            return false;
        if(i + to - from - firstIndex - 1 >= group->count())
            return false;
        if(i + to - from - firstIndex - 1 < 0)
            return false;
        if(i + to - from < 0)
            return false;
    }

    if (from > to)
    {
        foreach(int i, indexes)
        {
            if (i + to - from < 0)
                break;
            else
            {
                m_items.move(i,i + to - from);
                swapTrackNumbers(&m_items,i,i + to - from);
                group->trackList.move(i - firstIndex - 1,
                                      i + to - from  - firstIndex - 1);
            }
        }
    }
    else
    {
        for (int i = indexes.count() - 1; i >= 0; i--)
        {
            if (indexes[i] + to - from >= m_items.count())
                break;
            else
            {
                m_items.move(indexes[i], indexes[i] + to - from);
                swapTrackNumbers(&m_items,indexes[i], indexes[i] + to - from);
                group->trackList.move(indexes[i] - firstIndex - 1,
                                      indexes[i] + to - from - firstIndex - 1);
            }
        }
    }
    return true;
}

QList<PlayListTrack *> GroupedContainer::takeAllTracks()
{
    QList<PlayListTrack *> tracks;
    foreach (PlayListGroup *g, m_groups)
    {
        tracks.append(g->trackList);
        g->trackList.clear();
    }
    clear();
    return tracks;
}

void GroupedContainer::clear()
{
    while(!m_groups.isEmpty())
    {
        delete m_groups.takeFirst();
    }
    m_items.clear();
}

void GroupedContainer::reverseList()
{
    QList<PlayListTrack *> tracks = takeAllTracks();

    for (int i = 0; i < tracks.size()/2 ;i++)
        tracks.swap(i, tracks.size() - i - 1);

    addTracks(tracks);
}

void GroupedContainer::randomizeList()
{
    for(int i = 0; i < m_groups.count(); ++i)
    {
        for (int j = 0; j < m_groups[i]->trackList.size(); j++)
        {
            m_groups[i]->trackList.swap(qrand() % m_groups[i]->trackList.size(),
                                        qrand() % m_groups[i]->trackList.size());
        }
    }

    for(int i = 0; i < m_groups.count(); ++i)
    {
        m_groups.swap(qrand() % m_groups.size(), qrand() % m_groups.size());
    }

    m_update = true;
}

void GroupedContainer::updateCache() const
{
    if(!m_update)
        return;

    m_items.clear();
    int t = 0;

    for(int i = 0; i < m_groups.count(); ++i)
    {
        m_items.append(m_groups.at(i));
        foreach (PlayListTrack *track, m_groups.at(i)->trackList)
        {
            track->setTrackIndex(t++);
            m_items.append(track);
        }
    }
    m_update = false;
}
