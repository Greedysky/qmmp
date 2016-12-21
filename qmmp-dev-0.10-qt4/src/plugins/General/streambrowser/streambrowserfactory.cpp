/***************************************************************************
 *   Copyright (C) 2012-2016 by Ilya Kotov                                 *
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
#include "streambrowser.h"
#include "streambrowserfactory.h"

const GeneralProperties StreamBrowserFactory::properties() const
{
    GeneralProperties properties;
    properties.name = tr("Stream Browser Plugin");
    properties.shortName = "streambrowser";
    properties.hasAbout = true;
    properties.hasSettings = false;
    properties.visibilityControl = false;
    return properties;
}

QObject *StreamBrowserFactory::create(QObject *parent)
{
    return new StreamBrowser(parent);
}

QDialog *StreamBrowserFactory::createConfigDialog(QWidget *parent)
{
    Q_UNUSED(parent);
    return 0;
}

void StreamBrowserFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About Stream Browser Plugin"),
                        tr("Qmmp Stream Browser Plugin")+"\n"+
                        tr("This plugin allows one to add stream from IceCast stream directory")+"\n"+
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *StreamBrowserFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/streambrowser_plugin_") + locale);
    return translator;
}

Q_EXPORT_PLUGIN2(streambrowser, StreamBrowserFactory)
