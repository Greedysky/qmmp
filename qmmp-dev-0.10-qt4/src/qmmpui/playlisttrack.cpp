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

#include <QApplication>
#include <qmmp/metadatamanager.h>
#include "qmmpuisettings.h"
#include "metadatahelper_p.h"
#include "playlisttrack.h"

PlayListTrack::PlayListTrack() : QMap<Qmmp::MetaData, QString>(), PlayListItem()
{
    m_track_index = -1;
    m_settings = QmmpUiSettings::instance();
    m_helper = MetaDataHelper::instance();
    m_length = 0;
    m_refCount = 0;
    m_sheduledForDeletion = false;
}

PlayListTrack::PlayListTrack(const PlayListTrack &other) : QMap<Qmmp::MetaData, QString>(other),
    PlayListItem()
{
    m_track_index = -1;
    m_settings = QmmpUiSettings::instance();
    m_helper = MetaDataHelper::instance();
    m_refCount = 0;
    m_sheduledForDeletion = false;

    m_formattedTitles = other.m_formattedTitles;
    m_group = other.m_group;
    m_formattedLength = other.m_formattedLength;
    m_titleFormats = other.m_titleFormats;
    m_groupFormat = other.m_groupFormat;
    setSelected(other.isSelected());
    m_length = other.m_length;
    m_formattedLength = other.m_formattedLength;
}

PlayListTrack::PlayListTrack(FileInfo *info) :  QMap<Qmmp::MetaData, QString>(info->metaData()),
    PlayListItem()
{
    m_track_index = -1;
    m_settings = QmmpUiSettings::instance();
    m_helper = MetaDataHelper::instance();
    setLength(m_length = info->length());
    insert(Qmmp::URL, info->path());
    m_refCount = 0;
    m_sheduledForDeletion = false;
}

PlayListTrack::~PlayListTrack()
{
    if(m_refCount != 0)
        qWarning("PlayListTrack: deleting busy track");
}

void PlayListTrack::updateMetaData(const QMap <Qmmp::MetaData, QString> &metaData)
{
    QMap <Qmmp::MetaData, QString>::operator =(metaData);
    m_formattedTitles.clear();
    formatGroup();
}

void PlayListTrack::updateMetaData()
{
    QList <FileInfo *> list =  MetaDataManager::instance()->createPlayList(value(Qmmp::URL));
    if(!list.isEmpty() && !list.at(0)->path().contains("://"))
    {
        FileInfo *info = list.at(0);
        m_length = info->length();
        QMap <Qmmp::MetaData, QString>::operator =(info->metaData());
        insert(Qmmp::URL, info->path());
        m_formattedTitles.clear();
        formatGroup();
    }
    qDeleteAll(list);
}

const QString PlayListTrack::groupName()
{
    if(m_group.isEmpty() || m_groupFormat != m_settings->groupFormat())
    {
        m_groupFormat = m_settings->groupFormat();
        formatGroup();
    }
    return m_group;
}

bool PlayListTrack::isGroup() const
{
    return false;
}

void PlayListTrack::setTrackIndex(int track_index)
{
    m_track_index = track_index;
}

int PlayListTrack::trackIndex() const
{
    return m_track_index;
}

void PlayListTrack::beginUsage()
{
    m_refCount++;
}

void PlayListTrack::endUsage()
{
    m_refCount--;
}

void PlayListTrack::deleteLater()
{
    m_sheduledForDeletion = true;
}

bool PlayListTrack::isSheduledForDeletion() const
{
    return m_sheduledForDeletion;
}

bool PlayListTrack::isUsed() const
{
    return (m_refCount != 0);
}

const QString PlayListTrack::formattedTitle(int column)
{
    if(m_formattedTitles.count() != m_helper->columnCount())
    {
        while(m_formattedTitles.count() > m_helper->columnCount())
            m_formattedTitles.takeLast();

        while(m_formattedTitles.count() < m_helper->columnCount())
            m_formattedTitles.append(QString());

        while(m_titleFormats.count() > m_helper->columnCount())
            m_titleFormats.takeLast();

        while(m_titleFormats.count() < m_helper->columnCount())
            m_titleFormats.append(QString());
    }

    if(column < 0 || column >= m_formattedTitles.size())
    {
        qWarning("PlayListTrack: column number is out of range");
        return QString();
    }

    if(m_formattedTitles[column].isEmpty() || m_titleFormats[column] != m_helper->titleFormatter(column)->pattern())
    {
        m_titleFormats[column] = m_helper->titleFormatter(column)->pattern();
        formatTitle(column);
    }
    return m_formattedTitles[column];
}

const QStringList PlayListTrack::formattedTitles()
{
    if(m_formattedTitles.count() != m_helper->columnCount())
    {
        while(m_formattedTitles.count() > m_helper->columnCount())
            m_formattedTitles.takeLast();

        while(m_formattedTitles.count() < m_helper->columnCount())
            m_formattedTitles.append(QString());

        while(m_titleFormats.count() > m_helper->columnCount())
            m_titleFormats.takeLast();

        while(m_titleFormats.count() < m_helper->columnCount())
            m_titleFormats.append(QString());
    }

    for(int column = 0; column < m_helper->columnCount(); column++)
    {
        if(m_formattedTitles[column].isEmpty() ||
                m_titleFormats[column] != m_helper->titleFormatter(column)->pattern() ||
                m_titleFormats[column].contains("%I"))
        {
            m_titleFormats[column] = m_helper->titleFormatter(column)->pattern();
            formatTitle(column);
        }
    }

    return m_formattedTitles;
}

const QString PlayListTrack::formattedLength()
{
    if(m_length > 0 && m_formattedLength.isEmpty())
    {
        m_formattedLength = MetaDataFormatter::formatLength(m_length);
    }
    else if(m_length == 0)
        m_formattedLength.clear();
    return m_formattedLength;
}

qint64 PlayListTrack::length() const
{
    return m_length;
}

void PlayListTrack::setLength(qint64 length)
{
    m_length = length;
    m_formattedLength.clear();
}

const QString PlayListTrack::url() const
{
    return value(Qmmp::URL);
}

void PlayListTrack::formatTitle(int column)
{
    m_formattedTitles[column] = m_helper->titleFormatter(column)->format(this);
    if(m_formattedTitles.count() == 1)
    {
        if (m_formattedTitles[column].isEmpty())
            m_formattedTitles[column] = value(Qmmp::URL).section('/',-1);
        if (m_formattedTitles[column].isEmpty())
            m_formattedTitles[column] = value(Qmmp::URL);
    }
    if (m_settings->convertUnderscore())
        m_formattedTitles[column].replace("_", " ");
    if (m_settings->convertTwenty())
        m_formattedTitles[column].replace("%20", " ");
}

void PlayListTrack::formatGroup()
{
    if(length() == 0 && url().contains("://"))
    {
        m_group = qApp->translate("PlayListTrack", "Streams");
        return;
    }
    m_group = m_helper->groupFormatter()->format(this);
    if (m_group.isEmpty())
        m_group = qApp->translate("PlayListTrack", "Empty group");
    if (m_settings->convertUnderscore())
        m_group.replace("_", " ");
    if (m_settings->convertTwenty())
        m_group.replace("%20", " ");
}
