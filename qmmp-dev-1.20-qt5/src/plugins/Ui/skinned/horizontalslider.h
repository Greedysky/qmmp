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
#ifndef HORIZONTALSLIDER_H
#define HORIZONTALSLIDER_H

#include <QWidget>
#include <QColor>

class Skin;
class PixmapWidget;
/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class HorizontalSlider : public QWidget
{
Q_OBJECT
public:
    HorizontalSlider(QWidget *parent = 0);

    ~HorizontalSlider();

public slots:
    void setPos(int v, int max);

signals:
    void sliderMoved (int);

private slots:
    void updateSkin();

private:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    int convert(int) const;   // value = convert(position);
    int sliderSize() const;

    Skin *m_skin;
    int m_old_value, m_value;
    int m_press_pos;
    int m_min, m_max, m_slider_pos;
    QColor m_normal, m_normal_bg;

};

#endif
