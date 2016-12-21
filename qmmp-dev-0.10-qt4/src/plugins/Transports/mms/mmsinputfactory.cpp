/***************************************************************************
 *   Copyright (C) 2010-2013 by Ilya Kotov                                 *
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

#include <QtPlugin>
#include <QMessageBox>
#include <QTranslator>
#include <qmmp/qmmp.h>
#include "settingsdialog.h"
#include "mmsinputsource.h"
#include "mmsinputfactory.h"

const InputSourceProperties MMSInputFactory::properties() const
{
    InputSourceProperties p;
    p.protocols << "mms" << "mmsh" << "mmst" << "mmsu";
    p.name = tr("MMS Plugin");
    p.shortName = "mms";
    p.hasAbout = true;
    p.hasSettings = true;
    return p;
}

InputSource *MMSInputFactory::create(const QString &url, QObject *parent)
{
    return new MMSInputSource(url, parent);
}

void MMSInputFactory::showSettings(QWidget *parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}

void MMSInputFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About MMS Transport Plugin"),
                        tr("Qmmp MMS Transport Plugin")+"\n"+
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *MMSInputFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/mms_plugin_") + locale);
    return translator;
}
Q_EXPORT_PLUGIN2(mms, MMSInputFactory)
