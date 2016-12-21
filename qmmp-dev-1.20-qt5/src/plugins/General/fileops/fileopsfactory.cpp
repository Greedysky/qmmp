/***************************************************************************
 *   Copyright (C) 2009-2012 by Ilya Kotov                                 *
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


#include <QMessageBox>
#include "fileops.h"
#include "settingsdialog.h"
#include "fileopsfactory.h"

const GeneralProperties FileOpsFactory::properties() const
{
    GeneralProperties properties;
    properties.name = tr("File Operations Plugin");
    properties.shortName = "fileops";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.visibilityControl = false;
    return properties;
}

QObject *FileOpsFactory::create(QObject *parent)
{
    return new FileOps(parent);
}

QDialog *FileOpsFactory::createConfigDialog(QWidget *parent)
{
    return new SettingsDialog(parent);
}

void FileOpsFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About File Operations Plugin"),
                        tr("Qmmp File Operations Plugin")+"\n"+
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *FileOpsFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/fileops_plugin_") + locale);
    return translator;
}
