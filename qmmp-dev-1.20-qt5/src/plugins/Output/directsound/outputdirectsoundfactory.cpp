/***************************************************************************
 *   Copyright (C) 2014 by Ilya Kotov                                      *
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


#include "outputdirectsound.h"
#include "outputdirectsoundfactory.h"

const OutputProperties OutputDirectSoundFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("DirectSound Plugin");
    properties.hasAbout = true;
    properties.hasSettings = false;
    properties.shortName = "directsound";
    return properties;
}

Output* OutputDirectSoundFactory::create()
{
    return new OutputDirectSound();
}

Volume *OutputDirectSoundFactory::createVolume()
{
    return new VolumeDirectSound();
}

void OutputDirectSoundFactory::showSettings(QWidget* parent)
{
   Q_UNUSED(parent);
}

void OutputDirectSoundFactory::showAbout(QWidget *parent)
{
   QMessageBox::about (parent, tr("About DirectSound Output Plugin"),
                        tr("Qmmp DirectSound Output Plugin")+"\n"+
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *OutputDirectSoundFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/directsound_plugin_") + locale);
    return translator;
}


