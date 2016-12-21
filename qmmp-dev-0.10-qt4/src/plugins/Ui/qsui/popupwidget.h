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
#include <qmmpui/metadataformatter.h>

#define DEFAULT_TEMPLATE "<b>%if(%t,%t,%f)</b>\n%if(%p,<br>%p,)\n%if(%a,<br>%a,)"

class QTimer;
class QLabel;
class PlayListTrack;

namespace PlayListPopup {
/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class PopupWidget : public QWidget
{
    Q_OBJECT
public:
    PopupWidget(QWidget *parent = 0);

    ~PopupWidget();

    void prepare(PlayListTrack *track, QPoint pos);
    void deactivate();
    const QString url() const;

protected:
    virtual void mousePressEvent (QMouseEvent *);
    virtual void mouseMoveEvent (QMouseEvent *);

private slots:
    void loadCover();

private:
    QTimer *m_timer;
    QLabel *m_label1;
    QLabel *m_pixlabel;
    QString m_template;
    uint m_pos;
    int m_coverSize;
    QString m_url;
    MetaDataFormatter m_formatter;

};
}

#endif
