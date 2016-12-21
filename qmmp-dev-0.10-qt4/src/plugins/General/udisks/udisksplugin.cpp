/***************************************************************************
 *   Copyright (C) 2010-2014 by Ilya Kotov                                 *
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

#include <QtDBus>
#include <QActionGroup>
#include <QApplication>
#include <QStyle>
#include <qmmpui/uihelper.h>
#include <qmmpui/mediaplayer.h>
#include <qmmpui/playlistmanager.h>
#include <qmmpui/playlistitem.h>
#include <qmmp/qmmp.h>
#include "udisksdevice.h"
#include "udisksmanager.h"
#include "udisksplugin.h"

UDisksPlugin::UDisksPlugin(QObject *parent) : QObject(parent)
{
    m_manager = new UDisksManager(this);
    m_actions = new QActionGroup(this);
    connect(m_manager,SIGNAL(deviceAdded(QDBusObjectPath)), SLOT(addDevice(QDBusObjectPath)));
    connect(m_manager,SIGNAL(deviceChanged(QDBusObjectPath)), SLOT(addDevice(QDBusObjectPath)));
    connect(m_manager,SIGNAL(deviceRemoved(QDBusObjectPath)), SLOT(removeDevice(QDBusObjectPath)));
    connect(m_actions,SIGNAL(triggered (QAction *)), SLOT(processAction(QAction *)));
    //load settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("UDisks");
    m_detectCDA = settings.value("cda", true).toBool();
    m_detectRemovable = settings.value("removable", true).toBool();
    m_addTracks = false; //do not load tracks on startup
    m_addFiles = false;
    //find existing devices
    QList<QDBusObjectPath> devs = m_manager->findAllDevices();
    foreach(QDBusObjectPath o, devs)
        addDevice(o);
    //load remaining settings
    m_addTracks = settings.value("add_tracks", false).toBool();
    m_removeTracks = settings.value("remove_tracks", false).toBool();
    m_addFiles = settings.value("add_files", false).toBool();
    m_removeFiles = settings.value("remove_files", false).toBool();
    settings.endGroup();
}


UDisksPlugin::~UDisksPlugin()
{
}

void UDisksPlugin::removeDevice(QDBusObjectPath o)
{
    foreach(UDisksDevice *device, m_devices)
    {
        if (device->objectPath() == o)
        {
            m_devices.removeAll(device);
            delete device;
            qDebug("UDisksPlugin: device \"%s\" removed", qPrintable(o.path()));
            updateActions();
            break;
        }
    }
}

void UDisksPlugin::addDevice(QDBusObjectPath o)
{
    foreach(UDisksDevice *device, m_devices) //is it already exists?
    {
        if (device->objectPath() == o)
            return;
    }
    UDisksDevice *device = new UDisksDevice(o, this);

    //audio cd
    if (device->property("DeviceIsOpticalDisc").toBool() &&
            device->property("OpticalDiscNumAudioTracks").toInt())
    {
        if (m_detectCDA)
        {
            qDebug("UDisksPlugin: device \"%s\" added (cd audio)", qPrintable(o.path()));
            m_devices << device;
            connect(device, SIGNAL(changed()), SLOT(updateActions()));
            updateActions();
        }
        else
            delete device;
        return;
    }

    // filter mounted partitions
    if ((!device->property("DeviceIsPartition").toBool() ||
            device->property("DeviceIsSystemInternal").toBool()) &&
            !device->property("DeviceIsOpticalDisc").toBool())
    {
        delete device;
        return;
    }
    if (device->property("PartitionSize").toLongLong() < 40000000000LL &&
            (device->property("IdType").toString() == "vfat" ||
             device->property("IdType").toString() == "iso9660" ||
             device->property("IdType").toString() == "udf" ||
             device->property("IdType").toString() == "ext2"))
    {
        if (m_detectRemovable)
        {
            qDebug("UDisksPlugin: device \"%s\" added (removable)", qPrintable(o.path()));
            m_devices << device;
            updateActions();
            connect(device, SIGNAL(changed()), SLOT(updateActions()));
        }
        else
            delete device;
        return;
    }
    delete device;
}

void UDisksPlugin::updateActions()
{
    // add action for cd audio or mounted volume
    foreach(UDisksDevice *device, m_devices)
    {
        QString dev_path;
        if (device->property("DeviceIsOpticalDisc").toBool() &&
                device->property("OpticalDiscNumAudioTracks").toInt()) //cd audio
            dev_path = "cdda://" + device->property("DeviceFile").toString();
        else if (device->property("DeviceIsMounted").toBool()) //mounted volume
            dev_path = device->property("DeviceMountPaths").toStringList()[0];
        else
            continue;

        if (!findAction(dev_path))
        {
            QAction *action = new QAction(this);
            QString actionText;
            if (device->property("DeviceIsOpticalDisc").toBool()
                    && device->property("OpticalDiscNumAudioTracks").toInt())
                actionText = QString(tr("Add CD \"%1\"")).arg(device->property("DeviceFile").toString());
            else
            {
                QString name = device->property("IdLabel").toString();
                if (name.isEmpty())
                    name = dev_path;
                actionText = QString(tr("Add Volume \"%1\"")).arg(name);
            }

            if (device->property("DeviceIsOpticalDisc").toBool())
            {
                if(device->property("IdType").toString() == "iso9660")
                    action->setIcon(qApp->style()->standardIcon(QStyle::SP_DriveDVDIcon));
                else
                    action->setIcon(qApp->style()->standardIcon(QStyle::SP_DriveCDIcon));
            }
            else
                action->setIcon(qApp->style()->standardIcon(QStyle::SP_DriveHDIcon));

            action->setText(actionText);
            action->setData(dev_path);
            m_actions->addAction(action);
            UiHelper::instance()->addAction(action, UiHelper::TOOLS_MENU);
            addPath(dev_path);
        }
    }
    // remove action if device is unmounted/removed
    foreach(QAction *action, m_actions->actions ())
    {
        if (!findDevice(action))
        {
            m_actions->removeAction(action);
            UiHelper::instance()->removeAction(action);
            removePath(action->data().toString());
            action->deleteLater();
        }
    }
}

void UDisksPlugin::processAction(QAction *action)
{
    qDebug("UDisksPlugin: action triggered: %s", qPrintable(action->data().toString()));
    QString path = action->data().toString();
    MediaPlayer::instance()->playListManager()->selectedPlayList()->add(path);
}

QAction *UDisksPlugin::findAction(const QString &dev_path)
{
    foreach(QAction *action, m_actions->actions ())
    {
        if (action->data().toString() == dev_path)
            return action;
    }
    return 0;
}

UDisksDevice *UDisksPlugin::findDevice(QAction *action)
{
    foreach(UDisksDevice *device, m_devices)
    {
        QString dev_path;
        if (device->property("DeviceIsOpticalDisc").toBool() &&
                device->property("OpticalDiscNumAudioTracks").toInt())
        {
            dev_path = "cdda://" + device->property("DeviceFile").toString();
            if (dev_path == action->data().toString())
                return device;
        }
        if (device->property("DeviceIsMounted").toBool())
        {
            dev_path = device->property("DeviceMountPaths").toStringList()[0];
            if (dev_path == action->data().toString())
                return device;
        }
    }
    return 0;
}

void UDisksPlugin::addPath(const QString &path)
{
    PlayListModel *model = PlayListManager::instance()->selectedPlayList();

    foreach(PlayListItem *item, model->items()) // Is it already exist?
    {
        if(item->isGroup())
            continue;
        if (dynamic_cast<PlayListTrack *>(item)->url().startsWith(path))
            return;
    }

    if (path.startsWith("cdda://") && m_addTracks)
    {
        PlayListManager::instance()->selectedPlayList()->add(path);
        return;
    }
    else if (!path.startsWith("cdda://") && m_addFiles)
        PlayListManager::instance()->selectedPlayList()->add(path);
}

void UDisksPlugin::removePath(const QString &path)
{
    if ((path.startsWith("cdda://") && !m_removeTracks) ||
            (!path.startsWith("cdda://") && !m_removeFiles)) //process settings
        return;

    PlayListModel *model = PlayListManager::instance()->selectedPlayList();

    int i = 0;
    while (model->count() > 0 && i < model->count())
    {
        if (model->isTrack(i) && model->track(i)->url().startsWith(path))
            model->removeTrack(i);
        else
            ++i;
    }
}
