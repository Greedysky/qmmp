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
#ifndef TEXTSCROLLER_H
#define TEXTSCROLLER_H

#include <QWidget>
#include <qmmpui/metadataformatter.h>
#include <qmmp/qmmp.h>

class QTimer;
class QMenu;
class QAction;
class Skin;
class SoundCore;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class TextScroller : public QWidget
{
Q_OBJECT
public:
    TextScroller(QWidget *parent = 0);
    virtual ~TextScroller();

    void setText(const QString &text);

public slots:
    void clear();

private slots:
    void setProgress(int);
    void addOffset();
    void updateSkin();
    void processState(Qmmp::State state);
    void processMetaData();
    void updateText();

private:
    void hideEvent(QHideEvent *);
    void showEvent(QShowEvent *);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void preparePixmap(const QString &text, bool scrollable = false);
    QString m_defautText;
    QString m_bufferText;
    QString m_sliderText;
    QString m_titleText;
    QPixmap m_pixmap;
    int m_x1, m_x2, m_ratio;
    bool m_scroll, m_bitmap, m_pressed;
    int m_press_pos;
    QFont m_font;
    QFontMetrics *m_metrics;
    Skin *m_skin;
    QColor m_color;
    QTimer *m_timer;
    QMenu *m_menu;
    QAction *m_scrollAction, *m_transparencyAction;
    SoundCore *m_core;
    MetaDataFormatter m_formater;
};

#endif
