/***************************************************************************
 *   Copyright (C) 2010-2014 by Ilya Kotov                                 *
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
#include <QAction>
#include "ui_templateeditor.h"
#include "templateeditor.h"

TemplateEditor::TemplateEditor(QWidget *parent) : QDialog(parent), m_ui(new Ui::TemplateEditor)
{
    m_ui->setupUi(this);
    createMenu();
}

QString TemplateEditor::currentTemplate() const
{
    return m_ui->textEdit->toPlainText ();
}

void TemplateEditor::setTemplate(const QString &text)
{
    m_ui->textEdit->setPlainText(text);
}

void TemplateEditor::setDefaultTemplate(const QString &text)
{
    m_defaultTemplate = text;
}

void TemplateEditor::createMenu()
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
    m_ui->insertButton->setMenu(menu);
    connect(menu, SIGNAL(triggered (QAction *)), SLOT(insertExpression(QAction *)));
}

void TemplateEditor::insertExpression(QAction *a)
{
    m_ui->textEdit->insertPlainText(a->data().toString());
}

void TemplateEditor::on_resetButton_clicked()
{
    m_ui->textEdit->setPlainText(m_defaultTemplate);
}

QString TemplateEditor::getTemplate (QWidget *parent, const QString &title, const QString &text,
                                     const QString &default_template, bool *ok)
{
    TemplateEditor *editor = new TemplateEditor(parent);
    editor->setWindowTitle(title);
    editor->setTemplate(text);
    editor->setDefaultTemplate(default_template);
    if(editor->exec() == QDialog::Accepted)
    {
        if(ok)
            *ok = true;
        QString t = editor->currentTemplate();
        editor->deleteLater();
        return t;
    }
    else
    {
        if(ok)
            *ok = false;
        editor->deleteLater();
        return QString();
    }
}
