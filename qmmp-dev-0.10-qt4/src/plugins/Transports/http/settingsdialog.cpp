/***************************************************************************
 *   Copyright (C) 2010-2015 by Ilya Kotov                                 *
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
#include <QTextCodec>
#include <QSettings>
#include <qmmp/qmmp.h>
#ifdef WITH_ENCA
#include <enca.h>
#endif
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    findCodecs();
    foreach (QTextCodec *codec, codecs)
        m_ui.icyEncodingComboBox->addItem(codec->name());
#ifdef WITH_ENCA
    size_t n = 0;
    const char **langs = enca_get_languages(&n);
    for (size_t i = 0; i < n; ++i)
        m_ui.encaAnalyserComboBox->addItem(langs[i]);
#endif
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("HTTP");
    int pos = m_ui.icyEncodingComboBox->findText(settings.value("icy_encoding","UTF-8").toString());
    m_ui.icyEncodingComboBox->setCurrentIndex(pos);
    m_ui.bufferSizeSpinBox->setValue(settings.value("buffer_size",384).toInt());
    m_ui.userAgentCheckBox->setChecked(settings.value("override_user_agent",false).toBool());
    m_ui.userAgentLineEdit->setText(settings.value("user_agent").toString());
#ifdef WITH_ENCA
    m_ui.autoCharsetCheckBox->setChecked(settings.value("use_enca", false).toBool());
    pos = m_ui.encaAnalyserComboBox->findText(settings.value("enca_lang", langs[n-1]).toString());
    m_ui.encaAnalyserComboBox->setCurrentIndex(pos);
#else
    m_ui.autoCharsetCheckBox->setEnabled(false);
#endif
    settings.endGroup();
}


SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("HTTP");
    settings.setValue("icy_encoding", m_ui.icyEncodingComboBox->currentText());
    settings.setValue("buffer_size", m_ui.bufferSizeSpinBox->value());
    settings.setValue("override_user_agent",m_ui.userAgentCheckBox->isChecked());
    settings.setValue("user_agent",m_ui.userAgentLineEdit->text());
#ifdef WITH_ENCA
    settings.setValue("use_enca", m_ui.autoCharsetCheckBox->isChecked());
    settings.setValue("enca_lang", m_ui.encaAnalyserComboBox->currentText());
#endif
    settings.endGroup();
    QDialog::accept();
}

void SettingsDialog::findCodecs()
{
    QMap<QString, QTextCodec *> codecMap;
    QRegExp iso8859RegExp("ISO[- ]8859-([0-9]+).*");

    foreach (int mib, QTextCodec::availableMibs())
    {
        QTextCodec *codec = QTextCodec::codecForMib(mib);

        QString sortKey = codec->name().toUpper();
        int rank;

        if (sortKey.startsWith("UTF-8"))
        {
            rank = 1;
        }
        else if (sortKey.startsWith("UTF-16"))
        {
            rank = 2;
        }
        else if (iso8859RegExp.exactMatch(sortKey))
        {
            if (iso8859RegExp.cap(1).size() == 1)
                rank = 3;
            else
                rank = 4;
        }
        else
        {
            rank = 5;
        }
        sortKey.prepend(QChar('0' + rank));

        codecMap.insert(sortKey, codec);
    }
    codecs = codecMap.values();
}
