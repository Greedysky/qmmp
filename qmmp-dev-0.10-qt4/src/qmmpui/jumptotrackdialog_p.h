/***************************************************************************
 *   Copyright (C) 2007-2016 by Ilya Kotov                                 *
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

#ifndef JUMPTOTRACKDIALOG_P_H
#define JUMPTOTRACKDIALOG_P_H

#include <QDialog>
#include <QList>
#include "metadataformatter.h"
#include "ui_jumptotrackdialog.h"

class QStringListModel;
class QSortFilterProxyModel;
class QEvent;
class PlayListManager;
class PlayListModel;

/**
   @internal
   @author Vladimir Kuznetsov <vovanec@gmail.com>
 */
class JumpToTrackDialog : public QDialog, private Ui::JumpToTrackDialog
{
    Q_OBJECT

public:
    JumpToTrackDialog(PlayListModel *model, QWidget* parent = 0);
    ~JumpToTrackDialog();
    void refresh();

private slots:
    void on_refreshPushButton_clicked();
    void on_queuePushButton_clicked();
    void on_jumpToPushButton_clicked();
    void jumpTo(const QModelIndex&);
    void queueUnqueue(const QModelIndex&,const QModelIndex&);

private:
    bool eventFilter(QObject *o, QEvent *e);
    QStringListModel* m_listModel;
    QSortFilterProxyModel* m_proxyModel;
    PlayListManager *m_pl_manager;
    PlayListModel *m_model;
    QList<int> m_indexes;
    MetaDataFormatter m_titleFormatter;
};

#endif //JUMPTOTRACKDIALOG_P_H
