/***************************************************************************
 *   Copyright (C) 2007-2015 by Ilya Kotov                                 *
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
#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QStyledItemDelegate>
#include <QApplication>
#include <QMouseEvent>


namespace Ui {
    class ConfigDialog;
}

class QListWidgetItem;
class QTreeWidgetItem;

/*! @brief Configuration dialog class.
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Constructor.
     * @param parent Parent widget
     */
    ConfigDialog(QWidget *parent = 0);
    /*!
     * Destructor
     */
    virtual ~ConfigDialog();
    /*!
     * Adds custom page in configuration dialog
     * @param name Localized name of the custom page
     * @param widget Custom page instance
     * @param icon Custom page icon
     */
    void addPage(const QString &name, QWidget *widget, const QIcon &icon = QIcon());

private slots:
    void on_contentsWidget_currentItemChanged (QListWidgetItem *current, QListWidgetItem *previous);
    void on_preferencesButton_clicked();
    void on_informationButton_clicked();
    void addGroupString(QAction *a);
    void saveSettings();
    void on_treeWidget_itemChanged (QTreeWidgetItem *item, int column);
    void on_treeWidget_currentItemChanged (QTreeWidgetItem *current, QTreeWidgetItem *);

private:
    void readSettings();
    void findSkins(const QString &path);
    void loadPluginsInfo();
    void createMenus();
    void loadLanguages();
    int m_insert_row;
    Ui::ConfigDialog *m_ui;
    QAction *m_preferencesAction;
    QAction *m_informationAction;

};

#endif
