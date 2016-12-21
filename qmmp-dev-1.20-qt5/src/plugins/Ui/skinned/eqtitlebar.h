/***************************************************************************
 *   Copyright (C) 2007-2013 by Ilya Kotov                                 *
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
#ifndef EQTITLEBAR_H
#define EQTITLEBAR_H

#include "pixmapwidget.h"

class QMouseEvent;
class Skin;
class MainWindow;
class Button;
class ShadedBar;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class EqTitleBar : public PixmapWidget
{
Q_OBJECT
public:
    EqTitleBar(QWidget *parent = 0);

    ~EqTitleBar();

    void setActive(bool);

private slots:
    void shade();
    void updateSkin();

private:
    void updatePositions();
    Skin* m_skin;
    int m_left;
    int m_right;
    QPoint m_pos;
    QWidget* m_eq;
    MainWindow* m_mw;
    Button* m_close;
    Button* m_shade;
    Button* m_shade2;
    bool m_shaded, m_align;
    ShadedBar* m_volumeBar;
    ShadedBar* m_balanceBar;

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseDoubleClickEvent (QMouseEvent *);
};

#endif
