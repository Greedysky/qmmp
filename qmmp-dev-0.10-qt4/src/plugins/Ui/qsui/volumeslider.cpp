/***************************************************************************
 *   Copyright (C) 2011-2016 by Ilya Kotov                                 *
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
#include <QWheelEvent>
#include <QStyleOptionSlider>
#include <QStyle>
#include <QToolTip>
#include <math.h>
#include "volumeslider.h"

VolumeSlider::VolumeSlider(QWidget *parent) : QSlider(Qt::Horizontal, parent)
{
    connect(this, SIGNAL(sliderMoved(int)), SLOT(onSliderMoved(int)));
}

void VolumeSlider::mousePressEvent (QMouseEvent *event)
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

    if (event->button() == Qt::LeftButton &&
            sr.contains(event->pos()) == false)
    {
        int val;
        if (orientation() == Qt::Vertical)
            val = minimum() + ((maximum() - minimum()) * (height() - event->y())) / height();
        else if(layoutDirection() == Qt::RightToLeft)
            val = maximum() - ((maximum() - minimum()) * event->x()) / width();
        else
            val = minimum() + ((maximum() - minimum()) * event->x()) / width();

        setSliderDown (true);

        if (invertedAppearance() == true)
        {
            setValue(maximum() - val);
            onSliderMoved(maximum() - val);
        }
        else
        {
            setValue(val);
            onSliderMoved(val);
        }
        event->accept();
    }
    QSlider::mousePressEvent(event);
}

void VolumeSlider::mouseReleaseEvent (QMouseEvent *event)
{
    setSliderDown (false);
    QSlider::mouseReleaseEvent(event);
}

void VolumeSlider::wheelEvent(QWheelEvent *event)
{
    setSliderDown(true);
    setValue(value() + event->delta() / 20);
    setSliderDown(false);
}

void VolumeSlider::onSliderMoved(int pos)
{
    if(minimum() != maximum())
    {
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        QRect rect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        rect.moveTo(rect.x() - 10 , rect.y() - 45);

        int level = (pos - minimum()) * 100 / (maximum() - minimum());
        QToolTip::showText(mapToGlobal(rect.topLeft()), tr("%1%").arg(level), this, QRect());
    }
}
