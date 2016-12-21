/***************************************************************************
 *   Copyright (C) 2006-2012 by Ilya Kotov                                 *
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
#include <QDir>
#include <QMessageBox>
#include <QClipboard>
#include <qmmp/qmmpsettings.h>
#include <qmmp/metadatamanager.h>
#include <qmmp/qmmp.h>
#include "playlistparser.h"
#include "playlistformat.h"
#include "playlistmodel.h"
#include "playlistdownloader.h"
#include "qmmpuisettings.h"
#include "addurldialog_p.h"

#define HISTORY_SIZE 10

AddUrlDialog::AddUrlDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_QuitOnClose, false);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_history = settings.value("URLDialog/history").toStringList();
    urlComboBox->addItems(m_history);
    m_downloader = new PlayListDownloader(this);
    connect(m_downloader, SIGNAL(done(QStringList)), SLOT(add(QStringList)));
    connect(m_downloader, SIGNAL(error(QString)), SLOT(showError(QString)));
    if(QmmpUiSettings::instance()->useClipboard())
    {
        QUrl url(QApplication::clipboard()->text().trimmed());
        if(url.isValid() && MetaDataManager::instance()->protocols().contains(url.scheme()))
            urlComboBox->setEditText(QApplication::clipboard()->text().trimmed());
    }
}

AddUrlDialog::~AddUrlDialog()
{
    while (m_history.size() > HISTORY_SIZE)
        m_history.removeLast();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("URLDialog/history", m_history);
}

QPointer<AddUrlDialog> AddUrlDialog::m_instance = 0;

void AddUrlDialog::popup(QWidget* parent, PlayListModel* model)
{
    if (!m_instance)
    {
        m_instance = new AddUrlDialog(parent);
        m_instance->setModel(model);
    }
    m_instance->show();
    m_instance->raise();
}

void AddUrlDialog::accept()
{
    addButton->setEnabled(false);
    if(urlComboBox->currentText().isEmpty())
    {
         QDialog::accept();
         return;
    }

    QString s = urlComboBox->currentText().trimmed();

    if(!s.startsWith("http://") && !s.contains("://"))
        s.prepend("http://");

    if(!MetaDataManager::instance()->protocols().contains(QUrl(s).scheme()))
    {
        qWarning("AddUrlDialog: unsupported protocol");
        QDialog::accept();
        return;
    }

    m_history.removeAll(s);
    m_history.prepend(s);

    if (s.startsWith("http://")) //try to download playlist
    {
        m_downloader->start(QUrl(s));
        return;
    }
    m_model->add(s);
    QDialog::accept();
}

void AddUrlDialog::setModel(PlayListModel *m)
{
    m_model = m;
}

void AddUrlDialog::add(const QStringList &urls)
{
    addButton->setEnabled(true);
    m_model->add(urls);
    QDialog::accept();
}

void AddUrlDialog::showError(const QString &message)
{
    QMessageBox::warning(this, tr("Error"), message);
    addButton->setEnabled(true);
}
