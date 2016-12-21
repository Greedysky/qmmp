/***************************************************************************
 *   Based on Licq                                                         *
 *   Copyright (C) 2006-2009 Licq developers                               *
 *   Copyright (C) 2011-2016 Ilya Kotov                                    *
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

#include "windowsystem.h"
#include <QCoreApplication>
#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

#ifdef Q_WS_X11
void WindowSystem::ghostWindow(WId win)
{
  Display* dsp = QX11Info::display();
  Window root = DefaultRootWindow(dsp);

  Atom win_state = XInternAtom(dsp, "_NET_WM_STATE", False);
  Atom win_state_add = XInternAtom(dsp, "_NET_WM_STATE_ADD", False);
  Atom win_state_settings[] =
  {
    XInternAtom(dsp, "_NET_WM_STATE_SKIP_TASKBAR", False),
    XInternAtom(dsp, "_NET_WM_STATE_SKIP_PAGER", False)
  };
  XChangeProperty(dsp, win, win_state, XA_ATOM, 32, PropModeReplace,
      reinterpret_cast<unsigned char*>(&win_state_settings), 2);

  XEvent xev;
  xev.type = ClientMessage;
  xev.xclient.type = ClientMessage;
  xev.xclient.display = dsp;
  xev.xclient.window = win;
  xev.xclient.message_type = win_state;
  xev.xclient.format = 32;
  xev.xclient.data.l[0] = win_state_add;
  xev.xclient.data.l[1] = win_state_settings[0];
  xev.xclient.data.l[2] = win_state_settings[1];

  XSendEvent(dsp, root, false,
      SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}

QString WindowSystem::netWindowManagerName()
{
    Display* dsp = QX11Info::display();
    WId root = DefaultRootWindow(dsp);

    unsigned char* retValue1 = getWindowProperty(root, "_NET_SUPPORTING_WM_CHECK");
    if (retValue1 == NULL)
        return QString();

    WId win = *(reinterpret_cast<unsigned long*>(retValue1));

    unsigned char* retValue2 = getWindowProperty(win, "_NET_SUPPORTING_WM_CHECK");
    if (retValue2 == NULL)
    {
        XFree(retValue1);
        return QString();
    }

    if (win != *(reinterpret_cast<unsigned long*>(retValue2)))
    {
        XFree(retValue1);
        XFree(retValue2);
        return QString();
    }

    XFree(retValue2);
    retValue2 = getWindowProperty(win, "_NET_WM_NAME");

    XFree(retValue1);
    if (retValue2 == NULL)
        return QString();

    QString name = QString((char *)retValue2);
    XFree(retValue2);
    return name;
}

void WindowSystem::changeWinSticky(WId win, bool stick)
{
    qDebug("WindowSystem: setting sticky state of window 0x%lx to %s.",
           static_cast<unsigned long>(win), stick ? "true" : "false");
    Display* dsp = QX11Info::display();
    Window root  = DefaultRootWindow(dsp);

    unsigned long desktop = ~(0UL);

    if (!stick)
    {
        unsigned char* tmp = getWindowProperty(root, "_NET_CURRENT_DESKTOP");

        if (tmp == NULL)
            qWarning("WindowSystem: error reading current desktop property.");
        else
        {
            desktop = *(reinterpret_cast<unsigned long*>(tmp));
            XFree(tmp);
        }
    }

    XEvent xev;
    xev.type = ClientMessage;
    xev.xclient.type = ClientMessage;
    xev.xclient.display = dsp;
    xev.xclient.window = win;
    xev.xclient.message_type = XInternAtom(dsp, "_NET_WM_DESKTOP", False);
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = desktop;

    XSendEvent(dsp, root, False,
               SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}

void WindowSystem::setWinHint(WId win, const char *res_name, const char *res_class)
{
    Display* dsp = QX11Info::display();
    XClassHint hint;
    hint.res_name = strdup(res_name);
    hint.res_class = strdup(res_class);
    XSetClassHint(dsp, win, &hint);
    free(hint.res_name);
    free(hint.res_class);
}

unsigned char* WindowSystem::getWindowProperty(WId win, const char* prop)
{
  Display* dsp = QX11Info::display();

  // We inhibit new Atom creation since if you request for it
  // then such Atom most probably exists already.
  // Otherwise, no surprise we return NULL here.
  Atom reqAtom = XInternAtom(dsp, prop, True);

  if (reqAtom == None)
    return NULL;

  Atom retType = None;
  int retFormat = 0;
  unsigned long retItems = 0UL;
  unsigned long retMoreBytes = 0UL;
  unsigned char* retValue = NULL;

  // Check if the property exists and calculate its length.
  int retCheck = XGetWindowProperty(dsp, win,
      reqAtom, 0L, 0L, False, AnyPropertyType,
      &retType, &retFormat, &retItems, &retMoreBytes, &retValue);

  // The value is most probably empty, since we requested to read
  // only 0L length, thus, it's just useless...
  if (retValue != NULL)
  {
    XFree(retValue);
    retValue = NULL;
  }

  if (retCheck != Success ||
      retType == None ||
      retMoreBytes == 0)
    return NULL;

  // These are not needed for now.
  retCheck = None;
  retFormat = 0;
  retItems = 0UL;

  // Convert the byte length into 32bit multiples.
  if (retMoreBytes % 4 != 0)
    retMoreBytes += 4 - retMoreBytes % 4;
  retMoreBytes /= 4;

  // Now request the actual property value with correct length and type.
  retCheck = XGetWindowProperty(dsp, win,
      reqAtom, 0L, retMoreBytes, False, retType,
      &retType, &retFormat, &retItems, &retMoreBytes, &retValue);

  if (retCheck != Success ||
      retMoreBytes != 0)
  {
    if (retValue != NULL)
      XFree(retValue);
    return NULL;
  }

  return retValue;
}
//On RTL locales Qt sets flag NorthEastGravity for windows.
//This function reverts these changes.
void WindowSystem::revertGravity(WId win)
{
    Display* dsp = QX11Info::display();
    XSizeHints sh;
    memset(&sh, 0, sizeof(sh));
    long unused;
    XGetWMNormalHints(dsp, win, &sh, &unused);
    sh.flags |= PWinGravity;

    if(sh.win_gravity == NorthEastGravity)
        sh.win_gravity = NorthWestGravity;
    else
        return;
    sh.win_gravity = NorthWestGravity;
    XSetWMNormalHints(dsp, win, &sh);

    XSetWindowAttributes xs;
    memset(&xs, 0, sizeof(xs));
    xs.bit_gravity = NorthWestGravity;
    XChangeWindowAttributes(dsp, win, CWBitGravity, &xs);
}

#endif
