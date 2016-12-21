/***************************************************************************
 *   Copyright (C) 2011-2015 by Ilya Kotov                                 *
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

#ifndef SKINNEDSETTINGS_H
#define SKINNEDSETTINGS_H

#include <QWidget>
#include <QFileInfo>
#include "ui_skinnedsettings.h"

class SkinReader;
class Skin;

class SkinnedSettings : public QWidget
{
    Q_OBJECT
public:
    explicit SkinnedSettings(QWidget *parent = 0);
    virtual ~SkinnedSettings();
    void writeSettings();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *);
    void on_plFontButton_clicked();
    void on_headerFontButton_clicked();
    void on_mainFontButton_clicked();
    void on_resetFontsButton_clicked();
    void on_skinInstallButton_clicked();
    void loadSkins();
    void on_popupTemplateButton_clicked();
    void addWindowTitleString(QAction *a);



private:
    void showEvent(QShowEvent *);
    void loadFonts();
    void findSkins(const QString &path);
    void createActions();
    void readSettings();

    Ui::SkinnedSettings m_ui;
    QList <QFileInfo> m_skinList;
    QString m_currentSkinName;
    Skin *m_skin;
    SkinReader *m_reader;

};

#endif // SKINNEDSETTINGS_H
