/***************************************************************************
 *   Copyright (C) 2008 by Ilya Kotov                                      *
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

#include <QDBusMetaType>
#include <QDBusArgument>
#include <qmmpui/uihelper.h>
#include <qmmp/qmmp.h>
#include "rootobject.h"

//register << operator
QDBusArgument &operator << (QDBusArgument &arg, const Version &v)
{
    arg.beginStructure();
    arg << v.major;
    arg << v.minor;
    arg.endStructure();
    return arg;
}

//register >> operator
const QDBusArgument &operator >> (const QDBusArgument &arg, Version &v)
{
    arg.beginStructure();
    arg >> v.major;
    arg >> v.minor;
    arg.endStructure();
    return arg;
}

RootObject::RootObject(QObject *parent)
 : QObject(parent)
{
    qDBusRegisterMetaType<Version>();
}


RootObject::~RootObject()
{
}

QString RootObject::Identity()
{
    QString name = "Qmmp " + Qmmp::strVersion();
    return name;
}

Version RootObject::MprisVersion()
{
    struct Version v;
    v.major = 1;
    v.minor = 0;
    return v;
}

void RootObject::Quit()
{
    UiHelper::instance()->exit();
}
