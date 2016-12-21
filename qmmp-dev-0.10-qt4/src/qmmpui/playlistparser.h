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
#ifndef PLAYLISTPARSER_H
#define PLAYLISTPARSER_H

#include <QStringList>
#include <QUrl>
#include "playlisttrack.h"
#include "playlistformat.h"

/*! @brief The PlaylistParser class provides a simple api to access playlist format plugins.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class PlayListParser
{
public:
    /*!
     * Returns a list of the installed playlist formats.
     */
    static QList<PlayListFormat*> formats();
    /*!
     * Returns a list of the supported files name filters, i.e. "*.m3u *.pls"
     */
    static QStringList nameFilters();
    /*!
     * Returns PlayListFormat pointer which supports mime type \b mime
     * or \b 0 if mime type \b mime is unsupported
     */
    static PlayListFormat *findByMime(const QString &mime);
    /*!
     * Finds playlist format by file path \b filePath
     * Returns \b 0 if file \b filePath is unsupported.
     */
    static PlayListFormat *findByPath(const QString &filePath);
    /*!
     * Finds playlist format by url path \b url
     * Returns \b 0 if file \b filePath is unsupported.
     */
    static PlayListFormat *findByUrl(const QUrl &url);
    /*!
     * Saves a list of tracks to the file.
     * @param tracks A list of tracks.
     * @param f_name File name the  playlist.
     */
    static void savePlayList(QList <PlayListTrack *> tracks, const QString &f_name);
    /*!
     * Loads playlist from file \b f_name
     * @param f_name File name.
     * @return A list of URLs or file paths.
     */
    static QStringList loadPlaylist(const QString &f_name);

private:
    PlayListParser(){}
    static void checkFormats();
    static QList<PlayListFormat*> *m_formats;


};

#endif
