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

#include <QAction>
#include <QApplication>
#include <QStyle>
#include <QKeyEvent>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QIcon>
#include <qmmpui/playlistmanager.h>
#include "playlistbrowser.h"

PlayListBrowser::PlayListBrowser(PlayListManager *manager, QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowModality(Qt::NonModal);
    m_ui.setupUi(this);
    m_ui.filterLineEdit->installEventFilter(this);
    m_pl_manager = manager;
    connect(m_pl_manager, SIGNAL(playListsChanged()), SLOT(updateList()));
    connect(m_ui.newButton, SIGNAL(clicked()), m_pl_manager, SLOT(createPlayList()));
    //actions
    QAction *renameAct = new QAction(tr("Rename"), this);
    QAction *removeAct = new QAction(QIcon::fromTheme("window-close"), tr("Delete"), this);
    connect(renameAct,SIGNAL(triggered()), SLOT(rename()));
    connect(removeAct,SIGNAL(triggered()), SLOT(on_deleteButton_clicked()));
    m_ui.listView->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_ui.listView->addAction(renameAct);
    m_ui.listView->addAction(removeAct);
    m_listModel = new QStandardItemModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setDynamicSortFilter(true);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setSourceModel(m_listModel);
    m_ui.listView->setModel(m_proxyModel);
    m_ui.downButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowDown));
    m_ui.upButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowUp));
    m_ui.newButton->setIcon(QIcon::fromTheme("document-new"));
    m_ui.deleteButton->setIcon(QIcon::fromTheme("edit-delete"));
    connect(m_listModel, SIGNAL(itemChanged(QStandardItem*)), SLOT(updatePlayListName(QStandardItem*)));
    connect(m_ui.listView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(updateCurrentRow(QModelIndex,QModelIndex)));
    updateList();
}

PlayListBrowser::~PlayListBrowser()
{}

void PlayListBrowser::updateList()
{
    m_ui.listView->selectionModel()->blockSignals(true);
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
            m_ui.listView->setCurrentIndex(m_proxyModel->mapFromSource(m_listModel->indexFromItem(item)));
        }
    }
    m_ui.listView->selectionModel()->blockSignals(false);
}

void PlayListBrowser::on_filterLineEdit_textChanged(const QString &str)
{
    m_ui.listView->selectionModel()->blockSignals(true);
    m_proxyModel->setFilterFixedString(str);
    m_ui.listView->selectionModel()->blockSignals(false);
}

void PlayListBrowser::on_listView_activated(const QModelIndex &index)
{
    int row = m_proxyModel->mapToSource(index).row();
    if(row >= 0)
    {
        m_pl_manager->activatePlayList(row);
        m_pl_manager->selectPlayList(row);
    }
}

void PlayListBrowser::updatePlayListName(QStandardItem *item)
{
    disconnect(m_pl_manager, SIGNAL(playListsChanged()), this, SLOT(updateList()));
    m_pl_manager->playListAt(item->row())->setName(item->text());
    connect(m_pl_manager, SIGNAL(playListsChanged()), SLOT(updateList()));
}

void PlayListBrowser::updateCurrentRow(QModelIndex index, QModelIndex)
{
    int row = m_proxyModel->mapToSource(index).row();
    if(row >= 0)
        m_pl_manager->selectPlayList(row);
}

void PlayListBrowser::rename()
{
    QModelIndex index = m_ui.listView->currentIndex();
    if(!index.isValid())
        return;
    m_ui.listView->edit(index);
}

void PlayListBrowser::on_deleteButton_clicked()
{
    QModelIndexList selectedIndexes;
    foreach(QModelIndex index, m_ui.listView->selectionModel()->selectedIndexes())
    {
        selectedIndexes << m_proxyModel->mapToSource(index);
    }
    foreach (QModelIndex index, selectedIndexes)
    {
        m_pl_manager->removePlayList(index.row());
    }
}

void PlayListBrowser::on_downButton_clicked()
{
    int pos = m_pl_manager->indexOf(m_pl_manager->selectedPlayList());
    if(pos < m_pl_manager->count() - 1)
        m_pl_manager->move(pos, pos + 1);
}

void PlayListBrowser::on_upButton_clicked()
{
    int pos = m_pl_manager->indexOf(m_pl_manager->selectedPlayList());
    if(pos > 0)
        m_pl_manager->move(pos, pos - 1);
}

bool PlayListBrowser::eventFilter(QObject *o, QEvent *e)
{
    if(o == m_ui.filterLineEdit && e->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_event = static_cast<QKeyEvent *>(e);
        QModelIndex index = m_ui.listView->currentIndex();
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
                m_ui.listView->setCurrentIndex(index);
            return true;
        }
        else if(key_event->key() == Qt::Key_Down)
        {
            if(!select_first)
                index = m_proxyModel->index(index.row() + 1, index.column());
            if(index.isValid())
                m_ui.listView->setCurrentIndex(index);
            return true;
        }
    }
    return QDialog::eventFilter(o, e);
}
