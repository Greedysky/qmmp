/***************************************************************************
 *   Copyright (C) 2006-2016 by Ilya Kotov                                 *
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
#ifndef PIXMAPWIDGET_H
#define PIXMAPWIDGET_H

#include <QWidget>

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class QPixmap;

class PixmapWidget : public QWidget
{
Q_OBJECT
public:
    PixmapWidget(QWidget *parent = 0);

    virtual ~PixmapWidget();

    virtual void setPixmap(const QPixmap &pixmap, bool fixed_size = false);

protected:
    void paintEvent ( QPaintEvent * event );
    virtual void mousePressEvent(QMouseEvent*);

signals:
    void mouseClicked();

private:
      QPixmap m_pixmap;



};

#endif
