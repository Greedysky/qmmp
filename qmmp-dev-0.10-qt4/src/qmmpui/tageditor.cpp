/***************************************************************************
 *   Copyright (C) 2009-2016 by Ilya Kotov                                 *
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

#include <qmmp/tagmodel.h>
#include "tageditor_p.h"
#include "ui_tageditor.h"

TagEditor::TagEditor(TagModel *tagModel, QWidget *parent) : QWidget(parent), m_ui(new Ui::TagEditor)
{
    m_ui->setupUi(this);
    m_tagModel = tagModel;
    m_discs = -1;
    //check available keys
    m_ui->titleLineEdit->setEnabled(m_tagModel->keys().contains(Qmmp::TITLE));
    m_ui->artistLineEdit->setEnabled(m_tagModel->keys().contains(Qmmp::ARTIST));
    m_ui->albumArtistLineEdit->setEnabled(m_tagModel->keys().contains(Qmmp::ALBUMARTIST));
    m_ui->albumLineEdit->setEnabled(m_tagModel->keys().contains(Qmmp::ALBUM));
    m_ui->composerLineEdit->setEnabled(m_tagModel->keys().contains(Qmmp::COMPOSER));
    m_ui->genreLineEdit->setEnabled(m_tagModel->keys().contains(Qmmp::GENRE));
    m_ui->commentBrowser->setEnabled(m_tagModel->keys().contains(Qmmp::COMMENT));
    m_ui->discSpinBox->setEnabled(m_tagModel->keys().contains(Qmmp::DISCNUMBER));
    m_ui->yearSpinBox->setEnabled(m_tagModel->keys().contains(Qmmp::YEAR));
    m_ui->trackSpinBox->setEnabled(m_tagModel->keys().contains(Qmmp::TRACK));

    readTag();
}

TagEditor::~TagEditor()
{
    delete m_ui;
}

void TagEditor::save()
{
    if(m_ui->useCheckBox->isChecked())
    {
        if(!m_tagModel->exists())
            m_tagModel->create();
         m_tagModel->setValue(Qmmp::TITLE,  m_ui->titleLineEdit->text());
         m_tagModel->setValue(Qmmp::ARTIST, m_ui->artistLineEdit->text());
         m_tagModel->setValue(Qmmp::ALBUMARTIST, m_ui->albumArtistLineEdit->text());
         m_tagModel->setValue(Qmmp::ALBUM, m_ui->albumLineEdit->text());
         m_tagModel->setValue(Qmmp::COMPOSER, m_ui->composerLineEdit->text());
         m_tagModel->setValue(Qmmp::GENRE, m_ui->genreLineEdit->text());
         m_tagModel->setValue(Qmmp::COMMENT, m_ui->commentBrowser->toPlainText ());
         if(m_discs < 0)
             m_tagModel->setValue(Qmmp::DISCNUMBER,  m_ui->discSpinBox->value());
         else
             m_tagModel->setValue(Qmmp::DISCNUMBER, QString("%1/%2").arg(m_ui->discSpinBox->value()).arg(m_discs));
         m_tagModel->setValue(Qmmp::YEAR, m_ui->yearSpinBox->value());
         m_tagModel->setValue(Qmmp::TRACK, m_ui->trackSpinBox->value());
    }
    else
        m_tagModel->remove();
    m_tagModel->save();
    readTag();
}

void TagEditor::readTag()
{
    m_ui->tagWidget->setEnabled(m_tagModel->exists());
    m_ui->useCheckBox->setChecked(m_tagModel->exists());
    m_ui->useCheckBox->setVisible(m_tagModel->caps() & TagModel::CreateRemove);
    m_ui->titleLineEdit->setText(m_tagModel->value(Qmmp::TITLE));
    m_ui->artistLineEdit->setText(m_tagModel->value(Qmmp::ARTIST));
    m_ui->albumArtistLineEdit->setText(m_tagModel->value(Qmmp::ALBUMARTIST));
    m_ui->albumLineEdit->setText(m_tagModel->value(Qmmp::ALBUM));
    m_ui->composerLineEdit->setText(m_tagModel->value(Qmmp::COMPOSER));
    m_ui->genreLineEdit->setText(m_tagModel->value(Qmmp::GENRE));
    m_ui->commentBrowser->setText(m_tagModel->value(Qmmp::COMMENT));
    if(m_tagModel->value(Qmmp::DISCNUMBER).contains("/"))
    {
        m_ui->discSpinBox->setValue(m_tagModel->value(Qmmp::DISCNUMBER).section("/",0,0).toInt());
        m_discs = m_tagModel->value(Qmmp::DISCNUMBER).section("/",1,1).toInt();
        m_ui->discSpinBox->setSuffix(QString("/%1").arg(m_discs));
    }
    else
        m_ui->discSpinBox->setValue(m_tagModel->value(Qmmp::DISCNUMBER).toInt());
    m_ui->yearSpinBox->setValue(m_tagModel->value(Qmmp::YEAR).toInt());
    m_ui->trackSpinBox->setValue(m_tagModel->value(Qmmp::TRACK).toInt());
}
