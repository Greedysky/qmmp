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

#include <QAction>
#include <QDesktopWidget>
#include <QApplication>
#include "dock.h"

Dock *Dock::m_instance = 0;

Dock *Dock::instance()
{
    if (!m_instance)
        m_instance = new Dock();
    return m_instance;
}

Dock::Dock (QObject *parent)
        : QObject (parent)
{
    m_instance = this;
    m_mainWidget = 0;
}

Dock::~Dock()
{
    m_instance = 0;
}

void Dock::setMainWidget (QWidget *widget)
{
    m_mainWidget = widget;
    m_widgetList.prepend (widget);
    m_dockedList.prepend (false);
}

QPoint Dock::snapDesktop(QPoint npos, QWidget* mv)
{
    QRect desktopRect = QApplication::desktop()->availableGeometry(mv);
    int nx = abs (npos.x() - desktopRect.x()); //left-top
    int ny = abs (npos.y() - desktopRect.y());

    if(nx < 13)
        npos.rx() = desktopRect.x();
    if(ny < 13)
        npos.ry() = desktopRect.y();

    nx = abs (npos.x() + mv->width() - desktopRect.width() - desktopRect.x()); //right-bottom
    ny = abs (npos.y() + mv->height() - desktopRect.height() - desktopRect.y());

    if(nx < 13)
        npos.rx() = desktopRect.width() - mv->width() + desktopRect.x();
    if(ny < 13)
        npos.ry() = desktopRect.height() - mv->height() + desktopRect.y();

    return npos;
}

QPoint Dock::snap (QPoint npos, QWidget* mv, QWidget* st)
{
    int nx = npos.x() - st->x();
    int ny = abs (npos.y() - st->y() + mv->height());

    if (abs (nx) < 13 && ny < 13) //above
        npos.rx() = st->x();
    if (ny < 13 && nx > -mv->width() && nx < st->width())
        npos.ry() = st->y() - mv->height();
    nx = abs (npos.x() + mv->width() - st->x() - st->width());
    if (nx < 13 && ny < 13)
        npos.rx() = st->x() + st->width() - mv->width();

    /***********/
    nx = npos.x() - st->x();
    ny = abs (npos.y() - st->y() - st->height());

    if (abs (nx) < 13 && ny < 13) //near
        npos.rx() = st->x();
    if (ny < 13 && nx > -mv->width() && nx < st->width())
        npos.ry() = st->y() + st->height();
    nx = abs (npos.x() + mv->width() - st->x() - st->width());
    if (nx < 13 && ny < 13)
        npos.rx() = st->x() + st->width() - mv->width();
    /**************/
    nx = abs (npos.x() - st->x() + mv->width());
    ny = npos.y() - st->y();

    if (nx < 13 && abs (ny) < 13) //left
        npos.ry() = st->y();
    if (nx < 13 && ny > -mv->height() && ny < st->height())
        npos.rx() = st->x() - mv->width();

    ny = abs (npos.y() + mv->height() - st->y() - st->height());
    if (nx < 13 && ny < 13)
        npos.ry() = st->y() + st->height() - mv->height();
    /*****************/
    nx = abs (npos.x() - st->x() - st->width());
    ny = npos.y() - st->y();

    if (nx < 13 && abs (ny) < 13) //right
        npos.ry() = st->y();
    if (nx < 13 && ny > -mv->height() && ny < st->height())
        npos.rx() = st->x() + st->width();

    ny = abs (npos.y() + mv->height() - st->y() - st->height());
    if (nx < 13 && ny < 13)
        npos.ry() = st->y() + st->height() - mv->height();

    return (npos);
}

void Dock::addWidget (QWidget *widget)
{
    m_widgetList.append (widget);
    m_dockedList.append (false);
    if(m_mainWidget)
        widget->addActions(m_mainWidget->actions());
}

void Dock::move (QWidget* mv, QPoint npos)
{
    //QRect desktopRect = QApplication::desktop()->availableGeometry(m_mainWidget);

    /*if(npos.y() < desktopRect.y())
        npos.setY(desktopRect.y());*/

    if (mv == m_mainWidget)
    {

        for (int i = 1; i<m_widgetList.size(); ++i)
        {
            if (!m_dockedList.at (i))
            {
                if (m_widgetList.at (i)->isVisible())
                    npos = snap (npos, mv, m_widgetList.at (i));
            }
            else
            {
                QPoint pos = npos + m_delta_list.at(i);
                for (int j = 1; j<m_widgetList.size(); ++j)
                {
                    if (!m_dockedList.at (j) && m_widgetList.at (j)->isVisible())
                    {
                        pos = snap (pos, m_widgetList.at (i), m_widgetList.at (j));
                        npos = pos - m_delta_list.at(i);
                    }
                }
            }
        }
        npos = snapDesktop(npos, mv);
        for (int i = 1; i<m_widgetList.size(); ++i)
        {
            if (m_dockedList.at (i))
            {
                QPoint pos = npos + m_delta_list.at(i);
                pos = snapDesktop(pos, m_widgetList.at(i));
                m_widgetList.at (i)->move(pos);
                npos = pos - m_delta_list.at(i);
            }
        }
        mv->move (npos);
    }
    else
    {
        for (int i = 0; i<m_widgetList.size(); ++i)
        {
            m_dockedList[i] = false;
            if (mv != m_widgetList.at (i) && !m_dockedList.at (i) && m_widgetList.at (i)->isVisible())
            {
                npos = snap (npos, mv, m_widgetList.at (i));
                npos = snapDesktop(npos, mv);
            }
        }
        mv->move (npos);
    }
}

void Dock::calculateDistances()
{
    m_delta_list.clear();
    foreach (QWidget *w, m_widgetList)
    {
        if (w == m_mainWidget)
            m_delta_list.append(QPoint(0,0));
        else
            m_delta_list.append(w->pos() - m_mainWidget->pos());
    }
}

void Dock::updateDock()
{
    QWidget *mv = m_widgetList.at (0);
    for (int j = 1; j<m_widgetList.size(); ++j)
    {
        QWidget *st = m_widgetList.at (j);
        m_dockedList[j] = isDocked (mv, st);
    }
    for (int j = 1; j<m_widgetList.size(); ++j)
    {
        if (m_dockedList[j])
            for (int i = 1; i<m_widgetList.size(); ++i)
            {
                if (!m_dockedList[i])
                {
                    mv = m_widgetList.at (j);
                    QWidget *st = m_widgetList.at (i);
                    m_dockedList[i] = isDocked (mv, st);
                }
            }
    }
}

bool Dock::isDocked (QWidget* mv, QWidget* st)
{
    int nx = mv->x() - st->x();
    int ny = abs (mv->y() - st->y() + mv->height());
    if (ny < 2 && nx > -mv->width() && nx < st->width()) //above
        return true;

    /***********/
    nx = mv->x() - st->x();
    ny = abs (mv->y() - st->y() - st->height());
    if (ny < 2 && nx > -mv->width() && nx < st->width()) //near
        return true;

    /**************/
    nx = abs (mv->x() - st->x() + mv->width());
    ny = mv->y() - st->y();
    if (nx < 2 && ny > -mv->height() && ny < st->height())   //left
        return true;

    /*****************/
    nx = abs (mv->x() - st->x() - st->width());
    ny = mv->y() - st->y();
    if (nx < 2 && ny > -mv->height() && ny < st->height())   //right
        return true;
    return false;
}

void Dock::addActions (QList<QAction *> actions)
{
    if(!m_mainWidget)
    {
        qFatal("Dock: main widget is null");
    }
    for (int i = 0; i<m_widgetList.size(); ++i)
        m_widgetList.at (i)->addActions (actions);
}

bool Dock::isUnder(QWidget* upper, QWidget* nether, int dy)
{
    int nx = upper->x() - nether->x();
    return abs (upper->y() + upper->height() -dy - nether->y()) < 2 &&
           nx > -upper->width() && nx < nether->width();
}

void Dock::align(QWidget* w, int dy)
{
    for (int i = 0; i<m_dockedList.size(); ++i)
    {
        if (m_widgetList.at(i) != w && isUnder(w, m_widgetList.at(i), dy))
        {
            m_widgetList.at(i)->move(m_widgetList.at(i)->x(), m_widgetList.at(i)->y()+dy);
            align(m_widgetList.at(i), dy);
        }
    }
}
