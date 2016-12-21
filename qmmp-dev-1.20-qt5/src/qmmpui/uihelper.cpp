/***************************************************************************
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

#include <QDialog>
#include <QMenu>
#include <QWidget>
#include <QAction>
#include <QSettings>
#include <qmmp/soundcore.h>
#include <qmmp/metadatamanager.h>
#include <qmmpui/filedialog.h>
#include <qmmpui/playlistparser.h>
#include <qmmpui/playlistmanager.h>
#include <qmmpui/qmmpuisettings.h>
#include "general.h"
#include "generalfactory.h"
#include "jumptotrackdialog_p.h"
#include "aboutdialog_p.h"
#include "addurldialog_p.h"
#include "mediaplayer.h"
#include "uihelper.h"

UiHelper *UiHelper::m_instance = 0;

UiHelper::UiHelper(QObject *parent)
        : QObject(parent)
{
    m_instance = this;
    m_toolsMenu = 0;
    m_playlistMenu = 0;
    m_jumpDialog = 0;
    m_model = 0;
    General::create(parent);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_lastDir = settings.value("General/last_dir", QDir::homePath()).toString(); //last directory
}

UiHelper::~UiHelper()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("General/last_dir",m_lastDir);
}

bool UiHelper::visibilityControl()
{
    GeneralFactory* factory;
    foreach(factory, General::enabledFactories())
    {
        if (factory->properties().visibilityControl)
            return true;
    }
    return false;
}

void UiHelper::addAction(QAction *action, MenuType type)
{
    connect(action, SIGNAL(destroyed (QObject *)), SLOT(removeAction(QObject*)));
    switch ((int) type)
    {
    case TOOLS_MENU:
        if (!m_toolsActions.contains(action))
            m_toolsActions.append(action);
        if (m_toolsMenu && !m_toolsMenu->actions ().contains(action))
            m_toolsMenu->addAction(action);
        break;
    case PLAYLIST_MENU:
        if (!m_playlistActions.contains(action))
            m_playlistActions.append(action);
        if (m_playlistMenu && !m_playlistMenu->actions ().contains(action))
            m_playlistMenu->addAction(action);
    }
}

void UiHelper::removeAction(QAction *action)
{
    m_toolsActions.removeAll(action);
    if (m_toolsMenu)
        m_toolsMenu->removeAction(action);
    m_playlistActions.removeAll(action);
    if (m_playlistMenu)
        m_playlistMenu->removeAction(action);
}

QList<QAction *> UiHelper::actions(MenuType type)
{
    if (type == TOOLS_MENU)
        return m_toolsActions;
    else
        return m_playlistActions;
}

QMenu *UiHelper::createMenu(MenuType type, const QString &title, QWidget *parent)
{
    switch ((int) type)
    {
    case TOOLS_MENU:
        if (!m_toolsMenu)
        {
            m_toolsMenu = new QMenu(title, parent);
            m_toolsMenu->addActions(m_toolsActions);
        }
        else
            m_toolsMenu->setTitle(title);
        return m_toolsMenu;
    case PLAYLIST_MENU:
        if (!m_playlistMenu)
        {
            m_playlistMenu = new QMenu(title, parent);
            m_playlistMenu->addActions(m_playlistActions);
        }
        else
            m_playlistMenu->setTitle(title);
        return m_playlistMenu;
    }
    return 0;
}

void UiHelper::addFiles(QWidget *parent, PlayListModel *model)
{
    QStringList filters;
    filters << tr("All Supported Bitstreams")+" (" +
            MetaDataManager::instance()->nameFilters().join (" ") +")";
    filters << MetaDataManager::instance()->filters();
    m_model = model;
    FileDialog::popup(parent, FileDialog::PlayDirsFiles, &m_lastDir,
                      this, SLOT(addSelectedFiles(QStringList,bool)),
                      tr("Select one or more files to open"), filters.join(";;"));
}

void UiHelper::playFiles(QWidget *parent, PlayListModel *model)
{
    QStringList filters;
    filters << tr("All Supported Bitstreams")+" (" +
            MetaDataManager::instance()->nameFilters().join (" ") +")";
    filters << MetaDataManager::instance()->filters();
    m_model = model;
    FileDialog::popup(parent, FileDialog::AddDirsFiles, &m_lastDir,
                      this, SLOT(playSelectedFiles(QStringList)),
                      tr("Select one or more files to play"), filters.join(";;"));

}

void UiHelper::addDirectory(QWidget *parent, PlayListModel *model)
{
    FileDialog::popup(parent, FileDialog::AddDirs, &m_lastDir,
                      model, SLOT(add(QStringList)),
                      tr("Choose a directory"));
}

void UiHelper::addUrl(QWidget *parent, PlayListModel *model)
{
    AddUrlDialog::popup(parent, model);
}

void UiHelper::loadPlayList(QWidget *parent, PlayListModel *model)
{
    if(PlayListParser::nameFilters().isEmpty())
    {
        qWarning("UiHelper: There is no registered playlist parsers");
        return;
    }

    QString mask = tr("Playlist Files") + " (" + PlayListParser::nameFilters().join(" ") + ")";
    //TODO use nonmodal dialog and multiplier playlists
    QString f_path = FileDialog::getOpenFileName(parent, tr("Open Playlist"), m_lastDir, mask);
    if (!f_path.isEmpty())
    {
        if(QmmpUiSettings::instance()->clearPreviousPlayList())
        {
            model->clear();
            model->setName(QFileInfo(f_path).baseName());
        }
        model->loadPlaylist(f_path);
        m_lastDir = QFileInfo(f_path).absoluteDir().path();
    }
}

void UiHelper::savePlayList(QWidget *parent, PlayListModel *model)
{
    if(PlayListParser::nameFilters().isEmpty())
    {
        qWarning("UiHelper: There is no registered playlist parsers");
        return;
    }
    QString ext = PlayListParser::nameFilters().at(0);
    ext.replace("*.", "."); //extract extension from name filter
    QString mask = tr("Playlist Files") + " (" + PlayListParser::nameFilters().join(" ") + ")";
    QString f_name = FileDialog::getSaveFileName(parent, tr("Save Playlist"),m_lastDir + "/" +
                                                 model->name() + ext,mask);

    if (!f_name.isEmpty())
    {
        model->savePlaylist(f_name);
        m_lastDir = QFileInfo(f_name).absoluteDir().path();
    }
}

void UiHelper::jumpToTrack(QWidget *parent, PlayListModel *model)
{
    if(!m_jumpDialog)
    {
        m_jumpDialog = new JumpToTrackDialog(model, parent);
    }
    if(m_jumpDialog->isHidden())
    {
        m_jumpDialog->show();
        m_jumpDialog->refresh();
    }
    m_jumpDialog->raise();
}

void UiHelper::about(QWidget *parent)
{
    AboutDialog *dialog = new AboutDialog(parent);
    dialog->exec();
    dialog->deleteLater();
}

void UiHelper::toggleVisibility()
{
    emit toggleVisibilityCalled();
}

void UiHelper::showMainWindow()
{
    emit showMainWindowCalled();
}

void UiHelper::exit()
{
    qApp->closeAllWindows();
    qApp->quit();
}

UiHelper* UiHelper::instance()
{
    return m_instance;
}

void UiHelper::removeAction(QObject *action)
{
    removeAction((QAction *) action);
}

void UiHelper::addSelectedFiles(const QStringList &files, bool play)
{
    if(files.isEmpty() || !PlayListManager::instance()->playLists().contains(m_model))
        return;
    if(play)
        playSelectedFiles(files);
    else
        m_model->add(files);
}

void UiHelper::playSelectedFiles(const QStringList &files)
{
    if(files.isEmpty() || !PlayListManager::instance()->playLists().contains(m_model))
        return;
    m_model->clear();
    PlayListManager::instance()->activatePlayList(m_model);
    connect(m_model, SIGNAL(trackAdded(PlayListTrack*)), MediaPlayer::instance(), SLOT(play()));
    connect(m_model, SIGNAL(trackAdded(PlayListTrack*)), SLOT(disconnectPl()));
    m_model->add(files);
}

void UiHelper::disconnectPl()
{
    PlayListModel *model = qobject_cast<PlayListModel*>(sender());
    if(model)
    {
        disconnect(model, SIGNAL(trackAdded(PlayListTrack*)), MediaPlayer::instance(), SLOT(play()));
        disconnect(model, SIGNAL(trackAdded(PlayListTrack*)), this, SLOT(disconnectPl()));
    }
}
