/***************************************************************************
 *   Copyright (C) 2009-2014 by Ilya Kotov                                 *
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
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>

#include "coverwidget.h"

CoverWidget::CoverWidget(QWidget *parent)
        : QWidget(parent)
{}

CoverWidget::~CoverWidget()
{}

void CoverWidget::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    update();
}

void CoverWidget::paintEvent (QPaintEvent *p)
{
    QPainter paint(this);
    if(!m_pixmap.isNull())
        paint.drawPixmap(0,0, m_pixmap.scaled(p->rect().size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
