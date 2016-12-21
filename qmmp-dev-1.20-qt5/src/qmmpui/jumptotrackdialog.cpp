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

#include "jumptotrackdialog_p.h"
#include "playlistmanager.h"
#include "mediaplayer.h"
#include <qmmp/soundcore.h>
#include <QAction>
#include <QEvent>
#include <QKeyEvent>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QShortcut>
#include <QKeySequence>

JumpToTrackDialog::JumpToTrackDialog(PlayListModel *model, QWidget* parent)
        : QDialog (parent)
{
    setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_model = model;
    m_pl_manager = PlayListManager::instance();
    m_listModel = new QStringListModel(this);
    m_titleFormatter.setPattern("%if(%p,%p - %t,%t)");

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setDynamicSortFilter(true);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setSourceModel(m_listModel);
    m_proxyModel->setSortLocaleAware(true);
    songsListView->setModel(m_proxyModel);

    connect(songsListView,SIGNAL(doubleClicked(QModelIndex)),SLOT(jumpTo(QModelIndex)));
    connect(songsListView,SIGNAL(activated(QModelIndex)),SLOT(accept()));
    connect(songsListView->selectionModel(),SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            SLOT(queueUnqueue(QModelIndex, QModelIndex)));

    connect(m_model, SIGNAL(destroyed()), SLOT(close()));

    new QShortcut(tr("Q"),this,SLOT(on_queuePushButton_clicked()));
    new QShortcut(tr("J"),this,SLOT(on_jumpToPushButton_clicked()));
    new QShortcut(tr("F5"),this,SLOT(on_refreshPushButton_clicked()));

    filterLineEdit->installEventFilter(this);
    connect(filterLineEdit, SIGNAL(textChanged(QString)),
            m_proxyModel, SLOT(setFilterFixedString(QString)));

    //setup icons
    refreshPushButton->setIcon(QIcon::fromTheme("view-refresh"));
    jumpToPushButton->setIcon(QIcon::fromTheme("go-top"));
}

JumpToTrackDialog::~JumpToTrackDialog()
{
}

void JumpToTrackDialog::on_refreshPushButton_clicked()
{
    refresh();
}

void JumpToTrackDialog::on_queuePushButton_clicked()
{
    QModelIndexList mi_list = songsListView->selectionModel()->selectedRows();
    if (!mi_list.isEmpty())
    {
        int selected = (m_proxyModel->mapToSource(mi_list.at(0))).row();
        m_model->setQueued(m_model->track(m_indexes[selected]));
        if (m_model->isQueued(m_model->track(m_indexes[selected])))
            queuePushButton->setText(tr("Unqueue"));
        else
            queuePushButton->setText(tr("Queue"));
    }
}

void JumpToTrackDialog::on_jumpToPushButton_clicked()
{
    QModelIndexList mi_list = songsListView->selectionModel()->selectedRows();
    if (!mi_list.isEmpty())
    {
        jumpTo(mi_list.at(0));
    }
}

void JumpToTrackDialog::refresh()
{
    filterLineEdit->clear();
    m_indexes.clear();
    QStringList titles;
    QList<PlayListItem *> items = m_model->items();
    for(int i = 0; i < items.count(); ++i)
    {
        if(items[i]->isGroup())
            continue;
        titles.append(m_titleFormatter.format(dynamic_cast<PlayListTrack *>(items[i])));
        m_indexes.append(i);
    }
    m_listModel->setStringList(titles);
    filterLineEdit->setFocus();
}

void JumpToTrackDialog::jumpTo(const QModelIndex & index)
{
    int selected = (m_proxyModel->mapToSource(index)).row();
    m_model->setCurrent(m_indexes[selected]);
    SoundCore::instance()->stop();
    m_pl_manager->activatePlayList(m_model);
    MediaPlayer::instance()->play();
}

void JumpToTrackDialog::queueUnqueue(const QModelIndex& curr,const QModelIndex&)
{
    if(!curr.isValid())
        return;
    int row = m_proxyModel->mapToSource(curr).row();
    if (m_model->isQueued(m_model->track(m_indexes[row])))
        queuePushButton->setText(tr("Unqueue"));
    else
        queuePushButton->setText(tr("Queue"));
}

bool JumpToTrackDialog::eventFilter(QObject *o, QEvent *e)
{
    if(o == filterLineEdit && e->type() == QEvent::KeyPress)
    {
        QKeyEvent *key_event = static_cast<QKeyEvent *>(e);
        QModelIndex index = songsListView->currentIndex();
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
                songsListView->setCurrentIndex(index);
            return true;
        }
        else if(key_event->key() == Qt::Key_Down)
        {
            if(!select_first)
                index = m_proxyModel->index(index.row() + 1, index.column());
            if(index.isValid())
                songsListView->setCurrentIndex(index);
            return true;
        }
        else if(key_event->key() == Qt::Key_Return)
        {
            if(index.isValid())
            {
                jumpTo(index);
                accept();
            }
            return true;
        }
    }
    return QDialog::eventFilter(o, e);
}
