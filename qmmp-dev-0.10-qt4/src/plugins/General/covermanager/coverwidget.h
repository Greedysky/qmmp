/***************************************************************************
 *   Copyright (C) 2009-2012 by Ilya Kotov                                 *
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
#include <QPixmap>

class QMenu;
class QMouseEvent;

/**
	@author Ilya Kotov <forkotov02@hotmail.ru>
*/

class CoverWidget : public QWidget
{
Q_OBJECT
public:
    CoverWidget(QWidget *parent = 0);

    ~CoverWidget();

    virtual void setPixmap(const QPixmap&);

protected:
    void paintEvent (QPaintEvent *event);
    void mousePressEvent (QMouseEvent * event);

private slots:
    void saveAs();
    void processResizeAction(QAction *action);

private: 
    QPixmap m_pixmap;
    QMenu *m_menu;
    int m_size;
};

#endif
