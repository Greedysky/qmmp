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
#include <qmmpui/templateeditor.h>
#include <qmmp/qmmp.h>
#include "statusiconpopupwidget.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Tray");
    ui.messageGroupBox->setChecked(settings.value("show_message",true).toBool());
    ui.messageDelaySpinBox->setValue(settings.value("message_delay", 2000).toInt());
    ui.niceTooltipGroupBox->setChecked(settings.value("show_tooltip", true).toBool());
#ifdef QMMP_WS_X11
    ui.niceTooltipDelaySpinBox->setValue(settings.value("tooltip_delay",2000).toInt());
    ui.transparencySlider->setValue(settings.value("tooltip_transparency",0).toInt());
    ui.coverSizeSlider->setValue(settings.value("tooltip_cover_size", 100).toInt());
    ui.progressCheckBox->setChecked(settings.value("tooltip_progress",true).toBool());
    ui.niceTooltipSplitCheckBox->setChecked(settings.value("split_file_name",true).toBool());
#else
    ui.niceTooltipDelaySpinBox->setEnabled(false);
    ui.transparencySlider->setEnabled(false);
    ui.coverSizeSlider->setEnabled(false);
    ui.progressCheckBox->setEnabled(false);
    ui.niceTooltipSplitCheckBox->setEnabled(false);
    ui.templateButton->setEnabled(false);
#endif
    ui.standardIconsCheckBox->setChecked(settings.value("use_standard_icons",false).toBool());
    m_template = settings.value("tooltip_template", DEFAULT_TEMPLATE).toString();
    settings.endGroup();
}

SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Tray");
    settings.setValue("show_message", ui.messageGroupBox->isChecked());
    settings.setValue("message_delay", ui.messageDelaySpinBox->value());
    settings.setValue("use_standard_icons", ui.standardIconsCheckBox->isChecked());
    settings.setValue("show_tooltip", ui.niceTooltipGroupBox->isChecked());
#ifdef QMMP_WS_X11
    settings.setValue("split_file_name", ui.niceTooltipSplitCheckBox->isChecked());
    settings.setValue("tooltip_delay", ui.niceTooltipDelaySpinBox->value());
    settings.setValue("tooltip_transparency",  ui.transparencySlider->value());
    settings.setValue("tooltip_cover_size",  ui.coverSizeSlider->value());
    settings.setValue("tooltip_progress", ui.progressCheckBox->isChecked());
#endif
    settings.setValue("tooltip_template", m_template);
    settings.endGroup();
    QDialog::accept();
}

void SettingsDialog::on_templateButton_clicked()
{
    QString t = TemplateEditor::getTemplate(this, tr("Tooltip Template"), m_template, DEFAULT_TEMPLATE);
    if(!t.isEmpty())
        m_template = t;
}
