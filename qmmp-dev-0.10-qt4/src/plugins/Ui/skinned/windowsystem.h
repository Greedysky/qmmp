/***************************************************************************
 *   Based on Licq                                                         *
 *   Copyright (C) 2006-2009 Licq developers                               *
 *   Copyright (C) 2011-2013 Ilya Kotov                                    *
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

#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H

#include <QWidget>

class WindowSystem
{
public:
#ifdef Q_WS_X11
    static void ghostWindow(WId id);
    static QString netWindowManagerName();
    static void changeWinSticky(WId win, bool stick);
    static void setWinHint(WId win, const char *res_name, const char *res_class);
    static void revertGravity(WId win);
private:
    static unsigned char* getWindowProperty(WId win, const char* prop);
#endif
};
#endif // WINDOWSYSTEM_H
