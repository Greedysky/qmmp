/***************************************************************************
 *   Copyright (C) 2010-2014 by Ilya Kotov                                 *
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

#include <QIcon>
#include <QSettings>
#include <QDir>
#include "lxdesupport.h"

void LXDESupport::load()
{
    if(qgetenv("XDG_CURRENT_DESKTOP") != "LXDE" && qgetenv("DESKTOP_SESSION") != "LXDE")
        return;

    QString config_dir = qgetenv("XDG_CONFIG_HOME");
    if(config_dir.isEmpty())
        config_dir = QDir::homePath() + "/.config/";

    QString config_file = config_dir + "/lxsession/LXDE/desktop.conf";
    QString themeName = "nuoveXT2";
    if(qgetenv("DESKTOP_SESSION") == "Lubuntu")
    {
        qDebug("LXDESupport: using Lubuntu configuration");
        config_file = config_dir + "/lxsession/Lubuntu/desktop.conf";
        themeName = "lubuntu";
    }

    QSettings lxde_settings(config_file, QSettings::IniFormat);
    themeName = lxde_settings.value("GTK/sNet/IconThemeName", themeName).toString();
    if(!themeName.isEmpty())
        QIcon::setThemeName(themeName);
}
