/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   Copyright (C) 2003-2007 by Justin Karneges and Michail Pishchagin     *
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

#include <QtGlobal>
#ifdef QMMP_WS_X11
#include <QSettings>
#include <QX11Info>
#include <QEvent>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>
#define Visual XVisual
extern "C"
{
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#ifdef HAVE_XKBLIB_H
#include <X11/XKBlib.h>
#endif
}
#undef CursorShape
#undef Status
#undef Bool
#undef None
#undef KeyPress
#undef Visual

#include <qmmp/qmmp.h>
#include <qmmp/soundcore.h>
#include <qmmpui/mediaplayer.h>
#include <qmmpui/uihelper.h>
#include "hotkeymanager.h"

quint32 Hotkey::defaultKey()
{
    return defaultKey(action);
}

quint32 Hotkey::defaultKey(int act)
{
    //default key bindings
    QMap<int, quint32> keyMap;
    keyMap[PLAY] = 0;
    keyMap[STOP] = XF86XK_AudioStop;
    keyMap[PAUSE] = XF86XK_AudioPause;
    keyMap[PLAY_PAUSE] = XF86XK_AudioPlay;
    keyMap[NEXT] = XF86XK_AudioNext;
    keyMap[PREVIOUS] = XF86XK_AudioPrev;
    keyMap[SHOW_HIDE] = 0;
    keyMap[VOLUME_UP] = XF86XK_AudioRaiseVolume;
    keyMap[VOLUME_DOWN] = XF86XK_AudioLowerVolume;
    keyMap[FORWARD] = 0;
    keyMap[REWIND] = 0;
    keyMap[JUMP_TO_TRACK] = 0;
    keyMap[VOLUME_MUTE] = XF86XK_AudioMute;
    return keyMap[act];
}

HotkeyManager::HotkeyManager(QObject *parent) : QObject(parent)
{
    QCoreApplication::instance()->installEventFilter(this);
    WId rootWindow = QX11Info::appRootWindow();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat); //load settings
    settings.beginGroup("Hotkey");
    for (int i = Hotkey::PLAY, j = 0; i <= Hotkey::VOLUME_MUTE; ++i, ++j)
    {
        quint32 key = settings.value(QString("key_%1").arg(i), Hotkey::defaultKey(i)).toUInt();
        quint32 mod = settings.value(QString("modifiers_%1").arg(i), 0).toUInt();

        if (key)
        {
            foreach(long mask_mod, ignModifiersList())
            {
                Hotkey *hotkey = new Hotkey;
                hotkey->action = i;
                hotkey->key = key;
                hotkey->code = XKeysymToKeycode(QX11Info::display(), hotkey->key);
                if(!hotkey->code)
                    continue;
                XGrabKey(QX11Info::display(),  hotkey->code, mod | mask_mod, rootWindow, False,
                         GrabModeAsync, GrabModeAsync);
                hotkey->mod = mod | mask_mod;
                m_grabbedKeys << hotkey;
            }
        }
    }
    settings.endGroup();
    XSync(QX11Info::display(), False);
    //         XSetErrorHandler();
}

HotkeyManager::~HotkeyManager()
{
    foreach(Hotkey *key, m_grabbedKeys)
    {
        if(key->code)
            XUngrabKey(QX11Info::display(), key->code, key->mod, QX11Info::appRootWindow());
    }
    while (!m_grabbedKeys.isEmpty())
        delete m_grabbedKeys.takeFirst ();
}

const QString HotkeyManager::getKeyString(quint32 key, quint32 modifiers)
{
    QString strModList[] = { "Control", "Shift", "Alt", "Mod2", "Mod3", "Super", "Mod5" };
    quint32 modList[] = { ControlMask, ShiftMask, Mod1Mask, Mod2Mask, Mod3Mask, Mod4Mask, Mod5Mask };
    QString keyStr;
    for (int j = 0; j < 7; j++)
    {
        if (modifiers & modList[j])
            keyStr.append(strModList[j] + "+");
    }
    keyStr.append(XKeysymToString(key));
    return keyStr;
}

bool HotkeyManager::eventFilter(QObject* o, QEvent* e)
{
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent* k = static_cast<QKeyEvent*>(e);
        quint32 key = keycodeToKeysym(k->nativeScanCode());
        quint32 mod = k->nativeModifiers ();
        SoundCore *core = SoundCore::instance();
        MediaPlayer *player = MediaPlayer::instance();
        foreach(Hotkey *hotkey, m_grabbedKeys)
        {
            if (hotkey->key != key || hotkey->mod != mod)
                continue;
            qDebug("HotkeyManager: [%s] pressed", qPrintable(getKeyString(key, mod)));

            switch (hotkey->action)
            {
            case Hotkey::PLAY:
                player->play();
                break;
            case Hotkey::STOP:
                player->stop();
                break;
            case Hotkey::PAUSE:
                core->pause();
                break;
            case Hotkey::PLAY_PAUSE:
                if (core->state() == Qmmp::Stopped)
                    player->play();
                else if (core->state() != Qmmp::FatalError)
                    core->pause();
                break;
            case Hotkey::NEXT:
                player->next();
                break;
            case Hotkey::PREVIOUS:
                player->previous();
                break;
            case Hotkey::SHOW_HIDE:
                UiHelper::instance()->toggleVisibility();
                break;
            case Hotkey::VOLUME_UP:
                core->volumeUp();
                break;
            case Hotkey::VOLUME_DOWN:
                core->volumeDown();
                break;
            case Hotkey::FORWARD:
                core->seek(core->elapsed() + 5000);
                break;
            case Hotkey::REWIND:
                core->seek(qMax(qint64(0), core->elapsed() - 5000));
                break;
            case Hotkey::JUMP_TO_TRACK:
                UiHelper::instance()->jumpToTrack();
                break;
            case Hotkey::VOLUME_MUTE:
                SoundCore::instance()->setMuted(!SoundCore::instance()->isMuted());
                break;

            }
            return true;
        }
    }
    return QObject::eventFilter(o, e);
}
long HotkeyManager::m_alt_mask = 0;
long HotkeyManager::m_meta_mask = 0;
long HotkeyManager::m_super_mask = 0;
long HotkeyManager::m_hyper_mask = 0;
long HotkeyManager::m_numlock_mask = 0;
bool HotkeyManager::m_haveMods = false;

//copied from globalshortcutmanager_x11.cpp by Justin Karneges and Michail Pishchagin (Psi project)
void HotkeyManager::ensureModifiers()
{
    if (m_haveMods)
        return;

    Display* appDpy = QX11Info::display();
    XModifierKeymap* map = XGetModifierMapping(appDpy);
    if (map)
    {
        // XkbKeycodeToKeysym helper code adapeted from xmodmap
        int min_keycode, max_keycode, keysyms_per_keycode = 1;
        XDisplayKeycodes (appDpy, &min_keycode, &max_keycode);
        XFree(XGetKeyboardMapping (appDpy, min_keycode, (max_keycode - min_keycode + 1), &keysyms_per_keycode));

        int i, maskIndex = 0, mapIndex = 0;
        for (maskIndex = 0; maskIndex < 8; maskIndex++)
        {
            for (i = 0; i < map->max_keypermod; i++)
            {
                if (map->modifiermap[mapIndex])
                {
                    KeySym sym;
                    int symIndex = 0;
                    do
                    {
#ifdef HAVE_XKBLIB_H
                        sym = XkbKeycodeToKeysym(appDpy, map->modifiermap[mapIndex], symIndex, 0);
#else
                        sym = XKeycodeToKeysym(appDpy, map->modifiermap[mapIndex], symIndex);
#endif
                        symIndex++;
                    }
                    while ( !sym && symIndex < keysyms_per_keycode);
                    if (!m_alt_mask && (sym == XK_Alt_L || sym == XK_Alt_R))
                    {
                        m_alt_mask = 1 << maskIndex;
                    }
                    if (!m_meta_mask && (sym == XK_Meta_L || sym == XK_Meta_R))
                    {
                        m_meta_mask = 1 << maskIndex;
                    }
                    if (!m_super_mask && (sym == XK_Super_L || sym == XK_Super_R))
                    {
                        m_super_mask = 1 << maskIndex;
                    }
                    if (!m_hyper_mask && (sym == XK_Hyper_L || sym == XK_Hyper_R))
                    {
                        m_hyper_mask = 1 << maskIndex;
                    }
                    if (!m_numlock_mask && (sym == XK_Num_Lock))
                    {
                        m_numlock_mask = 1 << maskIndex;
                    }
                }
                mapIndex++;
            }
        }

        XFreeModifiermap(map);

        // logic from qt source see gui/kernel/qkeymapper_x11.cpp
        if (!m_meta_mask || m_meta_mask == m_alt_mask)
        {
            // no meta keys... s,meta,super,
            m_meta_mask = m_super_mask;
            if (!m_meta_mask || m_meta_mask == m_alt_mask)
            {
                // no super keys either? guess we'll use hyper then
                m_meta_mask = m_hyper_mask;
            }
        }
    }
    else
    {
        // assume defaults
        m_alt_mask = Mod1Mask;
        m_meta_mask = Mod4Mask;
    }

    m_haveMods = true;
}

QList<long> HotkeyManager::ignModifiersList()
{
    ensureModifiers();
    QList<long> ret;
    if (m_numlock_mask)
    {
        ret << 0 << LockMask << m_numlock_mask << (LockMask | m_numlock_mask);
    }
    else
    {
        ret << 0 << LockMask;
    }
    return ret;
}

quint32 HotkeyManager::keycodeToKeysym(quint32 keycode)
{
#ifdef HAVE_XKBLIB_H
    return XkbKeycodeToKeysym(QX11Info::display(), keycode, 0, 0);
#else
    return XKeycodeToKeysym(QX11Info::display(), keycode,0);
#endif
}

#include "moc_hotkeymanager.cpp"

#endif
