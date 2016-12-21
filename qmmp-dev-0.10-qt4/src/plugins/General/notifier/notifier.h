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
#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <QPointer>
#include <QStringList>
#include <qmmpui/general.h>
#include <qmmp/qmmp.h>

class PopupWidget;
class SoundCore;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/

class Notifier : public QObject
{
Q_OBJECT
public:
    Notifier(QObject *parent = 0);

    virtual~Notifier();

private slots:
    void showMetaData();
    void showVolume(int, int);
    void setState(Qmmp::State state);

private:
    QPointer<PopupWidget> m_popupWidget;
    void removePsiTuneFiles();
    bool m_desktop, m_showVolume;
    bool m_psi;
    int m_l, m_r;
    bool m_isPaused;
    bool m_resumeNotification;
    SoundCore *m_core;
    QStringList m_psiTuneFiles;

};

#endif
