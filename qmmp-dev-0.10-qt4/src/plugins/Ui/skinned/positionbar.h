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
#ifndef POSITIONBAR_H
#define POSITIONBAR_H

#include "pixmapwidget.h"

class QMouseEvent;
class QWheelEvent;
class Skin;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class PositionBar : public PixmapWidget
{
    Q_OBJECT
public:
    PositionBar(QWidget *parent = 0);
    virtual ~PositionBar();

public slots:
    void setValue(qint64);
    qint64 value()const{return m_value;}
    void setMaximum(qint64);
    qint64 maximum() const {return m_max;}

signals:
    void sliderMoved (qint64);
    void sliderPressed();
    void sliderReleased();

private slots:
    void updateSkin();

private:
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent *e);
    Skin *m_skin;
    bool m_moving;
    qint64 press_pos;
    qint64 m_max, m_min, m_pos, m_value, m_old;
    QPixmap m_pixmap;
    qint64 convert(qint64);   // value = convert(position);
    void draw(bool pressed = true);
};

#endif
