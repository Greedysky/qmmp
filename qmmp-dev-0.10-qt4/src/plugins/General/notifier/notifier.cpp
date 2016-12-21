/***************************************************************************
 *   Copyright (C) 2008-2013 by Ilya Kotov                                 *
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

#include <QTimer>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <qmmp/soundcore.h>

#include "popupwidget.h"
#include "notifier.h"

Notifier::Notifier(QObject *parent) : QObject(parent)
{
    m_popupWidget = 0;
    m_l = -1;
    m_r = -1;
    m_isPaused = false;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Notifier");
    m_desktop = settings.value("song_notification", true).toBool();
    m_resumeNotification = settings.value("resume_notification", false).toBool();
    m_showVolume = settings.value("volume_notification", true).toBool();
    m_psi = settings.value("psi_notification", false).toBool();
    settings.endGroup();
    m_core = SoundCore::instance();
    connect (m_core, SIGNAL(metaDataChanged ()), SLOT(showMetaData()));
    connect (m_core, SIGNAL(stateChanged (Qmmp::State)), SLOT(setState(Qmmp::State)));
    connect (m_core, SIGNAL(volumeChanged(int, int)), SLOT(showVolume(int, int)));

    //psi tune files (thousands of them!)
    QString psi_data_dir = qgetenv("PSIDATADIR");
    QString xdg_cache_home = qgetenv("XDG_CACHE_HOME");
    if(!psi_data_dir.isEmpty())
        m_psiTuneFiles << psi_data_dir+"/tune";
    else if(!xdg_cache_home.isEmpty())
    {
        m_psiTuneFiles << xdg_cache_home+"/psi/tune";
        m_psiTuneFiles << xdg_cache_home+"/psi+/tune";
    }
    else
    {
        m_psiTuneFiles << QDir::homePath()+"/.cache/psi/tune";
        m_psiTuneFiles << QDir::homePath()+"/.cache/psi+/tune";
    }
    //legacy psi support
    m_psiTuneFiles << QDir::homePath()+"/.psi/tune";
    m_psiTuneFiles << QDir::homePath()+"/.psi-plus/tune";
    m_psiTuneFiles << QDir::homePath()+"/.cache/Psi+/tune";

    if (m_core->state() == Qmmp::Playing) //test message
        showMetaData();
}

Notifier::~Notifier()
{
    removePsiTuneFiles();
    if (m_popupWidget)
        delete m_popupWidget;
}

void Notifier::setState(Qmmp::State state)
{
    switch ((uint) state)
    {
    case Qmmp::Playing:
    {
        if (m_isPaused)
        {
            showMetaData();
            m_isPaused = false;
            break;
        }
    }
    case Qmmp::Paused:
    {
        if(m_resumeNotification)
            m_isPaused = true;
        break;
    }
    case Qmmp::Stopped:
    {
        m_isPaused = false;
        removePsiTuneFiles();
        break;
    }
    default:
        m_isPaused = false;
    }
}

void Notifier::showMetaData()
{
    if (m_desktop)
    {
        if (!m_popupWidget)
            m_popupWidget = new PopupWidget();
        m_popupWidget->showMetaData();
    }

    if (!m_psi)
        return;

    QByteArray data;
    data.append(m_core->metaData(Qmmp::TITLE).toUtf8()+"\n");
    data.append(m_core->metaData(Qmmp::ARTIST).toUtf8()+"\n");
    data.append(m_core->metaData(Qmmp::ALBUM).toUtf8()+"\n");
    data.append(m_core->metaData(Qmmp::TRACK).toUtf8()+"\n");
    data.append(QString("%1").arg(m_core->totalTime()/1000).toUtf8()+"\n");

    foreach(QString path, m_psiTuneFiles)
    {
        QDir tuneDir = QFileInfo(path).absoluteDir();
        if(!tuneDir.exists())
            continue;

        QFile file(path);
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
        file.write(data);
        file.close();
        continue;
    }
}

void Notifier::showVolume(int l, int r)
{
    if (((m_l != l) || (m_r != r)) && m_showVolume)
    {
        if (m_l >= 0)
        {
            if (!m_popupWidget)
                m_popupWidget = new PopupWidget();
            m_popupWidget->showVolume(qMax(l,r));
        }
        m_l = l;
        m_r = r;
    }
}

void Notifier::removePsiTuneFiles()
{
    if(m_psi) //clear psi notification
    {
        foreach(QString path, m_psiTuneFiles)
            QFile::remove(path);
    }
}
