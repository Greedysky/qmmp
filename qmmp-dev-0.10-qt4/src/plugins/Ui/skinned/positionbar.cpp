/***************************************************************************
 *   Copyright (C) 2009-2016 by Ilya Kotov                                 *
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
#include <QTimer>
#include <math.h>
#include "skin.h"
#include "button.h"
#include "positionbar.h"

PositionBar::PositionBar(QWidget *parent) : PixmapWidget(parent)
{
    m_skin = Skin::instance();
    connect(m_skin, SIGNAL(skinChanged()), this, SLOT(updateSkin()));
    setPixmap(m_skin->getPosBar());
    m_moving = false;
    m_min = 0;
    m_max = 0;
    m_old = m_value = 0;
    draw(false);
    setCursor(m_skin->getCursor(Skin::CUR_POSBAR));
}


PositionBar::~PositionBar()
{}

void PositionBar::mousePressEvent(QMouseEvent *e)
{
    if(m_max <= 0)
        return;
    m_moving = true;
    press_pos = e->x();
    if (m_pos<e->x() && e->x()<m_pos+29*m_skin->ratio())
    {
        press_pos = e->x()-m_pos;
        emit sliderPressed();
    }
    else
    {
        m_value = convert(qMax(qMin(width()-30*m_skin->ratio(),e->x()-15*m_skin->ratio()),0));
        press_pos = 15*m_skin->ratio();
        emit sliderPressed();
        if (m_value!=m_old)
        {
            emit sliderMoved(m_value);

        }
    }
    draw();
}

void PositionBar::mouseMoveEvent (QMouseEvent *e)
{
    if (m_moving)
    {
        qint64 po = e->x();
        po = po - press_pos;

        if (0<=po && po<=width()-30*m_skin->ratio())
        {
            m_value = convert(po);
            draw();
            emit sliderMoved(m_value);
        }
    }
}

void PositionBar::wheelEvent(QWheelEvent *e)
{
    if(m_max == 0)
        return;

    if(e->delta() > 0)
        m_value += 5000;
    else
        m_value -= 5000;

    m_value = qBound(qint64(0), m_value, m_max);
    draw();
    emit sliderReleased();
}

void PositionBar::mouseReleaseEvent(QMouseEvent*)
{
    draw(false);
    if (m_value != m_old && m_max > 0)
        m_old = m_value;
    m_moving = false;
    if(m_max > 0)
        emit sliderReleased();
}

void PositionBar::setValue(qint64 v)
{
    if (m_moving || m_max == 0)
        return;
    m_value = v;
    draw(false);
}

void PositionBar::setMaximum(qint64 max)
{
    m_max = max;
    draw(false);
}

void PositionBar::updateSkin()
{
    resize(m_skin->getPosBar().size());
    draw(false);
    setCursor(m_skin->getCursor(Skin::CUR_POSBAR));
}

void PositionBar::draw(bool pressed)
{
    qint64 p=qint64(ceil(double(m_value-m_min)*(width()-30*m_skin->ratio())/(m_max-m_min)));
    m_pixmap = m_skin->getPosBar();
    if (m_max > 0)
    {
        QPainter paint(&m_pixmap);
        if (pressed)
            paint.drawPixmap(p,0,m_skin->getButton(Skin::BT_POSBAR_P));
        else
            paint.drawPixmap(p,0,m_skin->getButton(Skin::BT_POSBAR_N));
    }
    setPixmap(m_pixmap);
    m_pos = p;
}

qint64 PositionBar::convert(qint64 p)
{
    return qint64(ceil(double(m_max-m_min)*(p)/(width()-30*m_skin->ratio())+m_min));
}
