/***************************************************************************
 *   Copyright (C) 2013 by Ilya Kotov                                      *
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
#include <qmmp/qmmp.h>
#include "settingsdialog.h"
#include "udisks2plugin.h"
#include "udisks2factory.h"

const GeneralProperties UDisks2Factory::properties() const
{
    GeneralProperties properties;
    properties.name = tr("UDisks2 Plugin");
    properties.shortName = "udisks2";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.visibilityControl = false;
    return properties;
}

QObject *UDisks2Factory::create(QObject *parent)
{
    return new UDisks2Plugin(parent);
}

QDialog *UDisks2Factory::createConfigDialog(QWidget *parent)
{
    return new SettingsDialog(parent);
}

void UDisks2Factory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About UDisks2 Plugin"),
                        tr("Qmmp UDisks2 Plugin") + "\n" +
                        tr("This plugin provides removable devices detection using UDisks2") + "\n" +
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *UDisks2Factory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/udisks2_plugin_") + locale);
    return translator;
}
