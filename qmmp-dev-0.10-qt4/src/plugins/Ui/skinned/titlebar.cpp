/***************************************************************************
 *   Copyright (C) 2007-2016 by Ilya Kotov                                 *
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

#include <QMainWindow>
#include <QApplication>
#include <QMouseEvent>
#include <QMenu>
#include <QSettings>

#include "symboldisplay.h"
#include "skin.h"
#include "button.h"
#include "dock.h"
#include "titlebarcontrol.h"
#include "shadedvisual.h"
#include "display.h"
#include "titlebar.h"
#include "mainwindow.h"
#include "timeindicator.h"

// TODO skin cursor with shade mode
TitleBar::TitleBar(TimeIndicatorModel *model, QWidget *parent)
        : PixmapWidget(parent)
        , m_model(model)
{
    m_align = false;
    m_skin = Skin::instance();
    setPixmap(m_skin->getTitleBar(Skin::TITLEBAR_A));
    m_mw = qobject_cast<MainWindow*>(parent->parent());
    m_shaded = false;
    m_shade2 = 0;
    m_currentTime = 0;
    m_control = 0;
    m_visual = 0;
    //buttons
    m_menu = new Button(this,Skin::BT_MENU_N,Skin::BT_MENU_P, Skin::CUR_MAINMENU);
    connect(m_menu,SIGNAL(clicked()),this,SLOT(showMainMenu()));
    m_menu->move(6,3);
    m_minimize = new Button(this,Skin::BT_MINIMIZE_N,Skin::BT_MINIMIZE_P, Skin::CUR_MIN);
    connect(m_minimize, SIGNAL(clicked()), m_mw, SLOT(showMinimized()));
    m_shade = new Button(this,Skin::BT_SHADE1_N,Skin::BT_SHADE1_P, Skin::CUR_WINBUT);
    connect(m_shade, SIGNAL(clicked()), SLOT(shade()));
    m_close = new Button(this,Skin::BT_CLOSE_N,Skin::BT_CLOSE_P, Skin::CUR_CLOSE);
    connect(m_close, SIGNAL(clicked()), m_mw, SLOT(close()));
    setActive(false);
    connect(m_skin, SIGNAL(skinChanged()), this, SLOT(updateSkin()));
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    if (settings.value("Skinned/disp_shaded", false).toBool())
        shade();
    m_align = true;
    setCursor(m_skin->getCursor(Skin::CUR_TITLEBAR));
    updatePositions();
    connect(m_model, SIGNAL(changed()), this, SLOT(onModelChanged()));
}

TitleBar::~TitleBar()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Skinned/disp_shaded", m_shaded);
}

void TitleBar::updatePositions()
{
    int r = m_skin->ratio();
    m_menu->move(r*6, r*3);
    m_minimize->move(r*244, r*3);
    m_shade->move(r*254, r*3);
    m_close->move(r*264, r*3);
    if(m_shade2)
        m_shade2->move(r*254, r*3);
    if(m_currentTime)
        m_currentTime->move(r*127, r*4);
    if(m_control)
        m_control->move(r*168, r*2);
    if(m_visual)
        m_visual->move(r*79,r*5);
}

void TitleBar::mousePressEvent(QMouseEvent* event)
{
    switch ((int) event->button ())
    {
    case Qt::LeftButton:
        m_pos = event->pos();
        Dock::instance()->calculateDistances();
        Dock::instance()->updateDock();
        break;
    case Qt::RightButton:
        m_mw->menu()->exec(event->globalPos());
    }
}

void TitleBar::mouseReleaseEvent(QMouseEvent*)
{
    Dock::instance()->updateDock();
}
void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pos.x() < width() - m_skin->ratio() * 37)
    {
        QPoint npos = (event->globalPos()-m_pos);
        Dock::instance()->move(m_mw, npos);
    }
}

void TitleBar::setActive(bool a)
{
    if (a)
    {
        if (m_shaded)
            setPixmap(m_skin->getTitleBar(Skin::TITLEBAR_SHADED_A));
        else
            setPixmap(m_skin->getTitleBar(Skin::TITLEBAR_A));
    }
    else
    {
        if (m_shaded)
            setPixmap(m_skin->getTitleBar(Skin::TITLEBAR_SHADED_I));
        else
            setPixmap(m_skin->getTitleBar(Skin::TITLEBAR_I));
    }
}

void TitleBar::updateSkin()
{
    setActive(false);
    setCursor(m_skin->getCursor(Skin::CUR_TITLEBAR));
    updatePositions();
}

void TitleBar::showMainMenu()
{
    m_mw->menu()->exec(m_menu->mapToGlobal(m_menu->pos()));
}

void TitleBar::shade()
{
    m_shaded = !m_shaded;
    int r = m_skin->ratio();
    if (m_shaded)
    {
        setPixmap(m_skin->getTitleBar(Skin::TITLEBAR_SHADED_A));
        m_shade->hide();
        m_shade2 = new Button(this,Skin::BT_SHADE2_N, Skin::BT_SHADE2_P, Skin::CUR_WSNORMAL);
        connect(m_shade2, SIGNAL(clicked()), SLOT(shade()));
        m_shade2->show();
        m_currentTime = new SymbolDisplay(this, 6);
        m_currentTime->show();
        connect (m_currentTime, SIGNAL (mouseClicked()), m_model, SLOT (toggleElapsed()));
        m_control = new TitleBarControl(this);
        m_control->show();
        connect (m_control, SIGNAL (nextClicked()), m_mw, SLOT (next()));
        connect (m_control, SIGNAL (previousClicked()), m_mw, SLOT (previous()));
        connect (m_control, SIGNAL (playClicked()), m_mw, SLOT (play()));
        connect (m_control, SIGNAL (pauseClicked()), m_mw, SLOT (pause()));
        connect (m_control, SIGNAL (stopClicked()), m_mw, SLOT (stop()));
        connect (m_control, SIGNAL (ejectClicked()), m_mw, SLOT (playFiles()));
        m_visual = new ShadedVisual(this);
        Visual::add(m_visual);
        m_visual->show();
    }
    else
    {
        setPixmap(m_skin->getTitleBar(Skin::TITLEBAR_A));
        m_shade2->deleteLater();
        m_currentTime->deleteLater();
        m_control->deleteLater();
        Visual::remove(m_visual);
        m_visual->deleteLater();
        m_shade2 = 0;
        m_currentTime = 0;
        m_control = 0;
        m_visual = 0;
        m_shade->show();
    }
    qobject_cast<MainDisplay *> (parent())->setMinimalMode(m_shaded);
    if (m_align)
        Dock::instance()->align(m_mw, m_shaded? -r*102: r*102);
    onModelChanged();
    updatePositions();
}

void TitleBar::mouseDoubleClickEvent (QMouseEvent *)
{
       TitleBar::shade();
}

QString TitleBar::formatTime (int sec)
{
    bool sign = false;
    if (sec < 0) {
        sign = true;
        sec = -sec;
    }
    int minutes = sec / 60;
    int seconds = sec % 60;

    QString str_minutes = QString::number (minutes);
    QString str_seconds = QString::number (seconds);

    if (minutes < 10) str_minutes.prepend ("0");
    if (seconds < 10) str_seconds.prepend ("0");

    return (sign ? "-" : "") + str_minutes + ":" + str_seconds;
}

void TitleBar::onModelChanged()
{
    if (!m_currentTime)
        return;

    if (!m_model->visible())
    {
        m_currentTime->display("  :  ");
    }
    else if (m_model->position() < 0)
    {
        m_currentTime->display("--:--");
    }
    else
    {
        m_currentTime->display(formatTime(m_model->displayTime()));
    }
}
