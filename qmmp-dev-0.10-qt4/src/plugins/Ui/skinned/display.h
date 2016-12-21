/***************************************************************************
 *   Copyright (C) 2006-2013 by Ilya Kotov                                 *
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
#ifndef DISPLAY_H
#define DISPLAY_H

#include <QPixmap>
#include <qmmp/statehandler.h>
#include "pixmapwidget.h"

class TimeIndicator;
class TimeIndicatorModel;
class PositionBar;
class Skin;
class ToggleButton;
class TitleBar;
class NumberDisplay;
class SymbolDisplay;
class MonoStereo;
class PlayStatus;
class VolumeBar;
class BalanceBar;
class MainWindow;
class SoundCore;
class Button;
class TextScroller;
class MainVisual;
class TitleBar;

/**
   @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class MainDisplay : public PixmapWidget
{
    Q_OBJECT
public:
    MainDisplay(MainWindow *parent = 0);

    ~MainDisplay();

    void setEQ(QWidget*);
    void setPL(QWidget*);
    bool isEqualizerVisible()const;
    bool isPlaylistVisible()const;
    bool isRepeatable()const;
    bool isShuffle()const;
    void setIsRepeatable(bool);
    void setIsShuffle(bool);
    void setMinimalMode(bool b = true);
    void setActive(bool b);

public slots:
    void setDuration(qint64);

signals:
    void repeatableToggled(bool);
    void shuffleToggled(bool);

protected:
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent*);

private slots:
    void updateSkin();
    void displayVolume();
    void showPosition();
    void updatePosition();
    void setSampleRate(quint32 rate);
    void setTime(qint64);
    void setState(Qmmp::State state);

private:
    void updatePositions();
    void updateMask();
    QWidget* m_equlizer;
    QWidget* m_playlist;
    bool m_shaded;
    Skin *m_skin;
    PositionBar *m_posbar;
    Button *m_previous;
    Button *m_play;
    Button *m_pause;
    Button *m_stop;
    Button *m_next;
    Button *m_eject;
    TextScroller *m_text;
    ToggleButton *m_eqButton;
    ToggleButton *m_plButton;
    ToggleButton *m_shuffleButton;
    ToggleButton *m_repeatButton;
    SymbolDisplay* m_kbps;
    SymbolDisplay* m_freq;
    MonoStereo* m_monoster;
    PlayStatus* m_playstatus;
    VolumeBar* m_volumeBar;
    BalanceBar* m_balanceBar;
    MainWindow* m_mw;
    MainVisual* m_vis;
    TimeIndicatorModel* m_timeIndicatorModel;
    TimeIndicator* m_timeIndicator;
    TitleBar *m_titlebar;
    SoundCore *m_core;
    QWidget *m_aboutWidget;
};

#endif
