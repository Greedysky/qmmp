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
#include <qmmp/qmmp.h>
#include "mpris.h"
#include "mprisfactory.h"

const GeneralProperties MPRISFactory::properties() const
{
    GeneralProperties properties;
    properties.name = tr("MPRIS Plugin");
    properties.shortName = "mpris";
    properties.hasAbout = true;
    properties.hasSettings = false;
    properties.visibilityControl = false;
    return properties;
}

QObject *MPRISFactory::create(QObject *parent)
{
    return new MPRIS(parent);
}

QDialog *MPRISFactory::createConfigDialog(QWidget *)
{
    return 0;
}

void MPRISFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About MPRIS Plugin"),
                        tr("Qmmp MPRIS Plugin")+"\n"+
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *MPRISFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/mpris_plugin_") + locale);
    return translator;
}

Q_EXPORT_PLUGIN2(mpris, MPRISFactory)
