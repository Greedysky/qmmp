/***************************************************************************
 *   Copyright (C) 2013-2014 by Ilya Kotov                                 *
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

#include "playlistgroup.h"

PlayListGroup::PlayListGroup(const QString &name)
{
    m_name = name;
}

PlayListGroup::~PlayListGroup()
{
    while(!trackList.isEmpty())
    {
        PlayListTrack* mf = trackList.takeFirst();

        if (mf->isUsed())
            mf->deleteLater();
        else
            delete mf;
    }
}

const QString PlayListGroup::formattedTitle(int column)
{
    Q_UNUSED(column);
    return m_name;
}

const QStringList PlayListGroup::formattedTitles()
{
    return QStringList() << m_name;
}

bool PlayListGroup::contains(PlayListTrack *track) const
{
    return trackList.contains(track);
}

bool PlayListGroup::isEmpty() const
{
    return trackList.isEmpty();
}

QList<PlayListTrack *> PlayListGroup::tracks() const
{
    return trackList;
}

int PlayListGroup::count() const
{
    return trackList.count();
}

bool PlayListGroup::isGroup() const
{
    return true;
}
