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

#ifndef TOOLBAREDITOR_H
#define TOOLBAREDITOR_H

#include <QDialog>
#include <QModelIndex>
#include <QIcon>
#include "actionmanager.h"

namespace Ui {
class ToolBarEditor;
}

class QListWidgetItem;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class ToolBarEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ToolBarEditor(QWidget *parent);
    ~ToolBarEditor();

public slots:
    void accept();

private slots:
    void on_addToolButton_clicked();
    void on_removeToolButton_clicked();
    void on_upToolButton_clicked();
    void on_downToolButton_clicked();
    void on_resetPushButton_clicked();
    void on_toolbarNameComboBox_activated(int index);
    void onRowsAboutToBeRemoved(const QModelIndex &, int start, int);
    void on_createButton_clicked();
    void on_renameButton_clicked();
    void on_removeButton_clicked();

private:
    void populateActionList(bool reset = false);
    QListWidgetItem *createExtraItem(const QString &name, const QString &shortName, const QIcon &icon = QIcon());
    Ui::ToolBarEditor *m_ui;
    QList<ActionManager::ToolBarInfo> m_toolBarInfoList;
    int m_previousIndex;
};

#endif // TOOLBAREDITOR_H
