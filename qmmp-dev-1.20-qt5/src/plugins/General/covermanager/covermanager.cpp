/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
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
#include <QMessageBox>
#include <qmmpui/uihelper.h>
#include <qmmpui/playlistmodel.h>
#include <qmmpui/playlistitem.h>
#include <qmmpui/mediaplayer.h>
#include <qmmpui/metadataformatter.h>
#include <qmmp/metadatamanager.h>
#include "coverwidget.h"
#include "covermanager.h"

CoverManager::CoverManager(QObject *parent) : QObject(parent)
{
    m_action = new QAction(tr("Show Cover"), this);
    m_action->setShortcut(tr("Ctrl+M"));
    UiHelper::instance()->addAction(m_action, UiHelper::PLAYLIST_MENU);
    connect (m_action, SIGNAL(triggered ()), SLOT(showWindow()));
}

void CoverManager::showWindow()
{
    QList <PlayListTrack *> tracks = MediaPlayer::instance()->playListManager()->selectedPlayList()->selectedTracks();
    if (!tracks.isEmpty())
    {
        CoverWidget *w = new CoverWidget(qApp->activeWindow ());
        QPixmap pix = MetaDataManager::instance()->getCover(tracks.at(0)->url());
        if(pix.isNull())
            pix = QPixmap(":/cm_no_cover.png");
        w->setPixmap(pix);
        MetaDataFormatter formatter("%p%if(%p&%t, - ,)%if(%t,%t,%f)");
        w->setWindowTitle(formatter.format(tracks.at(0)));
        w->show();
    }
}
