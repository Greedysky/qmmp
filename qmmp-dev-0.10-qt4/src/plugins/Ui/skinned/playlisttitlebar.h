/***************************************************************************
 *   Copyright (C) 2007-2016 by Ilya Kotov                                 *
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
#ifndef PLAYLISTTITLEBAR_H
#define PLAYLISTTITLEBAR_H

#include <qmmpui/metadataformatter.h>
#include "playlist.h"
#include "pixmapwidget.h"
#include "mainwindow.h"

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class Skin;
class MainWindow;
class Button;
class PlayListModel;

class PlayListTitleBar : public PixmapWidget
{
Q_OBJECT
public:
    PlayListTitleBar(QWidget *parent = 0);
    ~PlayListTitleBar();
    void setActive(bool);
    void readSettings();

public slots:
    void showCurrent();
    void setModel(PlayListModel *selected, PlayListModel *previous = 0);

private slots:
    void updateSkin();
    void shade();

private:
    void updatePositions();
    void updatePixmap();
    Skin *m_skin;
    QPoint pos;
    bool m_active;
    PlayList* m_pl;
    MainWindow* m_mw;
    Button* m_close;
    Button* m_shade;
    Button* m_shade2;
    bool m_shaded;
    bool m_align, m_resize;
    int m_ratio;
    int m_height;
    PlayListModel* m_model;
    QString m_text;
    QString m_truncatedText;
    QFont m_font;
    MetaDataFormatter m_formatter;

protected:
    void resizeEvent(QResizeEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
};

#endif
