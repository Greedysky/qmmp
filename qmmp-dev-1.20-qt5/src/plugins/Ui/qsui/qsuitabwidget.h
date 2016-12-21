/***************************************************************************
 *   Copyright (C) 2016 The Qt Company Ltd.                                *
 *   Copyright (C) 2016 by Ilya Kotov                                      *
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
#ifndef QSUITABWIDGET_H
#define QSUITABWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QActionGroup>
#include "qsuitabbar.h"
#include "listwidget.h"

class QStyleOptionTabWidgetFrame;
/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class QSUiTabWidget : public QWidget
{
    Q_OBJECT
public:
    QSUiTabWidget(QWidget *parent = 0);

    QSUiTabBar *tabBar() const;
    ListWidget *listWidget() const;
    void setCornerWidget(QWidget *widget, Qt::Corner corner = Qt::TopRightCorner);
    void readSettings();

    QMenu *menu() const;
    void setTabText(int index, const QString &text);

    int addTab(const QString &label);
    int addTab(const QIcon& icon, const QString &label);

    int insertTab(int index, const QString &label);
    int insertTab(int index, const QIcon& icon, const QString &label);

    void removeTab(int index);

signals:
    void tabMoved(int from, int to);
    void createPlayListRequested();
    void tabCloseRequested(int);
    void currentChanged(int);

public slots:
    void setTabsVisible(bool visible);
    void setCurrentIndex(int index);

private slots:
    void onCurrentChanged(int index);
    void onActionTriggered(QAction *action);
    void updateActions();

private:
    void initStyleOption(QStyleOptionTabWidgetFrame *option) const;
    void setUpLayout();
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *);
    void setTabBar(QTabBar *);
    void changeEvent(QEvent *ev);
    bool event(QEvent *ev);
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void tabInserted(int index);
    void tabRemoved(int index);

    QSUiTabBar *m_tabBar;
    ListWidget *m_listWidget;
    QWidget *m_rightCornerWidget;
    QWidget *m_leftCornerWidget;
    QRect m_panelRect;
    QMenu *m_menu;
    QActionGroup *m_group;
};


#endif
