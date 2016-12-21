/***************************************************************************
 *   Copyright (C) 2013-2015 by Ilya Kotov                                 *
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
#include <QPaintEvent>
#include <QAction>
#include <qmmp/qmmp.h>
#include <qmmpui/filedialog.h>
#include "coverwidget.h"

CoverWidget::CoverWidget(QWidget *parent)
        : QWidget(parent)
{
    setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction *saveAsAction = new QAction(tr("&Save As..."), this);
    connect(saveAsAction, SIGNAL(triggered()), SLOT(saveAs()));
    addAction(saveAsAction);
    m_pixmap = QPixmap(":/qsui/ui_no_cover.png");
}

CoverWidget::~CoverWidget()
{}

void CoverWidget::setCover(const QPixmap &pixmap)
{
    m_pixmap = pixmap.isNull() ? QPixmap(":/qsui/ui_no_cover.png") : pixmap;
    update();
}

void CoverWidget::clearCover()
{
    setCover(QPixmap());
    update();
}

void CoverWidget::paintEvent(QPaintEvent *p)
{
    QPainter paint(this);
    if(!m_pixmap.isNull())
    {
        int w = qMin(p->rect().width(), p->rect().height());
        paint.drawPixmap(0,0, m_pixmap.scaled(w,w,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}

void CoverWidget::saveAs()
{
    QString path = FileDialog::getSaveFileName(this, tr("Save Cover As"),
                                                 QDir::homePath() + "/cover.jpg",
                                                 tr("Images") +" (*.png *.jpg)");

    if (!path.isEmpty())
        m_pixmap.save(path);
}
