/***************************************************************************
 *   Copyright (C) 2013-2014 by Ilya Kotov                                 *
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
#include "udisks2device.h"
#include "udisks2manager.h"
#include "udisks2plugin.h"

UDisks2Plugin::UDisks2Plugin(QObject *parent) : QObject(parent)
{
    qDBusRegisterMetaType<QVariantMapMap>();
    qDBusRegisterMetaType<QByteArrayList>();

    m_manager = new UDisks2Manager(this);
    m_actions = new QActionGroup(this);
    connect(m_manager,SIGNAL(deviceAdded(QDBusObjectPath)), SLOT(addDevice(QDBusObjectPath)));
    connect(m_manager,SIGNAL(deviceRemoved(QDBusObjectPath)), SLOT(removeDevice(QDBusObjectPath)));
    connect(m_actions,SIGNAL(triggered (QAction *)), SLOT(processAction(QAction *)));
    //load settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("UDisks2");
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

UDisks2Plugin::~UDisks2Plugin()
{
}

void UDisks2Plugin::removeDevice(QDBusObjectPath o)
{
    foreach(UDisks2Device *device, m_devices)
    {
        if (device->objectPath() == o)
        {
            m_devices.removeAll(device);
            delete device;
            qDebug("UDisks2Plugin: removed device: \"%s\"", qPrintable(o.path()));
            updateActions();
            break;
        }
    }
}

void UDisks2Plugin::addDevice(QDBusObjectPath o)
{
    foreach(UDisks2Device *device, m_devices) //is it already exists?
    {
        if (device->objectPath() == o)
            return;
    }
    UDisks2Device *device = new UDisks2Device(o, this);

    if(device->isRemovable()) //detect removable devices only
    {
        qDebug("UDisks2Plugin: added device: \"%s\"", qPrintable(o.path()));
        m_devices << device;
        updateActions();
        connect(device, SIGNAL(changed()), SLOT(updateActions()));
    }
    else
        delete device;
}

void UDisks2Plugin::updateActions()
{
    // add action for cd audio or mounted volume
    foreach(UDisks2Device *device, m_devices)
    {
        QString dev_path;
        if (m_detectCDA && device->isAudio()) //cd audio
        {
            dev_path = "cdda://" + device->deviceFile();
        }
        else if (m_detectRemovable && device->isMounted() &&
                 device->property("Size").toLongLong() < 40000000000LL &&
                 (device->property("IdType").toString() == "vfat" ||
                  device->property("IdType").toString() == "iso9660" ||
                  device->property("IdType").toString() == "udf" ||
                  device->property("IdType").toString() == "ext2")) //mounted volume
        {
            dev_path = device->mountPoints().first();
        }
        else
            continue;

        if (!findAction(dev_path))
        {
            QAction *action = new QAction(this);
            QString actionText;
            if (device->isAudio())
            {
                actionText = QString(tr("Add CD \"%1\"")).arg(device->deviceFile());
            }
            else
            {
                QString name = device->property("IdLabel").toString();
                if (name.isEmpty())
                    name = dev_path;

                actionText = QString(tr("Add Volume \"%1\"")).arg(name);
            }

            if (device->isOptical())
            {
                if(device->property("IdType").toString() == "iso9660")
                    action->setIcon(qApp->style()->standardIcon(QStyle::SP_DriveDVDIcon));
                else
                    action->setIcon(qApp->style()->standardIcon(QStyle::SP_DriveCDIcon));
            }
            else
                action->setIcon(qApp->style()->standardIcon(QStyle::SP_DriveHDIcon));

            qDebug("UDisks2Plugin: added menu item: \"%s\"", qPrintable(dev_path));

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
            qDebug("UDisks2Plugin: removed menu item: \"%s\"", qPrintable(action->data().toString()));
            m_actions->removeAction(action);
            UiHelper::instance()->removeAction(action);
            removePath(action->data().toString());
            action->deleteLater();
        }
    }
}

void UDisks2Plugin::processAction(QAction *action)
{
    qDebug("UDisks2Plugin: action triggered: %s", qPrintable(action->data().toString()));
    QString path = action->data().toString();
    MediaPlayer::instance()->playListManager()->selectedPlayList()->add(path);
}

QAction *UDisks2Plugin::findAction(const QString &dev_path)
{
    foreach(QAction *action, m_actions->actions ())
    {
        if (action->data().toString() == dev_path)
            return action;
    }
    return 0;
}

UDisks2Device *UDisks2Plugin::findDevice(QAction *action)
{
    foreach(UDisks2Device *device, m_devices)
    {
        QString dev_path;
        if (device->isAudio())
        {
            dev_path = "cdda://" + device->deviceFile();
            if (dev_path == action->data().toString())
                return device;
        }
        if (device->isMounted())
        {
            dev_path = device->mountPoints().first();
            if (dev_path == action->data().toString())
                return device;
        }
    }
    return 0;
}

void UDisks2Plugin::addPath(const QString &path)
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

void UDisks2Plugin::removePath(const QString &path)
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
