/***************************************************************************
 *   Copyright (C) 2006-2009 by Ilya Kotov                                 *
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

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include "playlistcontrol.h"
#include "skin.h"

PlaylistControl::PlaylistControl(QWidget* parent) : PixmapWidget(parent)
{
	m_skin = Skin::instance();
    setPixmap(m_skin->getPlPart(Skin::PL_CONTROL));
    m_ratio = m_skin->ratio();
    connect(m_skin, SIGNAL(skinChanged()), SLOT(updateSkin()));
}

void PlaylistControl::mouseReleaseEvent(QMouseEvent *me)
{
	QPoint pt = me->pos();
    if(QRect(4*m_ratio,m_ratio,7*m_ratio,7*m_ratio).contains(pt))
		emit previousClicked();
    else if(QRect(12*m_ratio,m_ratio,7*m_ratio,7*m_ratio).contains(pt))
		emit playClicked();
    else if(QRect(21*m_ratio,m_ratio,7*m_ratio,7*m_ratio).contains(pt))
		emit pauseClicked();
    else if(QRect(31*m_ratio,m_ratio,7*m_ratio,7*m_ratio).contains(pt))
		emit stopClicked();
    else if(QRect(40*m_ratio,m_ratio,7*m_ratio,7*m_ratio).contains(pt))
		emit nextClicked();
    else if(QRect(49*m_ratio,m_ratio,7*m_ratio,7*m_ratio).contains(pt))
		emit ejectClicked();
}

void PlaylistControl::updateSkin()
{
    setCursor(m_skin->getCursor(Skin::CUR_PNORMAL));
    setPixmap(m_skin->getPlPart(Skin::PL_CONTROL));
    m_ratio = m_skin->ratio();
}
