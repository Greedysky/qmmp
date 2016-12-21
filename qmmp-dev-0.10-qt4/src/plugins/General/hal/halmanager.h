/***************************************************************************
 *   Copyright (C) 2009 by Ilya Kotov                                      *
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
#ifndef HALMANAGER_H
#define HALMANAGER_H

#include <QObject>
#include <QStringList>

class QDBusInterface;

/**
	@author Ilya Kotov <forkotov02@hotmail.ru>
*/
class HalManager : public QObject
{
Q_OBJECT
public:
    HalManager(QObject *parent = 0);

    ~HalManager();

    QStringList findDeviceByCapability(const QString &cap);

signals:
    void deviceAdded(const QString &udi);
    void deviceRemoved(const QString &udi);

private:
    QDBusInterface *m_interface;

};

#endif
