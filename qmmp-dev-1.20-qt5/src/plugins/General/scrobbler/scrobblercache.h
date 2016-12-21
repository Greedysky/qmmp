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

#ifndef SCROBBLERCACHE_H
#define SCROBBLERCACHE_H

#include <QMap>
#include <QList>
#include <qmmp/qmmp.h>


/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class SongInfo
{
public:
    SongInfo();
    SongInfo(const QMap <Qmmp::MetaData, QString> metadata, qint64 length = 0);
    SongInfo(const SongInfo &other);

    ~SongInfo();

    SongInfo & operator=(const SongInfo &info);
    bool operator==(const SongInfo &info);
    bool operator!=(const SongInfo &info);

    void setMetaData(const QMap <Qmmp::MetaData, QString> metadata);
    void setMetaData(Qmmp::MetaData key, const QString &value);
    void setLength(qint64 l);
    const QMap <Qmmp::MetaData, QString> metaData() const;
    const QString metaData(Qmmp::MetaData) const;
    qint64 length () const;
    void clear();
    void setTimeStamp(uint ts);
    uint timeStamp() const;

private:
    QMap <Qmmp::MetaData, QString> m_metadata;
    qint64 m_length;
    uint m_start_ts;

};

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class ScrobblerCache
{
public:
    explicit ScrobblerCache(const QString &filePath);

    QList<SongInfo> load();
    void save(const QList<SongInfo> &songs);

private:
    QString m_filePath;

};

#endif // SCROBBLERCACHE_H
