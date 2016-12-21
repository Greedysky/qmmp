/***************************************************************************
 *   Copyright (C) 2007-2009 by Ilya Kotov                                 *
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

#include <QMouseEvent>
#include "skin.h"
#include "titlebarcontrol.h"

TitleBarControl::TitleBarControl(QWidget *parent) : QWidget(parent)
{
    //setAutoFillBackground(true);
    m_ratio = Skin::instance()->ratio();
    resize(m_ratio*57, m_ratio*10);
    connect(Skin::instance(), SIGNAL(skinChanged()),SLOT(updateSkin()));
}


TitleBarControl::~TitleBarControl()
{
}

void TitleBarControl::mousePressEvent (QMouseEvent *)
{}

void TitleBarControl::mouseReleaseEvent (QMouseEvent * event)
{
    QPoint pt = event->pos();
    if(QRect(0,0,m_ratio*8,m_ratio*10).contains(pt))
        emit previousClicked();
    else if(QRect(m_ratio*8,0,m_ratio*11,m_ratio*10).contains(pt))
        emit playClicked();
    else if(QRect(m_ratio*19,0,m_ratio*10,m_ratio*10).contains(pt))
        emit pauseClicked();
    else if(QRect(m_ratio*29,0,m_ratio*8,m_ratio*10).contains(pt))
        emit stopClicked();
    else if(QRect(m_ratio*37,0,m_ratio*10,m_ratio*10).contains(pt))
        emit nextClicked();
    else if(QRect(m_ratio*47,0,m_ratio*10,m_ratio*10).contains(pt))
        emit ejectClicked();
}

void TitleBarControl::mouseMoveEvent(QMouseEvent*)
{}

void TitleBarControl::updateSkin()
{
    m_ratio = Skin::instance()->ratio();
    resize(m_ratio*57, m_ratio*10);
}
