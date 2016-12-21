/***************************************************************************
 *   Copyright (C) 2014 by Ilya Kotov                                      *
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

#ifndef WINFILEASSOCPAGE_P_H
#define WINFILEASSOCPAGE_P_H

#include <QWidget>
#include <QStringList>

namespace Ui {
class WinFileAssocPage;
}

class QListWidgetItem;

/*! @internal
 */
class WinFileAssocPage : public QWidget
{
	Q_OBJECT
public:
    WinFileAssocPage(QWidget *parent = 0);
    virtual ~WinFileAssocPage();


private slots:
    void on_selectAll_clicked();
    void on_selectNone_clicked();
	
private:
    void loadAssociations();
    void saveAssociations();
    int ProcessAssociations(QStringList& current, QStringList& old);
    void createHelp();
    QStringList m_extensions;
    QStringList m_regExtensions;
    Ui::WinFileAssocPage *m_ui;
};

#endif
