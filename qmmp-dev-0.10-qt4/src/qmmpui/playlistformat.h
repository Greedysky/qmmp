/***************************************************************************
 *   Copyright (C) 2006-2013 by Ilya Kotov                                 *
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

#ifndef PALYLISTFORMAT_H
#define PALYLISTFORMAT_H

#include <QStringList>

class PlayListTrack;

/*! @brief Helper structure to store playlist format properies.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
struct PlayListFormatProperties
{
    QString shortName;        /*!< Unique name of the playlist format for internal usage */
    QStringList filters;      /*!< File filters (example: "*.m3u") */
    QStringList contentTypes; /*!< Supported content types */
};

/*! @brief Abstract interface for playlist formats.
 * @author Vladimir Kuznetsov <vovanec@gmail.com>
 */
class PlayListFormat
{
public:
    /*!
     * Object destructor
     */
    virtual ~PlayListFormat() {}
    /*!
     * Returns playlist format properties.
     */
    virtual const PlayListFormatProperties properties() const = 0;
    /*!
     * Takes raw contents of playlist file, should return string list of
     * ready file pathes to fill the playlist.
     */
    virtual QStringList decode(const QString& contents) = 0;
    /*!
     * Takes the list of AbstractPlaylistItem objects, should return string of
     * encoded playlist file
     */
    virtual QString encode(const QList<PlayListTrack*>& contents) = 0;
};

Q_DECLARE_INTERFACE(PlayListFormat,"PlayListFormat/1.0")

#endif
