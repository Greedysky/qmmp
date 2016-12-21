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
#include <QPainter>
#include <QResizeEvent>
#include <QMenu>
#include <QSettings>
#include <QApplication>
#include <qmmpui/playlistmodel.h>
#include "dock.h"
#include "windowsystem.h"
#include "button.h"
#include "playlisttitlebar.h"
#include "skin.h"

#define TITLE_FORMAT "%p%if(%p&%t, - ,)%t%if(%p,,%if(%t,,%f))%if(%l, %(%l%),)"

// TODO {shademode, updateskin} -> do we have the shaded cursor
PlayListTitleBar::PlayListTitleBar(QWidget *parent)
        : PixmapWidget(parent)
{
    m_active = false;
    m_resize = false;
    m_formatter.setPattern(TITLE_FORMAT);
    m_shade2 = 0;
    m_model = 0;
    m_shaded = false;
    m_align = false;
    m_skin = Skin::instance();
    m_ratio = m_skin->ratio();
    connect(m_skin, SIGNAL(skinChanged()), this, SLOT(updateSkin()));
    m_pl = qobject_cast<PlayList*>(parent);
    m_mw = qobject_cast<MainWindow*>(m_pl->parent());

    m_close = new Button(this,Skin::PL_BT_CLOSE_N, Skin::PL_BT_CLOSE_P, Skin::CUR_PCLOSE);
    connect (m_close, SIGNAL(clicked()), m_pl, SIGNAL(closed()));

    m_shade = new Button(this, Skin::PL_BT_SHADE1_N, Skin::PL_BT_SHADE1_P, Skin::CUR_PWINBUT);
    connect(m_shade, SIGNAL(clicked()), SLOT(shade()));

    resize(275*m_ratio,20*m_ratio);
    setMinimumWidth(275*m_ratio);

    readSettings();
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
#ifdef QMMP_WS_X11
    if(m_pl->useCompiz())
        m_pl->setFixedSize(settings.value ("Skinned/pl_size", QSize (m_ratio*275, m_ratio*116)).toSize());
    else
#endif
        m_pl->resize (settings.value ("Skinned/pl_size", QSize (m_ratio*275, m_ratio*116)).toSize());
    if (settings.value ("Skinned/pl_shaded", false).toBool())
        shade();
    resize(m_pl->width(),height());
    m_align = true;
    setCursor(m_skin->getCursor(Skin::CUR_PTBAR));
    updatePositions();
}


PlayListTitleBar::~PlayListTitleBar()
{
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue ("Skinned/pl_size", QSize (m_pl->width(), m_shaded ? m_height:m_pl->height()));
    settings.setValue ("Skinned/pl_shaded", m_shaded);
}

void PlayListTitleBar::updatePositions()
{
    m_ratio = m_skin->ratio();
    int sx = (width()-275*m_ratio)/25;
    m_close->move(m_ratio*264+sx*25,m_ratio*3);
    m_shade->move(m_ratio*255+sx*25,m_ratio*3);
    if (m_shade2)
        m_shade2->move(m_ratio*255+sx*25,m_ratio*3);
}

void PlayListTitleBar::updatePixmap()
{
    int sx = ((m_shaded ? m_pl->width() : width())-275*m_ratio)/25;
    QPixmap pixmap(275*m_ratio+sx*25,20*m_ratio);
    QPainter paint;
    paint.begin(&pixmap);
    if (m_shaded)
    {
        paint.drawPixmap(0,0,m_skin->getPlPart(Skin::PL_TITLEBAR_SHADED2));
        for (int i = 1; i<sx+9*m_ratio; i++)
        {
            paint.drawPixmap(25*i,0,m_skin->getPlPart(Skin::PL_TFILL_SHADED));
        }
    }

    if (m_active)
    {
        if (m_shaded)
            paint.drawPixmap(225*m_ratio+sx*25,0,m_skin->getPlPart(Skin::PL_TITLEBAR_SHADED1_A));
        else
        {
            paint.drawPixmap(0,0,m_skin->getPlPart(Skin::PL_CORNER_UL_A));
            for (int i = 1; i<sx+10*m_ratio; i++)
            {
                paint.drawPixmap(25*i,0,m_skin->getPlPart(Skin::PL_TFILL1_A));
            }
            paint.drawPixmap((100-12)*m_ratio+12*sx,0,m_skin->getPlPart(Skin::PL_TITLEBAR_A));
            paint.drawPixmap(250*m_ratio+sx*25,0,m_skin->getPlPart(Skin::PL_CORNER_UR_A));
        }
    }
    else
    {
        if (m_shaded)
            paint.drawPixmap(225*m_ratio+sx*25,0,m_skin->getPlPart(Skin::PL_TITLEBAR_SHADED1_I));
        else
        {
            paint.drawPixmap(0,0,m_skin->getPlPart(Skin::PL_CORNER_UL_I));
            for (int i = 1; i<sx+10*m_ratio; i++)
            {
                paint.drawPixmap(25*i,0,m_skin->getPlPart(Skin::PL_TFILL1_I));
            }
            paint.drawPixmap((100-12)*m_ratio+12*sx,0,m_skin->getPlPart(Skin::PL_TITLEBAR_I));
            paint.drawPixmap(250*m_ratio+sx*25,0,m_skin->getPlPart(Skin::PL_CORNER_UR_I));
        }
    }
    if (m_shaded)
    {
        QColor col;
        col.setNamedColor(QString(m_skin->getPLValue("normalbg")));
        paint.setBrush(QBrush(col));
        paint.setPen(col);
        paint.drawRect(8*m_ratio, m_ratio, 235*m_ratio + sx*25, 11*m_ratio);
        //draw text
        paint.setFont(m_font);
        paint.setPen(QString(m_skin->getPLValue("normal")));
        paint.drawText(9*m_ratio, 11*m_ratio, m_truncatedText);
    }
    paint.end();
    setPixmap(pixmap);
}

void PlayListTitleBar::resizeEvent(QResizeEvent *)
{
    QFontMetrics metrics(m_font);
    m_truncatedText = metrics.elidedText (m_text, Qt::ElideRight, width() -  35*m_ratio);
    updatePositions();
    updatePixmap();
}

void PlayListTitleBar::mousePressEvent(QMouseEvent* event)
{
    switch ((int) event->button ())
    {
    case Qt::LeftButton:
        pos = event->pos();
        if (m_shaded && (width() - 30*m_ratio) < pos.x() && pos.x() < (width() - 22*m_ratio))
        {
            m_resize = true;
            setCursor (Qt::SizeHorCursor);
        }
        break;
    case Qt::RightButton:
        m_mw->menu()->exec(event->globalPos());
    }
}

void PlayListTitleBar::mouseReleaseEvent(QMouseEvent*)
{
    Dock::instance()->updateDock();
    m_resize = false;
    setCursor(m_skin->getCursor(Skin::CUR_PTBAR));
}

void PlayListTitleBar::mouseMoveEvent(QMouseEvent* event)
{
    QPoint npos = event->globalPos()-pos;
    if (m_shaded && m_resize)
    {
#ifdef QMMP_WS_X11
        //avoid right corner moving during resize
        if(layoutDirection() == Qt::RightToLeft)
            WindowSystem::revertGravity(m_pl->winId());
#endif

        int dx = 25 * m_ratio;
        int sx = ((event->x() - 275 * m_ratio) + 14) / dx;
        sx = qMax(sx, 0);
        resize(275 * m_ratio + dx * sx, height());

#ifdef QMMP_WS_X11
        if(m_pl->useCompiz())

            m_pl->setFixedSize(275 * m_ratio + dx * sx, m_pl->height());
        else
#endif
            m_pl->resize(275 * m_ratio + dx * sx, m_pl->height());
    }
    else if (pos.x() < width() - 30*m_ratio)
        Dock::instance()->move(m_pl, npos);
}

void PlayListTitleBar::setActive(bool a)
{
    m_active = a;
    updatePixmap();
}

void PlayListTitleBar::setModel(PlayListModel *selected, PlayListModel *previous)
{
    if(previous)
        disconnect(previous, 0, this, 0); //disconnect previous model
    m_model = selected;
    connect (m_model, SIGNAL(listChanged(int)), SLOT(showCurrent()));
    showCurrent();
}

void PlayListTitleBar::readSettings()
{
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    m_font.fromString(settings.value("Skinned/pl_font", QApplication::font().toString()).toString());
    m_font.setPointSize(6);
}

void PlayListTitleBar::updateSkin()
{
    setCursor(m_skin->getCursor(Skin::CUR_PTBAR));
    if(m_ratio != m_skin->ratio())
    {
        m_ratio = m_skin->ratio();
        setMinimumWidth(275*m_ratio);
        updatePositions();
    }
    updatePixmap();
}

void PlayListTitleBar::shade()
{
    m_shaded = !m_shaded;
    if (m_shaded)
    {
        m_height = m_pl->height();
        m_shade->hide();
        m_shade2 = new Button(this, Skin::PL_BT_SHADE2_N, Skin::PL_BT_SHADE2_P, Skin::CUR_PWSNORM);
        m_shade2->move(254,3);
        connect(m_shade2, SIGNAL(clicked()), SLOT(shade()));
        m_shade2->show();
    }
    else
    {
        m_shade2->deleteLater();
        m_shade2 = 0;
        m_shade->show();
    }
    m_pl->setMinimalMode(m_shaded);
    showCurrent();
    update();
    if (m_align)
        Dock::instance()->align(m_pl, m_shaded? -m_height+14*m_ratio: m_height-14*m_ratio);
    updatePositions();
}

void PlayListTitleBar::mouseDoubleClickEvent (QMouseEvent *)
{
    PlayListTitleBar::shade();
}

void PlayListTitleBar::showCurrent()
{
    m_text.clear();

    if (m_model)
    {
        PlayListTrack* track = m_model->currentTrack();
        if (track)
            m_text = QString("%1. %2").arg(track->trackIndex() + 1).arg(m_formatter.format(track));
    }
    QFontMetrics metrics(m_font);
    m_truncatedText = metrics.elidedText (m_text, Qt::ElideRight, width() -  35*m_ratio);
    updatePixmap();
}
