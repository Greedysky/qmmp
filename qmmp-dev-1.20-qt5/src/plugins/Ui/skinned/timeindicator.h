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
#ifndef TIMEINDICATOR_H
#define TIMEINDICATOR_H

#include "pixmapwidget.h"

class QMouseEvent;

class Skin;


/** Class TimeIndicatorModel
 * @author Thomas Perl <m@thp.io>
 *
 * Encapsulates state and settings for the time indicator
 */
class TimeIndicatorModel : public QObject {
    Q_OBJECT
public:
    TimeIndicatorModel(QObject *parent = 0);
    ~TimeIndicatorModel();

    int position() { return m_position; }
    int duration() { return m_duration; }
    bool elapsed() { return m_elapsed; }
    bool visible() { return m_visible; }

    void setPosition(int position);
    void setDuration(int duration);
    void setElapsed(bool elapsed);
    void setVisible(bool visible);

    int displayTime();

public slots:
    void toggleElapsed();

signals:
    void changed();

private:
    void readSettings();
    void writeSettings();

    int m_position;
    int m_duration;
    bool m_elapsed;
    bool m_visible;
};


/** Class TimeIndicator
 *  @author Vladimir Kuznetsov <vovanec@gmail.com>
 *
 *  Represents time indicator in the main display. Can show elapsed
 *  and rest time of song (mouse press on indicator changes mode)
 */
class TimeIndicator : public PixmapWidget
{
    Q_OBJECT
public:
    TimeIndicator(TimeIndicatorModel *model, QWidget *parent = 0);
    ~TimeIndicator();

protected:
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);

private slots:
    void modelChanged();
    void updateSkin();

private:
    TimeIndicatorModel *m_model;
    QPixmap m_pixmap;
    Skin *m_skin;
};

#endif
