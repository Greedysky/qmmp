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
#ifndef UDISKS2PLUGIN_H
#define UDISKS2PLUGIN_H

#include <QDBusObjectPath>
#include <qmmpui/general.h>

class UDisks2Manager;
class UDisks2Device;
class QActionGroup;
class QAction;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/

class UDisks2Plugin : public QObject
{
Q_OBJECT
public:
    UDisks2Plugin(QObject *parent = 0);

    ~UDisks2Plugin();

private slots:
    void removeDevice(QDBusObjectPath);
    void addDevice(QDBusObjectPath);
    void processAction(QAction *action);
    void updateActions();

private:
    QAction *findAction(const QString &dev_path);
    UDisks2Device *findDevice(QAction *action);
    void addPath(const QString &path);
    void removePath(const QString &path);
    UDisks2Manager *m_manager;
    QList <UDisks2Device *> m_devices;
    QActionGroup *m_actions;
    bool m_detectCDA;
    bool m_addTracks;
    bool m_removeTracks;
    bool m_detectRemovable;
    bool m_addFiles;
    bool m_removeFiles;
};

#endif //UDISKS2PLUGIN_H
