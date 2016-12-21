/***************************************************************************
 *   Copyright (C) 2008-2015 by Ilya Kotov                                 *
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

#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QSettings>
#include <QApplication>
#include <qmmp/soundcore.h>
#include <qmmp/metadatamanager.h>
#include <qmmpui/metadataformatter.h>
#include <qmmpui/playlistitem.h>

#include "popupwidget.h"

using namespace PlayListPopup;

PopupWidget::PopupWidget(QWidget *parent)
        : QWidget(parent)
{
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::Dialog | Qt::FramelessWindowHint);
    //setFrameStyle(QFrame::Box | QFrame::Plain);
    setAttribute(Qt::WA_QuitOnClose, false);
    QHBoxLayout *hlayout = new QHBoxLayout(this); //layout
    m_pixlabel = new QLabel(this);
    hlayout->addWidget(m_pixlabel);

    m_label1 = new QLabel(this);
    hlayout->addWidget (m_label1);

    //settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Skinned");
    setWindowOpacity(settings.value("popup_opacity", 1.0).toDouble());
    m_coverSize = settings.value("popup_cover_size", 48).toInt();
    m_template = settings.value("popup_template",DEFAULT_TEMPLATE).toString();
    m_formatter.setPattern(m_template);
    int delay = settings.value("popup_delay", 2500).toInt();
    bool show_cover = settings.value("popup_show_cover",true).toBool();
    settings.endGroup();
    //timer
    m_timer = new QTimer(this);
    m_timer->setInterval(delay);
    m_timer->setSingleShot (true);
    connect(m_timer, SIGNAL(timeout ()), SLOT(show()));
    if(show_cover)
        connect(m_timer, SIGNAL(timeout ()), SLOT(loadCover()));
    else
        m_pixlabel->hide();
    setMouseTracking(true);
}

PopupWidget::~PopupWidget()
{}

void PopupWidget::mousePressEvent (QMouseEvent *)
{
    hide();
}

void PopupWidget::mouseMoveEvent (QMouseEvent *)
{
    hide();
}

void PopupWidget::prepare(PlayListTrack *item, QPoint pos)
{
    pos += QPoint(15,10);

    m_url = item->url();
    hide();
    if(!item)
    {
        m_timer->stop();
        return;
    }

    m_label1->setText(m_formatter.format(item));
    qApp->processEvents();
    updateGeometry ();
    resize(sizeHint());
    qApp->processEvents();
    m_timer->start();
    QRect rect = QApplication::desktop()->availableGeometry(this);
    if(pos.x() + width() > rect.x() + rect.width())
        pos.rx() -= width();
    move(pos);
}

void PopupWidget::deactivate()
{
    m_timer->stop();
    m_url.clear();
    hide();
}

const QString PopupWidget::url() const
{
    return m_url;
}

void PopupWidget::loadCover()
{
    if(m_url.isEmpty())
        return;
    QPixmap pix = MetaDataManager::instance()->getCover(m_url);
    if(pix.isNull())
        pix = QPixmap(":/skinned/ui_no_cover.png");
    m_pixlabel->setFixedSize(m_coverSize,m_coverSize);
    m_pixlabel->setPixmap(pix.scaled(m_coverSize,m_coverSize,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    qApp->processEvents();
    updateGeometry ();
    resize(sizeHint());
    qApp->processEvents();
}
