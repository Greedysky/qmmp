/***************************************************************************
 *   Copyright (C) 2011-2015 by Ilya Kotov                                 *
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

#include <QMessageBox>
#include "actionmanager.h"
#include "hotkeyeditor.h"
#include "shortcutdialog.h"
#include "shortcutitem.h"
#include "ui_hotkeyeditor.h"

HotkeyEditor::HotkeyEditor(QWidget *parent) : QWidget(parent), m_ui(new Ui::HotkeyEditor)
{
    m_ui->setupUi(this);
    loadShortcuts();
    m_ui->changeShortcutButton->setIcon(QIcon::fromTheme("configure"));
}

HotkeyEditor::~HotkeyEditor()
{
    delete m_ui;
}

void HotkeyEditor::on_changeShortcutButton_clicked()
{
    ShortcutItem *item = dynamic_cast<ShortcutItem *> (m_ui->shortcutTreeWidget->currentItem());
    if(!item)
        return;
    ShortcutDialog editor(item->action()->shortcut().toString(), this);
    if(editor.exec() == QDialog::Accepted)
    {
        item->action()->setShortcut(editor.key());
        item->setText(1, item->action()->shortcut().toString());
    }
}

void HotkeyEditor::on_resetShortcutsButton_clicked()
{
    if(QMessageBox::question(this, tr("Reset Shortcuts"),
                             tr("Do you want to restore default shortcuts?"),
                             QMessageBox::Yes | QMessageBox::No) ==  QMessageBox::Yes)
    {
        ActionManager::instance()->resetShortcuts();
        loadShortcuts();
    }
}

void HotkeyEditor::loadShortcuts()
{
    m_ui->shortcutTreeWidget->clear();
    //playback
    QTreeWidgetItem *item = new QTreeWidgetItem (m_ui->shortcutTreeWidget, QStringList() << tr("Playback"));
    for(int i = ActionManager::PLAY; i <= ActionManager::CLEAR_QUEUE; ++i)
        new ShortcutItem(item, i);
    item->setExpanded(true);
    m_ui->shortcutTreeWidget->addTopLevelItem(item);
    //view
    item = new QTreeWidgetItem (m_ui->shortcutTreeWidget, QStringList() << tr("View"));
    for(int i = ActionManager::WM_ALLWAYS_ON_TOP; i <= ActionManager::UI_BLOCK_TOOLBARS; ++i)
        new ShortcutItem(item, i);
    item->setExpanded(true);
    m_ui->shortcutTreeWidget->addTopLevelItem(item);
    //volume
    item = new QTreeWidgetItem (m_ui->shortcutTreeWidget, QStringList() << tr("Volume"));
    for(int i = ActionManager::VOL_ENC; i <= ActionManager::VOL_MUTE; ++i)
        new ShortcutItem(item, i);
    item->setExpanded(true);
    m_ui->shortcutTreeWidget->addTopLevelItem(item);
    //playlist
    item = new QTreeWidgetItem (m_ui->shortcutTreeWidget, QStringList() << tr("Playlist"));
    for(int i = ActionManager::PL_ADD_FILE; i <= ActionManager::PL_SHOW_HEADER; ++i)
        new ShortcutItem(item, i);
    item->setExpanded(true);
    m_ui->shortcutTreeWidget->addTopLevelItem(item);
    //misc
    item = new QTreeWidgetItem (m_ui->shortcutTreeWidget, QStringList() << tr("Misc"));
    for(int i = ActionManager::EQUALIZER; i <= ActionManager::QUIT; ++i)
        new ShortcutItem(item, i);
    item->setExpanded(true);
    m_ui->shortcutTreeWidget->addTopLevelItem(item);

    m_ui->shortcutTreeWidget->resizeColumnToContents(0);
    m_ui->shortcutTreeWidget->resizeColumnToContents(1);
}
