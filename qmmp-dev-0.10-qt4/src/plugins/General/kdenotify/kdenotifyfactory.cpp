/***************************************************************************
 *   Copyright (C) 2009 by Artur Guzik                                     *
 *   a.guzik88@gmail.com                                                   *
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
#include "kdenotifyfactory.h"
#include "kdenotify.h"
#include "settingsdialog.h"


const GeneralProperties KdeNotifyFactory::properties() const
{
    GeneralProperties properties;
    properties.name = tr("KDE 4 notification plugin");
    properties.shortName = "kdenotify_icon";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.visibilityControl = false;
    return properties;
}

QObject *KdeNotifyFactory::create(QObject *parent)
{
    return new KdeNotify(parent);
}

QDialog *KdeNotifyFactory::createConfigDialog(QWidget *parent)
{
    return new SettingsDialog(parent);
}

void KdeNotifyFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About KDE Notification Plugin"),
                        tr("KDE 4 notification plugin for Qmmp \n") +
                        "Artur Guzik <a.guzik88@gmail.com>");
}

QTranslator *KdeNotifyFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/kdenotify_plugin_") + locale);
    return translator;
}

Q_EXPORT_PLUGIN2(kdenotify, KdeNotifyFactory)
