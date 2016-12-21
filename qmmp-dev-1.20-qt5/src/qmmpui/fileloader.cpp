/***************************************************************************
 *   Copyright (C) 2006-2015 by Ilya Kotov                                 *
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

#include <qmmp/metadatamanager.h>
#include <QRegExp>
#include <QMetaType>
#include <QDir>
#include <QApplication>
#include "fileloader_p.h"
#include "qmmpuisettings.h"
#include "playlistitem.h"
#include "playlisttrack.h"

FileLoader::FileLoader(QObject *parent) : QThread(parent)
{
    qRegisterMetaType<QList<PlayListTrack*> >("QList<PlayListTrack*>");
    m_settings = QmmpUiSettings::instance();
    m_finished = false;
    m_use_meta = false;
    connect(qApp, SIGNAL(aboutToQuit()), SLOT(finish()));
}

FileLoader::~FileLoader()
{}

QList<PlayListTrack *> FileLoader::processFile(const QString &path, QStringList *ignoredPaths)
{
    QList<PlayListTrack *> tracks;
    QList <FileInfo *> infoList = MetaDataManager::instance()->createPlayList(path, m_use_meta, ignoredPaths);

    foreach (FileInfo *info, infoList)
    {
        tracks.append(new PlayListTrack(info));
    }
    qDeleteAll(infoList);
    return tracks;
}

void FileLoader::addDirectory(const QString& s, PlayListItem *before)
{
    QList<PlayListTrack *> tracks;
    QStringList ignoredPaths;
    QDir dir(s);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);
    QFileInfoList l = dir.entryInfoList(m_filters);

    foreach(QFileInfo info, l)
    {
        if(checkRestrictFilters(info) && checkExcludeFilters(info))
        {
            QStringList paths;
            tracks.append(processFile(info.absoluteFilePath (), &ignoredPaths));
            ignoredPaths.append(paths);
        }

        if (m_finished)
        {
            qDeleteAll(tracks);
            tracks.clear();
            return;
        }

        if(tracks.count() > 30) //do not send more than 30 tracks at once
        {
            removeIgnoredTracks(&tracks, ignoredPaths);
            emit newTracksToInsert(before, tracks);
            tracks.clear();
            ignoredPaths.clear();
        }
    }

    if(!tracks.isEmpty())
    {
        removeIgnoredTracks(&tracks, ignoredPaths);
        emit newTracksToInsert(before, tracks);
        ignoredPaths.clear();
    }

    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name);
    l.clear();
    l = dir.entryInfoList();

    for (int i = 0; i < l.size(); ++i)
    {
        QFileInfo fileInfo = l.at(i);
        addDirectory(fileInfo.absoluteFilePath (), before);
        if (m_finished)
            return;
    }
}

void FileLoader::run()
{
    m_finished = false;
    m_mutex.lock();
    if(m_tasks.isEmpty())
    {
        m_mutex.unlock();
        return;
    }
    m_mutex.unlock();

    while(!m_finished)
    {
        m_mutex.lock();
        LoaderTask i = m_tasks.dequeue();
        m_mutex.unlock();
        PlayListItem *before = i.before;
        QString path = i.path;

        QFileInfo info(path);

        if(info.isDir())
        {
            addDirectory(path, before);

        }
        else if(info.isFile() || path.contains("://"))
        {
            QList<PlayListTrack *> tracks = processFile(path);
            if(!tracks.isEmpty())
            {
                emit newTracksToInsert(before, tracks);
            }
        }

        m_mutex.lock();
        if(m_tasks.isEmpty())
        {
            m_mutex.unlock();
            break;
        }
        m_mutex.unlock();
    }
}

void FileLoader::add(const QString &path)
{
    insert(0, QStringList() << path);
}

void FileLoader::add(const QStringList &paths)
{
    insert(0, paths);
}

void FileLoader::insert(PlayListItem *before, const QString &path)
{
    insert(before, QStringList() << path);
}

void FileLoader::insert(PlayListItem *before, const QStringList &paths)
{
    m_mutex.lock();
    foreach (QString path, paths)
    {
        LoaderTask task;
        task.before = before;
        task.path = QDir::fromNativeSeparators(path);
        m_tasks.append(task);
    }
    m_mutex.unlock();

    if(!isRunning())
    {
        MetaDataManager::instance()->prepareForAnotherThread();
        m_filters = MetaDataManager::instance()->nameFilters();
        m_use_meta = m_settings->useMetadata();
    }
    start(QThread::IdlePriority);
}

void FileLoader::finish()
{
    m_finished = true;
    wait();
    m_tasks.clear();
}

bool FileLoader::checkRestrictFilters(const QFileInfo &info)
{
    if(m_settings->restrictFilters().isEmpty())
        return true;

    foreach(QString filter, m_settings->restrictFilters())
    {
        QRegExp regexp (filter, Qt::CaseInsensitive, QRegExp::Wildcard);
        if(regexp.exactMatch(info.absoluteFilePath()))
            return true;
    }
    return false;
}

bool FileLoader::checkExcludeFilters(const QFileInfo &info)
{
    if(m_settings->excludeFilters().isEmpty())
        return true;

    foreach(QString filter, m_settings->excludeFilters())
    {
        QRegExp regexp (filter, Qt::CaseInsensitive, QRegExp::Wildcard);
        if(regexp.exactMatch(info.absoluteFilePath()))
            return false;
    }
    return true;
}

void FileLoader::removeIgnoredTracks(QList<PlayListTrack *> *tracks, const QStringList &ignoredPaths)
{
    if(ignoredPaths.isEmpty())
        return;

    foreach(PlayListTrack *track, *tracks)
    {
        if(ignoredPaths.contains(track->url()))
        {
            tracks->removeAll(track);
            delete track;
        }
    }
}
