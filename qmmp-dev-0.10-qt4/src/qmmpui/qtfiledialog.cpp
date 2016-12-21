/***************************************************************************
 *   Copyright (C) 2008-2016 by Ilya Kotov                                 *
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

#include <QFileDialog>
#include "qtfiledialog_p.h"


FileDialog* QtFileDialogFactory::create()
{
    qDebug("QtFileDialogFactory::create()");
    return new QtFileDialog();
}

const FileDialogProperties QtFileDialogFactory::properties() const
{
    FileDialogProperties properties;
    properties.name = tr("Qt File Dialog");
    properties.shortName = "qt_dialog";
    properties.hasAbout = false;
    properties.modal = true;
    return properties;
}

void QtFileDialogFactory::showAbout(QWidget*){}

QTranslator *QtFileDialogFactory::createTranslator(QObject *parent)
{
    Q_UNUSED(parent)
    return 0;
}

QtFileDialog::~QtFileDialog()
{
    qDebug("QtFileDialog::~QtFileDialog()");
}

QStringList QtFileDialog::exec(QWidget *parent, const QString &dir, FileDialog::Mode mode,
                               const QString &caption, const QString &filter, QString *selectedFilter)
{
    QStringList list;
    if(mode == AddFile)
    {
        list << QFileDialog::getOpenFileName(parent,caption,dir,filter,selectedFilter);
    }
    else if(mode == AddDir || mode == AddDirs)
    {
        list << QFileDialog::getExistingDirectory(parent,caption,dir, QFileDialog::ShowDirsOnly);
    }
    else if(mode == AddFiles || mode == AddDirsFiles || mode == PlayDirsFiles)
    {
        list << QFileDialog::getOpenFileNames(parent,caption,dir,filter,selectedFilter);
    }
    else if(mode == SaveFile)
    {
        list << QFileDialog::getSaveFileName(parent,caption,dir,filter,selectedFilter);
    }
    return list;
}
