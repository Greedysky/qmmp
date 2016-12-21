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
#ifndef HALDEVICE_H
#define HALDEVICE_H

#include <QObject>
#include <QDBusMetaType>
#include <QList>
#include <QVariant>

class QDBusInterface;

/**
 *   This structure has been copied from Solid (KDE hardware library)
 */
struct ChangeDescription
{
    QString key;
    bool added;
    bool removed;
};

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class HalDevice : public QObject
{
    Q_OBJECT
public:
    HalDevice(const QString &udi, QObject *parent = 0);

    ~HalDevice();

    QVariant property (const QString &key);
    QString udi() const;

signals:
    void propertyModified(int count, const QList<ChangeDescription> &changes);
    void condition(const QString &condition, const QString &reason);

private:
    QDBusInterface *m_interface;
    QString m_udi;
};

#endif
