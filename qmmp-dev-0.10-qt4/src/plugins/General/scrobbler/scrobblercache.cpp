/***************************************************************************
 *   Copyright (C) 2013-2016 by Ilya Kotov                                 *
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

#include <QFile>
#include "scrobblercache.h"

SongInfo::SongInfo()
{
    m_length = 0;
}

SongInfo::SongInfo(const QMap <Qmmp::MetaData, QString> metadata, qint64 length)
{
    m_metadata = metadata;
    m_length = length;
}

SongInfo::SongInfo(const SongInfo &other)
{
    m_metadata = other.metaData();
    m_length  = other.length();
    m_start_ts = other.timeStamp();
}

SongInfo::~SongInfo()
{}

SongInfo & SongInfo::operator=(const SongInfo &info)
{
    m_metadata = info.metaData();
    m_length = info.length();
    m_start_ts = info.timeStamp();
    return *this;
}

bool SongInfo::operator==(const SongInfo &info)
{
    return (m_metadata == info.metaData()) && (m_length == info.length()) && (m_start_ts == info.timeStamp());
}

bool SongInfo::operator!=(const SongInfo &info)
{
    return !operator==(info);
}

void SongInfo::setMetaData(const QMap <Qmmp::MetaData, QString> metadata)
{
    m_metadata = metadata;
}

void SongInfo::setMetaData(Qmmp::MetaData key, const QString &value)
{
    m_metadata.insert(key, value);
}

void SongInfo::setLength(qint64 l)
{
    m_length = l;
}

const QMap <Qmmp::MetaData, QString> SongInfo::metaData() const
{
    return m_metadata;
}

const QString SongInfo::metaData(Qmmp::MetaData key) const
{
    return m_metadata.value(key);
}

qint64 SongInfo::length () const
{
    return m_length;
}

void SongInfo::clear()
{
    m_metadata.clear();
    m_length = 0;
}

void SongInfo::setTimeStamp(uint ts)
{
    m_start_ts = ts;
}

uint SongInfo::timeStamp() const
{
    return m_start_ts;
}

ScrobblerCache::ScrobblerCache(const QString &filePath)
{
    m_filePath = filePath;
}

QList<SongInfo> ScrobblerCache::load()
{
    QList<SongInfo> songs;
    int s = 0;
    QString line, param, value;
    QFile file(m_filePath);

    if(!file.open(QIODevice::ReadOnly))
        return QList<SongInfo>();

    while (!file.atEnd())
    {
        line = QString::fromUtf8(file.readLine()).trimmed();
        if ((s = line.indexOf("=")) < 0)
            continue;

        param = line.left(s);
        value = line.right(line.size() - s - 1);

        if (param == "title")
        {
            songs << SongInfo();
            songs.last().setMetaData(Qmmp::TITLE, value);
        }
        else if (songs.isEmpty())
            continue;
        else if (param == "artist")
            songs.last().setMetaData(Qmmp::ARTIST, value);
        else if (param == "album")
            songs.last().setMetaData(Qmmp::ALBUM, value);
        else if (param == "comment")
            songs.last().setMetaData(Qmmp::COMMENT, value);
        else if (param == "genre")
            songs.last().setMetaData(Qmmp::GENRE, value);
        else if (param == "year")
            songs.last().setMetaData(Qmmp::YEAR, value);
        else if (param == "track")
            songs.last().setMetaData(Qmmp::TRACK, value);
        else if (param == "length")
            songs.last().setLength(value.toInt());
        else if (param == "time")
            songs.last().setTimeStamp(value.toUInt());
    }
    file.close();
    return songs;
}

void ScrobblerCache::save(const QList<SongInfo> &songs)
{
    QFile file(m_filePath);
    if (songs.isEmpty())
    {
        file.remove();
        return;
    }
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning("ScrobblerCach: unable to save file %s", qPrintable(m_filePath));
        qWarning("ScrobblerCach: error %d: %s", file.error(), qPrintable(file.errorString()));
        return;
    }
    foreach(SongInfo m, songs)
    {
        file.write(QString("title=%1").arg(m.metaData(Qmmp::TITLE)).toUtf8() +"\n");
        file.write(QString("artist=%1").arg(m.metaData(Qmmp::ARTIST)).toUtf8() +"\n");
        file.write(QString("album=%1").arg(m.metaData(Qmmp::ALBUM)).toUtf8() +"\n");
        file.write(QString("comment=%1").arg(m.metaData(Qmmp::COMMENT)).toUtf8() +"\n");
        file.write(QString("genre=%1").arg(m.metaData(Qmmp::GENRE)).toUtf8() +"\n");
        file.write(QString("year=%1").arg(m.metaData(Qmmp::YEAR)).toUtf8() +"\n");
        file.write(QString("track=%1").arg(m.metaData(Qmmp::TRACK)).toUtf8() +"\n");
        file.write(QString("length=%1").arg(m.length()).toUtf8() +"\n");
        file.write(QString("time=%1").arg(m.timeStamp()).toUtf8() +"\n");
    }
    file.close();
}
