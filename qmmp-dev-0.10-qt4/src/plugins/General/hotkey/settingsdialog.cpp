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

#include <QSettings>
#include <QHeaderView>
#include <QMessageBox>
#include <qmmp/qmmp.h>
#include "hotkeydialog.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    m_ui.setupUi(this);
    m_ui.tableWidget->verticalHeader()->setDefaultSectionSize(fontMetrics().height());
    m_ui.tableWidget->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    m_ui.tableWidget->verticalHeader()->hide();
    m_ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_ui.tableWidget->setRowCount (13);
    m_ui.tableWidget->setItem(0,0, new QTableWidgetItem(tr("Play")));
    m_ui.tableWidget->setItem(1,0, new QTableWidgetItem(tr("Stop")));
    m_ui.tableWidget->setItem(2,0, new QTableWidgetItem(tr("Pause")));
    m_ui.tableWidget->setItem(3,0, new QTableWidgetItem(tr("Play/Pause")));
    m_ui.tableWidget->setItem(4,0, new QTableWidgetItem(tr("Next")));
    m_ui.tableWidget->setItem(5,0, new QTableWidgetItem(tr("Previous")));
    m_ui.tableWidget->setItem(6,0, new QTableWidgetItem(tr("Show/Hide")));
    m_ui.tableWidget->setItem(7,0, new QTableWidgetItem(tr("Volume +")));
    m_ui.tableWidget->setItem(8,0, new QTableWidgetItem(tr("Volume -")));
    m_ui.tableWidget->setItem(9,0, new QTableWidgetItem(tr("Forward 5 seconds")));
    m_ui.tableWidget->setItem(10,0, new QTableWidgetItem(tr("Rewind 5 seconds")));
    m_ui.tableWidget->setItem(11,0, new QTableWidgetItem(tr("Jump to track")));
    m_ui.tableWidget->setItem(12,0, new QTableWidgetItem(tr("Mute")));

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Hotkey");
    for (int i = Hotkey::PLAY, j = 0; i <= Hotkey::VOLUME_MUTE; ++i, ++j)
    {
        Hotkey *hotkey = new Hotkey;
        hotkey->action = i;
        hotkey->key = settings.value(QString("key_%1").arg(i), hotkey->defaultKey()).toUInt();
        hotkey->mod = settings.value(QString("modifiers_%1").arg(i), 0).toUInt();
        m_ui.tableWidget->setItem(j,1, new QTableWidgetItem(HotkeyManager::getKeyString(hotkey->key,
                                                                                      hotkey->mod), i));
        m_hotkeys << hotkey;
    }
    settings.endGroup();
}


SettingsDialog::~SettingsDialog()
{
    while (!m_hotkeys.isEmpty())
        delete m_hotkeys.takeFirst ();

}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Hotkey");
    foreach(Hotkey *k, m_hotkeys)
    {
        settings.setValue(QString("key_%1").arg(k->action), k->key);
        settings.setValue(QString("modifiers_%1").arg(k->action), k->mod);
    }
    settings.endGroup();
    QDialog::accept();
}

void SettingsDialog::on_tableWidget_itemDoubleClicked (QTableWidgetItem *item)
{
    Hotkey *k = 0;
    foreach(k, m_hotkeys)
    {
        if (k->action == item->type())
            break;
    }
    if (!k)
        return;

    HotkeyDialog *dialog = new HotkeyDialog(k->key, k->mod, this);
    if (item->type() >= QTableWidgetItem::UserType &&
            dialog->exec() == QDialog::Accepted)
    {
        QString keyString = HotkeyManager::getKeyString(dialog->keySym (), dialog->nativeModifiers ());
        QList<QTableWidgetItem*> items =  m_ui.tableWidget->findItems(keyString, Qt::MatchFixedString);
        if(keyString.isEmpty() || items.isEmpty() || items.first() == item)
        {
            item->setText(keyString);
            k->key = dialog->keySym ();
            k->mod = dialog->nativeModifiers ();
        }
        else
            QMessageBox::warning(this, tr("Warning"), tr("Key sequence '%1' is already used").arg(keyString));
    }
    delete dialog;
}

void SettingsDialog::on_resetButton_clicked ()
{
    for (int i = 0; i < m_hotkeys.size(); ++i)
    {
        m_hotkeys[i]->key = m_hotkeys[i]->defaultKey();
        m_hotkeys[i]->mod = 0;
        m_ui.tableWidget->item(i, 1)->setText(HotkeyManager::getKeyString(m_hotkeys[i]->key, m_hotkeys[i]->mod));
    }
}
