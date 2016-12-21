/***************************************************************************
 *   Copyright (C) 2013-2016 by Ilya Kotov                                 *
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

#include <QApplication>
#include <QSettings>
#include <QToolBar>
#include <QMenu>
#include <QWidgetAction>
#include <QUuid>
#include <QInputDialog>
#include <qmmp/qmmp.h>
#include "toolbareditor.h"
#include "ui_toolbareditor.h"

ToolBarEditor::ToolBarEditor(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ToolBarEditor)
{
    m_ui->setupUi(this);
    m_ui->upToolButton->setIcon(qApp->style()->standardIcon(QStyle::SP_ArrowUp));
    m_ui->downToolButton->setIcon(qApp->style()->standardIcon(QStyle::SP_ArrowDown));
    m_ui->addToolButton->setIcon(qApp->style()->standardIcon(QStyle::SP_ArrowRight));
    m_ui->removeToolButton->setIcon(qApp->style()->standardIcon(QStyle::SP_ArrowLeft));

    connect(m_ui->actionsListWidget->model(), SIGNAL(rowsAboutToBeRemoved(const QModelIndex &,int,int)),
            SLOT(onRowsAboutToBeRemoved(const QModelIndex &, int, int)));
    connect(m_ui->activeActionsListWidget->model(), SIGNAL(rowsAboutToBeRemoved(const QModelIndex &,int,int)),
            SLOT(onRowsAboutToBeRemoved(const QModelIndex &, int, int)));

    m_toolBarInfoList = ActionManager::instance()->readToolBarSettings();

    m_previousIndex = -1;
    populateActionList();
}

ToolBarEditor::~ToolBarEditor()
{
    delete m_ui;
}

void ToolBarEditor::accept()
{
    on_toolbarNameComboBox_activated(m_ui->toolbarNameComboBox->currentIndex());
    ActionManager::instance()->writeToolBarSettings(m_toolBarInfoList);
    QDialog::accept();
}

void ToolBarEditor::populateActionList(bool reset)
{
    m_ui->toolbarNameComboBox->clear();
    m_ui->actionsListWidget->clear();
    m_ui->activeActionsListWidget->clear();

    if(reset)
    {
        m_toolBarInfoList.clear();
        m_toolBarInfoList.append(ActionManager::instance()->defaultToolBar());
        m_previousIndex = -1;
    }

    QStringList actionNames;
    foreach (ActionManager::ToolBarInfo info, m_toolBarInfoList)
    {
        actionNames << info.actionNames;
        m_ui->toolbarNameComboBox->addItem(info.title);
    }

    for(int id = ActionManager::PLAY; id <= ActionManager::UI_VOL_SLIDER; ++id)
    {
        QAction *action = ACTION(id);
        if(!action || actionNames.contains(action->objectName()))
            continue;
        if(!qobject_cast<QWidgetAction *>(action) && action->icon().isNull())
            continue;

        QListWidgetItem *item = new QListWidgetItem();
        item->setIcon(action->icon());
        item->setText(action->text().replace("&", ""));
        item->setData(Qt::UserRole, action->objectName());
        m_ui->actionsListWidget->addItem(item);
    }

    m_ui->actionsListWidget->addItem(createExtraItem("-- " + tr("Separator") + " --", "separator"));
    on_toolbarNameComboBox_activated(m_ui->toolbarNameComboBox->currentIndex());
}

QListWidgetItem *ToolBarEditor::createExtraItem(const QString &name, const QString &shortName, const QIcon &icon)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(name);
    item->setData(Qt::UserRole, shortName);
    item->setIcon(icon);
    return item;
}

void ToolBarEditor::on_addToolButton_clicked()
{
    int index = m_ui->toolbarNameComboBox->currentIndex();
    if(index < 0)
        return;

    int row = m_ui->actionsListWidget->currentRow();
    if(row > -1)
    {
        QListWidgetItem *item = m_ui->actionsListWidget->takeItem(row);
        m_ui->activeActionsListWidget->addItem(item);
    }
}

void ToolBarEditor::on_removeToolButton_clicked()
{
    int index = m_ui->toolbarNameComboBox->currentIndex();
    if(index < 0)
        return;

    int row = m_ui->activeActionsListWidget->currentRow();
    if(row > -1)
    {
        QListWidgetItem *item = m_ui->activeActionsListWidget->takeItem(row);
        m_ui->actionsListWidget->addItem(item);
    }
}

void ToolBarEditor::on_upToolButton_clicked()
{
    int index = m_ui->toolbarNameComboBox->currentIndex();
    if(index < 0)
        return;

    int row = m_ui->activeActionsListWidget->currentRow();
    if(row > 0)
    {
        QListWidgetItem *item = m_ui->activeActionsListWidget->takeItem(row);
        m_ui->activeActionsListWidget->insertItem(row - 1, item);
        m_ui->activeActionsListWidget->setCurrentItem(item);
    }
}

void ToolBarEditor::on_downToolButton_clicked()
{
    int index = m_ui->toolbarNameComboBox->currentIndex();
    if(index < 0)
        return;

    int row = m_ui->activeActionsListWidget->currentRow();
    if(row > -1 && row < m_ui->activeActionsListWidget->count())
    {
        QListWidgetItem *item = m_ui->activeActionsListWidget->takeItem(row);
        m_ui->activeActionsListWidget->insertItem(row + 1, item);
        m_ui->activeActionsListWidget->setCurrentItem(item);
    }
}

void ToolBarEditor::on_resetPushButton_clicked()
{
    populateActionList(true);
}

void ToolBarEditor::on_toolbarNameComboBox_activated(int index)
{
    if(m_previousIndex >= 0 && m_previousIndex < m_toolBarInfoList.count())
    {
        m_toolBarInfoList[m_previousIndex].actionNames.clear();
        for(int i = 0; i < m_ui->activeActionsListWidget->count(); ++i)
        {
            QListWidgetItem *item = m_ui->activeActionsListWidget->item(i);
            m_toolBarInfoList[m_previousIndex].actionNames << item->data(Qt::UserRole).toString();
        }
    }
    m_ui->activeActionsListWidget->clear();
    m_previousIndex = index;

    if(index < 0)
        return;
    ActionManager::ToolBarInfo info = m_toolBarInfoList.at(index);

    foreach (QString name, info.actionNames)
    {
        if(name == "separator")
        {
            m_ui->activeActionsListWidget->addItem(createExtraItem("-- " + tr("Separator") + " --", name));
            continue;
        }

        QAction *action = ActionManager::instance()->findChild<QAction *>(name);
        if(action)
        {
            QListWidgetItem *item = new QListWidgetItem();
            item->setIcon(action->icon());
            item->setText(action->text().replace("&", ""));
            item->setData(Qt::UserRole, action->objectName());
            m_ui->activeActionsListWidget->addItem(item);
        }
    }
}

void ToolBarEditor::onRowsAboutToBeRemoved(const QModelIndex &, int start, int)
{
    if(sender() == m_ui->actionsListWidget->model())
    {
        //recreate separator
        QListWidgetItem *item = m_ui->actionsListWidget->item(start);
        if(item && item->data(Qt::UserRole).toString() == "separator")
            m_ui->actionsListWidget->addItem(item->clone());
    }
    else if(sender() == m_ui->activeActionsListWidget->model())
    {
        //remove separator
        QListWidgetItem *item = m_ui->activeActionsListWidget->item(start);
        if(item && item->data(Qt::UserRole).toString() == "separator")
        {
            for(int i = 0; i < m_ui->actionsListWidget->count(); ++i)
            {
                if(m_ui->actionsListWidget->item(i)->data(Qt::UserRole).toString() == "separator")
                {
                    m_ui->actionsListWidget->model()->blockSignals(true);
                    delete m_ui->actionsListWidget->takeItem(i);
                    m_ui->actionsListWidget->model()->blockSignals(false);
                    break;
                }
            }
        }
    }
}

void ToolBarEditor::on_createButton_clicked()
{
    ActionManager::ToolBarInfo info;
    int i = 0;
    //generate unique toolbar name
    QString title = tr("Toolbar");
    while (m_ui->toolbarNameComboBox->findText(title) >= 0)
        title = tr("Toolbar %1").arg(++i);

    info.title = title;
    info.uid = QUuid::createUuid().toString();
    m_toolBarInfoList.append(info);
    m_ui->toolbarNameComboBox->addItem(info.title);
}

void ToolBarEditor::on_renameButton_clicked()
{
    int index = m_ui->toolbarNameComboBox->currentIndex();
    if(index >= 0)
    {
        QString title = m_toolBarInfoList[index].title;
        title = QInputDialog::getText(this, tr("Rename Toolbar"), tr("Toolbar name:"),
                                      QLineEdit::Normal, title);
        if(!title.isEmpty())
        {
            m_toolBarInfoList[index].title = title;
            m_ui->toolbarNameComboBox->setItemText(index, title);
        }
    }
}

void ToolBarEditor::on_removeButton_clicked()
{
    if(m_ui->toolbarNameComboBox->count() == 1)
        return;

    int index = m_ui->toolbarNameComboBox->currentIndex();
    if(index >= 0)
    {
        m_ui->toolbarNameComboBox->removeItem(index);
        m_toolBarInfoList.removeAt(index);
    }

    populateActionList();
}
