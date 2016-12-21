/***************************************************************************
 *   Copyright (C) 2009-2012 by Ilya Kotov                                 *
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
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <qmmp/qmmp.h>
#include <qmmpui/filedialog.h>
#include "coverwidget.h"

CoverWidget::CoverWidget(QWidget *parent)
        : QWidget(parent)
{
    setWindowFlags(Qt::Window);
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_menu = new QMenu(this);
    m_menu->addAction(tr("&Save As..."), this, SLOT(saveAs()), tr("Ctrl+S"));
    QMenu *sizeMenu = m_menu->addMenu(tr("Size"));
    QActionGroup *sizeGroup = new QActionGroup(this);
    sizeGroup->addAction(tr("Actual Size"))->setData(0);
    sizeGroup->addAction(tr("128x128"))->setData(128);
    sizeGroup->addAction(tr("256x256"))->setData(256);
    sizeGroup->addAction(tr("512x512"))->setData(512);
    sizeGroup->addAction(tr("1024x1024"))->setData(1024);
    sizeMenu->addActions(sizeGroup->actions());
    connect(sizeMenu, SIGNAL(triggered (QAction *)), SLOT(processResizeAction(QAction *)));
    m_menu->addAction(tr("&Close"), this, SLOT(close()), tr("Alt+F4"));
    addActions(m_menu->actions());
    m_size = 0;
    //settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_size = settings.value("CoverManager/size", 0).toInt();
    foreach(QAction *a, sizeMenu->actions ())
    {
        a->setCheckable(true);
        if(a->data().toInt() == m_size)
        {
            a->setChecked(true);
            processResizeAction(a);
        }
    }
}

CoverWidget::~CoverWidget()
{}

void CoverWidget::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    if(m_size == 0)
       resize(m_pixmap.size());
    update();
}

void CoverWidget::paintEvent(QPaintEvent *p)
{
    QPainter paint(this);
    if(!m_pixmap.isNull())
        paint.drawPixmap(0,0, m_pixmap.scaled(p->rect().size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void CoverWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
        m_menu->exec(e->globalPos());
}

void CoverWidget::saveAs()
{
    QString path = FileDialog::getSaveFileName(this, tr("Save Cover As"),
                                                 QDir::homePath() + "/cover.jpg",
                                                 tr("Images") +" (*.png *.jpg)");

    if (!path.isEmpty())
        m_pixmap.save(path);
}

void CoverWidget::processResizeAction(QAction *action)
{
    m_size = action->data().toInt();
    if(m_size == 0)
        resize(m_pixmap.size());
    else
        resize(m_size, m_size);
    update();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("CoverManager/size", m_size);
}
