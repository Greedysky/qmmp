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
#ifndef EQSLIDER_H
#define EQSLIDER_H

#include "pixmapwidget.h"

class QMouseEvent;
class QWheelEvent;
class Skin;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class EqSlider : public PixmapWidget
{
Q_OBJECT
public:
    EqSlider(QWidget *parent = 0);

    ~EqSlider();

    double value();

public slots:
    void setValue(double);
    void setMax(double);

signals:
    void sliderMoved (double);

private slots:
    void updateSkin();

private:
    Skin *m_skin;
    bool m_moving;
    int press_pos;
    double m_max, m_min, m_pos, m_value, m_old;
    QPixmap m_pixmap;
    double convert(int);   // value = convert(position);
    void draw(bool pressed = true);

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent *);


};

#endif
