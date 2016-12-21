/***************************************************************************
 *   Copyright (C) 2006-2016 by Ilya Kotov                                 *
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
#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QDir>
#include <QContextMenuEvent>
#include <QPen>
#include "listwidgetdrawer.h"

class QFont;
class QFontMetrics;
class QMenu;
class QAction;
class QTimer;
class QScrollBar;
class PlayListModel;
class PlayListItem;
class QmmpUiSettings;
class PlayListHeader;
namespace PlayListPopup{
class PopupWidget;
}

/**
   @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class ListWidget : public QWidget
{
    Q_OBJECT
public:
    ListWidget(PlayListModel *model, QWidget *parent = 0);

    ~ListWidget();

    /*!
     * Returns count of currently visible rows.
     */
    int visibleRows() const;
    /*!
     * Returns index of the first visible item.
     */
    int firstVisibleIndex() const;

    int anchorIndex() const;
    void setAnchorIndex(int index);
    QMenu *menu();
    void setMenu(QMenu *menu);
    PlayListModel *model();

public slots:
    void readSettings();
    void updateList(int flags);
    void scroll(int); //0-99
    void recenterCurrent();

signals:
    void selectionChanged();
    void positionChanged(int, int); //current position, maximum value

protected:
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void wheelEvent(QWheelEvent *);
    void showEvent(QShowEvent *);
    int indexAt(int)const;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragMoveEvent(QDragMoveEvent *event);
    void contextMenuEvent (QContextMenuEvent * event);
    bool event (QEvent *e);

private slots:
    void updateSkin();
    void autoscroll();
    void updateRepeatIndicator();
    void scrollToCurrent();

private:
    enum ScrollDirection
    {
        NONE = 0,TOP,DOWN
    };
    /*!
     * Returns string with queue number or(and) repeate flag for the item number \b i.
     */
    const QString getExtraString(int i);

    bool updateRowCount();
    void updateScrollBars();

    bool m_update;
    int m_pressed_index;
    QMenu *m_menu;
    PlayListModel *m_model;
    int m_row_count, m_first;
    int m_anchor_index;
    /*!
     * Scroll direction that is preforming in current moment.
     */
    ScrollDirection m_scroll_direction;
    int m_prev_y;
    bool m_select_on_release;
    bool m_show_protocol;
    int m_drop_index;
    QList<ListWidgetRow *> m_rows;
    QmmpUiSettings *m_ui_settings;
    PlayListPopup::PopupWidget *m_popupWidget;
    QTimer *m_timer;
    QScrollBar *m_scrollBar;
    ListWidgetDrawer m_drawer;
    PlayListHeader *m_header;
    QScrollBar *m_hslider;
};

#endif
