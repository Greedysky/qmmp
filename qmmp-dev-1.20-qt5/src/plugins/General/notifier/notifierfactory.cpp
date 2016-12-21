/***************************************************************************
 *   Copyright (C) 2008-2012 by Ilya Kotov                                 *
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
#include "notifier.h"
#include "settingsdialog.h"
#include "notifierfactory.h"

const GeneralProperties NotifierFactory::properties() const
{
    GeneralProperties properties;
    properties.name = tr("Notifier Plugin");
    properties.shortName = "notifier";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.visibilityControl = false;
    return properties;
}

QObject *NotifierFactory::create(QObject *parent)
{
    return new Notifier(parent);
}

QDialog *NotifierFactory::createConfigDialog(QWidget *parent)
{
    return new SettingsDialog(parent);
}

void NotifierFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About Notifier Plugin"),
                        tr("Qmmp Notifier Plugin")+"\n"+
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *NotifierFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/notifier_plugin_") + locale);
    return translator;
}
