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

#include <QSettings>
#include <QMessageBox>
#include <qmmp/qmmp.h>
#include "scrobbler.h"
#include "defines.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
{
    m_ui.setupUi(this);
    m_lastfmAuth = new ScrobblerAuth(SCROBBLER_LASTFM_URL, LASTFM_AUTH_URL, "lastfm", this);
    m_librefmAuth = new ScrobblerAuth(SCROBBLER_LIBREFM_URL, LIBREFM_AUTH_URL, "librefm", this);
    connect(m_lastfmAuth, SIGNAL(tokenRequestFinished(int)), SLOT(processTokenResponse(int)));
    connect(m_lastfmAuth, SIGNAL(sessionRequestFinished(int)), SLOT(processSessionResponse(int)));
    connect(m_lastfmAuth, SIGNAL(checkSessionFinished(int)), SLOT(processCheckResponse(int)));
    connect(m_librefmAuth, SIGNAL(tokenRequestFinished(int)), SLOT(processTokenResponse(int)));
    connect(m_librefmAuth, SIGNAL(sessionRequestFinished(int)), SLOT(processSessionResponse(int)));
    connect(m_librefmAuth, SIGNAL(checkSessionFinished(int)), SLOT(processCheckResponse(int)));

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Scrobbler");
    m_ui.lastfmGroupBox->setChecked(settings.value("use_lastfm", false).toBool());
    m_ui.librefmGroupBox->setChecked(settings.value("use_librefm", false).toBool());
    m_ui.sessionLineEdit_lastfm->setText(settings.value("lastfm_session").toString());
    m_ui.sessionLineEdit_librefm->setText(settings.value("librefm_session").toString());
    settings.endGroup();
}

SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Scrobbler");
    settings.setValue("use_lastfm", m_ui.lastfmGroupBox->isChecked());
    settings.setValue("use_librefm", m_ui.librefmGroupBox->isChecked());
    settings.setValue("lastfm_session",m_ui.sessionLineEdit_lastfm->text());
    settings.setValue("librefm_session",m_ui.sessionLineEdit_librefm->text());
    settings.endGroup();
    QDialog::accept();
}

void SettingsDialog::on_newSessionButton_lastfm_clicked()
{
    m_ui.newSessionButton_lastfm->setEnabled(false);
    m_lastfmAuth->getToken();
}

void SettingsDialog::on_newSessionButton_librefm_clicked()
{
    m_ui.newSessionButton_librefm->setEnabled(false);
    m_librefmAuth->getToken();
}

void SettingsDialog::processTokenResponse(int error)
{
    if(sender() == m_lastfmAuth)
        m_ui.newSessionButton_lastfm->setEnabled(true);
    else if(sender() == m_librefmAuth)
        m_ui.newSessionButton_librefm->setEnabled(true);
    switch(error)
    {
    case ScrobblerAuth::NO_ERROR:
        QMessageBox::information(this,
                                 tr("Message"),
                                 tr("1. Wait for browser startup") + "\n" +
                                 tr("2. Allow Qmmp to scrobble tracks to your %1 account")
                                 .arg((sender() == m_lastfmAuth) ? "Last.fm" : "Libre.fm") + "\n" +
                                 tr("3. Press \"OK\""));
        if(sender() == m_lastfmAuth)
        {
            m_ui.newSessionButton_lastfm->setEnabled(false);
            m_lastfmAuth->getSession();
        }
        else if(sender() == m_librefmAuth)
        {
            m_ui.newSessionButton_librefm->setEnabled(false);
            m_librefmAuth->getSession();
        }
        break;
    case ScrobblerAuth::NETWORK_ERROR:
        QMessageBox::warning(this, tr("Error"), tr("Network error"));
        break;
    case ScrobblerAuth::LASTFM_ERROR:
    default:
        QMessageBox::warning(this, tr("Error"), tr("Unable to register new session"));
    }
}

void SettingsDialog::processSessionResponse(int error)
{
    if(sender() == m_lastfmAuth)
        m_ui.newSessionButton_lastfm->setEnabled(true);
    else if(sender() == m_librefmAuth)
        m_ui.newSessionButton_librefm->setEnabled(true);
    switch(error)
    {
    case ScrobblerAuth::NO_ERROR:
    {
        QMessageBox::information(this, tr("Message"), tr("New session has been received successfully"));
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        if(sender() == m_lastfmAuth)
        {
            m_ui.sessionLineEdit_lastfm->setText(m_lastfmAuth->session());
            settings.setValue("Scrobbler/lastfm_session",m_ui.sessionLineEdit_lastfm->text());
        }
        else if(sender() == m_librefmAuth)
        {
            m_ui.sessionLineEdit_librefm->setText(m_librefmAuth->session());
            settings.setValue("Scrobbler/librefm_session",m_ui.sessionLineEdit_librefm->text());
        }
        break;
    }
    case ScrobblerAuth::NETWORK_ERROR:
        QMessageBox::warning(this, tr("Error"), tr("Network error"));
        break;
    case ScrobblerAuth::LASTFM_ERROR:
    default:
        QMessageBox::warning(this, tr("Error"), tr("Unable to register new session"));
    }
}

void SettingsDialog::on_checkButton_lastfm_clicked()
{
    if(!m_ui.sessionLineEdit_lastfm->text().isEmpty())
    {
        m_ui.checkButton_lastfm->setEnabled(false);
        m_lastfmAuth->checkSession(m_ui.sessionLineEdit_lastfm->text());
    }
}

void SettingsDialog::on_checkButton_librefm_clicked()
{
    if(!m_ui.sessionLineEdit_librefm->text().isEmpty())
    {
        m_ui.checkButton_librefm->setEnabled(false);
        m_librefmAuth->checkSession(m_ui.sessionLineEdit_librefm->text());
    }
}

void SettingsDialog::processCheckResponse(int error)
{
    if(sender() == m_lastfmAuth)
        m_ui.checkButton_lastfm->setEnabled(true);
    else if(sender() == m_librefmAuth)
        m_ui.checkButton_librefm->setEnabled(true);
    switch(error)
    {
    case ScrobblerAuth::NO_ERROR:
    {
        QMessageBox::information(this, tr("Message"), tr("Permission granted"));
        if(sender() == m_lastfmAuth)
            m_ui.sessionLineEdit_lastfm->setText(m_lastfmAuth->session());
        else if(sender() == m_librefmAuth)
            m_ui.sessionLineEdit_librefm->setText(m_librefmAuth->session());
        break;
    }
    case ScrobblerAuth::NETWORK_ERROR:
        QMessageBox::warning(this, tr("Error"), tr("Network error"));
        break;
    case ScrobblerAuth::LASTFM_ERROR:
    default:
        QMessageBox::warning(this, tr("Error"), tr("Permission denied"));
    }
}
