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
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <math.h>

#include "skin.h"

#include "eqslider.h"


EqSlider::EqSlider(QWidget *parent): PixmapWidget(parent)
{
    m_skin = Skin::instance();
    connect(m_skin, SIGNAL(skinChanged()), this, SLOT(updateSkin()));
    setPixmap(m_skin->getEqSlider(0));
    m_moving = false;
    m_min = -20;
    m_max = 20;
    m_old = m_value = 0;
    draw(false);
    setCursor(m_skin->getCursor(Skin::CUR_EQSLID));
}

EqSlider::~EqSlider()
{}

void EqSlider::mousePressEvent(QMouseEvent *e)
{
    m_moving = true;
    press_pos = e->y();
    if (e->button() == Qt::MidButton)
    {
        m_value = 0;
        emit sliderMoved(m_value);
        m_old = m_value;
    }
    else if (m_pos<e->y() && e->y()<m_pos+11*m_skin->ratio())
    {
        press_pos = e->y()-m_pos;
    }
    else
    {
        m_value = convert(qMax(qMin(height()-12*m_skin->ratio(),e->y()-6*m_skin->ratio()),0));
        press_pos = 6*m_skin->ratio();
        if (m_value!=m_old)
        {
            emit sliderMoved(m_value);
            m_old = m_value;
        }
    }
    draw();
}

void EqSlider::mouseReleaseEvent(QMouseEvent*)
{
    m_moving = false;
    draw(false);
}

void EqSlider::mouseMoveEvent(QMouseEvent* e)
{
    if (m_moving)
    {
        int po = e->y();
        po = po - press_pos;

        if (0<=po && po<=height()-12*m_skin->ratio())
        {
            m_value = convert(po);
            draw();
            if (m_value!=m_old)
            {

                m_old = m_value;
                //qDebug ("%d",-m_value);
                emit sliderMoved(-m_value);
            }
        }
    }
}

double EqSlider::value()
{
    return - m_value;
}

void EqSlider::setValue(double p)
{
    if (m_moving)
        return;
    m_value = -p;
    draw(false);
}

void EqSlider::setMax(double m)
{
    m_max = m;
    draw(false);
}

void EqSlider::updateSkin()
{
    resize(m_skin->getEqSlider(0).size());
    draw(false);
    setCursor(m_skin->getCursor(Skin::CUR_EQSLID));
}

void EqSlider::draw(bool pressed)
{
    int p=int(ceil(double(m_value-m_min)*(height()-12*m_skin->ratio())/(m_max-m_min)));
    m_pixmap = m_skin->getEqSlider(27-27*(m_value-m_min)/(m_max-m_min));
    QPainter paint(&m_pixmap);
    if (pressed)
        paint.drawPixmap(1,p,m_skin->getButton(Skin::EQ_BT_BAR_P));
    else
        paint.drawPixmap(1,p,m_skin->getButton(Skin::EQ_BT_BAR_N));
    setPixmap(m_pixmap);
    m_pos = p;
}

double EqSlider::convert(int p)
{
    return (m_max - m_min)*(p)/(height() - 12*m_skin->ratio()) + m_min;
}

void EqSlider::wheelEvent(QWheelEvent *e)
{
    m_value -= e->delta()/60;
    m_value = m_value > m_max ? m_max : m_value;
    m_value = m_value < m_min ? m_min : m_value;
    draw(false);
    emit sliderMoved(m_value);
}
