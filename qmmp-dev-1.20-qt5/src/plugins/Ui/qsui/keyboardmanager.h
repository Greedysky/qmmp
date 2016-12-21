/***************************************************************************
 *   Copyright (C) 2011-2013 by Ilya Kotov                                 *
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

#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H

#include <QObject>
#include <QList>

class QAction;
class ListWidget;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class KeyboardManager : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardManager(QObject *parent = 0);

    QList<QAction*> actions();
    void setListWidget(ListWidget *listWidget);

private slots:
    void processUp();
    void processDown();
    void processEnter();
    void processPgUp();
    void processPgDown();
    void processHome();
    void processEnd();

private:
    QList<QAction*> m_actions;
    ListWidget *m_listWidget;
    void addAction(int keys, const char * method);

    enum SelectMode
    {
        SELECT_TOP = 0,
        SELECT_BOTTOM,
        SELECT_NEXT
    };

};

#endif // KEYBOARDMANAGER_H
