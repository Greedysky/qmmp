/***************************************************************************
 *   Copyright (C) 2009-2012 by Artur Guzik                                *
 *   a.guzik88@gmail.com                                                   *
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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "kdenotify.h"
#include <qmmp/qmmp.h>
#include <qmmpui/templateeditor.h>

#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QSettings settings(Qmmp::configFile(),QSettings::IniFormat);
    settings.beginGroup("Kde_Notifier");
    ui->notifyDelaySpinBox->setValue(settings.value("notify_duration",5000).toInt()/1000);
    ui->showCoversCheckBox->setChecked(settings.value("show_covers",true).toBool());
    ui->updateNotifyCheckBox->setChecked(settings.value("update_notify",true).toBool());
    m_template = settings.value("template", DEFAULT_TEMPLATE).toString();
    settings.endGroup();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(),QSettings::IniFormat);
    settings.beginGroup("Kde_Notifier");
    settings.setValue("notify_duration",ui->notifyDelaySpinBox->value()*1000);
    settings.setValue("show_covers",ui->showCoversCheckBox->isChecked());
    settings.setValue("template",m_template);
    settings.setValue("update_notify",ui->updateNotifyCheckBox->isChecked());
    settings.endGroup();
    QDialog::accept();
}

void SettingsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsDialog::on_templateButton_clicked()
{
    QString t = TemplateEditor::getTemplate(this, tr("Notification Template"), m_template,
                                            DEFAULT_TEMPLATE);
    if(!t.isEmpty())
        m_template = t;
}
