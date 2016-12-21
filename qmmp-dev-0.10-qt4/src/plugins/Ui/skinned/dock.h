/***************************************************************************
 *   Copyright (C) 2006-2012 by Ilya Kotov                                 *
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
#ifndef DOCK_H
#define DOCK_H

#include <QObject>
#include <QPoint>
#include <QWidget>

class QAction;
/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class Dock : public QObject
{
    Q_OBJECT
public:
    Dock(QObject *parent = 0);

    ~Dock();

    static Dock *instance();
    void setMainWidget(QWidget*);
    void addWidget(QWidget *);
    void move(QWidget*, QPoint);
    void calculateDistances();
    void updateDock();
    void addActions(QList<QAction *> actions);
    void align(QWidget*, int dy);

private:
    QPoint snapDesktop(QPoint, QWidget*);
    QPoint snap(QPoint, QWidget*, QWidget*);
    bool isDocked(QWidget*, QWidget*);
    bool isUnder(QWidget*, QWidget*, int);
    static Dock *m_instance;
    QWidget *m_mainWidget;
    QList <QWidget *> m_widgetList;
    QList <bool> m_dockedList;
    QList <QPoint> m_delta_list;
};

#endif
