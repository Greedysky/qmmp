/***************************************************************************
 *   Copyright (C) 2007-2012 by Ilya Kotov                                 *
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
#include <QSettings>
#include <QPainter>
#include <QMenu>
#include <QActionGroup>
#include <qmmp/buffer.h>
#include <qmmp/qmmp.h>
#include <math.h>
#include <stdlib.h>
#include "skin.h"
#include "fft.h"
#include "inlines.h"
#include "mainvisual.h"

#define VISUAL_NODE_SIZE 512 //samples
#define VISUAL_BUFFER_SIZE (5*VISUAL_NODE_SIZE)

MainVisual *MainVisual::m_instance = 0;

MainVisual *MainVisual::instance()
{
    if (!m_instance)
        qFatal ("MainVisual: this object is not created!");
    return m_instance;
}

MainVisual::MainVisual (QWidget *parent) : Visual (parent), m_vis (0)
{
    m_skin = Skin::instance();
    m_ratio = m_skin->ratio();
    connect(m_skin, SIGNAL(skinChanged()), this, SLOT(readSettings()));
    m_timer = new QTimer (this);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));
    m_buffer = new float[VISUAL_BUFFER_SIZE];
    m_buffer_at = 0;
    m_instance = this;
    m_update = false;
    createMenu();
    readSettings();
}

MainVisual::~MainVisual()
{
    writeSettings();
    if (m_vis)
    {

        delete m_vis;
        m_vis = 0;
    }
    delete [] m_buffer;
    m_instance = 0;
}

void MainVisual::setVisual (VisualBase *newvis)
{
    m_timer->stop();
    if (m_vis)
        delete m_vis;
    m_vis = newvis;
    if (m_vis)
        m_timer->start();
    else
    {
        m_pixmap.fill (Qt::transparent);
        update();
    }
}

void MainVisual::clear()
{
    m_buffer_at = 0;
    if (m_vis)
        m_vis->clear();
    m_pixmap = m_bg;
    update();
}

void MainVisual::add (float *data, size_t samples, int chan)
{
    if (!m_timer->isActive () || !m_vis)
        return;

    if(VISUAL_BUFFER_SIZE == m_buffer_at)
    {
        m_buffer_at -= VISUAL_NODE_SIZE;
        memmove(m_buffer, m_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
        return;
    }

    int frames = qMin(int(samples/chan), VISUAL_BUFFER_SIZE - m_buffer_at);
    mono16_from_multichannel(m_buffer + m_buffer_at, data, frames, chan);

    m_buffer_at += frames;
}

void MainVisual::timeout()
{
    mutex()->lock ();

    if(m_buffer_at < VISUAL_NODE_SIZE)
    {
        mutex()->unlock ();
        return;
    }

    if (m_vis)
    {
        m_vis->process (m_buffer);
        m_buffer_at -= VISUAL_NODE_SIZE;
        memmove(m_buffer, m_buffer + VISUAL_NODE_SIZE, m_buffer_at*sizeof(float));
        m_pixmap = m_bg;
        QPainter p(&m_pixmap);
        m_vis->draw (&p);
    }
    mutex()->unlock ();
    update();
}

void MainVisual::paintEvent (QPaintEvent *)
{
    QPainter painter (this);
    painter.drawPixmap (0,0, m_pixmap);
}

void MainVisual::hideEvent (QHideEvent *)
{
    m_timer->stop();
}

void MainVisual::showEvent (QShowEvent *)
{
    if (m_vis)
        m_timer->start();
}

void MainVisual::mousePressEvent (QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        m_menu->exec(e->globalPos());
    else
    {
        m_pixmap = m_bg;
        if (!m_vis)
            setVisual(new mainvisual::Analyzer);
        else if (m_vis->name() == "Analyzer")
            setVisual(new mainvisual::Scope);
        else if (m_vis->name() == "Scope")
            setVisual(0);

        QString str = m_vis ? m_vis->name() : "Off";
        foreach(QAction *act, m_visModeGroup->actions ())
        {
            if (str == act->data().toString())
            {
                act->setChecked(true);
                break;
            }
        }
        writeSettings();
    }
}

void MainVisual::drawBackGround()
{
    m_bg = QPixmap (76 * m_ratio, 16 * m_ratio);
    if (m_transparentAction->isChecked())
    {
        m_bg.fill (Qt::transparent);
        return;
    }
    QPainter painter(&m_bg);
    for (int x = 0; x < 76 * m_ratio; x += 2)
    {
        painter.setPen(m_skin->getVisColor(0));
        painter.drawLine(x + 1, 0, x + 1, 16 *m_ratio);
        for (int y = 0; y < 16 *m_ratio; y += 2)
        {
            painter.setPen(m_skin->getVisColor(0));
            painter.drawPoint(x,y);
            painter.setPen(m_skin->getVisColor(1));
            painter.drawPoint(x,y + 1);
        }
    }
}

void MainVisual::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Skinned");
    QAction *act = m_peaksFalloffGroup->checkedAction ();
    settings.setValue("vis_peaks_falloff", act ? act->data().toDouble() : 0.2);
    act = m_analyzerFalloffGroup->checkedAction ();
    settings.setValue("vis_analyzer_falloff", act ? act->data().toDouble() : 2.2);

    settings.setValue("vis_show_peaks", m_peaksAction->isChecked());

    act = m_analyzerModeGroup->checkedAction();
    settings.setValue("vis_analyzer_mode", act ? act->data().toInt() : 0);
    act = m_analyzerTypeGroup->checkedAction();
    settings.setValue("vis_analyzer_type", act ? act->data().toInt() : 1);
    settings.setValue("vis_transparent_bg", m_transparentAction->isChecked());

    act = m_visModeGroup->checkedAction ();
    settings.setValue("vis_type", act ? act->data().toString() : "Off");

    act = m_fpsGroup->checkedAction();
    settings.setValue("vis_rate", act ? act->data().toInt() : 25);
}

void MainVisual::createMenu()
{
    m_menu = new QMenu (this);
    connect(m_menu, SIGNAL(triggered (QAction *)),SLOT(writeSettings()));
    connect(m_menu, SIGNAL(triggered (QAction *)),SLOT(readSettings()));
    QMenu *visMode = m_menu->addMenu(tr("Visualization Mode"));
    m_visModeGroup = new QActionGroup(this);
    m_visModeGroup->setExclusive(true);
    m_visModeGroup->addAction(tr("Analyzer"))->setData("Analyzer");
    m_visModeGroup->addAction(tr("Scope"))->setData("Scope");
    m_visModeGroup->addAction(tr("Off"))->setData("Off");
    foreach(QAction *act, m_visModeGroup->actions ())
    {
        act->setCheckable(true);
        visMode->addAction(act);
    }

    QMenu *analyzerMode = m_menu->addMenu(tr("Analyzer Mode"));
    m_analyzerModeGroup = new QActionGroup(this);
    m_analyzerTypeGroup = new QActionGroup(this);
    m_analyzerModeGroup->addAction(tr("Normal"))->setData(0);
    m_analyzerModeGroup->addAction(tr("Fire"))->setData(1);
    m_analyzerModeGroup->addAction(tr("Vertical Lines"))->setData(2);
    m_analyzerTypeGroup->addAction(tr("Lines"))->setData(0);
    m_analyzerTypeGroup->addAction(tr("Bars"))->setData(1);
    foreach(QAction *act, m_analyzerModeGroup->actions ())
    {
        act->setCheckable(true);
        analyzerMode->addAction(act);
    }
    analyzerMode->addSeparator ();
    foreach(QAction *act, m_analyzerTypeGroup->actions ())
    {
        act->setCheckable(true);
        analyzerMode->addAction(act);
    }
    analyzerMode->addSeparator ();
    m_peaksAction = analyzerMode->addAction(tr("Peaks"));
    m_peaksAction->setCheckable(true);


    QMenu *refreshRate = m_menu->addMenu(tr("Refresh Rate"));
    m_fpsGroup = new QActionGroup(this);
    m_fpsGroup->setExclusive(true);
    m_fpsGroup->addAction(tr("50 fps"))->setData(50);
    m_fpsGroup->addAction(tr("25 fps"))->setData(25);
    m_fpsGroup->addAction(tr("10 fps"))->setData(10);
    m_fpsGroup->addAction(tr("5 fps"))->setData(5);
    foreach(QAction *act, m_fpsGroup->actions ())
    {
        act->setCheckable(true);
        refreshRate->addAction(act);
    }

    QMenu *analyzerFalloff = m_menu->addMenu(tr("Analyzer Falloff"));
    m_analyzerFalloffGroup = new QActionGroup(this);
    m_analyzerFalloffGroup->setExclusive(true);
    m_analyzerFalloffGroup->addAction(tr("Slowest"))->setData(1.2);
    m_analyzerFalloffGroup->addAction(tr("Slow"))->setData(1.8);
    m_analyzerFalloffGroup->addAction(tr("Medium"))->setData(2.2);
    m_analyzerFalloffGroup->addAction(tr("Fast"))->setData(2.4);
    m_analyzerFalloffGroup->addAction(tr("Fastest"))->setData(2.8);
    foreach(QAction *act, m_analyzerFalloffGroup->actions ())
    {
        act->setCheckable(true);
        analyzerFalloff->addAction(act);
    }

    QMenu *peaksFalloff = m_menu->addMenu(tr("Peaks Falloff"));
    m_peaksFalloffGroup = new QActionGroup(this);
    m_peaksFalloffGroup->setExclusive(true);
    m_peaksFalloffGroup->addAction(tr("Slowest"))->setData(0.05);
    m_peaksFalloffGroup->addAction(tr("Slow"))->setData(0.1);
    m_peaksFalloffGroup->addAction(tr("Medium"))->setData(0.2);
    m_peaksFalloffGroup->addAction(tr("Fast"))->setData(0.4);
    m_peaksFalloffGroup->addAction(tr("Fastest"))->setData(0.8);
    foreach(QAction *act, m_peaksFalloffGroup->actions ())
    {
        act->setCheckable(true);
        peaksFalloff->addAction(act);
    }
    QMenu *background = m_menu->addMenu(tr("Background"));
    m_transparentAction = background->addAction(tr("Transparent"));
    m_transparentAction->setCheckable(true);
    update();
}


void MainVisual::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Skinned");
    QString vis_name = settings.value("vis_type","Analyzer").toString();
    if(!m_update)
    {
        m_update = true;

        foreach(QAction *act, m_visModeGroup->actions ())
        {
            if (vis_name == act->data().toString())
                act->setChecked(true);
        }
        m_peaksAction->setChecked(settings.value("vis_show_peaks", true).toBool());
        int fps = settings.value("vis_rate", 25).toInt();
        foreach(QAction *act, m_fpsGroup->actions ())
        {
            if (fps == act->data().toInt())
                act->setChecked(true);
        }
        int mode = settings.value("vis_analyzer_mode", 0).toInt();
        foreach(QAction *act, m_analyzerModeGroup->actions ())
        {
            if (mode == act->data().toInt())
                act->setChecked(true);
        }
        int type = settings.value("vis_analyzer_type", 1).toInt();
        foreach(QAction *act, m_analyzerTypeGroup->actions ())
        {
            if (type == act->data().toInt())
                act->setChecked(true);
        }
        double speed = settings.value("vis_peaks_falloff", 0.2).toDouble();
        foreach(QAction *act, m_peaksFalloffGroup->actions ())
        {
            if (speed == act->data().toDouble())
                act->setChecked(true);
        }
        speed = settings.value("vis_analyzer_falloff", 2.2).toDouble();
        foreach(QAction *act, m_analyzerFalloffGroup->actions ())
        {
            if (speed == act->data().toDouble())
                act->setChecked(true);
        }
        m_transparentAction->setChecked(settings.value("vis_transparent_bg", false).toBool());

        //update settings from previous version
        if(!m_analyzerFalloffGroup->checkedAction() || !m_peaksFalloffGroup->checkedAction())
        {
            m_analyzerFalloffGroup->actions()[2]->setChecked(true);
            m_peaksFalloffGroup->actions()[2]->setChecked(true);
            writeSettings();
        }
    }
    m_ratio = m_skin->ratio();
    drawBackGround();
    m_pixmap = m_bg;
    QAction *act = m_fpsGroup->checkedAction ();
    m_timer->setInterval (act ? 1000 / act->data().toInt() : 25);
    if (vis_name == "Analyzer")
        setVisual(new mainvisual::Analyzer);
    else if (vis_name == "Scope")
        setVisual(new mainvisual::Scope);
    else
        setVisual(0);
    resize(76 * m_ratio, 16 * m_ratio);
    update();
}

using namespace mainvisual;

Analyzer::Analyzer()
{
    clear();
    m_skin = Skin::instance();
    m_size = QSize(76*m_skin->ratio(), 16*m_skin->ratio());

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Skinned");
    m_peaks_falloff = settings.value("vis_peaks_falloff", 0.2).toDouble();
    m_analyzer_falloff = settings.value("vis_analyzer_falloff", 2.2).toDouble();
    m_show_peaks = settings.value("vis_show_peaks", true).toBool();

    m_lines = settings.value("vis_analyzer_type", 1).toInt() == 0;
    m_mode = settings.value("vis_analyzer_mode", 0).toInt();
}

Analyzer::~Analyzer()
{}

void Analyzer::clear()
{
    for (int i = 0; i< 75; ++i)
    {
        m_intern_vis_data[i] = 0;
        m_peaks[i] = 0;
    }
}

bool Analyzer::process (float *l)
{
    static fft_state *state = 0;
    if (!state)
        state = fft_init();
    short dest[256];

    static const int xscale_long[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
        19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
        35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
        52, 53, 54, 55, 56, 57, 58, 61, 66, 71, 76, 81, 87, 93, 100, 107,
        114, 122, 131, 140, 150, 161, 172, 184, 255
    };

    static const int xscale_short[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 15, 20, 27,
        36, 47, 62, 82, 107, 141, 184, 255
    };

    calc_freq (dest, l);

    const double y_scale = 3.60673760222;   /* 20.0 / log(256) */
    int max = m_lines ? 75 : 19, y, j;

    for (int i = 0; i < max; i++)
    {
        if (m_lines)
            for (j = xscale_long[i], y = 0; j < xscale_long[i + 1]; j++)
            {
                if (dest[j] > y)
                    y = dest[j];
            }
        else
            for (j = xscale_short[i], y = 0; j < xscale_short[i + 1]; j++)
            {
                if (dest[j] > y)
                    y = dest[j];
            }
        y >>= 7;
        int magnitude = 0;
        if (y != 0)
        {
            magnitude = int(log (y) * y_scale);
            if (magnitude > 15)
                magnitude = 15;
            if (magnitude < 0)
                magnitude = 0;
        }

        m_intern_vis_data[i] -= m_analyzer_falloff;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i]
                               ? magnitude : m_intern_vis_data[i];
        if (m_show_peaks)
        {
            m_peaks[i] -= m_peaks_falloff;
            m_peaks[i] = magnitude > m_peaks[i]
                         ? magnitude : m_peaks[i];
        }
    }
    return true;
}

void Analyzer::draw (QPainter *p)
{
    int r = m_skin->ratio();
    if (m_lines)
        for (int j = 0; j < 75; ++j)
        {
            for (int i = 0; i <= m_intern_vis_data[j]; ++i)
            {
                if (m_mode == 0)
                    p->setPen (m_skin->getVisColor (18-i));
                else if (m_mode == 1)
                    p->setPen (m_skin->getVisColor (3+(int(m_intern_vis_data[j])-i)));
                else
                    p->setPen (m_skin->getVisColor (18-int(m_intern_vis_data[j])));
                p->drawPoint (j*r, m_size.height() - r*i);
                if(r == 2)
                    p->drawPoint (j*r+1, m_size.height() - r*i);
            }
            p->setPen (m_skin->getVisColor (23));
            if (m_show_peaks)
            {
                p->drawPoint (j*r, m_size.height() - r*m_peaks[j]);
                if(r == 2)
                    p->drawPoint (j*r+1, m_size.height() - r*m_peaks[j]);
            }
        }
    else
        for (int j = 0; j < 19; ++j)
        {
            for (int i = 0; i <= m_intern_vis_data[j]; ++i)
            {
                if (m_mode == 0)
                    p->setPen (m_skin->getVisColor (18-i));
                else if (m_mode == 1)
                    p->setPen (m_skin->getVisColor (3+(int(m_intern_vis_data[j])-i)));
                else
                    p->setPen (m_skin->getVisColor (18-int(m_intern_vis_data[j])));

                p->drawLine (j*4*r,m_size.height()-r*i, (j*4+2)*r,m_size.height()-r*i);
                if(r == 2)
                    p->drawLine (j*4*r, m_size.height()-r*i +1, (j*4+2)*r,m_size.height()-r*i+1);
            }
            p->setPen (m_skin->getVisColor (23));
            if (m_show_peaks)
            {
                p->drawLine (j*4*r,m_size.height()-r*m_peaks[j],
                             (j*4+2)*r,m_size.height()-r*m_peaks[j]);
                if(r == 2)
                    p->drawLine (j*4*r,m_size.height()-r*m_peaks[j]+1,
                                 (j*4+2)*r,m_size.height()-r*m_peaks[j]+1);
            }
        }
}

Scope::Scope()
{
    clear();
    m_skin = Skin::instance();
    m_ratio = m_skin->ratio();
}

void Scope::clear()
{
    for (int i = 0; i< 76; ++i)
        m_intern_vis_data[i] = 5;
}

Scope::~Scope()
{}

bool Scope::process(float *l)
{
    int step = (VISUAL_NODE_SIZE << 8)/76;
    int pos = 0;

    for (int i = 0; i < 76; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(l[pos >> 8] * 8.0);
        m_intern_vis_data[i] = qBound(-4, m_intern_vis_data[i], 4);
    }
    return true;
}

void Scope::draw(QPainter *p)
{
    for (int i = 0; i<75; ++i)
    {
        int h1 = 8 - m_intern_vis_data[i];
        int h2 = 8 - m_intern_vis_data[i+1];
        if (h1 > h2)
            qSwap(h1, h2);
        p->setPen (m_skin->getVisColor(18 + qAbs(8 - h2)));
        p->drawLine(i*m_ratio, h1*m_ratio, (i+1)*m_ratio, h2*m_ratio);
    }
    for (int i = 0; i< 76; ++i)
        m_intern_vis_data[i] = 0;
}
