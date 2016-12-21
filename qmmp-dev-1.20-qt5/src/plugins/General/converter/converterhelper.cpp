/***************************************************************************
 *   Copyright (C) 2011-2016 by Ilya Kotov                                 *
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
#include <QApplication>
#include <qmmp/soundcore.h>
#include <qmmpui/uihelper.h>
#include <qmmpui/playlistmanager.h>
#include <qmmpui/playlistitem.h>
#include <qmmpui/mediaplayer.h>
#include "converterdialog.h"
#include "converterhelper.h"

ConverterHelper::ConverterHelper(QObject *parent) : QObject(parent)
{
    m_action = new QAction(tr("Convert"), this);
    m_action->setShortcut(tr("Meta+C"));
    UiHelper::instance()->addAction(m_action, UiHelper::PLAYLIST_MENU);
    connect (m_action, SIGNAL(triggered ()), SLOT(openConverter()));

}

ConverterHelper::~ConverterHelper()
{}

void ConverterHelper::openConverter()
{
    PlayListManager *pl_manager = MediaPlayer::instance()->playListManager();
    QList <PlayListTrack *> tracks = pl_manager->selectedPlayList()->selectedTracks();
    if (tracks.isEmpty())
        return;

    ConverterDialog d(tracks, qApp->activeWindow ());
    d.exec();
}
