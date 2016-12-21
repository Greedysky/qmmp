/***************************************************************************
 *   Copyright (C) 2012-2015 by Ilya Kotov                                 *
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

#include <QMouseEvent>
#include <QTabBar>
#include <QSettings>
#include <QApplication>
#include <qmmp/qmmp.h>
#include "qsuitabbar.h"
#include "qsuitabwidget.h"

QSUiTabWidget::QSUiTabWidget(QWidget *parent) : QTabWidget(parent)
{
    setTabBar(new QSUiTabBar(this));
    setMovable(true);
    m_menu = new QMenu(this);
    m_group = new QActionGroup(this);
    m_group->setExclusive(true);
    connect(tabBar(), SIGNAL(tabMoved(int,int)), SIGNAL(tabMoved(int,int)));
    connect(tabBar(), SIGNAL(tabMoved(int,int)), SLOT(updateActions()));
    connect(tabBar(), SIGNAL(tabCloseRequested(int)), SLOT(onTabCloseRequest(int)));
    connect(this, SIGNAL(currentChanged(int)), SLOT(onCurrentChanged(int)));
    connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(onActionTriggered(QAction*)));
    readSettings();
}

QMenu *QSUiTabWidget::menu()
{
    return m_menu;
}

void QSUiTabWidget::onTabCloseRequest(int i)
{
    if(!tabsClosable())
        emit tabCloseRequested(i);
}

void QSUiTabWidget::onCurrentChanged(int index)
{
    if(index >= m_menu->actions().count())
        return;
    m_menu->actions().at(index)->setChecked(true);
}

void QSUiTabWidget::onActionTriggered(QAction *action)
{
    setCurrentIndex(m_menu->actions().indexOf(action));
}

void QSUiTabWidget::updateActions()
{
    for(int i = 0; i < m_menu->actions().size(); ++i)
    {
         m_menu->actions().at(i)->setText(tabText(i));
    }
    m_menu->actions().at(currentIndex())->setChecked(true);
}

void QSUiTabWidget::tabInserted(int index)
{
    QAction *action = new QAction(m_menu);
    action->setCheckable(true);
    action->setActionGroup(m_group);
    action->setText(tabText(index));

    if(m_menu->actions().isEmpty() || index == m_menu->actions().count())
    {
        m_menu->addAction(action);
    }
    else
    {
        QAction *before = m_menu->actions().at(index);
        m_menu->insertAction(before, action);
    }
    if(currentIndex() == index)
        action->setChecked(true);
    QTabWidget::tabInserted(index);
}

void QSUiTabWidget::tabRemoved(int index)
{
    QAction *a = m_menu->actions().at(index);
    m_menu->removeAction(a);
    delete a;
    QTabWidget::tabRemoved(index);
}

void QSUiTabWidget::setTabText(int index, const QString &text)
{
    QTabWidget::setTabText(index, text);
    m_menu->actions().at(index)->setText(text);
}

void QSUiTabWidget::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Simple");
    QFont tab_font = qApp->font(tabBar());
    if(!settings.value("use_system_fonts", true).toBool())
    {
        tab_font.fromString(settings.value("pl_tabs_font", tab_font.toString()).toString());
    }
    tabBar()->setFont(tab_font);
}

void QSUiTabWidget::setTabsVisible(bool visible)
{
    tabBar()->setVisible(visible);
}

void QSUiTabWidget::mousePressEvent(QMouseEvent *e)
{
    if((e->button() == Qt::MidButton) &&
            (childAt(e->pos()) == 0))
    {
        e->accept();
        emit createPlayListRequested();
    }
    QTabWidget::mousePressEvent(e);
}

void QSUiTabWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if((e->button() == Qt::LeftButton) &&
            (childAt(e->pos()) == 0))
    {
        e->accept();
        emit createPlayListRequested();
    }
    QTabWidget::mouseDoubleClickEvent(e);
}
