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
#include <QDBusReply>
#include <QDBusArgument>
#include <QDBusMetaType>
#include <QXmlStreamReader>
#include "udisks2manager.h"

UDisks2Manager::UDisks2Manager(QObject *parent)
        : QObject(parent)
{
    m_interface = new QDBusInterface("org.freedesktop.UDisks2", "/org/freedesktop/UDisks2",
                                     "org.freedesktop.DBus.ObjectManager",
                                     QDBusConnection::systemBus(), this);

    m_interface->connection().connect("org.freedesktop.UDisks2", "/org/freedesktop/UDisks2",
                                      "org.freedesktop.DBus.ObjectManager", "InterfacesAdded",
                                      this, SLOT(onInterfacesAdded(QDBusObjectPath,QVariantMapMap)));

    m_interface->connection().connect("org.freedesktop.UDisks2", "/org/freedesktop/UDisks2",
                                      "org.freedesktop.DBus.ObjectManager", "InterfacesRemoved",
                                      this, SIGNAL(onInterfacesRemoved(QDBusObjectPath, QStringList)));
}


UDisks2Manager::~UDisks2Manager()
{
}

QList<QDBusObjectPath> UDisks2Manager::findAllDevices()
{
    QList<QDBusObjectPath> paths;
    QDBusMessage call = QDBusMessage::createMethodCall("org.freedesktop.UDisks2",
                                                       "/org/freedesktop/UDisks2/block_devices",
                                                       "org.freedesktop.DBus.Introspectable",
                                                       "Introspect");
    QDBusPendingReply<QString> reply = QDBusConnection::systemBus().call(call);


    if (!reply.isValid())
    {
        qWarning("UDisks2Manager: error: %s", qPrintable(reply.error().name()));
        return paths;
    }
    QXmlStreamReader xml(reply.value());
    while (!xml.atEnd())
    {
        xml.readNext();
        if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name().toString() == "node" )
        {
            QString name = xml.attributes().value("name").toString();
            if(!name.isEmpty())
                paths << QDBusObjectPath("/org/freedesktop/UDisks2/block_devices/" + name);
        }
    }
    return paths;
}

void UDisks2Manager::onInterfacesAdded(const QDBusObjectPath &object_path, const QVariantMapMap &)
{
    if(object_path.path().startsWith("/org/freedesktop/UDisks2/jobs"))
        return;
    emit deviceAdded(object_path);
}

void UDisks2Manager::onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &)
{
    if(object_path.path().startsWith("/org/freedesktop/UDisks2/jobs"))
        return;
    emit deviceRemoved(object_path);
}
