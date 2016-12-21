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

#ifndef QSUITABWIDGET_H
#define QSUITABWIDGET_H

#include <QTabWidget>
#include <QMenu>

class QActionGroup;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class QSUiTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit QSUiTabWidget(QWidget *parent = 0);

    QMenu *menu();
    void setTabText(int index, const QString &text);
    void readSettings();

signals:
    void tabMoved(int from, int to);
    void createPlayListRequested();

public slots:
    void setTabsVisible(bool visible);

private slots:
    void onTabCloseRequest(int i);
    void onCurrentChanged(int index);
    void onActionTriggered(QAction *action);
    void updateActions();

private:
    void tabInserted(int index);
    void tabRemoved(int index);
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    QMenu *m_menu;
    QActionGroup *m_group;

};

#endif // QSUITABWIDGET_H
