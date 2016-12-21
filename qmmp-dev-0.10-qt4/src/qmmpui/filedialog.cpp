/**************************************************************************
*   Copyright (C) 2008-2016 by Ilya Kotov                                 *
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
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QPluginLoader>
#include <QMetaObject>
#include <QLibrary>
#include <qmmp/qmmp.h>
#include "qmmpuiplugincache_p.h"
#include "filedialog.h"
#include "qtfiledialog_p.h"

//static functions
FileDialog* FileDialog::m_instance = 0;
QList<QmmpUiPluginCache*> *FileDialog::m_cache = 0;
FileDialogFactory *FileDialog::m_currentFactory = 0;

void FileDialog::loadPlugins()
{
    if (m_cache)
        return;

    m_cache = new QList<QmmpUiPluginCache*>;
    m_cache->append(new QmmpUiPluginCache(new QtFileDialogFactory));

    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    QDir pluginsDir (Qmmp::pluginsPath());
    pluginsDir.cd("FileDialogs");
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

QList<FileDialogFactory *> FileDialog::factories()
{
    loadPlugins();
    QList<FileDialogFactory *> list;
    foreach (QmmpUiPluginCache *item, *m_cache)
    {
        if(item->fileDialogFactory())
            list.append(item->fileDialogFactory());
    }
    return list;
}

void FileDialog::setEnabled(FileDialogFactory *factory)
{
    loadPlugins();
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("FileDialog", factory->properties().shortName);
}

bool FileDialog::isEnabled(FileDialogFactory *factory)
{
    loadPlugins();
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    QString name = settings.value("FileDialog", "qt_dialog").toString();
    return factory->properties().shortName == name;
}

QString FileDialog::file(FileDialogFactory *factory)
{
    loadPlugins();
    foreach(QmmpUiPluginCache *item, *m_cache)
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

QString FileDialog::getExistingDirectory(QWidget *parent,
        const QString &caption,
        const QString &dir)
{
    QStringList l = instance()->exec(parent, dir, FileDialog::AddDir, caption);
    return l.isEmpty() ? QString() : l.at(0);
}

QString FileDialog::getOpenFileName(QWidget *parent,
                                    const QString &caption,
                                    const QString &dir,
                                    const QString &filter,
                                    QString *selectedFilter)
{
   QStringList l = instance()->exec(parent, dir, FileDialog::AddFile, caption, filter, selectedFilter);
   return l.isEmpty() ? QString() : l.at(0);
}

QStringList FileDialog::getOpenFileNames(QWidget *parent, const QString &caption,
        const QString &dir,const QString &filter,
        QString *selectedFilter)
{
    return instance()->exec(parent, dir, FileDialog::AddFiles, caption, filter, selectedFilter);
}

QString FileDialog::getSaveFileName (QWidget *parent, const QString &caption,
                                     const QString& dir, const QString &filter,
                                     QString *selectedFilter)
{
    QStringList l = instance()->exec(parent, dir, FileDialog::SaveFile, caption, filter, selectedFilter);
    return l.isEmpty() ? QString() : l.at(0);
}

void FileDialog::popup(QWidget *parent,
                       Mode m,
                       QString *dir,
                       QObject *receiver,
                       const char *member,
                       const QString &caption,
                       const QString &filters)

{
    if (!dir)
        qFatal("FileDialog: empty last dir pointer");
    FileDialog* inst = instance();
    inst->setParent(parent);
    inst->init(receiver, member, dir);
    if (!m_currentFactory->properties().modal)
        inst->raise(*dir, m, caption, filters.split(";;"));
    else
    {
        QStringList files;
        if (m == AddFiles || m == AddFile || m == AddDirsFiles || m == PlayDirsFiles)
        {
            QString selectedFilter;
            files = getOpenFileNames(parent, caption, *dir, filters, &selectedFilter);
        }
        else if (m == AddDirs || m == AddDir)
        {
            QString path = getExistingDirectory(parent, caption, *dir);
            if (!path.isEmpty())
                files << path;
        }
        QMetaObject::invokeMethod(inst, "filesSelected", Q_ARG(QStringList, files));
    }
}

FileDialog* FileDialog::instance()
{
    loadPlugins();
    FileDialogFactory *selected = 0;

    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    QString name = settings.value("FileDialog", "qt_dialog").toString();
    foreach(QmmpUiPluginCache *item, *m_cache)
    {
        if(item->shortName() == name)
        {
            selected = item->fileDialogFactory();
            break;
        }
    }

    if(!selected)
        selected = m_cache->at(0)->fileDialogFactory();

    if(selected == m_currentFactory && m_instance)
        return m_instance;

    if(m_instance)
    {
        delete m_instance;
        m_instance = 0;
    }

    m_currentFactory = selected;
    m_instance = m_currentFactory->create();
    return m_instance;
}

//base implementation
FileDialog::FileDialog() : QObject(), m_initialized(false)
{
    m_lastDir = 0;
}

FileDialog::~FileDialog() {}

void FileDialog::raise(const QString &dir, Mode mode, const QString &caption, const QStringList &mask)
{
    Q_UNUSED(dir);
    Q_UNUSED(mode);
    Q_UNUSED(caption);
    Q_UNUSED(mask);
}

void FileDialog::init(QObject* receiver, const char* member, QString *dir)
{
    m_lastDir = dir;
    if (m_initialized)
        disconnect();
    if (receiver &&  member)
    {
        connect(this,SIGNAL(filesSelected(QStringList, bool)), receiver, member);
        connect(this,SIGNAL(filesSelected(QStringList)), SLOT(updateLastDir(QStringList)));
        m_initialized = true;
    }
}

void FileDialog::updateLastDir(const QStringList& list)
{
    if (!list.isEmpty() && m_lastDir)
    {
        QString path = list[0];
        if (path.endsWith('/'))
            path.remove(path.size()-1, 1);
        *m_lastDir = path.left(path.lastIndexOf ('/'));
    }
}
