/***************************************************************************
 *   Copyright (C) 2007 by Zhuravlev Uriy                                  *
 *   stalkerg@gmail.com                                                    *
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
#include "outputoss.h"
#include "outputossfactory.h"


Output* OutputOSSFactory::create()
{
    return new OutputOSS();
}

const OutputProperties OutputOSSFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("OSS Plugin");
    properties.shortName = "oss";
    properties.hasAbout = true;
    properties.hasSettings = true;
    return properties;
}

Volume *OutputOSSFactory::createVolume()
{
    return new VolumeOSS;
}

void OutputOSSFactory::showSettings(QWidget* parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}

void OutputOSSFactory::showAbout(QWidget *parent)
{
QMessageBox::about (parent, tr("About OSS Output Plugin"),
                        tr("Qmmp OSS Output Plugin")+"\n"+
                        tr("Written by: Yuriy Zhuravlev <slalkerg@gmail.com>")+"\n"+
                        tr("Based on code by: Brad Hughes <bhughes@trolltech.com>"));
}

QTranslator *OutputOSSFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/oss_plugin_") + locale);
    return translator;
}

Q_EXPORT_PLUGIN2(oss,OutputOSSFactory)
