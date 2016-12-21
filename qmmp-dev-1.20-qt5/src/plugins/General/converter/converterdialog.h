/***************************************************************************
 *   Copyright (C) 2011-2016 by Ilya Kotov                                 *
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

#ifndef CONVERTERDIALOG_H
#define CONVERTERDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QCloseEvent>
#include "ui_converterdialog.h"

class QAction;
class PlayListTrack;
class ConverterPreset;
class Converter;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class ConverterDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConverterDialog(QList <PlayListTrack *> items,  QWidget *parent = 0);
    virtual ~ConverterDialog();

public slots:
    virtual void reject();

private slots:
    void on_dirButton_clicked();
    void on_convertButton_clicked();
    void on_stopButton_clicked();
    void onStateChanged(int row, QString message);
    void onConvertFinished(Converter *c);
    void addTitleString(QAction *a);
    void createPreset();
    void editPreset();
    void copyPreset();
    void deletePreset();

private:
    void createMenus();
    void readPresets(const QString &path);
    void savePresets();
    QVariantMap preset() const;
    QString uniqueName(const QString &name);
    bool checkPreset(const QVariantMap &preset);

    Ui::ConverterDialog m_ui;
    QList <Converter *> m_converters;

};

#endif // CONVERTERDIALOG_H
