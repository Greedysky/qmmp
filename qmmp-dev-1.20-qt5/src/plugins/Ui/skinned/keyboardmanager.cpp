/***************************************************************************
 *   Copyright (C) 2006-2014 by Ilya Kotov                                 *
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

#include <QKeyEvent>
#include <qmmpui/playlistmodel.h>
#include <qmmpui/playlistmanager.h>
#include <qmmpui/mediaplayer.h>
#include "playlist.h"
#include "listwidget.h"
#include "keyboardmanager.h"

KeyboardManager::KeyboardManager (ListWidget *l)
{
    m_listWidget = l;
}

bool KeyboardManager::handleKeyPress (QKeyEvent* ke)
{
    bool handled = true;
    switch (ke->key())
    {
        case Qt::Key_Up:
            keyUp (ke);
            break;
        case Qt::Key_Down:
            keyDown (ke);
            break;
        case Qt::Key_PageUp:
            keyPgUp (ke);
            break;
        case Qt::Key_PageDown:
            keyPgDown (ke);
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            keyEnter (ke);
            break;
        case Qt::Key_Home:
            keyHome(ke);
            break;
         case Qt::Key_End:
            keyEnd(ke);
            break;
        default:
            handled = false;
    }
    return handled;
}

bool KeyboardManager::handleKeyRelease (QKeyEvent*)
{
    return false;
}

void KeyboardManager::keyUp (QKeyEvent * ke)
{
    QList<int> rows = m_listWidget->model()->selectedIndexes();

    if(rows.isEmpty())
    {
        m_listWidget->model()->setSelected(m_listWidget->firstVisibleIndex(), true);
        m_listWidget->setAnchorIndex(m_listWidget->firstVisibleIndex());
        return;
    }

    if (! (ke->modifiers() & Qt::ShiftModifier ||
           ke->modifiers() & Qt::AltModifier ||
           ke->modifiers() & Qt::ControlModifier))
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

    if (ke->modifiers() == Qt::AltModifier)
    {
        if(rows.first() == 0)
            return;
        m_listWidget->model()->moveItems (rows.first(), rows.first() - 1);
        m_listWidget->setAnchorIndex (rows.first() - 1);
    }
    else if(ke->modifiers() == Qt::ControlModifier)
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

void KeyboardManager::keyDown (QKeyEvent * ke)
{
    QList<int> rows = m_listWidget->model()->selectedIndexes();

    if(rows.isEmpty())
    {
        m_listWidget->model()->setSelected(m_listWidget->firstVisibleIndex(), true);
        m_listWidget->setAnchorIndex(m_listWidget->firstVisibleIndex());
        return;
    }

    if (!(ke->modifiers() & Qt::ShiftModifier ||
          ke->modifiers() & Qt::AltModifier ||
          ke->modifiers() & Qt::ControlModifier))
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

    if (ke->modifiers() == Qt::AltModifier)
    {
        if(rows.last() == m_listWidget->model()->count() - 1)
            return;
        m_listWidget->model()->moveItems (rows.last(), rows.last() + 1);
        m_listWidget->setAnchorIndex (rows.last() + 1);
    }
    else if(ke->modifiers() == Qt::ControlModifier)
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

void KeyboardManager::keyPgUp (QKeyEvent *)
{
    int page_size = m_listWidget->visibleRows();
    int first = m_listWidget->firstVisibleIndex();
    int offset = qMax(first - page_size, 0);
    m_listWidget->scroll (offset);

    m_listWidget->model()->clearSelection();
    if(m_listWidget->firstVisibleIndex() == first)
        m_listWidget->setAnchorIndex(0);
    else
        m_listWidget->setAnchorIndex(m_listWidget->firstVisibleIndex() + page_size/2);
    m_listWidget->model()->setSelected(m_listWidget->anchorIndex(), true);
}

void KeyboardManager::keyPgDown (QKeyEvent *)
{
    int page_size = m_listWidget->visibleRows();
    int first = m_listWidget->firstVisibleIndex();
    int offset = qMin(first + page_size, m_listWidget->model()->count() - 1);
    m_listWidget->scroll (offset);

    m_listWidget->model()->clearSelection();
    if(m_listWidget->firstVisibleIndex() == first)
        m_listWidget->setAnchorIndex(m_listWidget->model()->count() - 1);
    else
        m_listWidget->setAnchorIndex(m_listWidget->firstVisibleIndex() + page_size/2);
    m_listWidget->model()->setSelected(m_listWidget->anchorIndex(), true);
}

void KeyboardManager::keyEnter (QKeyEvent *)
{
    QList<int> indexes = m_listWidget->model()->selectedIndexes();
    if (indexes.count() > 0)
    {
        m_listWidget->model()->setCurrent (indexes.first());
        MediaPlayer::instance()->stop();
        PlayListManager::instance()->activatePlayList(m_listWidget->model());
        MediaPlayer::instance()->play();
    }
}

void KeyboardManager::keyHome(QKeyEvent *ke)
{
    m_listWidget->scroll (0);
    if(ke->modifiers() & Qt::ShiftModifier)
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

void KeyboardManager::keyEnd(QKeyEvent *ke)
{
   int scroll_to = qMax(m_listWidget->model()->count() - m_listWidget->visibleRows(), 0);
   m_listWidget->scroll(scroll_to);
   if(ke->modifiers() & Qt::ShiftModifier)
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
