/***************************************************************************
 *   Copyright (C) 2008-2015 by Ilya Kotov                                 *
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

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFileInfo>
#include <QUrl>
#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "xspfplaylistformat.h"

// Needs more work - it's better use libSpiff there and put it as plugin.

const PlayListFormatProperties XSPFPlaylistFormat::XSPFPlaylistFormat::properties() const
{
    PlayListFormatProperties p;
    p.filters << "*.xspf";
    p.shortName = "xspf";
    p.contentTypes << "application/xspf+xml";
    return p;
}

QStringList XSPFPlaylistFormat::decode(const QString & contents)
{
    QStringList out;
    QString currentTag;
    QString contents_copy = contents;

    //remove control symbols to avoid xml errors
    for(int i = 0; i < contents_copy.size(); ++i)
    {
        if(contents_copy[i] <= 0x1F)
        {
            contents_copy.replace(i, 1, "");
            i--;
        }
    }

    QXmlStreamReader xml(contents_copy);
    while(!xml.atEnd())
    {
        xml.readNext();
        if (xml.isStartElement())
        {
            currentTag = xml.name().toString();

        }
        else if (xml.isCharacters() && !xml.isWhitespace())
        {
            if (currentTag == "location")
            {

                QUrl url(xml.text().toString());
                if (url.scheme() == "file")  //remove scheme for local files only
                    out << QUrl::fromPercentEncoding(url.toString().toLatin1()).remove("file://");
                else
                    out << QUrl::fromPercentEncoding(url.toString().toLatin1());
            }
            else
                xml.skipCurrentElement();
        }
    }

    if(xml.hasError())
    {
        qDebug("XSPFPlaylistFormat: parse error: %s (row:%lld, col:%lld",
               qPrintable(xml.errorString()), xml.lineNumber(), xml.columnNumber());
    }
    return out;
}

// Needs more work - it's better use libSpiff there and put it as plugin.

QString XSPFPlaylistFormat::encode(const QList<PlayListTrack*> & files)
{
    QString out;
    QXmlStreamWriter xml(&out);
    xml.setCodec("UTF-8");
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("playlist");
    xml.writeAttribute("version", "1");
    xml.writeAttribute("xmlns", "http://xspf.org/ns/0/");
    xml.writeTextElement("creator", "qmmp-" + Qmmp::strVersion());
    xml.writeStartElement("trackList");

    int counter = 1;
    foreach(PlayListTrack* f,files)
    {
        xml.writeStartElement("track");

        QString url;
        if (f->url().contains("://"))
            url = QUrl::toPercentEncoding(f->url(), ":/");
        else  //append protocol
            url = QUrl::toPercentEncoding(QString("file://") +
                                          QFileInfo(f->url()).absoluteFilePath(), ":/");

        xml.writeTextElement("location", url);
        xml.writeTextElement("title", f->value(Qmmp::TITLE));
        xml.writeTextElement("creator", f->value(Qmmp::ARTIST));
        xml.writeTextElement("annotation", f->value(Qmmp::COMMENT));
        xml.writeTextElement("album", f->value(Qmmp::ALBUM));
        xml.writeTextElement("trackNum", QString::number(counter));

        xml.writeStartElement("meta");
        xml.writeAttribute("rel", "year");
        xml.writeCharacters(f->value(Qmmp::YEAR));
        xml.writeEndElement(); // meta
        xml.writeEndElement(); // track

        counter ++;
    }
    xml.writeEndElement(); //trackList
    xml.writeEndElement(); //playlist
    xml.writeEndDocument();
    return out;

}
