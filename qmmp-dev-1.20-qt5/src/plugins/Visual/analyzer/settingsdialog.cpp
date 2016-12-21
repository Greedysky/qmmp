/***************************************************************************
 *   Copyright (C) 2007-2013 by Ilya Kotov                                 *
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
#include <QSize>
#include <qmmp/qmmp.h>
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
{
    m_ui.setupUi(this);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Analyzer");
    m_ui.colorWidget1->setColor(settings.value("color1", "Green").toString());
    m_ui.colorWidget2->setColor(settings.value("color2", "Yellow").toString());
    m_ui.colorWidget3->setColor(settings.value("color3", "Red").toString());
    m_ui.bgColorWidget->setColor(settings.value("bg_color", "Black").toString());
    m_ui.peakColorWidget->setColor(settings.value("peak_color", "Cyan").toString());
    QSize cells_size = settings.value("cells_size", QSize(15, 6)).toSize();
    m_ui.cellWidthSpinBox->setValue(cells_size.width());
    m_ui.cellHeightSpinBox->setValue(cells_size.height());
    settings.endGroup();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Analyzer");
    settings.setValue("color1", m_ui.colorWidget1->colorName());
    settings.setValue("color2", m_ui.colorWidget2->colorName());
    settings.setValue("color3", m_ui.colorWidget3->colorName());
    settings.setValue("bg_color", m_ui.bgColorWidget->colorName());
    settings.setValue("peak_color", m_ui.peakColorWidget->colorName());
    settings.setValue("cells_size", QSize(m_ui.cellWidthSpinBox->value(),
                                                   m_ui.cellHeightSpinBox->value()));
    settings.endGroup();
    QDialog::accept();
}
