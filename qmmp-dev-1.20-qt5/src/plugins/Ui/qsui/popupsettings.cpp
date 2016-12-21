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
#include <QMenu>
#include <qmmp/qmmp.h>
#include "popupwidget.h"
#include "popupsettings.h"

PopupSettings::PopupSettings(QWidget *parent)
        : QDialog(parent)
{
    m_ui.setupUi(this);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Simple");
    m_ui.transparencySlider->setValue(100 - settings.value("popup_opacity", 1.0).toDouble()*100);
    m_ui.coverSizeSlider->setValue(settings.value ("popup_cover_size", 48).toInt());
    m_ui.textEdit->setPlainText(settings.value ("popup_template", DEFAULT_TEMPLATE).toString());
    m_ui.delaySpinBox->setValue(settings.value("popup_delay", 2500).toInt());
    m_ui.coverCheckBox->setChecked(settings.value("popup_show_cover",true).toBool());
    settings.endGroup();
    createMenu();
}


PopupSettings::~PopupSettings()
{}

void PopupSettings::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Simple");
    settings.setValue("popup_opacity", 1.0 -  (double)m_ui.transparencySlider->value()/100);
    settings.setValue("popup_cover_size", m_ui.coverSizeSlider->value());
    settings.setValue("popup_template", m_ui.textEdit->toPlainText());
    settings.setValue("popup_delay",  m_ui.delaySpinBox->value());
    settings.setValue("popup_show_cover", m_ui.coverCheckBox->isChecked());
    settings.endGroup();
    QDialog::accept();
}

void PopupSettings::createMenu()
{
    QMenu *menu = new QMenu(this);
    menu->addAction(tr("Artist"))->setData("%p");
    menu->addAction(tr("Album"))->setData("%a");
    menu->addAction(tr("Album Artist"))->setData("%aa");
    menu->addAction(tr("Title"))->setData("%t");
    menu->addAction(tr("Track Number"))->setData("%n");
    menu->addAction(tr("Two-digit Track Number"))->setData("%NN");
    menu->addAction(tr("Genre"))->setData("%g");
    menu->addAction(tr("Comment"))->setData("%c");
    menu->addAction(tr("Composer"))->setData("%C");
    menu->addAction(tr("Duration"))->setData("%l");
    menu->addAction(tr("Disc Number"))->setData("%D");
    menu->addAction(tr("File Name"))->setData("%f");
    menu->addAction(tr("File Path"))->setData("%F");
    menu->addAction(tr("Year"))->setData("%y");
    menu->addAction(tr("Condition"))->setData("%if(%p&%t,%p - %t,%f)");
    m_ui.insertButton->setMenu(menu);
    connect(menu, SIGNAL(triggered (QAction *)), SLOT(insertExpression(QAction *)));
}

void PopupSettings::insertExpression(QAction *a)
{
    m_ui.textEdit->insertPlainText(a->data().toString());
}

void PopupSettings::on_resetButton_clicked()
{
    m_ui.textEdit->setPlainText(DEFAULT_TEMPLATE);
}
