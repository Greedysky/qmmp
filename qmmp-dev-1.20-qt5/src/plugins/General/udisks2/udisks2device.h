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
#ifndef UDISKS2DEVICE_H
#define UDISKS2DEVICE_H

#include <QObject>
#include <QDBusObjectPath>
#include <QDBusMetaType>
#include <QDBusArgument>
#include <QByteArray>
#include <QList>
#include <QVariant>

class QDBusInterface;

typedef QList<QByteArray> QByteArrayList;
Q_DECLARE_METATYPE(QByteArrayList)

inline const QDBusArgument &operator>>(const QDBusArgument &arg, QByteArrayList &list)
{
    arg.beginArray();
    list.clear();

    while (!arg.atEnd())
    {
        QByteArray element;
        arg >> element;
        list.append( element );
    }
    arg.endArray();
    return arg;
}

inline QDBusArgument &operator<<(QDBusArgument &arg, const QByteArrayList &list)
{
    arg.beginArray(qMetaTypeId<QByteArrayList>());
    for (int i = 0; i < list.count(); ++i)
        arg << list[i];
    arg.endArray();
    return arg;
}


/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class UDisks2Device : public QObject
{
    Q_OBJECT
public:
    UDisks2Device(QDBusObjectPath o, QObject *parent = 0);

    ~UDisks2Device();

    QVariant property(const QString &key) const;
    bool isRemovable() const;
    bool isMediaRemovable() const;
    bool isAudio() const;
    bool isMounted() const;
    bool isOptical() const;
    QStringList mountPoints() const;
    QString deviceFile() const;
    QDBusObjectPath objectPath() const;

signals:
    void changed();

private:
    QDBusInterface *m_block_interface;
    QDBusInterface *m_drive_interface;
    QDBusObjectPath m_path;
};

#endif
