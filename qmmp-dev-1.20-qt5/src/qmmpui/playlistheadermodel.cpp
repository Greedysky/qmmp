/***************************************************************************
 *   Copyright (C) 2015 by Ilya Kotov                                      *
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
#include <QMetaObject>
#include <qmmp/qmmp.h>
#include <qmmpui/playlistmanager.h>
#include "columneditor_p.h"
#include "metadatahelper_p.h"
#include "playlistheadermodel.h"

PlayListHeaderModel::PlayListHeaderModel(QObject *parent) :
    QObject(parent)
{
    m_helper = MetaDataHelper::instance();
    m_settings_loaded = false;

    ColumnHeader col;
    col.name = tr("Artist - Title");
    col.pattern = "%if(%p,%p - %t,%t)";
    m_columns.append(col);
    m_helper->setTitleFormats(QStringList() << col.pattern);
}

PlayListHeaderModel::~PlayListHeaderModel()
{
    m_columns.clear();
}

void PlayListHeaderModel::restoreSettings(const QString &groupName)
{
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup(groupName);
    restoreSettings(&settings);
    settings.endGroup();
}

void PlayListHeaderModel::restoreSettings(QSettings *settings)
{
    QStringList names = settings->value("pl_column_names").toStringList();
    QStringList patterns = settings->value("pl_column_patterns").toStringList();

    if(!names.isEmpty() && (names.count() == patterns.count()))
    {
        m_columns.clear();
        for(int i = 0; i < names.count(); ++i)
        {
            ColumnHeader h = {names.at(i), patterns.at(i), QHash<int, QVariant>()};
            m_columns.append(h);
        }
        m_helper->setTitleFormats(patterns);
    }
    m_settings_loaded = true;
}

void PlayListHeaderModel::saveSettings(const QString &groupName)
{
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup(groupName);
    saveSettings(&settings);
    settings.endGroup();
}

void PlayListHeaderModel::saveSettings(QSettings *settings)
{
    QStringList names, patterns;
    for(int i = 0; i < m_columns.count(); ++i)
    {
        names << m_columns[i].name;
        patterns << m_columns[i].pattern;
    }

    settings->setValue("pl_column_names", names);
    settings->setValue("pl_column_patterns", patterns);
}

bool PlayListHeaderModel::isSettingsLoaded() const
{
    return m_settings_loaded;
}

void PlayListHeaderModel::insert(int index, const QString &name, const QString &pattern)
{
    if(index < 0 || index > m_columns.size())
    {
        qWarning("ColumnManager: index is out of range");
        return;
    }

    ColumnHeader col;
    col.name = name;
    col.pattern = pattern;
    m_columns.insert(index, col);
    emit columnAdded(index);
    emit headerChanged();
    updatePlayLists();
}

void PlayListHeaderModel::remove(int index)
{
    if(index < 0 || index >= m_columns.size())
    {
        qWarning("ColumnManager: index is out of range");
        return;
    }

    if(m_columns.count() == 1)
        return;

    m_columns.takeAt(index);
    emit columnRemoved(index);
    emit headerChanged();
    updatePlayLists();
}

void PlayListHeaderModel::move(int from, int to)
{
    if(from < 0 || from >= m_columns.size())
    {
        qWarning("ColumnManager: index is out of range");
        return;
    }

    if(to < 0 || to >= m_columns.size())
    {
        qWarning("ColumnManager: index is out of range");
        return;
    }

    m_columns.move(from, to);
    emit columnMoved(from, to);
    emit headerChanged();
    updatePlayLists();
}

void PlayListHeaderModel::execEdit(int index, QWidget *parent)
{
    if(index < 0 || index >= m_columns.size())
    {
        qWarning("ColumnManager: index is out of range");
        return;
    }

    if(!parent)
        parent = qApp->activeWindow();

    ColumnEditor editor(m_columns[index].name, m_columns[index].pattern, parent);
    if(editor.exec() == QDialog::Accepted)
    {
        m_columns[index].name = editor.name();
        m_columns[index].pattern = editor.pattern();
        emit columnChanged(index);
        emit headerChanged();
        updatePlayLists();
    }
}

void PlayListHeaderModel::execInsert(int index, QWidget *parent)
{
    if(index < 0 || index > m_columns.size())
    {
        qWarning("ColumnManager: index is out of range");
        return;
    }

    if(!parent)
        parent = qApp->activeWindow();

    ColumnEditor editor(tr("Title"),"%t",parent);
    editor.setWindowTitle(tr("Add Column"));
    if(editor.exec() == QDialog::Accepted)
        insert(index, editor.name(), editor.pattern());
}

int PlayListHeaderModel::count()
{
    return m_columns.count();
}

const QString PlayListHeaderModel::name(int index) const
{
    if(index < 0 || index >= m_columns.size())
    {
        qWarning("ColumnManager: index is out of range");
        return QString();
    }
    return m_columns[index].name;
}
const QString PlayListHeaderModel::pattern(int index) const
{
    if(index < 0 || index >= m_columns.size())
    {
        qWarning("ColumnManager: index is out of range");
        return QString();
    }
    return m_columns[index].pattern;
}

void PlayListHeaderModel::setData(int index, int key, const QVariant &data)
{
    if(index < 0 || index >= m_columns.size())
    {
        qWarning("ColumnManager: index is out of range");
        return;
    }
    m_columns[index].data.insert(key, data);
}

const QVariant PlayListHeaderModel::data(int index, int key) const
{
    if(index < 0 || index >= m_columns.size())
    {
        qWarning("ColumnManager: index is out of range");
        return QString();
    }
    return m_columns[index].data.value(key);
}

void PlayListHeaderModel::updatePlayLists()
{
    QStringList patterns;
    for(int i = 0; i < m_columns.count(); ++i)
        patterns.append(m_columns[i].pattern);
    m_helper->setTitleFormats(patterns);

    foreach(PlayListModel *model, PlayListManager::instance()->playLists())
    {
         QMetaObject::invokeMethod(model, "listChanged", Q_ARG(int, PlayListModel::METADATA));
    }
}
