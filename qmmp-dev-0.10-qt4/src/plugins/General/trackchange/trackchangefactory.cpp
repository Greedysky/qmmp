/***************************************************************************
 *   Copyright (C) 2013-2016 by Ilya Kotov                                 *
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
#include "trackchange.h"
#include "settingsdialog.h"
#include "trackchangefactory.h"

const GeneralProperties TrackChangeFactory::properties() const
{
    GeneralProperties properties;
    properties.name = tr("Track Change Plugin");
    properties.shortName = "trackchange";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.visibilityControl = false;
    return properties;
}

QObject *TrackChangeFactory::create(QObject *parent)
{
    return new TrackChange(parent);
}

QDialog *TrackChangeFactory::createConfigDialog(QWidget *parent)
{
    return new SettingsDialog(parent);
}

void TrackChangeFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About Track Change Plugin"),
                        tr("Qmmp Track Change Plugin")+"\n"+
                        tr("This plugin executes external command when current track is changed") + "\n" +
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *TrackChangeFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/trackchange_plugin_") + locale);
    return translator;
}

Q_EXPORT_PLUGIN2(trackchange, TrackChangeFactory)
