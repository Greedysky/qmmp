/***************************************************************************
 *   Copyright (C) 2009-2012 by Ilya Kotov                                 *
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
#ifndef DEVDETECT_H
#define DEVDETECT_H


#include <qmmpui/general.h>

class HalManager;
class HalDevice;
class QActionGroup;
class QAction;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/

class HalPlugin : public QObject
{
Q_OBJECT
public:
    HalPlugin(QObject *parent = 0);

    ~HalPlugin();

private slots:
    void removeDevice(const QString &udi);
    void addDevice(const QString &udi);
    void processAction(QAction *action);
    void updateActions();

private:
    QAction *findAction(const QString &dev_path);
    HalDevice *findDevice(QAction *action);
    void addPath(const QString &path);
    void removePath(const QString &path);
    HalManager *m_manager;
    QList <HalDevice *> m_devices;
    QActionGroup *m_actions;
    bool m_detectCDA;
    bool m_addTracks;
    bool m_removeTracks;
    bool m_detectRemovable;
    bool m_addFiles;
    bool m_removeFiles;
};

#endif
