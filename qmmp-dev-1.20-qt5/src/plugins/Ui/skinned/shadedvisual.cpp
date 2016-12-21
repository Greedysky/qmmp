/***************************************************************************
 *   Copyright (C) 2007-2015 by Ilya Kotov                                 *
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

#include <QTimer>
#include <math.h>
#include "skin.h"
#include "mainvisual.h"
#include "inlines.h"
#include "shadedvisual.h"

#define VISUAL_NODE_SIZE 512 //samples
#define VISUAL_BUFFER_SIZE (5*VISUAL_NODE_SIZE)

ShadedVisual::ShadedVisual(QWidget *parent) : Visual(parent)
{
    m_skin = Skin::instance();
    m_ratio = m_skin->ratio();
    resize(m_ratio*38,m_ratio*5);
    m_pixmap = QPixmap (m_ratio*38,m_ratio*5);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));
    connect(m_skin, SIGNAL(skinChanged()), this, SLOT(updateSkin()));
    m_left_buffer = new float[VISUAL_BUFFER_SIZE];
    m_right_buffer = new float[VISUAL_BUFFER_SIZE];
    m_buffer_at = 0;
    m_timer->setInterval(50);
    m_timer->start();
    clear();
}

ShadedVisual::~ShadedVisual()
{
    delete [] m_left_buffer;
    delete [] m_right_buffer;
}

void ShadedVisual::add(float *data, size_t samples, int chan)
{
    if (!m_timer->isActive ())
        return;

    if(VISUAL_BUFFER_SIZE == m_buffer_at)
    {
        m_buffer_at -= VISUAL_NODE_SIZE;
        memmove(m_left_buffer, m_left_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
        memmove(m_right_buffer, m_right_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
        return;
    }

    int frames = qMin(int(samples/chan), VISUAL_BUFFER_SIZE - m_buffer_at);

    stereo16_from_multichannel(m_left_buffer + m_buffer_at,
                               m_right_buffer + m_buffer_at, data, frames, chan);
    m_buffer_at += frames;
}

void ShadedVisual::clear()
{
    m_buffer_at = 0;
    m_l = 0;
    m_r = 0;
    m_pixmap.fill(m_skin->getVisColor(0));
    update();
}

void ShadedVisual::timeout()
{
    m_pixmap.fill(m_skin->getVisColor(0));

    mutex()->lock ();
    if(m_buffer_at < VISUAL_NODE_SIZE)
    {
        mutex()->unlock ();
        return;
    }

    process (m_left_buffer, m_right_buffer);
    m_buffer_at -= VISUAL_NODE_SIZE;
    memmove(m_left_buffer, m_left_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
    memmove(m_right_buffer, m_right_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
    QPainter p(&m_pixmap);
    draw (&p);
    mutex()->unlock ();
    update();
}

void ShadedVisual::process (float *left, float *right)
{
    int step = (VISUAL_NODE_SIZE << 8)/74;
    int pos = 0;
    int l = 0;
    int r = 0;
    int j_l = 0, j_r = 0;

    for (int i = 0; i < 75; ++i)
    {
        pos += step;

        if (left)
        {
            j_l = abs(left[pos >> 8] * 8.0);

            if (j_l > 15)
                j_l = 15;
            l = qMax(l, j_l);
        }
        if (right)
        {
            j_r = abs(right[pos >> 8] * 8.0);
            if (j_r > 15)
                j_r = 15;
            r = qMax(r, j_r);
        }
    }
    m_l -= 0.5;
    m_l = qMax(m_l, (double)l);
    m_r -= 0.5;
    m_r = qMax(m_r, (double)r);
}

void ShadedVisual::draw (QPainter *p)
{
    for (int i = 0; i < m_l; ++i)
    {
        p->fillRect (i*3*m_ratio, 0, 3*m_ratio, 2*m_ratio, QBrush(m_skin->getVisColor (17-i)));
    }
    for (int i = 0; i < m_r; ++i)
    {
        p->fillRect (i*3*m_ratio, 3*m_ratio, 3*m_ratio, 2*m_ratio, QBrush(m_skin->getVisColor (17-i)));
    }
}

void ShadedVisual::paintEvent (QPaintEvent *)
{
    QPainter painter (this);
    painter.drawPixmap (0,0,m_pixmap);
}

void ShadedVisual::hideEvent (QHideEvent *)
{
    m_timer->stop();
}

void ShadedVisual::showEvent (QShowEvent *)
{
    m_timer->start();
}

void ShadedVisual::updateSkin()
{
    m_ratio = m_skin->ratio();
    resize(m_ratio*38,m_ratio*5);
    m_pixmap = QPixmap (m_ratio*38,m_ratio*5);
    clear();
}
