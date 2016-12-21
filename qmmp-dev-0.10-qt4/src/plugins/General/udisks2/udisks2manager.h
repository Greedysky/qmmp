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
#ifndef UDISKS2MANAGER_H
#define UDISKS2MANAGER_H

#include <QObject>
#include <QList>
#include <QVariantMap>
#include <QDBusObjectPath>

class QDBusInterface;

typedef QMap<QString,QVariantMap> QVariantMapMap;
Q_DECLARE_METATYPE(QVariantMapMap)

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class UDisks2Manager : public QObject
{
Q_OBJECT
public:
    UDisks2Manager(QObject *parent = 0);

    ~UDisks2Manager();

    QList<QDBusObjectPath> findAllDevices();

signals:
    void deviceAdded(QDBusObjectPath);
    void deviceRemoved(QDBusObjectPath);

private slots:
    void onInterfacesAdded(const QDBusObjectPath &object_path, const QVariantMapMap &);
    void onInterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &);

private:
    QDBusInterface *m_interface;

};

#endif
