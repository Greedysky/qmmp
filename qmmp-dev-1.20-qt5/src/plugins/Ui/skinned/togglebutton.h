/***************************************************************************
 *   Copyright (C) 2006 by Ilya Kotov                                      *
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
#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "pixmapwidget.h"

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class Skin;

class ToggleButton : public PixmapWidget
{
Q_OBJECT
public:
   ToggleButton( QWidget *parent, uint on_n, uint on_p, uint off_n, uint off_p );
   ~ToggleButton();

    bool isChecked();

public slots:
    void setChecked(bool);
    void click();

signals:
    void clicked(bool);

private slots:
    void updateSkin();

private:
    Skin *skin;
    bool m_cursorin, m_old_on;
    uint m_on_n, m_on_p, m_off_n, m_off_p;
    bool m_on;

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
};


#endif
