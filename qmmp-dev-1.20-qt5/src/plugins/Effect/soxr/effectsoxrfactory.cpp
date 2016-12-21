/***************************************************************************
 *   Copyright (C) 2016 by Ilya Kotov                                      *
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
#include <qmmp/qmmp.h>
#include "settingsdialog.h"
#include "soxresampler.h"
#include "effectsoxrfactory.h"

const EffectProperties EffectSoXRFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("SoX Resampler Plugin");
    properties.shortName = "soxr";
    properties.hasSettings = true;
    properties.hasAbout = true;
    properties.priority = EffectProperties::EFFECT_PRIORITY_HIGH;
    return properties;
}

Effect *EffectSoXRFactory::create()
{
    return new SoXResampler();
}

void EffectSoXRFactory::showSettings(QWidget *parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s ->show();
}

void EffectSoXRFactory::showAbout(QWidget *parent)
{
     QMessageBox::about (parent, tr("About SoX Resampler Plugin"),
                        tr("Qmmp SoX Resampler Plugin")+"\n"+
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *EffectSoXRFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/soxr_plugin_") + locale);
    return translator;
}
