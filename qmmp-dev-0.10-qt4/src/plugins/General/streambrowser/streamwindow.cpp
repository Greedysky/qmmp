/***************************************************************************
 *   Copyright (C) 2012-2016 by Ilya Kotov                                 *
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

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QUrl>
#include <QRegExp>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QMenu>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <qmmp/qmmpsettings.h>
#include <qmmp/qmmp.h>
#include <qmmpui/playlistmanager.h>
#include "streamwindow.h"

StreamWindow::StreamWindow(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::Window);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_QuitOnClose, false);
    m_requestReply = 0;
    //buttons
    ui.addPushButton->setIcon(QIcon::fromTheme("list-add"));
    ui.updatePushButton->setIcon(QIcon::fromTheme("view-refresh"));
    //icecast model
    m_iceCastModel = new QStandardItemModel(this);
    m_iceCastModel->setHorizontalHeaderLabels(QStringList() << tr("Name")
                                       << tr("Genre")
                                       << tr("Bitrate")
                                       << tr("Format"));
    m_iceCastFilterModel = new StreamsProxyModel(this);
    m_iceCastFilterModel->setSourceModel(m_iceCastModel);
    m_iceCastFilterModel->setDynamicSortFilter(true);
    m_iceCastFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    //icecast table
    ui.icecastTableView->setModel(m_iceCastFilterModel);
    ui.icecastTableView->verticalHeader()->setDefaultSectionSize(fontMetrics().height() + 3);
    ui.icecastTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    ui.icecastTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.icecastTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.icecastTableView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(execIceCastMenu(QPoint)));
    //favorites model
    m_favoritesModel = new QStandardItemModel(this);
    m_favoritesModel->setHorizontalHeaderLabels(QStringList() << tr("Name")
                                                     << tr("Genre")
                                                     << tr("Bitrate")
                                                     << tr("Format"));
    m_favoritesFilterModel = new StreamsProxyModel(this);
    m_favoritesFilterModel->setSourceModel(m_favoritesModel);
    m_favoritesFilterModel->setDynamicSortFilter(true);
    m_favoritesFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    //favorites table
    ui.favoritesTableView->setModel(m_favoritesFilterModel);
    ui.favoritesTableView->verticalHeader()->setDefaultSectionSize(fontMetrics().height() + 3);
    ui.favoritesTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    ui.favoritesTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.favoritesTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.favoritesTableView, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(execFavoritesMenu(QPoint)));

    ui.statusLabel->hide();

    m_http = new QNetworkAccessManager(this);
     //load global proxy settings
    QmmpSettings *gs = QmmpSettings::instance();
    if (gs->isProxyEnabled())
    {
        QNetworkProxy proxy(QNetworkProxy::HttpProxy, gs->proxy().host(),  gs->proxy().port());
        if(gs->useProxyAuth())
        {
            proxy.setUser(gs->proxy().userName());
            proxy.setPassword(gs->proxy().password());
        }
        m_http->setProxy(proxy);
    }
    connect(m_http, SIGNAL(finished (QNetworkReply *)), SLOT(showText(QNetworkReply *)));
    //read settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("StreamBrowser");
    restoreGeometry(settings.value("geometry").toByteArray());
    ui.icecastTableView->horizontalHeader()->restoreState(settings.value("icecast_headers").toByteArray());
    ui.favoritesTableView->horizontalHeader()->restoreState(settings.value("favorites_headers").toByteArray());
    ui.tabWidget->setCurrentIndex(settings.value("current_tab", 1).toInt());
    settings.endGroup();
    //create cache dir
    QDir dir(Qmmp::configDir());
    if(!dir.exists("streambrowser"))
        dir.mkdir("streambrowser");
    //read cache
    QFile file(Qmmp::configDir() + "/streambrowser/icecast.xml");
    if(file.open(QIODevice::ReadOnly))
        readXml(&file, m_iceCastModel);
    else
        on_updatePushButton_clicked();
    QFile file2(Qmmp::configDir() + "/streambrowser/favorites.xml");
    if(file2.open(QIODevice::ReadOnly))
        readXml(&file2, m_favoritesModel);
    //create menus
    m_iceCastMenu = new QMenu(this);
    m_iceCastMenu->addAction(tr("&Add to favorites"), this, SLOT(addToFavorites()));
    QAction *addAction = m_iceCastMenu->addAction(tr("&Add to playlist"), this,
                                                  SLOT(on_addPushButton_clicked()));
    m_favoritesMenu = new QMenu(this);
    m_favoritesMenu->addAction(addAction);
    m_favoritesMenu->addSeparator();
    m_favoritesMenu->addAction(tr("&Remove"), this, SLOT(removeFromFavorites()), QKeySequence::Delete);
    addActions(m_favoritesMenu->actions());
}

StreamWindow::~StreamWindow()
{
}

void StreamWindow::showText(QNetworkReply *reply)
{
    ui.statusLabel->setText(tr("Done"));
    if (reply->error() != QNetworkReply::NoError)
    {
        ui.statusLabel->setText(tr("Error"));
        QMessageBox::warning (this, tr("Error"), reply->errorString());
        m_requestReply = 0;
        reply->deleteLater();
        return;
    }
    if(m_requestReply == reply)
    {
        m_requestReply = 0;
        readXml(reply, m_iceCastModel);
    }
    reply->deleteLater();
}

void StreamWindow::on_updatePushButton_clicked()
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://dir.xiph.org/yp.xml"));
    request.setRawHeader("User-Agent", QString("qmmp/%1").arg(Qmmp::strVersion()).toAscii());
    m_requestReply = m_http->get(request);
    ui.statusLabel->setText(tr("Receiving"));
    ui.statusLabel->show();
}

void StreamWindow::on_addPushButton_clicked()
{
    QStringList urls;
    if(ui.tabWidget->currentIndex() == 0)
    {
        QModelIndexList indexes = ui.favoritesTableView->selectionModel()->selectedRows(0);
        foreach(QModelIndex index, indexes)
        {
            QModelIndex source_index = m_favoritesFilterModel->mapToSource(index);
            urls.append(m_favoritesModel->item(source_index.row(),0)->data().toString());
        }
    }
    else
    {
        QModelIndexList indexes = ui.icecastTableView->selectionModel()->selectedRows(0);
        foreach(QModelIndex index, indexes)
        {
            QModelIndex source_index = m_iceCastFilterModel->mapToSource(index);
            urls.append(m_iceCastModel->item(source_index.row(),0)->data().toString());
        }
    }
    urls.removeDuplicates();
    PlayListManager::instance()->add(urls);
}

void StreamWindow::on_filterLineEdit_textChanged(const QString &text)
{
    m_iceCastFilterModel->setFilterFixedString(text);
    m_favoritesFilterModel->setFilterFixedString(text);
}

void StreamWindow::execIceCastMenu(const QPoint &pos)
{
    m_iceCastMenu->exec(ui.icecastTableView->viewport()->mapToGlobal(pos));
}

void StreamWindow::execFavoritesMenu(const QPoint &pos)
{
    m_favoritesMenu->exec(ui.favoritesTableView->viewport()->mapToGlobal(pos));
}

void StreamWindow::addToFavorites()
{
    QModelIndexList indexes = ui.icecastTableView->selectionModel()->selectedRows(0);
    foreach(QModelIndex index, indexes)
    {
        QModelIndex source_index = m_iceCastFilterModel->mapToSource(index);
        int row = source_index.row();
        m_favoritesModel->appendRow(QList<QStandardItem *> ()
                                    << m_iceCastModel->item(row, 0)->clone()
                                    << m_iceCastModel->item(row, 1)->clone()
                                    << m_iceCastModel->item(row, 2)->clone()
                                    << m_iceCastModel->item(row, 3)->clone());
    }
}

void StreamWindow::removeFromFavorites()
{
    if(ui.tabWidget->currentIndex() != 0)
        return;
    QModelIndexList indexes = ui.favoritesTableView->selectionModel()->selectedRows(0);
    QList<int> rows_to_remove;
    foreach(QModelIndex index, indexes)
    {
        rows_to_remove.append(m_favoritesFilterModel->mapToSource(index).row());
    }
    qStableSort(rows_to_remove);
    int prev_row = -1;
    for(int i = rows_to_remove.count() - 1; i >= 0; i -= 1 )
    {
        int current = rows_to_remove[i];
        if(current != prev_row)
        {
            m_favoritesFilterModel->removeRows(current, 1);
            prev_row = current;
        }
    }
}

void StreamWindow::closeEvent(QCloseEvent *)
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("StreamBrowser");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("icecast_headers", ui.icecastTableView->horizontalHeader()->saveState());
    settings.setValue("favorites_headers", ui.favoritesTableView->horizontalHeader()->saveState());
    settings.setValue("current_tab", ui.tabWidget->currentIndex());
    settings.endGroup();

     //save icecast directory
    QFile file(Qmmp::configDir() + "/streambrowser/icecast.xml");
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter writer(&file);
    writer.setCodec("UTF-8");
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("directory");
    for(int i = 0; i < m_iceCastModel->rowCount(); ++i)
    {
        writer.writeStartElement("entry");
        writer.writeTextElement("server_name", m_iceCastModel->item(i,0)->text());
        writer.writeTextElement("listen_url", m_iceCastModel->item(i,0)->data().toString());
        writer.writeTextElement("genre", m_iceCastModel->item(i,1)->text());
        writer.writeTextElement("bitrate", m_iceCastModel->item(i,2)->text());
        writer.writeTextElement("server_type", m_iceCastModel->item(i,3)->text());
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
    //save favorites
    QFile file2(Qmmp::configDir() + "/streambrowser/favorites.xml");
    file2.open(QIODevice::WriteOnly);
    QXmlStreamWriter writer2(&file2);
    writer2.setCodec("UTF-8");
    writer2.setAutoFormatting(true);
    writer2.writeStartDocument();
    writer2.writeStartElement("directory");
    for(int i = 0; i < m_favoritesModel->rowCount(); ++i)
    {
        writer2.writeStartElement("entry");
        writer2.writeTextElement("server_name", m_favoritesModel->item(i,0)->text());
        writer2.writeTextElement("listen_url", m_favoritesModel->item(i,0)->data().toString());
        writer2.writeTextElement("genre", m_favoritesModel->item(i,1)->text());
        writer2.writeTextElement("bitrate", m_favoritesModel->item(i,2)->text());
        writer2.writeTextElement("server_type", m_favoritesModel->item(i,3)->text());
        writer2.writeEndElement();
    }
    writer2.writeEndElement();
    writer2.writeEndDocument();
    file2.close();
}

void StreamWindow::readXml(QIODevice *input, QStandardItemModel *model)
{
    model->removeRows(0, model->rowCount());
    QXmlStreamReader xml(input);
    QString currentTag, server_name, listen_url, genre, bitrate, server_type;
    while (!xml.atEnd())
    {
        xml.readNext();
        if (xml.isStartElement())
        {
            currentTag = xml.name().toString();
        }
        else if (xml.isEndElement())
        {
            if (xml.name() == "entry")
            {
                if(server_name == "Unspecified name" || server_name.isEmpty())
                    server_name = listen_url.section("/", -1);

                model->appendRow(QList<QStandardItem *> ()
                                          << new QStandardItem(server_name)
                                          << new QStandardItem(genre)
                                          << new QStandardItem(bitrate)
                                          << new QStandardItem(server_type));

                QStandardItem *item = model->item(model->rowCount()-1, 0);
                item->setToolTip(server_name + "\n" + listen_url);
                item->setData(listen_url);

                server_name.clear();
                listen_url.clear();
                genre.clear();
                bitrate.clear();
                server_type.clear();
            }
        }
        else if (xml.isCharacters() && !xml.isWhitespace())
        {
            if (currentTag == "server_name")
                server_name += xml.text().toString();
            else if (currentTag == "listen_url")
                listen_url += xml.text().toString();
            else if (currentTag == "genre")
                genre += xml.text().toString();
            else if (currentTag == "bitrate")
                bitrate += xml.text().toString();
            else if(currentTag == "server_type")
                server_type += xml.text().toString();
        }
    }

    if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError)
    {
#ifdef Q_OS_WIN
        qWarning("StreamWindow: xml error: %d: %s", (int)xml.lineNumber(), qPrintable(xml.errorString()));
#else
        qWarning("StreamWindow: xml error: %lld: %s", xml.lineNumber(), qPrintable(xml.errorString()));
#endif
    }
}
