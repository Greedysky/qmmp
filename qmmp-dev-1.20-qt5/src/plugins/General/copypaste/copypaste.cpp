/***************************************************************************
 *   Copyright (C) 2013 by Ilya Kotov                                      *
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

#include <QAction>
#include <QSettings>
#include <QApplication>
#include <QSignalMapper>
#include <QProgressDialog>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <qmmp/soundcore.h>
#include <qmmpui/uihelper.h>
#include <qmmpui/playlistmodel.h>
#include <qmmpui/playlistmanager.h>
#include <qmmpui/playlistitem.h>
#include <qmmpui/mediaplayer.h>
#include <qmmpui/metadataformatter.h>
#include "copypaste.h"

CopyPaste::CopyPaste(QObject *parent) : QObject(parent)
{
    m_pl_manager = PlayListManager::instance();
    //actions
    QAction *cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setShortcut(tr("Ctrl+X"));
    QAction *copyAction = new QAction(tr("&Copy"), this);
    copyAction->setShortcut(tr("Ctrl+C"));
    QAction *pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    //register all actions
    connect(cutAction, SIGNAL(triggered()), SLOT(cut()));
    connect(copyAction, SIGNAL(triggered()), SLOT(copy()));
    connect(pasteAction, SIGNAL(triggered()), SLOT(paste()));
    UiHelper::instance()->addAction(cutAction, UiHelper::PLAYLIST_MENU);
    UiHelper::instance()->addAction(copyAction, UiHelper::PLAYLIST_MENU);
    UiHelper::instance()->addAction(pasteAction, UiHelper::PLAYLIST_MENU);
}

CopyPaste::~CopyPaste()
{
    qDeleteAll(m_buffer);
    m_buffer.clear();
}

void CopyPaste::cut()
{
    qDebug("%s", Q_FUNC_INFO);
    qDeleteAll(m_buffer);
    m_buffer.clear();
    foreach(PlayListTrack *track,  m_pl_manager->selectedPlayList()->selectedTracks())
    {
        m_buffer.append(new PlayListTrack(*track));
    }
    m_pl_manager->selectedPlayList()->removeSelected();
}

void CopyPaste::copy()
{
    qDebug("%s", Q_FUNC_INFO);
    qDeleteAll(m_buffer);
    m_buffer.clear();
    foreach(PlayListTrack *track,  m_pl_manager->selectedPlayList()->selectedTracks())
    {
        m_buffer.append(new PlayListTrack(*track));
    }
}

void CopyPaste::paste()
{
    qDebug("%s", Q_FUNC_INFO);
    foreach(PlayListTrack *track,  m_buffer)
    {
        m_pl_manager->selectedPlayList()->add(new PlayListTrack(*track));
    }
}
