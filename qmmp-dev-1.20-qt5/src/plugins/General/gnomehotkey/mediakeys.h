/***************************************************************************
 *   Copyright (C) 2014 by Ilya Kotov                                      *
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

#ifndef MEDIAKEYS_H
#define MEDIAKEYS_H

#include <QObject>
#include <QDBusPendingReply>

class QDBusInterface;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class MediaKeys : public QObject
{
    Q_OBJECT
public:
    explicit MediaKeys(QObject *parent = 0);
    virtual ~MediaKeys();

private slots:
    QDBusPendingReply<> grabMediaPlayerKeys(const QString &application, uint time);
    QDBusPendingReply<> releaseMediaPlayerKeys(const QString &application);
    void onRegisterFinished(QDBusPendingCallWatcher *watcher);
    void onKeyPressed(const QString &in0, const QString &in1);

private:
    QDBusInterface *m_interface;
    bool m_isRegistered;


};

#endif // MEDIAKEYS_H
