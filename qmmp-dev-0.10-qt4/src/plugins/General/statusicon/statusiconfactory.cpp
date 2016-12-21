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

#include <QMessageBox>
#include <QTranslator>
#include <QtPlugin>
#include "statusicon.h"
#include "settingsdialog.h"
#include "statusiconfactory.h"

const GeneralProperties StatusIconFactory::properties() const
{
    GeneralProperties properties;
    properties.name = tr("Status Icon Plugin");
    properties.shortName = "status_icon";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.visibilityControl = true;
    return properties;
}

QObject *StatusIconFactory::create(QObject *parent)
{
    return new StatusIcon(parent);
}

QDialog *StatusIconFactory::createConfigDialog(QWidget *parent)
{
    return new SettingsDialog(parent);
}

void StatusIconFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About Status Icon Plugin"),
                        tr("Qmmp Status Icon Plugin")+"\n"+
                        tr("Written by:") + "\n"+
                        tr("Ilya Kotov <forkotov02@hotmail.ru>")+"\n"+
                        tr("Artur Guzik <a.guzik88@gmail.com>"));
}

QTranslator *StatusIconFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/statusicon_plugin_") + locale);
    return translator;
}

Q_EXPORT_PLUGIN2(statusicon, StatusIconFactory)
