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
#ifndef VOLUMEBAR_H
#define VOLUMEBAR_H

#include "pixmapwidget.h"

class Skin;
class MainWindow;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class VolumeBar : public PixmapWidget
{
Q_OBJECT
public:
    VolumeBar(QWidget *parent = 0);

    ~VolumeBar();

    int value()
    {
        return m_value;
    }
    int isPressed()
    {
        return m_moving;
    }

public slots:
    void setValue(int);
    void setMax(int);

signals:
    void sliderMoved (int);
    void sliderPressed();
    void sliderReleased();

private slots:
    void updateSkin();

private:
    Skin *m_skin;
    bool m_moving;
    int press_pos;
    int m_max, m_min, m_pos, m_value, m_old;
    QPixmap m_pixmap;
    MainWindow *mw;
    int convert(int);   // value = convert(position);
    void draw(bool pressed = true);

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
};

#endif
