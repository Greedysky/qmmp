/***************************************************************************
 *   Copyright (C) 2012 by Ilya Kotov                                      *
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
#include "streamwindow.h"
#include "streambrowser.h"

StreamBrowser::StreamBrowser(QObject *parent) : QObject(parent)
{
    m_action = new QAction(tr("Stream Browser"), this);
    m_action->setIcon(QIcon::fromTheme("applications-internet"));
    m_action->setShortcut(tr("Ctrl+U"));
    UiHelper::instance()->addAction(m_action, UiHelper::TOOLS_MENU);
    connect (m_action, SIGNAL(triggered ()), SLOT(showStreamWindow()));
}

StreamBrowser::~StreamBrowser()
{}

void StreamBrowser::showStreamWindow()
{
    if(!m_streamWindow)
        m_streamWindow = new StreamWindow(qApp->activeWindow ());
    m_streamWindow->show();
    m_streamWindow->activateWindow();
}
