/***************************************************************************
 *   Copyright (C) 2013 by Ilya Kotov                                      *
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

#ifndef PLAYLISTBROWSER_H
#define PLAYLISTBROWSER_H

#include <QWidget>
#include <QModelIndex>

class PlayListManager;
class QStandardItemModel;
class QSortFilterProxyModel;
class QStandardItem;
class QListView;
class QLineEdit;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class PlayListBrowser : public QWidget
{
Q_OBJECT
public:
    PlayListBrowser(PlayListManager *manager, QWidget *parent = 0);
    ~PlayListBrowser();

private slots:
    void updateList();
    void onLineEditTextChanged(const QString &str);
    void onListViewActivated(const QModelIndex &index);
    void updateCurrentRow(QModelIndex index, QModelIndex);

private:
    bool eventFilter(QObject *o, QEvent *e);
    QLineEdit *m_lineEdit;
    QListView *m_listView;
    PlayListManager *m_pl_manager;
    QStandardItemModel* m_listModel;
    QSortFilterProxyModel* m_proxyModel;
};

#endif // PLAYLISTBROWSER_H
