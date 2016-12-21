/***************************************************************************
*   Copyright (C) 2006-2013 by Ilya Kotov                                 *
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

#ifndef ABOUTDIALOG_P_H
#define ABOUTDIALOG_P_H

#include <QDialog>
#include "ui_aboutdialog.h"

/**
    @internal
    @author Vladimir Kuznetsov <vovanec@gmail.com>
 */

class AboutDialog : public QDialog, private Ui::AboutDialog
{
    Q_OBJECT
public:
    AboutDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~AboutDialog();

private:
    QString loadAbout();
    QString getStringFromResource(const QString& res_file);

};

#endif
