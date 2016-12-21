/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
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

#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QSettings>
#include <QApplication>
#include <QMouseEvent>
#include <QMenu>
#include <QLineEdit>
#include <QInputDialog>
#include <QIcon>
#include <qmmp/qmmp.h>
#include <qmmpui/playlistmanager.h>
#include "actionmanager.h"
#include "skin.h"
#include "playlistselector.h"

PlayListSelector::PlayListSelector(PlayListManager *manager, QWidget *parent) : QWidget(parent)
{
    m_scrollable = false;
    m_moving = false;
    m_offset = 0;
    m_offset_max = 0;
    m_metrics = 0;
    m_pressed_button = BUTTON_UNKNOWN;
    m_skin = Skin::instance();
    m_pl_manager = manager;
    connect(m_pl_manager, SIGNAL(playListsChanged()), SLOT(updateTabs()));
    connect(m_skin, SIGNAL(skinChanged()), this, SLOT(updateSkin()));
    loadColors();
    readSettings();
    m_menu = new QMenu(this);
    m_menu->addAction(ACTION(ActionManager::PL_LOAD));
    m_menu->addAction(ACTION(ActionManager::PL_SAVE));
    m_menu->addSeparator();
    m_menu->addAction(ACTION(ActionManager::PL_RENAME));
    m_menu->addAction(ACTION(ActionManager::PL_CLOSE));
}

PlayListSelector::~PlayListSelector()
{
    if (m_metrics)
        delete m_metrics;
    m_metrics = 0;
}

void PlayListSelector::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_font.fromString(settings.value("Skinned/pl_font", QApplication::font().toString()).toString());
    if (m_metrics)
    {
        delete m_metrics;
        m_metrics = 0;
    }
    m_metrics = new QFontMetrics(m_font);
    m_pl_separator = settings.value("Skinned/pl_separator", "::").toString();
    m_show_new_pl_button = settings.value("Skinned/pl_show_create_button", false).toBool();
    m_pl_button = "[+]";
    m_pl_separator.append(" ");
    m_pl_separator.prepend(" ");
    resize(width(), m_metrics->height () +1);
    drawButtons();
    updateTabs();
}

void PlayListSelector::updateTabs()
{
    m_rects.clear();
    m_extra_rects.clear();
    QRect rect;
    foreach(QString text, m_pl_manager->playListNames())
    {
        if(m_rects.isEmpty())
            rect.setX(9);
        else
            rect.setX(m_rects.last().x() + m_rects.last().width() + m_metrics->width(m_pl_separator));
        rect.setY(0);
        rect.setWidth(m_metrics->width(text));
        rect.setHeight(m_metrics->ascent ());
        m_rects.append(rect);
    }

    if(m_show_new_pl_button)
    {
        rect.setX(m_rects.last().x() + m_rects.last().width() + m_metrics->width("  "));
        rect.setY(0);
        rect.setWidth(m_metrics->width(m_pl_button));
        rect.setHeight(m_metrics->ascent ());
        m_extra_rects.append(rect);
    }

    updateScrollers();
    update();
}

void PlayListSelector::updateSkin()
{
    loadColors();
    drawButtons();
    updateTabs();
}

void PlayListSelector::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(m_font);
    painter.setBrush(QBrush(m_normal_bg));
    painter.drawRect(-1,-1,width()+1,height()+1);
    QStringList names = m_pl_manager->playListNames();
    int current = m_pl_manager->indexOf(m_pl_manager->currentPlayList());
    int selected = m_pl_manager->indexOf(m_pl_manager->selectedPlayList());

    if(m_moving)
    {
        painter.setBrush(QBrush(m_normal_bg));
        painter.setPen(m_current);
        painter.drawRect(m_rects.at(selected).x() - 2 - m_offset, 0,
                         m_rects.at(selected).width() + 3, height()-1);
    }
    else
    {
        painter.setBrush(QBrush(m_selected_bg));
        painter.setPen(m_selected_bg);
        painter.drawRect(m_rects.at(selected).x() - 2 - m_offset, 0,
                         m_rects.at(selected).width() + 3, height()-1);
    }

    for (int i = 0; i < m_rects.size(); ++i)
    {
        if(i == current)
             painter.setPen(m_current);
        else
             painter.setPen(m_normal);

        if(!m_moving || i != selected)
            painter.drawText(m_rects[i].x() - m_offset, m_metrics->ascent(), names.at(i));
        if(i < m_rects.size() - 1)
        {
            painter.setPen(m_normal);
            painter.drawText(m_rects[i].x() + m_rects[i].width() - m_offset, m_metrics->ascent(),
                             m_pl_separator);
        }
    }


    for(int i = 0; i < m_extra_rects.size(); ++i)
    {
        painter.setPen(m_pressed_button == BUTTON_NEW_PL ? m_current : m_normal);
        painter.drawText(m_extra_rects[i].x() - m_offset, m_metrics->ascent(), m_pl_button);
    }


    if(m_moving)
    {
        painter.setBrush(QBrush(m_selected_bg));
        painter.setPen(m_selected_bg);
        painter.drawRect(m_mouse_pos.x() - m_press_offset - 2, 0,
                         m_rects.at(selected).width() + 3, height());

        painter.setPen(selected == current ? m_current : m_normal);
        painter.drawText(m_mouse_pos.x() - m_press_offset,
                         m_metrics->ascent(), names.at(selected));
    }

    if(m_scrollable)
    {
        painter.drawPixmap(width()-40, 0, m_pixmap);
        painter.setBrush(QBrush(m_normal_bg));
        painter.setPen(m_normal_bg);
        painter.drawRect(0,0,6,height());
    }
}

void PlayListSelector::mousePressEvent (QMouseEvent *e)
{
    m_pressed_button = findButton(e->pos());
    if(m_pressed_button != BUTTON_UNKNOWN)
    {
        drawButtons();
        update();
        return;
    }

    bool selected = false;
    int index = findPlayList(e->pos());
    if(index != -1)
    {
        selected = true;
        m_pl_manager->selectPlayList(index);
    }


    QPoint pp = e->pos();
    pp.rx() += m_offset;

    if(e->button() == Qt::RightButton)
    {
        m_moving = false;
        update();
        m_menu->exec(e->globalPos());
        return;
    }
    else if(e->button() == Qt::MidButton && selected)
    {
        m_moving = false;
        m_pl_manager->removePlayList(m_pl_manager->selectedPlayList());
    }
    else if(e->button() == Qt::LeftButton)
    {
        m_moving = true;
        m_mouse_pos = e->pos();
        m_press_offset = pp.x() - m_rects.at(m_pl_manager->selectedPlayListIndex()).x();
        QWidget::mousePressEvent(e);
    }
    update();
}

void PlayListSelector::mouseReleaseEvent (QMouseEvent *e)
{
    m_moving = false;
    int released_button = findButton(e->pos());
    if(released_button == m_pressed_button)
    {
        switch(released_button)
        {
        case BUTTON_NEW_PL:
            m_pl_manager->createPlayList();
            break;
        case BUTTON_LEFT:
            m_offset -= m_offset - firstVisible().x() + 9 + 2;
            m_offset = qMax(0, m_offset);
            break;
        case BUTTON_RIGHT:
            m_offset += lastVisible().right() - m_offset - width() + 42;
            m_offset = qMin(m_offset, m_offset_max);
            break;
        default:
            ;
        }
    }
    m_pressed_button = BUTTON_UNKNOWN;
    drawButtons();
    update();
    QWidget::mouseReleaseEvent(e);
}

void PlayListSelector::mouseDoubleClickEvent (QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton && !(m_scrollable && (e->x() > width() - 40)))
        ACTION(ActionManager::PL_RENAME)->trigger();
    else
        QWidget::mouseDoubleClickEvent(e);
}

void PlayListSelector::mouseMoveEvent(QMouseEvent *e)
{
    if(!m_moving)
    {
        QWidget::mouseMoveEvent(e);
        return;
    }

    m_mouse_pos = e->pos();
    QPoint mp = e->pos();
    mp.rx() += m_offset;

    int dest = -1;
    for(int i = 0; i < m_rects.count(); ++i)
    {
        int x_delta = mp.x() - m_rects.at(i).x();
        if(x_delta < 0 || x_delta > m_rects.at(i).width())
            continue;

        if((x_delta > m_rects.at(i).width()/2 && m_pl_manager->selectedPlayListIndex() < i) ||
                (x_delta < m_rects.at(i).width()/2 && m_pl_manager->selectedPlayListIndex() > i))
        {
            dest = i;
            break;
        }
    }
    if(dest == -1 || dest == m_pl_manager->selectedPlayListIndex())
    {
        update();
        QWidget::mouseMoveEvent(e);
        return;
    }
    m_pl_manager->move(m_pl_manager->selectedPlayListIndex(), dest);
    update();
}

void PlayListSelector::resizeEvent (QResizeEvent *)
{
    updateScrollers();
}

void PlayListSelector::loadColors()
{
    m_normal.setNamedColor(m_skin->getPLValue("normal"));
    m_current.setNamedColor(m_skin->getPLValue("current"));
    m_normal_bg.setNamedColor(m_skin->getPLValue("normalbg"));
    m_selected_bg.setNamedColor(m_skin->getPLValue("selectedbg"));
}

void PlayListSelector::drawButtons()
{
    m_pixmap = QPixmap(40, height());
    m_pixmap.fill(m_normal_bg);
    QPainter painter(&m_pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(m_pressed_button == BUTTON_LEFT ? m_current : m_normal);
    painter.setBrush(QBrush(m_pressed_button == BUTTON_LEFT ? m_current : m_normal));
    QPoint points[3] = {
        QPoint(m_pixmap.width() - 25, height()/2 - 5),
        QPoint(m_pixmap.width() - 35, height()/2-1),
        QPoint(m_pixmap.width() - 25, height()/2 + 3),
    };
    painter.drawPolygon(points, 3);
    painter.setPen(m_pressed_button == BUTTON_RIGHT ? m_current : m_normal);
    painter.setBrush(QBrush(m_pressed_button == BUTTON_RIGHT ? m_current : m_normal));
    QPoint points2[3] = {
        QPoint(m_pixmap.width() - 20, height()/2 - 5),
        QPoint(m_pixmap.width() - 10, height()/2-1),
        QPoint(m_pixmap.width() - 20, height()/2 + 3),
    };
    painter.drawPolygon(points2, 3);
}

void PlayListSelector::updateScrollers()
{
    int last_x = m_extra_rects.isEmpty() ? m_rects.last().right() : m_extra_rects.last().right();

    m_scrollable = last_x > width();
    if(m_scrollable)
    {
        m_offset_max = last_x - width() + 42;
        m_offset = qMin(m_offset, m_offset_max);
    }
    else
    {
         m_offset = 0;
         m_offset_max = 0;
    }
}

QRect PlayListSelector::firstVisible()
{
    for(int i = 0; i < m_rects.size(); ++i)
    {
        if(m_rects.at(i).right() + m_metrics->width(m_pl_separator) >= 9 + m_offset)
            return m_rects.at(i);
    }
    return m_rects.at(0);
}

QRect PlayListSelector::lastVisible()
{
    for(int i = m_extra_rects.size() - 1; i >= 0; --i)
    {
        if(m_extra_rects.at(i).x() - m_offset -  m_metrics->width("  ") - 2 <= width() - 40)
            return m_extra_rects.at(i);
    }
    for(int i = m_rects.size() - 1; i >= 0; --i)
    {
        if(m_rects.at(i).x() - m_offset - m_metrics->width(m_pl_separator) - 2 <= width() - 40)
            return m_rects.at(i);
    }
    return m_extra_rects.isEmpty() ? m_rects.last() : m_extra_rects.last();
}

int PlayListSelector::findPlayList(QPoint pos)
{
    pos.rx() += m_offset;
    for(int i = 0; i < m_rects.count(); ++i)
    {
        if(m_rects.at(i).contains(pos))
            return i;
    }
    return -1;
}

int PlayListSelector::findButton(QPoint pos)
{
    if(m_scrollable)
    {
        if(pos.x() > width() - 20)
            return BUTTON_RIGHT;
        else if ((width() - 40 < pos.x()) && (pos.x() <= width() - 20))
            return BUTTON_LEFT;
    }

    pos.rx() += m_offset;
    for(int i = 0; i < m_extra_rects.count(); ++i)
    {
        if(m_extra_rects.at(i).contains(pos))
            return BUTTON_NEW_PL;
    }
    return BUTTON_UNKNOWN;
}
