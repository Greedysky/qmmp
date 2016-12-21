/***************************************************************************
 *   Copyright (C) 2012 by Ilya Kotov                                      *
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
#ifndef STREAMWINDOW_H
#define STREAMWINDOW_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include "ui_streamwindow.h"

class QNetworkAccessManager;
class QNetworkReply;
class QStandardItemModel;
class QSortFilterProxyModel;
class QMenu;
class StreamsProxyModel;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class StreamWindow : public QWidget
{
Q_OBJECT
public:
    StreamWindow(QWidget *parent = 0);

    ~StreamWindow();

private slots:
    void showText(QNetworkReply *reply);
    void on_updatePushButton_clicked();
    void on_addPushButton_clicked();
    void on_filterLineEdit_textChanged(const QString &text);
    void execIceCastMenu(const QPoint &pos);
    void execFavoritesMenu(const QPoint &pos);
    void addToFavorites();
    void removeFromFavorites();

private:
    void closeEvent(QCloseEvent *);
    void readXml(QIODevice *input, QStandardItemModel *model);

    Ui::StreamWindow ui;
    QNetworkAccessManager *m_http;
    QNetworkReply *m_requestReply;
    QString m_artist, m_title;
    QStandardItemModel *m_iceCastModel, *m_favoritesModel;
    QSortFilterProxyModel *m_iceCastFilterModel, *m_favoritesFilterModel;
    QMenu *m_iceCastMenu;
    QMenu *m_favoritesMenu;
};

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class StreamsProxyModel: public QSortFilterProxyModel
{
Q_OBJECT
public:
    StreamsProxyModel(QObject *parent) : QSortFilterProxyModel(parent){}

protected:
    bool lessThan (const QModelIndex &left, const QModelIndex &right) const
    {
        if(left.column() == 2 && right.column() == 2)
        {
            return sourceModel()->data(left).toInt() < sourceModel()->data(right).toInt();
        }
        return QSortFilterProxyModel::lessThan(left, right);
    }
};


#endif
