/***************************************************************************
 *   Copyright (C) 2008-2015 by Ilya Kotov                                 *
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
#ifndef POPUPWIDGET_H
#define POPUPWIDGET_H

#include <QWidget>
#include <QFrame>
#include <qmmpui/metadataformatter.h>

#define DEFAULT_TEMPLATE "<b>%if(%t,%t,%f)</b> %if(%l,\\(%l\\),) \n%if(%p,<br>%p,)\n%if(%a,<br>%a,)"

class QTimer;
class QLabel;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class PopupWidget : public QFrame
{
    Q_OBJECT
public:
    PopupWidget(QWidget *parent = 0);

    ~PopupWidget();

    enum Position
    {
        TOPLEFT = 0,
        TOP,
        TOPRIGHT,
        RIGHT,
        BOTTOMRIGHT,
        BOTTOM,
        BOTTOMLEFT,
        LEFT,
        CENTER
    };

    void showMetaData();
    void showVolume(int);

protected:
    virtual void mousePressEvent (QMouseEvent *);

private:
    void updatePosition();
    QTimer *m_timer;
    QLabel *m_label1;
    QLabel *m_pixlabel;
    MetaDataFormatter m_formatter;
    uint m_pos;
    int m_coverSize;

};

#endif
