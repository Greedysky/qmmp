/***************************************************************************
 *   Copyright (C) 2010 by Ilya Kotov                                      *
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

#include "udisksmanager.h"

UDisksManager::UDisksManager(QObject *parent)
        : QObject(parent)
{
    m_interface = new QDBusInterface("org.freedesktop.UDisks", "/org/freedesktop/UDisks",
                                     "org.freedesktop.UDisks", QDBusConnection::systemBus(), this);

    m_interface->connection().connect("org.freedesktop.UDisks", "/org/freedesktop/UDisks",
                                      "org.freedesktop.UDisks", "DeviceAdded",
                                      this, SIGNAL(deviceAdded(QDBusObjectPath)));

    m_interface->connection().connect("org.freedesktop.UDisks", "/org/freedesktop/UDisks",
                                      "org.freedesktop.UDisks", "DeviceRemoved",
                                      this, SIGNAL(deviceRemoved(QDBusObjectPath)));

    m_interface->connection().connect("org.freedesktop.UDisks", "/org/freedesktop/UDisks",
                                      "org.freedesktop.UDisks", "DeviceChanged",
                                      this, SIGNAL(deviceChanged(QDBusObjectPath)));

}


UDisksManager::~UDisksManager()
{
}

QList<QDBusObjectPath> UDisksManager::findAllDevices()
{
    QDBusReply <QList<QDBusObjectPath> > reply = m_interface->call("EnumerateDevices");
    if (!reply.isValid())
    {
        qWarning("UDisksManager: error: %s", qPrintable(reply.error().name()));
        return QList<QDBusObjectPath>();
    }
    return reply.value();
}
