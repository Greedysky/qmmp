/***************************************************************************
 *   Copyright (C) 2012-2015 by Ilya Kotov                                 *
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

#include <QSettings>
#include <QApplication>
#include <QTimer>
#include <qmmp/qmmp.h>
#include "metadatahelper_p.h"
#include "playlistmanager.h"
#include "qmmpuisettings.h"

QmmpUiSettings *QmmpUiSettings::m_instance = 0;

QmmpUiSettings::QmmpUiSettings(QObject *parent) : QObject(parent)
{
    if(m_instance)
        qFatal("QmmpUiSettings: only one instance is allowed");
    m_instance = this;
    m_helper = new MetaDataHelper;
    QSettings s (Qmmp::configFile(), QSettings::IniFormat);
    s.beginGroup("PlayList");
    m_group_format = s.value("group_format", "%p%if(%p&%a, - %if(%y,[%y] ,),)%a").toString();
    m_convertUnderscore = s.value ("convert_underscore", true).toBool();
    m_convertTwenty = s.value ("convert_twenty", true).toBool();
    m_useMetadata = s.value ("load_metadata", true).toBool();
    m_autosave_playlist = s.value("autosave", true).toBool();
    m_repeate_list = s.value("repeate_list",false).toBool();
    m_shuffle = s.value("shuffle",false).toBool();
    m_groups_enabled = s.value("groups",false).toBool();
    m_repeat_track = s.value("repeate_track",false).toBool();
    m_no_pl_advance = s.value("no_advance",false).toBool();
    m_clear_prev_playlist = s.value("clear_previous", false).toBool();
    s.endGroup();
    s.beginGroup("General");
    m_resume_on_startup = s.value("resume_on_startup", false).toBool();
    m_restrict_filters = s.value("restrict_filters").toStringList();
    m_exclude_filters = s.value("exclude_filters").toStringList();
    m_use_default_pl = s.value("use_default_pl", false).toBool();
    m_default_pl_name = s.value("default_pl_name", tr("Playlist")).toString();
    s.endGroup();
    m_use_clipboard = s.value("URLDialog/use_clipboard", false).toBool();
    m_timer = new QTimer(this);
    m_timer->setInterval(5000);
    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), SLOT(sync()));

    m_helper->setGroupFormat(m_group_format);
}

QmmpUiSettings::~QmmpUiSettings()
{
    m_instance = 0;
    sync();
    delete m_helper;
}

const QString QmmpUiSettings::groupFormat() const
{
    return m_group_format;
}

bool QmmpUiSettings::isRepeatableList() const
{
    return m_repeate_list;
}

bool QmmpUiSettings::isShuffle() const
{
    return m_shuffle;
}

bool QmmpUiSettings::isGroupsEnabled() const
{
    return m_groups_enabled;
}

bool QmmpUiSettings::isRepeatableTrack() const
{
    return m_repeat_track;
}

bool QmmpUiSettings::isNoPlayListAdvance() const
{
    return m_no_pl_advance;
}

bool QmmpUiSettings::convertUnderscore() const
{
    return m_convertUnderscore;
}

bool QmmpUiSettings::convertTwenty() const
{
    return m_convertTwenty;
}

bool QmmpUiSettings::useMetadata() const
{
    return m_useMetadata;
}

void QmmpUiSettings::setConvertUnderscore(bool yes)
{
    m_convertUnderscore = yes;
}

void  QmmpUiSettings::setConvertTwenty(bool yes)
{
    m_convertTwenty = yes;
}

void QmmpUiSettings::setGroupFormat(const QString &groupFormat)
{
    if(groupFormat != m_group_format)
    {
        m_group_format = groupFormat;
        m_helper->setGroupFormat(m_group_format);
        foreach(PlayListModel *model, PlayListManager::instance()->playLists())
        {
            model->rebuildGroups();
        }
    }
}

void QmmpUiSettings::setUseMetadata(bool yes)
{
    m_useMetadata = yes;
}

bool QmmpUiSettings::resumeOnStartup() const
{
    return m_resume_on_startup;
}

void QmmpUiSettings::setResumeOnStartup(bool enabled)
{
    m_resume_on_startup = enabled;
}

void QmmpUiSettings::setUseClipboard(bool enabled)
{
    m_use_clipboard = enabled;
}

bool QmmpUiSettings::useClipboard() const
{
    return m_use_clipboard;
}

void QmmpUiSettings::sync()
{
    qDebug("%s", Q_FUNC_INFO);
    QSettings s(Qmmp::configFile(), QSettings::IniFormat);
    s.setValue("PlayList/group_format", m_group_format);
    s.setValue("PlayList/convert_underscore", m_convertUnderscore);
    s.setValue("PlayList/convert_twenty", m_convertTwenty);
    s.setValue("PlayList/load_metadata", m_useMetadata);
    s.setValue("PlayList/autosave", m_autosave_playlist);
    s.setValue("PlayList/repeate_list", m_repeate_list);
    s.setValue("PlayList/shuffle", m_shuffle);
    s.setValue("PlayList/groups", m_groups_enabled);
    s.setValue("PlayList/repeate_track", m_repeat_track);
    s.setValue("PlayList/no_advance", m_no_pl_advance);
    s.setValue("PlayList/clear_previous", m_clear_prev_playlist);
    s.setValue("General/resume_on_startup", m_resume_on_startup);
    s.setValue("General/restrict_filters", m_restrict_filters);
    s.setValue("General/exclude_filters", m_exclude_filters);
    s.setValue("General/use_default_pl", m_use_default_pl);
    s.setValue("General/default_pl_name", m_default_pl_name);
    s.setValue("URLDialog/use_clipboard", m_use_clipboard);
}

void QmmpUiSettings::setRepeatableList(bool r)
{
    if(m_repeate_list == r)
        return;
    m_repeate_list = r;
    m_timer->start();
    emit repeatableListChanged(r);
}

void QmmpUiSettings::setShuffle(bool s)
{
    if(m_shuffle == s)
        return;
    m_shuffle = s;
    m_timer->start();
    emit shuffleChanged(s);
}

void QmmpUiSettings::setGroupsEnabled(bool enabled)
{
    if(m_groups_enabled == enabled)
        return;
    m_groups_enabled = enabled;
    m_timer->start();
    emit groupsChanged(enabled);
}

void QmmpUiSettings::setRepeatableTrack(bool enabled)
{
    if(m_repeat_track == enabled)
        return;
    m_repeat_track = enabled;
    m_timer->start();
    emit repeatableTrackChanged(enabled);
}

void QmmpUiSettings::setNoPlayListAdvance(bool enabled)
{
    if(m_no_pl_advance == enabled)
        return;
    m_no_pl_advance = enabled;
    m_timer->start();
    emit noPlayListAdvanceChanged(enabled);
}

QStringList QmmpUiSettings::restrictFilters() const
{
    return m_restrict_filters;
}

void QmmpUiSettings::setRestrictFilters(const QString &filters)
{
    m_restrict_filters = filters.trimmed().split(",", QString::SkipEmptyParts);
    m_timer->start();
}

QStringList QmmpUiSettings::excludeFilters() const
{
    return m_exclude_filters;
}

void QmmpUiSettings::setExcludeFilters(const QString &filters)
{
    m_exclude_filters = filters.trimmed().split(",", QString::SkipEmptyParts);
    m_timer->start();
}

bool QmmpUiSettings::useDefaultPlayList() const
{
    return m_use_default_pl;
}

QString QmmpUiSettings::defaultPlayListName() const
{
    return m_default_pl_name;
}

QmmpUiSettings *QmmpUiSettings::instance()
{
    if(!m_instance)
        return new QmmpUiSettings(qApp);
    return m_instance;
}

void QmmpUiSettings::setDefaultPlayList(const QString &name, bool enabled)
{
    m_use_default_pl = enabled;
    m_default_pl_name = name;
    m_timer->start();
}

void QmmpUiSettings::setAutoSavePlayList(bool enabled)
{
    m_autosave_playlist = enabled;
    m_timer->start();
}

bool QmmpUiSettings::autoSavePlayList() const
{
    return m_autosave_playlist;
}

void QmmpUiSettings::setClearPreviousPlayList(bool enabled)
{
    m_clear_prev_playlist = enabled;
    m_timer->start();
}

bool QmmpUiSettings::clearPreviousPlayList() const
{
    return m_clear_prev_playlist;
}
