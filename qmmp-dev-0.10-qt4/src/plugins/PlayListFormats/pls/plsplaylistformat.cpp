/***************************************************************************
 *   Copyright (C) 2008-2012 by Ilya Kotov                                 *
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

#include <QFileInfo>
#include <QtPlugin>
#include "plsplaylistformat.h"

const PlayListFormatProperties PLSPlaylistFormat::properties() const
{
    PlayListFormatProperties p;
    p.filters << "*.pls";
    p.contentTypes << "audio/x-scpls";
    p.shortName = "pls";
    return p;
}

QStringList PLSPlaylistFormat::decode(const QString & contents)
{
    QStringList out;
    QStringList splitted = contents.split("\n");
    if (!splitted.isEmpty())
    {
        if (splitted.takeAt(0).toLower().contains("[playlist]"))
        {
            foreach(QString str, splitted)
            {
                if (str.startsWith("File"))
                {
                    QString unverified = str.remove(0,str.indexOf(QChar('=')) + 1);
                    unverified = unverified.trimmed();
                    if (unverified.startsWith("http://"))
                    {
                        out << unverified;
                    }
                    else /*if (QFileInfo(unverified).exists())*/
                        out << QFileInfo(unverified).absoluteFilePath();
                    /*else
                        qWarning("File %s does not exist", qPrintable(unverified));*/
                }
            }
            return out;
        }
    }
    else
        qWarning("Error parsing PLS format");

    return QStringList();
}

QString PLSPlaylistFormat::encode(const QList<PlayListTrack *> & contents)
{
    QStringList out;
    out << QString("[playlist]");
    int counter = 1;
    foreach(PlayListTrack* f,contents)
    {
        QString begin = "File" + QString::number(counter) + "=";
        out.append(begin + f->url());
        begin = "Title" + QString::number(counter) + "=";
        out.append(begin + f->value(Qmmp::TITLE));
        begin = "Length" + QString::number(counter) + "=";
        out.append(begin + QString::number(f->length()));
        counter ++;
    }
    out << "NumberOfEntries=" + QString::number(contents.count());
    return out.join("\n");
}

Q_EXPORT_PLUGIN2(plsplaylistformat, PLSPlaylistFormat)
