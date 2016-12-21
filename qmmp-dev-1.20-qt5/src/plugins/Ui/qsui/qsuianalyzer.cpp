/***************************************************************************
 *   Copyright (C) 2012-2015 by Ilya Kotov                                 *
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
#include <QLabel>
#include <qmmp/qmmp.h>
#include <qmmp/buffer.h>
#include <math.h>
#include <stdlib.h>
#include "fft.h"
#include "inlines.h"
#include "qsuianalyzer.h"

#define VISUAL_NODE_SIZE 512 //samples
#define VISUAL_BUFFER_SIZE (5*VISUAL_NODE_SIZE)

QSUiAnalyzer::QSUiAnalyzer (QWidget *parent) : Visual (parent)
{
    m_intern_vis_data = 0;
    m_peaks = 0;
    m_x_scale = 0;
    m_buffer_at = 0;
    m_rows = 0;
    m_cols = 0;
    m_offset = 0;
    m_update = false;
    m_show_cover = false;
    m_running = false;
    m_pixLabel = new QLabel(this);
    createMenu();

    m_timer = new QTimer (this);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));
    m_left_buffer = new float[VISUAL_BUFFER_SIZE];
    m_right_buffer = new float[VISUAL_BUFFER_SIZE];

    readSettings();
    clear();
}

QSUiAnalyzer::~QSUiAnalyzer()
{
    delete [] m_left_buffer;
    delete [] m_right_buffer;

    if(m_peaks)
        delete [] m_peaks;
    if(m_intern_vis_data)
        delete [] m_intern_vis_data;
    if(m_x_scale)
        delete [] m_x_scale;
}

void QSUiAnalyzer::clear()
{
    m_buffer_at = 0;
    m_rows = 0;
    m_cols = 0;
    update();
}

void QSUiAnalyzer::clearCover()
{
    m_cover = QPixmap();
    updateCover();
    update();
}

QSize QSUiAnalyzer::sizeHint() const
{
    return QSize(200, 100);
}

void QSUiAnalyzer::add (float *data, size_t samples, int chan)
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

    stereo_from_multichannel(m_left_buffer + m_buffer_at,
                               m_right_buffer + m_buffer_at, data, frames, chan);


    m_buffer_at += frames;
}

void QSUiAnalyzer::setCover(const QPixmap &pixmap)
{
    m_cover = pixmap;
    updateCover();
}

void QSUiAnalyzer::timeout()
{
    mutex()->lock();
    if(m_buffer_at < VISUAL_NODE_SIZE)
    {
        mutex()->unlock ();
        return;
    }

    process (m_left_buffer, m_right_buffer);
    m_buffer_at -= VISUAL_NODE_SIZE;
    memmove(m_left_buffer, m_left_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
    memmove(m_right_buffer, m_right_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
    mutex()->unlock ();
    update();
}

void QSUiAnalyzer::paintEvent (QPaintEvent * e)
{
    QPainter painter (this);
    painter.fillRect(e->rect(),m_bgColor);
    draw(&painter);
}

void QSUiAnalyzer::hideEvent (QHideEvent *)
{
    m_timer->stop();
}

void QSUiAnalyzer::showEvent (QShowEvent *)
{
    if(m_running)
        m_timer->start();
}

void QSUiAnalyzer::resizeEvent(QResizeEvent *)
{
    updateCover();
}

void QSUiAnalyzer::process (float *left, float *right)
{
    int rows = qMax((height() - 2) / m_cell_size.height(),2);
    int cols = qMax((width() - m_offset - 2) / m_cell_size.width(),1);

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;
        if(m_peaks)
            delete [] m_peaks;
        if(m_intern_vis_data)
            delete [] m_intern_vis_data;
        if(m_x_scale)
            delete [] m_x_scale;
        m_peaks = new double[m_cols];
        m_intern_vis_data = new double[m_cols];
        m_x_scale = new int[m_cols + 1];

        for(int i = 0; i < m_cols; ++i)
        {
            m_peaks[i] = 0;
            m_intern_vis_data[i] = 0;
        }
        for(int i = 0; i < m_cols + 1; ++i)
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
    }
    short dest[256];
    short y;
    int k, magnitude;
    float data[512];

    for(int i = 0; i < VISUAL_NODE_SIZE; ++i)
    {
        data[i] = left[i] / 2 + right[i] / 2;
        data[i] = qBound(-1.0f, data[i], 1.0f);
    }
    calc_freq (dest, data);

    double y_scale = (double) 1.25 * m_rows / log(256);

    for (int i = 0; i < m_cols; i++)
    {
        y = 0;
        magnitude = 0;

        if(m_x_scale[i] == m_x_scale[i + 1])
        {
            y = dest[i];
        }
        for (k = m_x_scale[i]; k < m_x_scale[i + 1]; k++)
        {
            y = qMax(dest[k], y);
        }

        y >>= 7; //256


        if (y)
        {
            magnitude = int(log (y) * y_scale);
            magnitude = qBound(0, magnitude, m_rows);
        }


        m_intern_vis_data[i] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i] ? magnitude : m_intern_vis_data[i];

        if (m_show_peaks)
        {
            m_peaks[i] -= m_peaks_falloff * m_rows / 15;
            m_peaks[i] = magnitude > m_peaks[i] ? magnitude : m_peaks[i];
        }
    }
}

void QSUiAnalyzer::draw (QPainter *p)
{
    QBrush brush(Qt::SolidPattern);
    int x = 0;

    for (int j = 0; j < m_cols; ++j)
    {
        x = m_offset + j * m_cell_size.width() + 1;

        for (int i = 0; i <= m_intern_vis_data[j]; ++i)
        {
            if (i <= m_rows/3)
                brush.setColor(m_color1);
            else if (i > m_rows/3 && i <= 2 * m_rows / 3)
                brush.setColor(m_color2);
            else
                brush.setColor(m_color3);

            p->fillRect (x, height() - i * m_cell_size.height(),
                         m_cell_size.width() - 1, m_cell_size.height() - 4, brush);
        }

        if (m_show_peaks)
        {
            p->fillRect (x, height() - int(m_peaks[j]) * m_cell_size.height(),
                         m_cell_size.width() - 1, m_cell_size.height() - 4, m_peakColor);
        }
    }
}

void QSUiAnalyzer::createMenu()
{
    m_menu = new QMenu (this);
    connect(m_menu, SIGNAL(triggered (QAction *)),SLOT(writeSettings()));
    connect(m_menu, SIGNAL(triggered (QAction *)),SLOT(readSettings()));

    m_coverAction = m_menu->addAction(tr("Cover"));
    m_coverAction->setCheckable(true);

    m_peaksAction = m_menu->addAction(tr("Peaks"));
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
    update();
}

void QSUiAnalyzer::updateCover()
{
    if(m_show_cover && !m_cover.isNull())
    {
        m_offset = height();
        m_pixLabel->setGeometry(10,10, height() - 20, height() - 20);
        m_pixLabel->setPixmap(m_cover.scaled(m_pixLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        m_pixLabel->show();
    }
    else
    {
        m_offset = 0;
        m_pixLabel->hide();
    }
}

void QSUiAnalyzer::mousePressEvent (QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        m_menu->exec(e->globalPos());
}

void QSUiAnalyzer::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Simple");
    m_peaks_falloff = settings.value("vis_peaks_falloff", 0.2).toDouble();
    m_analyzer_falloff = settings.value("vis_analyzer_falloff", 2.2).toDouble();
    m_show_peaks = settings.value("vis_show_peaks", true).toBool();
    m_show_cover = settings.value("vis_show_cover", true).toBool();
    m_timer->setInterval(1000 / settings.value("vis_refresh_rate", 25).toInt());
    m_color1.setNamedColor(settings.value("vis_color1", "#BECBFF").toString());
    m_color2.setNamedColor(settings.value("vis_color2", "#BECBFF").toString());
    m_color3.setNamedColor(settings.value("vis_color3", "#BECBFF").toString());
    m_bgColor.setNamedColor(settings.value("vis_bg_color", "Black").toString());
    m_peakColor.setNamedColor(settings.value("vis_peak_color", "#DDDDDD").toString());
    m_cell_size =  QSize(14, 8);
    if(!m_update)
    {
        m_update = true;
        m_coverAction->setChecked(m_show_cover);
        m_peaksAction->setChecked(m_show_peaks);

        foreach(QAction *act, m_fpsGroup->actions ())
        {
            if (m_timer->interval() == 1000 / act->data().toInt())
                act->setChecked(true);
        }
        foreach(QAction *act, m_peaksFalloffGroup->actions ())
        {
            if (m_peaks_falloff == act->data().toDouble())
                act->setChecked(true);
        }
        foreach(QAction *act, m_analyzerFalloffGroup->actions ())
        {
            if (m_analyzer_falloff == act->data().toDouble())
                act->setChecked(true);
        }
    }
    updateCover();
    settings.endGroup();
}

void QSUiAnalyzer::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Simple");

    QAction *act = m_fpsGroup->checkedAction ();
    settings.setValue("vis_refresh_rate", act ? act->data().toInt() : 25);
    act = m_peaksFalloffGroup->checkedAction ();
    settings.setValue("vis_peaks_falloff", act ? act->data().toDouble() : 0.2);
    act = m_analyzerFalloffGroup->checkedAction ();
    settings.setValue("vis_analyzer_falloff", act ? act->data().toDouble() : 2.2);
    settings.setValue("vis_show_peaks", m_peaksAction->isChecked());
    settings.setValue("vis_show_cover", m_coverAction->isChecked());
    settings.endGroup();
}

void QSUiAnalyzer::start()
{
    m_running = true;
    if(isVisible())
        m_timer->start();
}

void QSUiAnalyzer::stop()
{
    m_running = false;
    m_timer->stop();
}
