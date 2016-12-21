/***************************************************************************
 *   Copyright (C) 2006-2012 by Ilya Kotov                                 *
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
#include "balancebar.h"

BalanceBar::BalanceBar(QWidget *parent)
        : PixmapWidget(parent)
{
    m_skin = Skin::instance();
    connect(m_skin, SIGNAL(skinChanged()), this, SLOT(updateSkin()));
    setPixmap(m_skin->getBalanceBar(0));
    m_moving = false;
    m_min = -100;
    m_max = 100;
    m_old = m_value = 0;
    draw(false);
}


BalanceBar::~BalanceBar()
{}

void BalanceBar::mousePressEvent(QMouseEvent *e)
{
    m_moving = true;
    press_pos = e->x();
    if(e->button() == Qt::MidButton)
    {
        m_value = 0;
        emit sliderPressed();
        emit sliderMoved(m_value);
    }
    else if(m_pos<e->x() && e->x()<m_pos+11*m_skin->ratio())
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
        {
            emit sliderMoved(m_value);
        }
    }
    draw();
}

void BalanceBar::mouseMoveEvent (QMouseEvent *e)
{
    if(m_moving)
    {
        int po = e->x();
        po = po - press_pos;

        if(0 <= po && po <= width()-13*m_skin->ratio())
        {
            m_value = convert(po);
            draw();
            emit sliderMoved(m_value);
        }
    }
}

void BalanceBar::mouseReleaseEvent(QMouseEvent*)
{
    m_moving = false;
    draw(false);
    m_old = m_value;
    emit sliderReleased();
}

void BalanceBar::setValue(int v)
{
    if (m_moving || m_max == 0)
        return;
    m_value = v;
    draw(false);
}

void BalanceBar::setMax(int max)
{
    m_max = max;
    draw(false);
}

void BalanceBar::updateSkin()
{
    resize(m_skin->getBalanceBar(0).size());
    draw(false);
}

void BalanceBar::draw(bool pressed)
{
    if(abs(m_value)<6)
        m_value = 0;
    int p=int(ceil(double(m_value-m_min)*(width()-13*m_skin->ratio())/(m_max-m_min)));
    m_pixmap = m_skin->getBalanceBar(abs(27*m_value/m_max));
    QPainter paint(&m_pixmap);
    if(pressed)
        paint.drawPixmap(p,m_skin->ratio(),m_skin->getButton(Skin::BT_BAL_P));
    else
        paint.drawPixmap(p,m_skin->ratio(),m_skin->getButton(Skin::BT_BAL_N));
    setPixmap(m_pixmap);
    m_pos = p;
}

int BalanceBar::convert(int p)
{
    return int(ceil(double(m_max-m_min)*(p)/(width()-13*m_skin->ratio())+m_min));
}
