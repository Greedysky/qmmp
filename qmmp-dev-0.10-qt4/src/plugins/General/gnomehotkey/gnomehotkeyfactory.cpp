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

#include <QMessageBox>
#include <QTranslator>
#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "mediakeys.h"
#include "gnomehotkeyfactory.h"

const GeneralProperties GnomeHotkeyFactory::properties() const
{
    GeneralProperties properties;
    properties.name = tr("Gnome Hotkey Plugin");
    properties.shortName = "gnomehotkey";
    properties.hasAbout = true;
    properties.hasSettings = false;
    properties.visibilityControl = false;
    return properties;
}

QObject *GnomeHotkeyFactory::create(QObject *parent)
{
    return new MediaKeys(parent);
}

QDialog *GnomeHotkeyFactory::createConfigDialog(QWidget *parent)
{
    Q_UNUSED(parent);
    return 0;
}

void GnomeHotkeyFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About Gnome Hotkey Plugin"),
                        tr("Qmmp Gnome Hotkey Plugin") + "\n" +
                        tr("This plugin adds support of the GNOME/Cinnamon hotkeys") + "\n" +
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *GnomeHotkeyFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/gnomehotkey_plugin_") + locale);
    return translator;
}

Q_EXPORT_PLUGIN2(gnomehotkey, GnomeHotkeyFactory)
