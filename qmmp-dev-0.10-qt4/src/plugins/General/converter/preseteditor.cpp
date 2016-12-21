/***************************************************************************
 *   Copyright (C) 2011 by Ilya Kotov                                      *
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

#include <QMenu>
#include "preseteditor.h"
#include "ui_preseteditor.h"

PresetEditor::PresetEditor(const QVariantMap &data, QWidget *parent) :
    QDialog(parent), m_ui(new Ui::PresetEditor)
{
    m_ui->setupUi(this);
    m_ui->nameLineEdit->setText(data.value("name").toString());
    m_ui->extensionLineEdit->setText(data.value("ext").toString());
    m_ui->commandLineEdit->setText(data.value("command").toString());
    m_ui->us16bitCheckBox->setChecked(data.value("use_16bit").toBool());
    m_ui->tagsCheckBox->setChecked(data.value("tags").toBool());

    if(data["read_only"].toBool())
    {
        setWindowTitle(tr("%1 (Read Only)").arg(windowTitle()));
        m_ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
        m_ui->nameLineEdit->setReadOnly(true);
        m_ui->extensionLineEdit->setReadOnly(true);
        m_ui->commandLineEdit->setReadOnly(true);
        m_ui->us16bitCheckBox->setDisabled(true);
        m_ui->tagsCheckBox->setDisabled(true);
        m_ui->commandToolButton->setDisabled(true);
    }
    else
        createMenus();
}

PresetEditor::~PresetEditor()
{
    delete m_ui;
}

const QVariantMap PresetEditor::data() const
{
    QVariantMap data;
    data.insert("name", m_ui->nameLineEdit->text());
    data.insert("ext", m_ui->extensionLineEdit->text());
    data.insert("command", m_ui->commandLineEdit->text());
    data.insert("use_16bit",  m_ui->us16bitCheckBox->isChecked());
    data.insert("tags", m_ui->tagsCheckBox->isChecked());
    data.insert("read_only", false);
    return data;
}

void PresetEditor::createMenus()
{
    QMenu *commandMenu = new QMenu(this);
    commandMenu->addAction(tr("Output file"))->setData("%o");
    commandMenu->addAction(tr("Input file"))->setData("%i");

    m_ui->commandToolButton->setMenu(commandMenu);
    m_ui->commandToolButton->setPopupMode(QToolButton::InstantPopup);
    connect(commandMenu, SIGNAL(triggered(QAction *)), SLOT(addCommandString(QAction *)));
}

void PresetEditor::addCommandString(QAction *a)
{
    m_ui->commandLineEdit->insert(a->data().toString());
}
