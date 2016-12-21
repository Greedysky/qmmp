/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
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

#include <QTranslator>
#include <QMessageBox>
#include <bs2b/bs2bversion.h>
#include <qmmp/qmmp.h>
#include "effectbs2bfactory.h"
#include "settingsdialog.h"
#include "bs2bplugin.h"

const EffectProperties EffectBs2bFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("BS2B Plugin");
    properties.shortName = "bs2b";
    properties.hasSettings = true;
    properties.hasAbout = true;
    return properties;
}

Effect *EffectBs2bFactory::create()
{
    return new Bs2bPlugin();
}

void EffectBs2bFactory::showSettings(QWidget *parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s ->show();
}

void EffectBs2bFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About BS2B Effect Plugin"),
                        "<p>"+tr("This is the Qmmp plugin version of Boris Mikhaylov's headphone DSP "
                                 "effect \"Bauer stereophonic-to-binaural\", abbreviated bs2b.")+"</p>"+
                        QString("<p>" + tr("Visit %1 for more details")+"</p>")
                        .arg("<a href=\"http://bs2b.sourceforge.net/\">http://bs2b.sourceforge.net/</a>")+
                        QString("<p>" + tr("Compiled against libbs2b-%1") + "</p>").arg(BS2B_VERSION_STR)+
                        "<p>"+tr("Developers:")+"<br>"+
                        tr("Ilya Kotov") + " &lt;forkotov02@hotmail.ru&gt;"+"<br>"+
                        tr("Sebastian Pipping") + " &lt;sebastian@pipping.org&gt;"+"</p>");
}

QTranslator *EffectBs2bFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/bs2b_plugin_") + locale);
    return translator;
}
