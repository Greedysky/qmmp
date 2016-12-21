/***************************************************************************
 *   Copyright (C) 2013 by Ilya Kotov                                      *
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
#include <QByteArray>
#include <QDBusMetaType>
#include "udisks2device.h"

UDisks2Device::UDisks2Device(QDBusObjectPath o, QObject *parent) : QObject(parent)
{
    m_block_interface = new QDBusInterface("org.freedesktop.UDisks2", o.path(),
                                           "org.freedesktop.UDisks2.Block", QDBusConnection::systemBus(),
                                           this);

    QDBusObjectPath drive_object = property("Drive").value<QDBusObjectPath>();

    QDBusConnection::systemBus().connect("org.freedesktop.UDisks2", o.path(),
                                         "org.freedesktop.DBus.Properties","PropertiesChanged",
                                         this, SIGNAL(changed()));

    m_drive_interface = new QDBusInterface("org.freedesktop.UDisks2", drive_object.path(),
                                           "org.freedesktop.UDisks2.Drive", QDBusConnection::systemBus(),
                                           this);
    m_path = o;
}

UDisks2Device::~UDisks2Device()
{
}

QVariant UDisks2Device::property(const QString &key) const
{
    return m_block_interface->property(key.toAscii().data());
}

bool UDisks2Device::isRemovable() const
{
    return m_drive_interface->property("Removable").toBool();
}

bool UDisks2Device::isMediaRemovable() const
{
    return m_drive_interface->property("MediaRemovable").toBool();
}

bool UDisks2Device::isAudio() const
{
    return m_drive_interface->property("OpticalNumAudioTracks").toInt() > 0;
}

bool UDisks2Device::isMounted() const
{
    return !mountPoints().isEmpty();
}

bool UDisks2Device::isOptical() const
{
    return m_drive_interface->property("Optical").toBool();
}

QStringList UDisks2Device::mountPoints() const
{
    QStringList points;
    QDBusMessage message = QDBusMessage::createMethodCall("org.freedesktop.UDisks2", m_path.path(),
                                                          "org.freedesktop.DBus.Properties", "Get");

    QList<QVariant> arguments;
    arguments << "org.freedesktop.UDisks2.Filesystem" << "MountPoints";
    message.setArguments(arguments);

    QDBusMessage reply = QDBusConnection::systemBus().call(message);

    QList<QVariant> args = reply.arguments();

    foreach (QVariant arg, args)
    {
        QByteArrayList list;
        arg.value<QDBusVariant>().variant().value<QDBusArgument>() >> list;

        foreach (QByteArray p, list)
            points.append(p);
    }
    return points;
}

QString UDisks2Device::deviceFile() const
{
    return QString::fromAscii(m_block_interface->property("Device").toByteArray());
}

QDBusObjectPath UDisks2Device::objectPath() const
{
    return m_path;
}
