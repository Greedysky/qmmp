/***************************************************************************
 *   Copyright (C) 2011-2014 by Ilya Kotov                                 *
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
#include <QKeySequence>
#include <qmmpui/playlistmanager.h>
#include <qmmpui/playlistmodel.h>
#include <qmmpui/mediaplayer.h>
#include <qmmp/soundcore.h>
#include "listwidget.h"
#include "keyboardmanager.h"

KeyboardManager::KeyboardManager(QObject *parent) :
    QObject(parent)
{
    m_listWidget = 0;

    addAction(Qt::Key_Up, SLOT(processUp()));
    addAction(Qt::Key_Up + Qt::ShiftModifier, SLOT(processUp()));
    addAction(Qt::Key_Up + Qt::AltModifier, SLOT(processUp()));
    addAction(Qt::Key_Up + Qt::ControlModifier, SLOT(processUp()));

    addAction(Qt::Key_Down, SLOT(processDown()));
    addAction(Qt::Key_Down + Qt::ShiftModifier, SLOT(processDown()));
    addAction(Qt::Key_Down + Qt::AltModifier, SLOT(processDown()));
    addAction(Qt::Key_Down + Qt::ControlModifier, SLOT(processDown()));

    addAction(Qt::Key_Return, SLOT(processEnter()));
    addAction(Qt::Key_PageUp, SLOT(processPgUp()));
    addAction(Qt::Key_PageUp + Qt::ShiftModifier, SLOT(processPgUp()));
    addAction(Qt::Key_PageDown, SLOT(processPgDown()));
    addAction(Qt::Key_PageDown + Qt::ShiftModifier, SLOT(processPgDown()));
    addAction(Qt::Key_Home, SLOT(processHome()));
    addAction(Qt::Key_Home + Qt::ShiftModifier, SLOT(processHome()));
    addAction(Qt::Key_End, SLOT(processEnd()));
    addAction(Qt::Key_End + Qt::ShiftModifier, SLOT(processEnd()));
}

QList<QAction *> KeyboardManager::actions()
{
    return m_actions;
}

void KeyboardManager::processUp()
{
    if(!m_listWidget)
        return;

    int keys = qobject_cast<QAction *>(sender())->shortcut()[0];

    QList<int> rows = m_listWidget->model()->selectedIndexes();

    if(rows.isEmpty())
    {
        m_listWidget->model()->setSelected(m_listWidget->firstVisibleIndex(), true);
        m_listWidget->setAnchorIndex(m_listWidget->firstVisibleIndex());
        return;
    }

    if (!(keys & Qt::ShiftModifier || keys & Qt::AltModifier || keys & Qt::ControlModifier))
    {
        m_listWidget->model()->clearSelection();
        m_listWidget->setAnchorIndex(-1);
    }

    int first_visible = m_listWidget->firstVisibleIndex();
    int last_visible = m_listWidget->visibleRows() + first_visible - 1;

    int s = SELECT_NEXT;

    if(rows.last() < first_visible)
        s = SELECT_TOP;
    else if(rows.first() > last_visible)
        s = SELECT_BOTTOM;

    if (keys & Qt::AltModifier)
    {
        if(rows.first() == 0)
            return;
        m_listWidget->model()->moveItems (rows.first(), rows.first() - 1);
        m_listWidget->setAnchorIndex (rows.first() - 1);
    }
    else if(keys & Qt::ControlModifier)
    {
        m_listWidget->setAnchorIndex (qMax(m_listWidget->anchorIndex() - 1, 0));
    }
    else
    {
        if(s == SELECT_TOP)
        {
            m_listWidget->model()->setSelected (first_visible, true);
            m_listWidget->setAnchorIndex(first_visible);
        }
        else if(s == SELECT_BOTTOM)
        {
            m_listWidget->model()->setSelected (last_visible, true);
            m_listWidget->setAnchorIndex(last_visible);
        }
        else if(rows.first() == 0)
        {
            m_listWidget->model()->setSelected (rows.first(), true);
            m_listWidget->setAnchorIndex(rows.first());
        }
        else if(rows.contains(m_listWidget->anchorIndex()) || m_listWidget->anchorIndex() < 0)
        {
            m_listWidget->model()->setSelected (rows.first() - 1, true);
            m_listWidget->setAnchorIndex(rows.first() - 1);
        }
        else if(m_listWidget->anchorIndex() >= 0)
        {
            m_listWidget->model()->setSelected (m_listWidget->anchorIndex(), true);
        }
    }

    if(m_listWidget->anchorIndex() < first_visible)
    {
        m_listWidget->scroll (m_listWidget->firstVisibleIndex() - 1);
    }
}

void KeyboardManager::processDown()
{
    if(!m_listWidget)
        return;

    int keys = qobject_cast<QAction *>(sender())->shortcut()[0];

    QList<int> rows = m_listWidget->model()->selectedIndexes();

    if(rows.isEmpty())
    {
        m_listWidget->model()->setSelected(m_listWidget->firstVisibleIndex(), true);
        m_listWidget->setAnchorIndex(m_listWidget->firstVisibleIndex());
        return;
    }

    if (!(keys & Qt::ShiftModifier || keys & Qt::AltModifier || keys & Qt::ControlModifier))
    {
        m_listWidget->model()->clearSelection();
        m_listWidget->setAnchorIndex(-1);
    }

    int first_visible = m_listWidget->firstVisibleIndex();
    int last_visible = m_listWidget->visibleRows() + first_visible - 1;

    int s = SELECT_NEXT;

    if(rows.last() < first_visible)
        s = SELECT_TOP;
    else if(rows.first() > last_visible)
        s = SELECT_BOTTOM;

    if (keys & Qt::AltModifier)
    {
        if(rows.last() == m_listWidget->model()->count() - 1)
            return;
        m_listWidget->model()->moveItems (rows.last(), rows.last() + 1);
        m_listWidget->setAnchorIndex (rows.last() + 1);
    }
    else if(keys & Qt::ControlModifier)
    {
        m_listWidget->setAnchorIndex (qMin(m_listWidget->anchorIndex() + 1,
                                           m_listWidget->model()->count() - 1));
    }
    else
    {
        if(s == SELECT_TOP)
        {
            m_listWidget->model()->setSelected (first_visible, true);
            m_listWidget->setAnchorIndex(first_visible);
        }
        else if(s == SELECT_BOTTOM)
        {
            m_listWidget->model()->setSelected (last_visible, true);
            m_listWidget->setAnchorIndex(last_visible);
        }
        else if(rows.last() == m_listWidget->model()->count() - 1)
        {
            m_listWidget->model()->setSelected (rows.last(), true);
            m_listWidget->setAnchorIndex(rows.last());
        }
        else if(rows.contains(m_listWidget->anchorIndex()) || m_listWidget->anchorIndex() < 0)
        {
            m_listWidget->model()->setSelected (rows.last() + 1, true);
            m_listWidget->setAnchorIndex(rows.last() + 1);
        }
        else if(m_listWidget->anchorIndex() >= 0)
        {
            m_listWidget->model()->setSelected (m_listWidget->anchorIndex(), true);
        }
    }

    if(m_listWidget->anchorIndex() > last_visible)
    {
        m_listWidget->scroll (m_listWidget->firstVisibleIndex() + 1);
    }
}

void KeyboardManager::setListWidget(ListWidget *listWidget)
{
    m_listWidget = listWidget;
}

void KeyboardManager::processEnter()
{
    if(!m_listWidget)
        return;
    QList<int> rows = m_listWidget->model()->selectedIndexes();
    if(rows.isEmpty())
        return;
    SoundCore::instance()->stop();
    PlayListManager::instance()->activatePlayList(m_listWidget->model());
    m_listWidget->model()->setCurrent (rows.first());
    MediaPlayer::instance()->play();
}

void KeyboardManager::processPgUp()
{
    if(!m_listWidget)
        return;

    int first = m_listWidget->firstVisibleIndex();
    int offset = qMax(m_listWidget->firstVisibleIndex() - m_listWidget->visibleRows(), 0);
    m_listWidget->scroll (offset);

    m_listWidget->model()->clearSelection();
    if(m_listWidget->firstVisibleIndex() == first)
        m_listWidget->setAnchorIndex(0);
    else
        m_listWidget->setAnchorIndex(m_listWidget->firstVisibleIndex() + m_listWidget->visibleRows()/2);
    m_listWidget->model()->setSelected(m_listWidget->anchorIndex(), true);
}

void KeyboardManager::processPgDown()
{
    if(!m_listWidget)
        return;

    int first = m_listWidget->firstVisibleIndex();
    int offset = qMin(first + m_listWidget->visibleRows(),
                      m_listWidget->model()->count() - 1);
    m_listWidget->scroll (offset);

    m_listWidget->model()->clearSelection();
    if(m_listWidget->firstVisibleIndex() == first)
        m_listWidget->setAnchorIndex(m_listWidget->model()->count() - 1);
    else
        m_listWidget->setAnchorIndex(m_listWidget->firstVisibleIndex() + m_listWidget->visibleRows()/2);
    m_listWidget->model()->setSelected(m_listWidget->anchorIndex(), true);
}

void KeyboardManager::processHome()
{
    if(!m_listWidget)
        return;
    int keys = qobject_cast<QAction *>(sender())->shortcut()[0];
    m_listWidget->scroll (0);
    if(keys & Qt::ShiftModifier)
    {
        m_listWidget->model()->setSelected (0, m_listWidget->anchorIndex(), true);
    }
    else if(m_listWidget->model()->count() != 0)
    {
        m_listWidget->model()->clearSelection();
        m_listWidget->setAnchorIndex(0);
        m_listWidget->model()->setSelected(0, true);
    }
}

void KeyboardManager::processEnd()
{
    if(!m_listWidget)
        return;

    int keys = qobject_cast<QAction *>(sender())->shortcut()[0];
    int scroll_to = m_listWidget->model()->count() - m_listWidget->visibleRows();
    if(scroll_to >= 0)
        m_listWidget->scroll(scroll_to);

    if(keys & Qt::ShiftModifier)
    {
        m_listWidget->model()->setSelected (m_listWidget->anchorIndex(), m_listWidget->model()->count() - 1, true);
    }
    else if(m_listWidget->model()->count() > 0)
    {
        m_listWidget->model()->clearSelection();
        m_listWidget->setAnchorIndex(m_listWidget->model()->count() - 1);
        m_listWidget->model()->setSelected(m_listWidget->anchorIndex(), true);
    }
}

void KeyboardManager::addAction(int keys, const char *method)
{
    QAction *action = new QAction(this);
    action->setShortcut(QKeySequence(keys));
    connect(action, SIGNAL(triggered()), method);
    m_actions << action;
}
