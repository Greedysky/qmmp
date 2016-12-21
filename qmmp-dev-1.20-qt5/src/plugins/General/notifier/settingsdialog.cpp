/***************************************************************************
 *   Copyright (C) 2008-2013 by Ilya Kotov                                 *
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
#include <QFontDialog>
#include <QMenu>
#include <qmmp/qmmp.h>
#include <qmmpui/templateeditor.h>
#include "popupwidget.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    m_ui.setupUi(this);

    m_buttons.insert(PopupWidget::TOPLEFT, m_ui.topLeftButton);
    m_buttons.insert(PopupWidget::TOP, m_ui.topButton);
    m_buttons.insert(PopupWidget::TOPRIGHT, m_ui.topRightButton);
    m_buttons.insert(PopupWidget::RIGHT, m_ui.rightButton);
    m_buttons.insert(PopupWidget::BOTTOMRIGHT, m_ui.bottomRightButton);
    m_buttons.insert(PopupWidget::BOTTOM, m_ui.bottomButton);
    m_buttons.insert(PopupWidget::BOTTOMLEFT, m_ui.bottomLeftButton);
    m_buttons.insert(PopupWidget::LEFT, m_ui.leftButton);
    m_buttons.insert(PopupWidget::CENTER, m_ui.centerButton);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Notifier");
    m_ui.messageDelaySpinBox->setValue(settings.value("message_delay", 2000).toInt());
    uint pos = settings.value("message_pos", PopupWidget::BOTTOMLEFT).toUInt();
    m_buttons.value(pos)->setChecked(true);
    m_ui.psiCheckBox->setChecked(settings.value("psi_notification", false).toBool());
    m_ui.resumeCheckBox->setChecked(settings.value("resume_notification", false).toBool());
    m_ui.songCheckBox->setChecked(settings.value("song_notification", true).toBool());
    m_ui.volumeCheckBox->setChecked(settings.value("volume_notification", true).toBool());
    m_ui.disableForFScheckBox->setChecked(settings.value("disable_fullscreen", false).toBool());
    m_ui.transparencySlider->setValue(100 - settings.value("opacity", 1.0).toDouble()*100);
    QString fontname = settings.value ("font").toString();
    m_ui.coverSizeSlider->setValue(settings.value ("cover_size", 64).toInt());
    m_template = settings.value ("template", DEFAULT_TEMPLATE).toString();
    settings.endGroup();
    QFont font;
    if(!fontname.isEmpty())
        font.fromString(fontname);
    m_ui.fontLabel->setText (font.family () + " " + QString::number(font.pointSize ()));
    m_ui.fontLabel->setFont(font);
}


SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Notifier");
    settings.setValue ("message_delay", m_ui.messageDelaySpinBox->value());
    uint pos = PopupWidget::BOTTOMLEFT;
    foreach (QPushButton *button, m_buttons.values())
    {
        if(button->isChecked())
            pos = m_buttons.key(button);
    }
    settings.setValue("message_pos", pos);
    settings.setValue("psi_notification", m_ui.psiCheckBox->isChecked());
    settings.setValue("resume_notification", m_ui.resumeCheckBox->isChecked());
    settings.setValue("song_notification", m_ui.songCheckBox->isChecked());
    settings.setValue("volume_notification", m_ui.volumeCheckBox->isChecked());
    settings.setValue("disable_fullscreen", m_ui.disableForFScheckBox->isChecked());
    settings.setValue("opacity", 1.0 -  (double)m_ui.transparencySlider->value()/100);
    settings.setValue("font", m_ui.fontLabel->font().toString());
    settings.setValue("cover_size", m_ui.coverSizeSlider->value());
    settings.setValue("template", m_template);
    settings.endGroup();
    QDialog::accept();
}

void SettingsDialog::on_fontButton_pressed()
{
    bool ok;
    QFont font = m_ui.fontLabel->font();
    font = QFontDialog::getFont (&ok, font, this);
    if (ok)
    {
        m_ui.fontLabel->setText (font.family () + " " + QString::number(font.pointSize ()));
        m_ui.fontLabel->setFont(font);
    }
}

void SettingsDialog::on_templateButton_pressed()
{
    QString t = TemplateEditor::getTemplate(this, tr("Notification Template"), m_template,
                                            DEFAULT_TEMPLATE);
    if(!t.isEmpty())
        m_template = t;
}
