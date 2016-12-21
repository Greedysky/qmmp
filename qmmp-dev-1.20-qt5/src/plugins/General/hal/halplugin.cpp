/***************************************************************************
 *   Copyright (C) 2009-2014 by Ilya Kotov                                 *
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
#include "haldevice.h"
#include "halmanager.h"
#include "halplugin.h"

HalPlugin::HalPlugin(QObject *parent) : QObject(parent)
{
    m_manager = new HalManager(this);
    m_actions = new QActionGroup(this);
    connect(m_manager,SIGNAL(deviceAdded(const QString &)), SLOT(addDevice(const QString &)));
    connect(m_manager,SIGNAL(deviceRemoved(const QString &)), SLOT(removeDevice(const QString &)));
    connect(m_actions,SIGNAL(triggered (QAction *)), SLOT(processAction(QAction *)));
    //load settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("HAL");
    m_detectCDA = settings.value("cda", true).toBool();
    m_detectRemovable = settings.value("removable", true).toBool();
    m_addTracks = false; //do not load tracks on startup
    m_addFiles = false;
    //find existing devices
    QStringList udis = m_manager->findDeviceByCapability("volume");
    foreach(QString udi, udis)
    addDevice(udi);
    //load remaining settings
    m_addTracks = settings.value("add_tracks", false).toBool();
    m_removeTracks = settings.value("remove_tracks", false).toBool();
    m_addFiles = settings.value("add_files", false).toBool();
    m_removeFiles = settings.value("remove_files", false).toBool();
    settings.endGroup();
}

HalPlugin::~HalPlugin()
{
}

void HalPlugin::removeDevice(const QString &udi)
{
    foreach(HalDevice *device, m_devices)
    {
        if (device->udi() == udi)
        {
            m_devices.removeAll(device);
            delete device;
            qDebug("HalPlugin: device \"%s\" removed", qPrintable(udi));
            updateActions();
            break;
        }
    }
}

void HalPlugin::addDevice(const QString &udi)
{
    foreach(HalDevice *device, m_devices) //is it already exists?
    {
        if (device->udi() == udi)
            return;
    }
    HalDevice *device = new HalDevice(udi, this);
    QStringList caps = device->property("info.capabilities").toStringList();
    if (!caps.contains("block") || !caps.contains("volume") ||
            device->property("info.category").toString() != "volume" ||
            device->property("info.volume.ignore").toBool()) //filter unsupported devices
    {
        delete device;
        return;
    }
    //audio cd
    if (caps.contains("volume.disc") && device->property("volume.disc.has_audio").toBool())
    {
        if (m_detectCDA)
        {
            qDebug("HalPlugin: device \"%s\" added (cd audio)", qPrintable(udi));
            m_devices << device;
            updateActions();
        }
        else
            delete device;
        return;
    }

    HalDevice parentDevice(device->property("info.parent").toString());

    caps = parentDevice.property("info.capabilities").toStringList();
    // filter removable devices
    if (!(caps.contains("storage") && parentDevice.property("storage.removable").toBool()))
    {
        delete device;
        return;
    }

    if (device->property("volume.size").toLongLong() < 40000000000LL &&
            (device->property("volume.fstype").toString() == "vfat" ||
             device->property("volume.fstype").toString() == "iso" ||
             device->property("volume.fstype").toString() == "udf" ||
             device->property("volume.fstype").toString() == "ext2" ||
             device->property("volume.fstype").toString() == "iso9660"))
    {
        if (m_detectRemovable)
        {
            qDebug("HalPlugin: device \"%s\" added (removable)", qPrintable(udi));
            m_devices << device;
            updateActions();
            connect(device, SIGNAL(propertyModified(int, const QList<ChangeDescription> &)),
                    SLOT(updateActions()));
        }
        else
            delete device;
        return;
    }
    delete device;
}

void HalPlugin::updateActions()
{
    // add action for cd audio or mounted volume
    foreach(HalDevice *device, m_devices)
    {
        QStringList caps = device->property("info.capabilities").toStringList();
        QString dev_path;
        if (caps.contains("volume.disc") && device->property("volume.disc.has_audio").toBool()) //cd audio
            dev_path = "cdda://" + device->property("block.device").toString();
        else if (device->property("volume.is_mounted").toBool()) //mounted volume
            dev_path = device->property("volume.mount_point").toString();
        else
            continue;

        if (!findAction(dev_path))
        {
            QAction *action = new QAction(this);
            QString actionText;
            if (caps.contains("volume.disc") && device->property("volume.disc.has_audio").toBool())
                actionText = QString(tr("Add CD \"%1\"")).arg(device->property("block.device").toString());
            else
            {
                QString name = device->property("volume.label").toString();
                if (name.isEmpty())
                    name = dev_path;
                actionText = QString(tr("Add Volume \"%1\"")).arg(name);
            }
            action->setText(actionText);
            action->setData(dev_path);

            if (caps.contains("volume.disc"))
            {
                if(device->property("volume.fstype").toString() == "iso9660")
                    action->setIcon(qApp->style()->standardIcon(QStyle::SP_DriveDVDIcon));
                else
                    action->setIcon(qApp->style()->standardIcon(QStyle::SP_DriveCDIcon));
            }
            else
                action->setIcon(qApp->style()->standardIcon(QStyle::SP_DriveHDIcon));

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

void HalPlugin::processAction(QAction *action)
{
    qDebug("HalPlugin: action triggered: %s", qPrintable(action->data().toString()));
    QString path = action->data().toString();
    MediaPlayer::instance()->playListManager()->selectedPlayList()->add(path);
}

QAction *HalPlugin::findAction(const QString &dev_path)
{
    foreach(QAction *action, m_actions->actions ())
    {
        if (action->data().toString() == dev_path)
            return action;
    }
    return 0;
}

HalDevice *HalPlugin::findDevice(QAction *action)
{
    foreach(HalDevice *device, m_devices)
    {
        QStringList caps = device->property("info.capabilities").toStringList();
        QString dev_path;
        if (caps.contains("volume.disc") && device->property("volume.disc.has_audio").toBool())
        {
            dev_path = "cdda://" + device->property("block.device").toString();
            if (dev_path == action->data().toString())
                return device;
        }
        if (device->property("volume.is_mounted").toBool())
        {
            dev_path = device->property("volume.mount_point").toString();
            if (dev_path == action->data().toString())
                return device;
        }
    }
    return 0;
}

void HalPlugin::addPath(const QString &path)
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

void HalPlugin::removePath(const QString &path)
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
