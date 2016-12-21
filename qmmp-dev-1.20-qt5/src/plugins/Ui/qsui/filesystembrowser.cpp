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

#include <QFileSystemModel>
#include <QListView>
#include <QVBoxLayout>
#include <QSettings>
#include <QAction>
#include <QApplication>
#include <qmmp/metadatamanager.h>
#include <qmmpui/playlistmanager.h>
#include <qmmpui/filedialog.h>
#include <qmmpui/playlistmanager.h>
#include <qmmp/qmmp.h>
#include "elidinglabel.h"
#include "filesystembrowser.h"

FileSystemBrowser::FileSystemBrowser(QWidget *parent) :
    QWidget(parent)
{
    m_update = false;

    m_listView = new QListView(this);
    m_listView->setFrameStyle(QFrame::NoFrame);
    m_listView->setDragEnabled(true);
    m_listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(m_listView, SIGNAL(activated(QModelIndex)), SLOT(onListViewActivated(QModelIndex)));

    m_label = new Utils::ElidingLabel(this);
    m_label->setContentsMargins(5,5,5,0);
    m_label->setMargin(0);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_label);
    layout->addWidget(m_listView);
    setLayout(layout);

    m_model = new QFileSystemModel(this);
    m_model->setReadOnly(true);
    m_model->setNameFilterDisables(false);
    m_model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDot);
    m_listView->setModel(m_model);

    setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction *addToPlaylistAction = new QAction(tr("Add to Playlist"), this);
    connect(addToPlaylistAction, SIGNAL(triggered()), SLOT(addToPlayList()));
    addAction(addToPlaylistAction);
    QAction *selectDirAction = new QAction(tr("Change Directory"), this);
    connect(selectDirAction, SIGNAL(triggered()), SLOT(selectDirectory()));
    addAction(selectDirAction);

    readSettings();
}

FileSystemBrowser::~FileSystemBrowser()
{
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Simple");
    settings.setValue("fsbrowser_current_dir", m_model->rootDirectory().canonicalPath());
    settings.endGroup();
}

void FileSystemBrowser::readSettings()
{
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Simple");
    if(!m_update)
    {
        m_update = true;
        setCurrentDirectory(settings.value("fsbrowser_current_dir",
                                           QDir::homePath()).toString());
    }
    settings.endGroup();
    m_model->setNameFilters(MetaDataManager::instance()->nameFilters());
}

void FileSystemBrowser::onListViewActivated(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    QString name = m_model->fileName(index);

    if(name == "..")
    {
        setCurrentDirectory(m_model->fileInfo(index).absoluteFilePath());
    }
    else if(m_model->isDir(index))
    {
        QFileInfo info = m_model->fileInfo(index);
        if(info.isExecutable() && info.isReadable())
            setCurrentDirectory(m_model->filePath(index));
    }
}

void FileSystemBrowser::addToPlayList()
{
    foreach (QModelIndex index, m_listView->selectionModel()->selectedIndexes())
    {
        if(!index.isValid())
            continue;

        QString name = m_model->fileName(index);
        if(name == "..")
            continue;
        PlayListManager::instance()->selectedPlayList()->add(m_model->filePath(index));
    }
}

void FileSystemBrowser::selectDirectory()
{
    QString dir = FileDialog::getExistingDirectory(qApp->activeWindow(),
                                                   tr("Select Directory"), m_model->rootDirectory().canonicalPath());
    if(!dir.isEmpty())
        setCurrentDirectory(dir);
}

void FileSystemBrowser::setCurrentDirectory(const QString &path)
{
    if(path.isEmpty())
        return;

    QModelIndex index = m_model->setRootPath(QDir(path).exists() ? path : QDir::homePath());
    if(index.isValid())
    {
        m_listView->setRootIndex(index);
        m_label->setText(QDir(m_model->rootPath()).dirName());
    }
    else
        m_label->clear();
}
