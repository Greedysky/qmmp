/***************************************************************************
 *   Copyright (C) 2006-2015 by Ilya Kotov                                 *
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
#include <QSettings>
#include <QMouseEvent>
#include <qmmp/qmmp.h>

#include "skin.h"
#include "timeindicator.h"


TimeIndicatorModel::TimeIndicatorModel(QObject *parent)
    : QObject (parent)
    , m_position (0)
    , m_duration (0)
    , m_elapsed (true)
    , m_visible (false)
{
    readSettings();
}

TimeIndicatorModel::~TimeIndicatorModel()
{
    writeSettings();
}

void TimeIndicatorModel::setPosition(int position)
{
    if (m_position != position)
    {
        m_position = position;
        emit changed();
    }
}

void TimeIndicatorModel::setDuration(int duration)
{
    if (m_duration != duration) {
        m_duration = duration;
        emit changed();
    }
}

void TimeIndicatorModel::setElapsed(bool elapsed)
{
    if (m_elapsed != elapsed) {
        m_elapsed = elapsed;
        emit changed();
    }
}

void TimeIndicatorModel::setVisible(bool visible)
{
    if (m_visible != visible)
    {
        m_visible = visible;
        emit changed();
    }
}

int TimeIndicatorModel::displayTime()
{
    int t = m_position;

    if (t < 0)
    {
        return 0;
    }

    if (!m_elapsed)
    {
        t = m_position - m_duration;
    }

    if(qAbs(t) >= 3600)
    {
        t /= 60;
    }

    return t;
}

void TimeIndicatorModel::toggleElapsed()
{
    setElapsed(!elapsed());
}

void TimeIndicatorModel::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Skinned");
    m_elapsed  = settings.value("disp_elapsed",true).toBool();
    settings.endGroup();
}

void TimeIndicatorModel::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Skinned");
    settings.setValue("disp_elapsed",m_elapsed);
    settings.endGroup();
}


TimeIndicator::TimeIndicator (TimeIndicatorModel *model, QWidget *parent)
    : PixmapWidget (parent)
    , m_model (model)
{
    m_skin = Skin::instance();
    updateSkin();
    connect(m_skin, SIGNAL(skinChanged()), this, SLOT(updateSkin()));
    connect(m_model, SIGNAL(changed()), this, SLOT(modelChanged()));
}

void TimeIndicator::modelChanged()
{
    m_pixmap.fill (Qt::transparent);

    if (m_model->visible()) {
        int r = m_skin->ratio();
        QPainter paint (&m_pixmap);

        if (!m_model->elapsed())
        {
            paint.drawPixmap(r*2,0,m_skin->getNumber(10));
        }

        int t = qAbs(m_model->displayTime());

        paint.drawPixmap(r*13,0,m_skin->getNumber(t/600%10));
        paint.drawPixmap(r*26,0,m_skin->getNumber(t/60%10));
        paint.drawPixmap(r*43,0,m_skin->getNumber(t%60/10));
        paint.drawPixmap(r*56,0,m_skin->getNumber(t%60%10));
    }

    setPixmap (m_pixmap);
}

void TimeIndicator::mousePressEvent(QMouseEvent* e)
{
    if (m_model->visible() && e->button() & Qt::LeftButton)
    {
        m_model->toggleElapsed();
    }
    PixmapWidget::mousePressEvent(e);
}

TimeIndicator::~TimeIndicator()
{
}


void TimeIndicator::updateSkin()
{
    m_pixmap = QPixmap (65 * m_skin->ratio(),13 * m_skin->ratio());
    modelChanged();
}

void TimeIndicator::mouseMoveEvent(QMouseEvent *)
{}

void TimeIndicator::mouseReleaseEvent(QMouseEvent *)
{}
