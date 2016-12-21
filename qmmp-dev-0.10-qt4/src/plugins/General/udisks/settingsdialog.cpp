/***************************************************************************
 *   Copyright (C) 2010 by Ilya Kotov                                      *
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
#include <QTextCodec>
#include <QSettings>

#include <qmmp/qmmp.h>

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("UDisks");
    ui.cdGroupBox->setChecked(settings.value("cda", true).toBool());
    ui.addTracksCheckBox->setChecked(settings.value("add_tracks", false).toBool());
    ui.removeTracksCheckBox->setChecked(settings.value("remove_tracks", false).toBool());
    ui.removableGroupBox->setChecked(settings.value("removable", true).toBool());
    ui.addFilesCheckBox->setChecked(settings.value("add_files", false).toBool());
    ui.removeFilesCheckBox->setChecked(settings.value("remove_files", false).toBool());
    settings.endGroup();
}


SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("UDisks");
    settings.setValue("cda", ui.cdGroupBox->isChecked());
    settings.setValue("add_tracks", ui.addTracksCheckBox->isChecked());
    settings.setValue("remove_tracks", ui.removeTracksCheckBox->isChecked());
    settings.setValue("removable", ui.removableGroupBox->isChecked());
    settings.setValue("add_files", ui.addFilesCheckBox->isChecked());
    settings.setValue("remove_files", ui.removeFilesCheckBox->isChecked());
    settings.endGroup();
    QDialog::accept();
}
