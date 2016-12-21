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
#ifndef ROOTOBJECT_H
#define ROOTOBJECT_H

#include <QObject>
#include <QVariantMap>

/**
	@author Ilya Kotov <forkotov02@hotmail.ru>
*/


struct Version
{
    quint16 major;
    quint16 minor;
};

Q_DECLARE_METATYPE(Version);

class RootObject : public QObject
{
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.freedesktop.MediaPlayer")
public:
    RootObject(QObject *parent = 0);

    ~RootObject();

public slots:
    QString Identity();
    Version MprisVersion();
    void Quit();
};

#endif
