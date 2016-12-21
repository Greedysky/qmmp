/***************************************************************************
 *   Copyright (C) 2006-2012 by Ilya Kotov                                 *
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

#include <QIcon>
#include "eqpreset.h"
#include "preseteditor.h"

PresetEditor::PresetEditor(QWidget *parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui.loadButton,SIGNAL(clicked()),SLOT(loadPreset()));
    connect(ui.deleteButton,SIGNAL(clicked()),SLOT(deletePreset()));
    ui.loadButton->setIcon(QIcon::fromTheme("document-open"));
    ui.deleteButton->setIcon(QIcon::fromTheme("edit-delete"));
}


PresetEditor::~PresetEditor()
{
    while (ui.presetListWidget->count () !=0)
        ui.presetListWidget->takeItem (0);

    while (ui.autoPresetListWidget->count () !=0)
        ui.autoPresetListWidget->takeItem (0);
}

void PresetEditor::addPresets(const QList<EQPreset*> &presets)
{
    foreach(QListWidgetItem *item, presets)
    {
        ui.presetListWidget->addItem(item);
    }
}

void PresetEditor::addAutoPresets(const QList<EQPreset*> &presets)
{
    foreach(QListWidgetItem *item, presets)
    {
        ui.autoPresetListWidget->addItem(item);
    }
}

void PresetEditor::loadPreset()
{
    EQPreset* preset = 0;
    if (ui.tabWidget->currentIndex () == 0)
        preset = (EQPreset *) ui.presetListWidget->currentItem ();
    if (ui.tabWidget->currentIndex () == 1)
        preset = (EQPreset *) ui.autoPresetListWidget->currentItem ();
    if (preset)
        emit presetLoaded(preset);
}

void PresetEditor::deletePreset()
{
    EQPreset* preset = 0;
    if (ui.tabWidget->currentIndex () == 0)
        preset = (EQPreset *) ui.presetListWidget->currentItem ();
    if (ui.tabWidget->currentIndex () == 1)
        preset = (EQPreset *) ui.autoPresetListWidget->currentItem ();
    if (preset)
        emit presetDeleted(preset);
}
