/***************************************************************************
 *   Copyright (C) 2006-2013 by Ilya Kotov                                 *
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
#include <QPainter>
#include <math.h>
#include "skin.h"
#include "button.h"
#include "mainwindow.h"
#include "volumebar.h"


VolumeBar::VolumeBar(QWidget *parent) : PixmapWidget(parent)
{
    m_skin = Skin::instance();
    connect(m_skin, SIGNAL(skinChanged()), this, SLOT(updateSkin()));
    setPixmap(m_skin->getVolumeBar(0));
    m_moving = false;
    m_min = 0;
    m_max = 100;
    m_old = m_value = 0;
    draw(false);
}

VolumeBar::~VolumeBar()
{}

void VolumeBar::mousePressEvent(QMouseEvent *e)
{
    m_moving = true;
    press_pos = e->x();
    if(m_pos<e->x() && e->x()<m_pos+11*m_skin->ratio())
    {
        press_pos = e->x()-m_pos;
        emit sliderPressed();
    }
    else
    {
        m_value = convert(qMax(qMin(width()-18*m_skin->ratio(),e->x()-6*m_skin->ratio()),0));
        press_pos = 6*m_skin->ratio();
        emit sliderPressed();
        if (m_value != m_old)
            emit sliderMoved(m_value);
    }
    draw();
}

void VolumeBar::mouseMoveEvent (QMouseEvent *e)
{
    if(m_moving)
    {
        int po = e->x();
        po = po - press_pos;

        if(0<=po && po<=width()-18*m_skin->ratio())
        {
            m_value = convert(po);
            draw();
            emit sliderMoved(m_value);
        }
    }
}

void VolumeBar::mouseReleaseEvent(QMouseEvent*)
{
    m_moving = false;
    draw(false);
    m_old = m_value;
    emit sliderReleased();
}

void VolumeBar::setValue(int v)
{
    if (m_moving || m_max == 0)
        return;
    m_value = v;
    draw(false);
}

void VolumeBar::setMax(int max)
{
    m_max = max;
    draw(false);
}

void VolumeBar::updateSkin()
{
    resize(m_skin->getVolumeBar(0).size());
    draw(false);
    setCursor(m_skin->getCursor(Skin::CUR_VOLBAL));
}

void VolumeBar::draw(bool pressed)
{
    int p=int(ceil(double(m_value-m_min)*(width()-18*m_skin->ratio())/(m_max-m_min)));
    m_pixmap = m_skin->getVolumeBar(27*(m_value-m_min)/(m_max-m_min));
    QPainter paint(&m_pixmap);
    if(pressed)
        paint.drawPixmap(p,1,m_skin->getButton(Skin::BT_VOL_P));
    else
        paint.drawPixmap(p,1,m_skin->getButton(Skin::BT_VOL_N));
    setPixmap(m_pixmap);
    m_pos = p;
}

int VolumeBar::convert(int p)
{
    return int(ceil(double(m_max-m_min)*(p)/(width()-18*m_skin->ratio())+m_min));
}
