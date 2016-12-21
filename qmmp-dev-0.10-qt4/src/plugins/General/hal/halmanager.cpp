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

#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>

#include "halmanager.h"

HalManager::HalManager(QObject *parent)
        : QObject(parent)
{
    m_interface = new QDBusInterface("org.freedesktop.Hal", "/org/freedesktop/Hal/Manager",
                                     "org.freedesktop.Hal.Manager", QDBusConnection::systemBus(), this);

    m_interface->connection().connect("org.freedesktop.Hal", "/org/freedesktop/Hal/Manager",
                                      "org.freedesktop.Hal.Manager", "DeviceAdded",
                                      this, SIGNAL(deviceAdded(const QString &)));

    m_interface->connection().connect("org.freedesktop.Hal", "/org/freedesktop/Hal/Manager",
                                      "org.freedesktop.Hal.Manager", "DeviceRemoved",
                                      this, SIGNAL(deviceRemoved(const QString &)));

}


HalManager::~HalManager()
{
}

QStringList HalManager::findDeviceByCapability(const QString &cap)
{
    QDBusReply <QStringList> reply = m_interface->call("FindDeviceByCapability", cap);
    if (!reply.isValid())
    {
        qWarning("HalManager: error: %s", qPrintable(reply.error().name()));
        return QStringList();
    }
    return reply.value();
}
