/***************************************************************************
 *   Copyright (C) 2009-2013 by Ilya Kotov                                 *
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
#ifndef HOTKEYMANAGER_H
#define HOTKEYMANAGER_H

#include <QObject>
#include <QString>
#include <QTableWidgetItem>
#include <QList>
#include <qmmpui/general.h>

class QEvent;
class QWidget;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/

class Hotkey
{
public:
    enum Action
    {
        PLAY = QTableWidgetItem::UserType,
        STOP,
        PAUSE,
        PLAY_PAUSE,
        NEXT,
        PREVIOUS,
        SHOW_HIDE,
        VOLUME_UP,
        VOLUME_DOWN,
        FORWARD,
        REWIND,
        JUMP_TO_TRACK,
        VOLUME_MUTE
    };

    Hotkey()
    {
        action = PLAY;
        key = 0;
        mod = 0;
        code = 0;
    }

    quint32 mod;
    quint32 key;
    int action;
    int code;
    quint32 defaultKey();
    static quint32 defaultKey(int act);
};

class HotkeyManager : public QObject
{
    Q_OBJECT
public:
    HotkeyManager(QObject *parent = 0);

    ~HotkeyManager();

    static const QString getKeyString(quint32 key, quint32 modifiers);
    static QList<long> ignModifiersList();
    static quint32 keycodeToKeysym(quint32 keycode);

#ifdef QMMP_WS_X11
protected:
    virtual bool eventFilter(QObject* o, QEvent* e);
#endif

private:
#ifdef QMMP_WS_X11
    static void ensureModifiers();
    QList <Hotkey *> m_grabbedKeys;
    static long m_alt_mask;
    static long m_meta_mask;
    static long m_super_mask;
    static long m_hyper_mask;
    static long m_numlock_mask;
    static bool m_haveMods;
#endif

#ifdef Q_OS_WIN
    QList<QWidget *> m_filters;
#endif
};

#endif
