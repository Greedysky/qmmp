/***************************************************************************
 *   Copyright (C) 2015 by Ivan Ponomarev                                  *
 *   ivantrue@gmail.com                                                    *
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


#include "outputqtmultimediafactory.h"
#include "settingsdialog.h"
#include <QMessageBox>
#include <QTranslator>
#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "outputqtmultimedia.h"


const OutputProperties OutputQtMultimediaFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("Qt Multimedia Plugin");
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.shortName = "qtmultimedia";
    return properties;
}

Output* OutputQtMultimediaFactory::create()
{
    return new OutputQtMultimedia();
}

Volume *OutputQtMultimediaFactory::createVolume()
{
    return 0;
}

void OutputQtMultimediaFactory::showSettings(QWidget* parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}

void OutputQtMultimediaFactory::showAbout(QWidget *parent)
{
   QMessageBox::about (parent, tr("About Qt Multimedia Output Plugin"),
                        tr("Qmmp Qt Multimedia Output Plugin")+"\n"+
                        tr("Written by: Ivan Ponomarev <ivantrue@gmail.com>"));
}

QTranslator *OutputQtMultimediaFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/qtmultimedia_plugin_") + locale);
    return translator;
}

Q_EXPORT_PLUGIN2(qtmultimedia, OutputQtMultimediaFactory)
