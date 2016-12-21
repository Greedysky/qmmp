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
#ifndef DETAILSDIALOG_H
#define DETAILSDIALOG_H

#include <QDialog>
#include <QList>
#include <QMap>
#include <qmmp/qmmp.h>

class QTextCodec;
class QAbstractButton;
class PlayListTrack;
class MetaDataModel;

namespace Ui {
    class DetailsDialog;
}

/** @brief The DetailsDialog class provides dialog to show/edit metadata.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class DetailsDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Constructor.
     * @param tracks a list of tracks which should be used.
     * @param parent Parent widget.
     */
    DetailsDialog(QList<PlayListTrack *> tracks, QWidget *parent = 0);
    /*!
     * Destructor.
     */
    ~DetailsDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_tabWidget_currentChanged(int index);
    void on_directoryButton_clicked();
    void on_prevButton_clicked();
    void on_nextButton_clicked();

private:
    Ui::DetailsDialog *m_ui;
    void updatePage();
    void printInfo();
    QString formatRow(const QString key, const QString value);
    QString m_path;
    MetaDataModel *m_metaDataModel;
    PlayListTrack *m_track;
    QList<PlayListTrack *> m_tracks;
    QMap <Qmmp::MetaData, QString> m_metaData;
    int m_page;
};

#endif
