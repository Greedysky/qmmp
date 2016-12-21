/***************************************************************************
 *   Copyright (C) 2011-2014 by Ilya Kotov                                 *
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

#include <QDir>
#include <qmmp/qmmp.h>
#include "qmmpuiplugincache_p.h"
#include "uiloader.h"

QList<QmmpUiPluginCache*> *UiLoader::m_cache = 0;

void UiLoader::loadPlugins()
{
    if (m_cache)
        return;

    m_cache = new QList<QmmpUiPluginCache*>;
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    QDir pluginsDir (Qmmp::pluginsPath());
    pluginsDir.cd("Ui");
    QStringList filters;
    filters << "*.dll" << "*.so";
    foreach (QString fileName, pluginsDir.entryList(filters, QDir::Files))
    {
        QmmpUiPluginCache *item = new QmmpUiPluginCache(pluginsDir.absoluteFilePath(fileName), &settings);
        if(item->hasError())
        {
            delete item;
            continue;
        }
        m_cache->append(item);
    }
}

QList<UiFactory *> UiLoader::factories()
{
    loadPlugins();
    QList<UiFactory *> list;
    foreach (QmmpUiPluginCache *item, *m_cache)
    {
        if(item->uiFactory())
            list.append(item->uiFactory());
    }
    return list;
}

QString UiLoader::file(UiFactory *factory)
{
    loadPlugins();
    foreach(QmmpUiPluginCache *item, *m_cache)
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

void UiLoader::select(UiFactory* factory)
{
    loadPlugins();
    if (!factories().contains(factory))
        return;
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue ("Ui/current_plugin", factory->properties().shortName);
}

UiFactory *UiLoader::selected()
{
    loadPlugins();
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    QString name = settings.value("Ui/current_plugin", "skinned").toString();
    foreach(QmmpUiPluginCache *item, *m_cache)
    {
        if(item->shortName() == name && item->uiFactory())
            return item->uiFactory();
    }
    if (!m_cache->isEmpty())
        return m_cache->at(0)->uiFactory();
    return 0;
}
