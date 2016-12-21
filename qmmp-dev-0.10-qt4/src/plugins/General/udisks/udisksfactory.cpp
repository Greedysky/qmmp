/***************************************************************************
 *   Copyright (C) 2010 by Ilya Kotov                                      *
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
#include <QTranslator>
#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "settingsdialog.h"
#include "udisksplugin.h"
#include "udisksfactory.h"

const GeneralProperties UDisksFactory::properties() const
{
    GeneralProperties properties;
    properties.name = tr("UDisks Plugin");
    properties.shortName = "udisks";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.visibilityControl = false;
    return properties;
}

QObject *UDisksFactory::create(QObject *parent)
{
    return new UDisksPlugin(parent);
}

QDialog *UDisksFactory::createConfigDialog(QWidget *parent)
{
    return new SettingsDialog(parent);
}

void UDisksFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About UDisks Plugin"),
                        tr("Qmmp UDisks Plugin") + "\n" +
                        tr("This plugin provides removable devices detection using UDisks") + "\n" +
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *UDisksFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/udisks_plugin_") + locale);
    return translator;
}

Q_EXPORT_PLUGIN2(udisks, UDisksFactory)
