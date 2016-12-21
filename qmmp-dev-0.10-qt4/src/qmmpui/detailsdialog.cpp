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
#include <QDesktopServices>
#include <QTextCodec>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QPushButton>
#include <qmmp/metadatamanager.h>
#include <qmmp/metadatamodel.h>
#include <qmmp/tagmodel.h>
#include <qmmp/soundcore.h>
#include "ui_detailsdialog.h"
#include "playlisttrack.h"
#include "tageditor_p.h"
#include "detailsdialog.h"

DetailsDialog::DetailsDialog(QList<PlayListTrack *> tracks, QWidget *parent)
        : QDialog(parent)
{
    m_ui = new Ui::DetailsDialog;
    m_ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_ui->directoryButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon));
    m_ui->nextButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight));
    m_ui->prevButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowLeft));
    m_metaDataModel = 0;
    m_page = 0;
    m_tracks = tracks;
    updatePage();
    on_tabWidget_currentChanged(0);
}

DetailsDialog::~DetailsDialog()
{
    delete m_ui;
}

void DetailsDialog:: on_directoryButton_clicked()
{
    QString dir_path;
    if(!m_path.contains("://")) //local file
        dir_path = QFileInfo(m_path).absolutePath();
    else if (m_path.contains(":///")) //pseudo-protocol
    {
        dir_path = QUrl(m_path).path();
        dir_path.replace(QString(QUrl::toPercentEncoding("#")), "#");
        dir_path.replace(QString(QUrl::toPercentEncoding("?")), "?");
        dir_path.replace(QString(QUrl::toPercentEncoding("%")), "%");
        dir_path = QFileInfo(dir_path).absolutePath();
    }
    else
        return;
#ifdef Q_WS_X11
    QProcess::execute("xdg-open", QStringList() << dir_path); //works with lxde
#else
    QDesktopServices::openUrl(QUrl::fromLocalFile(dir_path));
#endif
}

void DetailsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(m_ui->buttonBox->standardButton(button) == QDialogButtonBox::Save)
    {
        TagEditor *tab = qobject_cast<TagEditor *> (m_ui->tabWidget->currentWidget());
        if(tab)
            tab->save();
    }
    else
        reject();
}

void DetailsDialog::on_tabWidget_currentChanged(int index)
{
    TagEditor *tab = qobject_cast<TagEditor *> (m_ui->tabWidget->widget(index));
    m_ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(tab != 0);
}

void DetailsDialog::on_prevButton_clicked()
{
    if(m_page == 0)
        m_page = m_tracks.count() - 1;
    else
        m_page--;
    updatePage();
}

void DetailsDialog::on_nextButton_clicked()
{
    if(m_page >= m_tracks.count() - 1)
        m_page = 0;
    else
        m_page++;
    updatePage();
}

void DetailsDialog::updatePage()
{
    if(m_metaDataModel)
    {
        delete m_metaDataModel;
        m_metaDataModel = 0;
    }

    while (m_ui->tabWidget->count() > 1)
    {
        int index = m_ui->tabWidget->count() - 1;
        QWidget *w = m_ui->tabWidget->widget(index);
        m_ui->tabWidget->removeTab(index);
        w->deleteLater();
    }

    m_ui->pageLabel->setText(tr("%1/%2").arg(m_page + 1).arg(m_tracks.count()));
    m_track = m_tracks.at(m_page);
    m_path = m_track->url();
    setWindowTitle (m_path.section('/',-1));
    m_ui->pathEdit->setText(m_path);

    //load metadata and create metadata model
    QList <FileInfo *> flist = MetaDataManager::instance()->createPlayList(m_path, true);
    if(!flist.isEmpty() && QFile::exists(m_track->url()))
        m_metaData = flist.at(0)->metaData();
    else
        m_metaData = *m_track;
    qDeleteAll(flist);

    m_metaDataModel = MetaDataManager::instance()->createMetaDataModel(m_path, this);

    if(m_metaDataModel)
    {
        if(QFile::exists(m_path) && QFileInfo(m_path).isWritable())
        {
            foreach(TagModel *tagModel, m_metaDataModel->tags())
                m_ui->tabWidget->addTab(new TagEditor(tagModel, this), tagModel->name());
        }

        foreach(QString title, m_metaDataModel->descriptions().keys())
        {
            QTextEdit *textEdit = new QTextEdit(this);
            textEdit->setReadOnly(true);
            textEdit->setPlainText(m_metaDataModel->descriptions().value(title));
            m_ui->tabWidget->addTab(textEdit, title);
        }
    }
    printInfo();
}

void DetailsDialog::printInfo()
{
    SoundCore *core = SoundCore::instance();
    QString formattedText;
    if(layoutDirection() == Qt::RightToLeft)
        formattedText.append("<DIV align=\"right\" dir=\"rtl\">");
    else
        formattedText.append("<DIV>");
    formattedText.append("<TABLE>");
    //tags
    formattedText += formatRow(tr("Title"), m_metaData[Qmmp::TITLE]);
    formattedText += formatRow(tr("Artist"), m_metaData[Qmmp::ARTIST]);
    formattedText += formatRow(tr("Album artist"), m_metaData[Qmmp::ALBUMARTIST]);
    formattedText += formatRow(tr("Album"), m_metaData[Qmmp::ALBUM]);
    formattedText += formatRow(tr("Comment"), m_metaData[Qmmp::COMMENT]);
    formattedText += formatRow(tr("Genre"), m_metaData[Qmmp::GENRE]);
    formattedText += formatRow(tr("Composer"), m_metaData[Qmmp::COMPOSER]);
    if(m_metaData[Qmmp::YEAR] != "0")
        formattedText += formatRow(tr("Year"), m_metaData[Qmmp::YEAR]);
    if(m_metaData[Qmmp::TRACK] != "0")
        formattedText += formatRow(tr("Track"), m_metaData[Qmmp::TRACK]);
    if(m_metaData[Qmmp::DISCNUMBER] != "0")
        formattedText += formatRow(tr("Disc number"), m_metaData[Qmmp::DISCNUMBER]);
    //stream information
    if(core->state() == Qmmp::Playing && core->url() == m_metaData.value(Qmmp::URL))
    {
        if(!core->streamInfo().isEmpty())
        {
            formattedText.append("<tr>");
            formattedText.append("<td colspan=2>");
            formattedText.append("<hr>");
            formattedText.append("</td>");
            formattedText.append("</tr>");

            foreach(QString key, core->streamInfo().keys())
                formattedText += formatRow(key, core->streamInfo().value(key));
        }
    }
    //audio info
    if(!m_metaDataModel)
    {
        formattedText.append("</TABLE>");
        formattedText.append("</DIV>");
        m_ui->textEdit->setHtml(formattedText);
        return;
    }
    QHash <QString, QString> ap = m_metaDataModel->audioProperties();
    //line
    if(formattedText.contains("<tr>"))
    {
        formattedText.append("<tr>");
        formattedText.append("<td colspan=2>");
        formattedText.append("<hr>");
        formattedText.append("</td>");
        formattedText.append("</tr>");
    }

    foreach(QString key, ap.keys())
        formattedText += formatRow(key, ap.value(key));

    formattedText.append("</TABLE>");
    formattedText.append("</DIV>");
    m_ui->textEdit->setHtml(formattedText);
}

QString DetailsDialog::formatRow(const QString key, const QString value)
{
    if(value.isEmpty())
        return QString();
    QString str("<tr>");
    if(layoutDirection() == Qt::RightToLeft)
        str.append("<td>" + value + "</td> <td style=\"padding-left: 15px;\"><b>" + key + "</b></td>");
    else
        str.append("<td><b>" + key + "</b></td> <td style=\"padding-left: 15px;\">" + value + "</td>");
    str.append("</tr>");
    return str;
}
