/***************************************************************************
 *   Copyright (C) 2009 by Ilya Kotov                                      *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   Copyright (C) 2006 by Kevin Ottens <ervin@kde.org>                    *
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

#include "haldevice.h"


Q_DECLARE_METATYPE(ChangeDescription);
Q_DECLARE_METATYPE(QList<ChangeDescription>);

/**
 *   This function has been copied from Solid (KDE hardware library)
 */

const QDBusArgument &operator<<(QDBusArgument &arg, const ChangeDescription &change)
{
    arg.beginStructure();
    arg << change.key << change.added << change.removed;
    arg.endStructure();
    return arg;
}

/**
 *   This function has been copied from Solid (KDE hardware library)
 */

const QDBusArgument &operator>>(const QDBusArgument &arg, ChangeDescription &change)
{
    arg.beginStructure();
    arg >> change.key >> change.added >> change.removed;
    arg.endStructure();
    return arg;
}

HalDevice::HalDevice(const QString &udi, QObject *parent)
        : QObject(parent)
{
    qDBusRegisterMetaType<ChangeDescription>();
    qDBusRegisterMetaType< QList<ChangeDescription> >();
    m_interface = new QDBusInterface("org.freedesktop.Hal", udi,
                                     "org.freedesktop.Hal.Device", QDBusConnection::systemBus(), this);
    m_interface->connection().connect("org.freedesktop.Hal", udi,
                                      "org.freedesktop.Hal.Device","PropertyModified",
                                      this, SIGNAL(propertyModified(int, const QList<ChangeDescription> &)));
    m_interface->connection().connect("org.freedesktop.Hal", udi,
                                      "org.freedesktop.Hal.Device","Condition",
                                      this, SIGNAL(condition(const QString &, const QString &)));
    m_udi = udi;
}

QVariant HalDevice::property (const QString &key)
{
    QDBusMessage reply = m_interface->call("GetProperty", key);
    if (reply.type() != QDBusMessage::ReplyMessage && reply.errorName() != "org.freedesktop.Hal.NoSuchProperty")
    {
        qWarning("%s error: %s, key: %s", Q_FUNC_INFO, qPrintable(reply.errorName()), qPrintable(key));
        return QVariant();
    }

    if (reply.errorName() != "org.freedesktop.Hal.NoSuchProperty")
        return reply.arguments().at(0);
    else
        return QVariant();
}

HalDevice::~HalDevice()
{
}

QString HalDevice::udi() const
{
    return m_udi;
}
