/***************************************************************************
 *   Copyright (C) 2008-2014 by Ilya Kotov                                 *
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
#ifndef XSPFPLAYLISTFORMAT_H
#define XSPFPLAYLISTFORMAT_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <qmmpui/playlistformat.h>
#include <qmmpui/playlisttrack.h>

/*!
 * Class for XSPF playlist format parsing
 * @author Vladimir Kuznetsov <vovanec@gmail.com>
 */
class XSPFPlaylistFormat : public QObject, public PlayListFormat
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qmmp.qmmpui.PlayListFormatInterface.1.0")
    Q_INTERFACES(PlayListFormat)
public:
    const PlayListFormatProperties properties() const;
    QStringList decode(const QString& contents);
    QString encode(const QList<PlayListTrack*>& contents);
};

#endif
