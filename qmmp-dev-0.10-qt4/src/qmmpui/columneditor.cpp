/***************************************************************************
 *   Copyright (C) 2015 by Ilya Kotov                                      *
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
#include "columneditor_p.h"
#include "ui_columneditor.h"

ColumnEditor::ColumnEditor(const QString &name, const QString &patt, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ColumnEditor)
{
    m_ui->setupUi(this);
    createMenu();
    fillTypes();

    //load inital values
    m_ui->nameLineEdit->setText(name);
    m_ui->formatLineEdit->setText(patt);
}

ColumnEditor::~ColumnEditor()
{
    delete m_ui;
}

QString ColumnEditor::name() const
{
    return m_ui->nameLineEdit->text();
}

QString ColumnEditor::pattern() const
{
    return m_ui->formatLineEdit->text();
}

void ColumnEditor::insertExpression(QAction *a)
{
    if (m_ui->formatLineEdit->cursorPosition () < 1)
        m_ui->formatLineEdit->insert(a->data().toString());
    else
        m_ui->formatLineEdit->insert(" - "+a->data().toString());
}

void ColumnEditor::on_comboBox_activated(int index)
{
    m_ui->formatLineEdit->setText(m_ui->comboBox->itemData(index).toString());
    m_ui->nameLineEdit->setText(m_ui->comboBox->itemText(index));
}

void ColumnEditor::on_formatLineEdit_textChanged(const QString &text)
{
    int index = m_ui->comboBox->findData(text);
    if(index < 0)
        index = m_ui->comboBox->findData("custom");
    m_ui->comboBox->setCurrentIndex(index);
}

void ColumnEditor::createMenu()
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
    menu->addAction(tr("Track Index"))->setData("%I");
    menu->addAction(tr("Condition"))->setData("%if(%p,%p - %t,%t)");
    m_ui->formatButton->setMenu(menu);
    connect(menu, SIGNAL(triggered (QAction *)), SLOT(insertExpression(QAction *)));
}

void ColumnEditor::fillTypes()
{
    m_ui->comboBox->addItem(tr("Artist"),"%p");
    m_ui->comboBox->addItem(tr("Album"),"%a");
    m_ui->comboBox->addItem(tr("Artist - Album"),"%if(%p&%a,%p - %a,)");
    m_ui->comboBox->addItem(tr("Artist - Title"),"%if(%p,%p - %t,%t)");
    m_ui->comboBox->addItem(tr("Album Artist"),"%aa");
    m_ui->comboBox->addItem(tr("Title"),"%t");
    m_ui->comboBox->addItem(tr("Track Number"),"%n");
    m_ui->comboBox->addItem(tr("Two-digit Track Number"),"%NN");
    m_ui->comboBox->addItem(tr("Genre"),"%g");
    m_ui->comboBox->addItem(tr("Comment"),"%c");
    m_ui->comboBox->addItem(tr("Composer"),"%C");
    m_ui->comboBox->addItem(tr("Duration"),"%l");
    m_ui->comboBox->addItem(tr("Disc Number"),"%D");
    m_ui->comboBox->addItem(tr("File Name"),"%f");
    m_ui->comboBox->addItem(tr("File Path"),"%F");
    m_ui->comboBox->addItem(tr("Track Index"), "%I");
    m_ui->comboBox->addItem(tr("Year"),"%y");
    m_ui->comboBox->addItem(tr("Custom"),"custom");
}
