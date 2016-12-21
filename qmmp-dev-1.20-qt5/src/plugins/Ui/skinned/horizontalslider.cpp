/***************************************************************************
 *   Copyright (C) 2015 by Ilya Kotov                                      *
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
#include <QResizeEvent>
#include <math.h>
#include "skin.h"
#include "horizontalslider.h"
#include "pixmapwidget.h"

#define MIN_SLIDER_SIZE 18

HorizontalSlider::HorizontalSlider(QWidget *parent)
        : QWidget(parent)
{
    m_skin = Skin::instance();
    m_min = 0;
    m_max = 100;
    m_value = 0;
    connect(m_skin, SIGNAL(skinChanged()), this, SLOT(updateSkin()));
    updateSkin();
}

HorizontalSlider::~HorizontalSlider()
{}

void HorizontalSlider::paintEvent(QPaintEvent *)
{
    if(m_max <= m_min)
        return;

    bool rtl = (layoutDirection() == Qt::RightToLeft);
    int p = ceil((m_value - m_min)*(width() - sliderSize())/(m_max - m_min));
    if(rtl)
        p = width() - p - sliderSize();

    QPainter paint(this);

    paint.fillRect(0, 0, width(), height(), m_normal_bg);
    paint.setPen(m_normal);

    paint.drawRect(0, 0, width() - 1, height() - 1);
    paint.fillRect(p, 0, sliderSize(), height() - 1, m_normal);

    m_slider_pos = p;
}

void HorizontalSlider::mousePressEvent(QMouseEvent *e)
{
    m_press_pos = e->x();
    if (m_slider_pos < e->x() && e->x() < m_slider_pos + sliderSize())
    {
        m_press_pos = e->x() - m_slider_pos;
    }
    update();
}

void HorizontalSlider::mouseMoveEvent(QMouseEvent* e)
{
    int po = e->x() - m_press_pos;
    bool rtl = (layoutDirection() == Qt::RightToLeft);

    if (0 <= po && po <= width() - sliderSize())
    {
        if(rtl)
            po = width() - po - sliderSize();

        m_value = convert(po);
        update();
        if (m_value != m_old_value)
        {
            m_old_value = m_value;
            emit sliderMoved(m_value);
        }
    }
}

void HorizontalSlider::setPos(int v, int max)
{
    m_max = max;
    m_value = v;
    update();
}

void HorizontalSlider::updateSkin()
{
    m_normal.setNamedColor(m_skin->getPLValue("normal"));
    m_normal_bg.setNamedColor(m_skin->getPLValue("normalbg"));
    update();
}

int HorizontalSlider::convert(int p) const
{
    if(m_max > m_min)
        return ceil((m_max - m_min) * p / (width() - sliderSize())) + m_min;
    else
        return 0;
}

int HorizontalSlider::sliderSize() const
{
    if(m_max > m_min)
        return qMax(width() - abs(m_max - m_min), MIN_SLIDER_SIZE * m_skin->ratio());
    else
        return MIN_SLIDER_SIZE;
}
