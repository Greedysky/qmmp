/***************************************************************************
 *   Copyright (C) 2015 by Ivan Ponomarev                                  *
 *   ivantrue@gmail.com                                                    *
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
#include <QSettings>
#include <QAudioDeviceInfo>
#include <QDebug>
#include <qmmp/qmmp.h>

#include "settingsdialog.h"

SettingsDialog::SettingsDialog (QWidget *parent) : QDialog (parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

	const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
	const QString default_device = settings.value("QTMULTIMEDIA/device").toString();

	//Default item always has index = 0
    ui.deviceComboBox->addItem(tr("Default"));
    ui.deviceComboBox->setCurrentIndex(0);

    const QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
	int index = 1;
    foreach (const QAudioDeviceInfo &info, devices)
	{
    	const QString device_name = info.deviceName();
		ui.deviceComboBox->addItem(device_name);
		if (device_name==default_device)
			ui.deviceComboBox->setCurrentIndex(index);
		++index;
	}
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::accept()
{
    qDebug("%s", Q_FUNC_INFO);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    //0 index means default value, we save empty string for it.
    settings.setValue("QTMULTIMEDIA/device", ui.deviceComboBox->currentIndex() ? ui.deviceComboBox->currentText() : QString());
    QDialog::accept();
}
