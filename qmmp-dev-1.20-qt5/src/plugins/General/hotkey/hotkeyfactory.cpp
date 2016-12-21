/***************************************************************************
 *   Copyright (C) 2009-2012 by Ilya Kotov                                 *
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
#include <qmmp/qmmp.h>
#include "settingsdialog.h"
#include "hotkeymanager.h"
#include "hotkeyfactory.h"

const GeneralProperties HotkeyFactory::properties() const
{
    GeneralProperties properties;
    properties.name = tr("Global Hotkey Plugin");
    properties.shortName = "hotkey";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.visibilityControl = false;
    return properties;
}

QObject *HotkeyFactory::create(QObject *parent)
{
    return new HotkeyManager(parent);
}

QDialog *HotkeyFactory::createConfigDialog(QWidget *parent)
{
    return new SettingsDialog(parent);
}

void HotkeyFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About Global Hotkey Plugin"),
                        tr("Qmmp Global Hotkey Plugin")+"\n"+
                        tr("This plugin adds support for multimedia keys or global key combinations")+"\n"+
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *HotkeyFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/hotkey_plugin_") + locale);
    return translator;
}
