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
#include <qmmp/soundcore.h>
#include <qmmpui/uihelper.h>
#include <qmmpui/playlistmanager.h>
#include <qmmpui/playlistitem.h>
#include <qmmpui/mediaplayer.h>
#include "rgscandialog.h"
#include "rgscanhelper.h"

RGScanHelper::RGScanHelper(QObject *parent) : QObject(parent)
{
    QAction *action = new QAction(tr("ReplayGain Scanner"), this);
    action->setShortcut(tr("Meta+R"));
    UiHelper::instance()->addAction(action, UiHelper::PLAYLIST_MENU);
    connect (action, SIGNAL(triggered ()), SLOT(openRGScaner()));
}

RGScanHelper::~RGScanHelper()
{}

void RGScanHelper::openRGScaner()
{
    PlayListManager *pl_manager = MediaPlayer::instance()->playListManager();
    QList <PlayListTrack *> tracks = pl_manager->selectedPlayList()->selectedTracks();
    if (tracks.isEmpty())
        return;

    RGScanDialog *d = new RGScanDialog(tracks, qApp->activeWindow ());
    d->exec();
    d->deleteLater();
}
