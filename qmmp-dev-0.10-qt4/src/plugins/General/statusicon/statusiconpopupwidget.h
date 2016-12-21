/***************************************************************************
 *   Copyright (C) 2009 by Artur Guzik                                     *
 *   a.guzik88@gmail.com                                                   *
 *                                                                         *
 *   Copyright (C) 2009-2012 by Ilya Kotov                                 *
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

#ifndef STATUSICONPOPUPWIDGET_H
#define STATUSICONPOPUPWIDGET_H

#define DEFAULT_TEMPLATE "<b>%if(%t,%t,%f)</b>\n%if(%p,<br>%p,)\n%if(%a,<br>%a,)\n%if(%l,<br><b>%l</b>,)"

#include <QFrame>
#include <QWidget>
#include <QProgressBar>

class QLabel;
class QTimer;
class QHBoxLayout;
class QVBoxLayout;
class QSpacerItem;
class TimeBar;
class CoverWidget;

class StatusIconPopupWidget : public QFrame
{
    Q_OBJECT
public:

    StatusIconPopupWidget(QWidget * parent = 0);
    ~StatusIconPopupWidget();

    void showInfo(int x, int y); //x,y are tray icon position

protected:
    virtual void mousePressEvent(QMouseEvent *);

private slots:
    void updatePosition(int trayx, int trayy);
    void updateMetaData();
    void updateTime(qint64 elapsed);

private:
    QLabel *m_textLabel;
    QHBoxLayout *m_hLayout;
    QVBoxLayout *m_vLayout;
    QTimer *m_timer;
    CoverWidget * m_cover;
    QString m_totalTime;
    QSpacerItem *m_spacer;
    QProgressBar *m_bar;
    int m_lastTrayX;
    int m_lastTrayY;
    int m_splitFileName;
    QString m_template;
    bool m_showProgress;
};

class TimeBar : public QProgressBar
{
    Q_OBJECT
public:
    TimeBar(QWidget *parent = 0);
    virtual QString text() const;
};


#endif // STATUSICONPOPUPWIDGET_H
