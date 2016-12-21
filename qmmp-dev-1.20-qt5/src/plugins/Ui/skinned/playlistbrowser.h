/***************************************************************************
 *   Copyright (C) 2009-2013 by Ilya Kotov                                 *
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

#include <QDialog>
#include "ui_playlistbrowser.h"

class PlayListManager;
class QStandardItemModel;
class QSortFilterProxyModel;
class QStandardItem;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class PlayListBrowser : public QDialog
{
Q_OBJECT
public:
    PlayListBrowser(PlayListManager *manager, QWidget *parent = 0);
    ~PlayListBrowser();

private slots:
    void updateList();
    void on_filterLineEdit_textChanged(const QString &str);
    void on_listView_activated(const QModelIndex & index);
    void updatePlayListName(QStandardItem *item);
    void updateCurrentRow(QModelIndex index, QModelIndex);
    void rename();
    void on_deleteButton_clicked();
    void on_downButton_clicked();
    void on_upButton_clicked();

private:
    bool eventFilter(QObject *o, QEvent *e);
    Ui::PlayListBrowser m_ui;
    PlayListManager *m_pl_manager;
    QStandardItemModel* m_listModel;
    QSortFilterProxyModel* m_proxyModel;
};

#endif // PLAYLISTBROWSER_H
