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

#include <QAction>
#include <QApplication>
#include <QStyle>
#include <QKeyEvent>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QListView>
#include <QLineEdit>
#include <QVBoxLayout>
#include <qmmpui/playlistmanager.h>
#include "actionmanager.h"
#include "playlistbrowser.h"

PlayListBrowser::PlayListBrowser(PlayListManager *manager, QWidget *parent) : QWidget(parent)
{
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->installEventFilter(this);
    m_lineEdit->setContentsMargins(5,5,5,0);
    m_listView = new QListView(this);
    m_listView->setFrameStyle(QFrame::NoFrame);
    m_listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_listView->installEventFilter(this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_listView);
    setLayout(layout);

    m_pl_manager = manager;
    connect(m_pl_manager, SIGNAL(playListsChanged()), SLOT(updateList()));
    //actions
    m_listView->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_listView->addAction(ACTION(ActionManager::PL_RENAME));
    m_listView->addAction(ACTION(ActionManager::PL_CLOSE));

    m_listModel = new QStandardItemModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setDynamicSortFilter(true);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setSourceModel(m_listModel);
    m_listView->setModel(m_proxyModel);
    connect(m_lineEdit, SIGNAL(textChanged(QString)), SLOT(onLineEditTextChanged(QString)));
    connect(m_listView, SIGNAL(activated(QModelIndex)), SLOT(onListViewActivated(QModelIndex)));
    connect(m_listView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(updateCurrentRow(QModelIndex,QModelIndex)));
    updateList();
}

PlayListBrowser::~PlayListBrowser()
{}

void PlayListBrowser::updateList()
{
    m_listView->selectionModel()->blockSignals(true);
    m_listModel->clear();
    foreach(PlayListModel *model, m_pl_manager->playLists())
    {
        QStandardItem *item = new QStandardItem(model->name());
        if(m_pl_manager->currentPlayList() == model)
        {
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
        }
        m_listModel->appendRow(item);
        if(m_pl_manager->selectedPlayList() == model)
        {
            m_listView->setCurrentIndex(m_proxyModel->mapFromSource(m_listModel->indexFromItem(item)));
        }
    }
    m_listView->selectionModel()->blockSignals(false);
}

void PlayListBrowser::onLineEditTextChanged(const QString &str)
{
    m_listView->selectionModel()->blockSignals(true);
    m_proxyModel->setFilterFixedString(str);
    m_listView->selectionModel()->blockSignals(false);
}

void PlayListBrowser::onListViewActivated(const QModelIndex &index)
{
    int row = m_proxyModel->mapToSource(index).row();
    if(row >= 0)
    {
        m_pl_manager->activatePlayList(row);
        m_pl_manager->selectPlayList(row);
    }
}

void PlayListBrowser::updateCurrentRow(QModelIndex index, QModelIndex)
{
    int row = m_proxyModel->mapToSource(index).row();
    if(row >= 0)
        m_pl_manager->selectPlayList(row);
}

bool PlayListBrowser::eventFilter(QObject *o, QEvent *e)
{
    if((o == m_lineEdit || o == m_listView) && e->type() == QEvent::ShortcutOverride)
    {
        e->accept();
        return false;
    }

    if(o == m_lineEdit && e->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_event = static_cast<QKeyEvent *>(e);
        QModelIndex index = m_listView->currentIndex();
        bool select_first = false;
        if(!index.isValid() && m_proxyModel->rowCount())
        {
            select_first = true;
            index = m_proxyModel->index(0,0);
        }

        if(key_event->key() == Qt::Key_Up)
        {
            if(!select_first)
                index = m_proxyModel->index(index.row() - 1, index.column());
            if(index.isValid())
                m_listView->setCurrentIndex(index);
            return true;
        }
        else if(key_event->key() == Qt::Key_Down)
        {
            if(!select_first)
                index = m_proxyModel->index(index.row() + 1, index.column());
            if(index.isValid())
                m_listView->setCurrentIndex(index);
            return true;
        }
    }
    return QWidget::eventFilter(o, e);
}
