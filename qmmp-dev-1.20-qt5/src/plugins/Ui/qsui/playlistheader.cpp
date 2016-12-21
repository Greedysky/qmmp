/***************************************************************************
 *   Copyright (C) 2015-2016 by Ilya Kotov                                 *
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

#include <QPainter>
#include <QBitmap>
#include <QFont>
#include <QFontMetrics>
#include <QSettings>
#include <QApplication>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QLineEdit>
#include <QInputDialog>
#include <QIcon>
#include <QStyleOptionHeader>
#include <QActionGroup>
#include <qmmp/qmmp.h>
#include <qmmpui/playlistmanager.h>
#include <qmmpui/playlistheadermodel.h>
#include <qmmpui/playlistmanager.h>
#include "playlistheader.h"
#include "listwidgetdrawer.h"

#define INITAL_SIZE 150
#define MIN_SIZE 30
#define MAX_COLUMNS 7

PlayListHeader::PlayListHeader(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);

    m_pl_padding = 0;
    m_number_width = 0;
    m_offset = 0;
    m_sorting_column = -1;
    m_scrollbar_width = 0;
    m_reverted = false;
    m_auto_resize = false;
    m_metrics = 0;
    m_task = NO_TASK;

    m_model = PlayListManager::instance()->headerModel();

    //menus
    m_menu = new QMenu(this);
    m_menu->addAction(QIcon::fromTheme("list-add"), tr("Add Column"), this, SLOT(addColumn()));
    m_menu->addAction(QIcon::fromTheme("configure"), tr("Edit Column"), this, SLOT(editColumn()));
    m_trackStateAction = m_menu->addAction(tr("Show Queue/Protocol"), this, SLOT(showTrackState(bool)));
    m_trackStateAction->setCheckable(true);
    m_autoResizeAction = m_menu->addAction(tr("Auto-resize"), this, SLOT(setAutoResize(bool)));
    m_autoResizeAction->setCheckable(true);

    m_alignmentMenu = m_menu->addMenu(tr("Alignment"));
    m_alignmentMenu->addAction(tr("Left", "alignment"))->setData(ListWidgetRow::ALIGN_LEFT);
    m_alignmentMenu->addAction(tr("Right", "alignment"))->setData(ListWidgetRow::ALIGN_RIGHT);
    m_alignmentMenu->addAction(tr("Center", "alignment"))->setData(ListWidgetRow::ALIGN_CENTER);
    connect(m_alignmentMenu, SIGNAL(triggered(QAction*)), SLOT(setAlignment(QAction*)));
    QActionGroup *alignmentGroup = new QActionGroup(this);
    foreach (QAction *a, m_alignmentMenu->actions())
    {
        a->setCheckable(true);
        alignmentGroup->addAction(a);
    }

    m_menu->addSeparator();
    m_menu->addAction(QIcon::fromTheme("list-remove"), tr("Remove Column"), this, SLOT(removeColumn()));

    readSettings();

    connect(m_model, SIGNAL(columnAdded(int)), SLOT(onColumnAdded(int)));
    connect(m_model, SIGNAL(columnRemoved(int)), SLOT(onColumnRemoved()));
    connect(m_model, SIGNAL(columnMoved(int,int)), SLOT(updateColumns()));
    connect(m_model, SIGNAL(columnChanged(int)), SLOT(updateColumns()));
}

PlayListHeader::~PlayListHeader()
{
    if (m_metrics)
        delete m_metrics;
    m_metrics = 0;
}

void PlayListHeader::readSettings()
{
    if (m_metrics)
    {
        delete m_metrics;
        m_metrics = 0;
    }

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Simple");

    QFont header_font = qApp->font("QAbstractItemView");
    if(!settings.value("use_system_fonts", true).toBool())
    {
        header_font.fromString(settings.value("pl_header_font", header_font.toString()).toString());
    }
    m_metrics = new QFontMetrics(header_font);
    setFont(header_font);

    QStyleOptionHeader opt;
    opt.initFrom(this);
    m_size_hint = style()->sizeFromContents(QStyle::CT_HeaderSection, &opt, QSize(), this);

    QFont pl_font;
    pl_font.fromString(settings.value("pl_font", qApp->font().toString()).toString());
    m_pl_padding = QFontMetrics(pl_font).width("9")/2;

    if(!m_model->isSettingsLoaded()) //do not load settings several times
    {
        m_model->restoreSettings(&settings);
        QList<QVariant> sizes = settings.value("pl_column_sizes").toList();
        QList<QVariant> alignment = settings.value("pl_column_alignment").toList();
        int autoResizeColumn = settings.value("pl_autoresize_column", -1).toInt();
        int trackStateColumn = settings.value("pl_track_state_column", -1).toInt();
        for(int i = 0; i < m_model->count(); ++i)
        {
            m_model->setData(i, SIZE, INITAL_SIZE);
            m_model->setData(i, ALIGNMENT, (layoutDirection() == Qt::RightToLeft) ?
                                 ListWidgetRow::ALIGN_LEFT : ListWidgetRow::ALIGN_LEFT);

            if(i < sizes.count())
                m_model->setData(i, SIZE, sizes.at(i).toInt());
            if(i < alignment.count())
                m_model->setData(i, ALIGNMENT, alignment.at(i).toInt());
            if(i == autoResizeColumn)
            {
                m_model->setData(i, AUTO_RESIZE, true);
                m_auto_resize = true;
            }
            if(i == trackStateColumn)
                m_model->setData(i,TRACK_STATE, true);
        }
    }
    else
    {
        m_auto_resize = autoResizeColumn() >= 0;
    }

    if(isVisible())
        updateColumns();

    settings.endGroup();
}

void PlayListHeader::setNumberWidth(int width)
{
    if(width != m_number_width)
    {
        m_number_width = width;
        if(m_model->count() == 1)
            updateColumns();
    }
}

void PlayListHeader::setScrollBarWidth(int width)
{
    if(!isVisible())
        return;

    if(m_scrollbar_width != width)
    {
        m_scrollbar_width = width;

        if(m_model->count() == 1)
        {
            updateColumns();
            return;
        }

        int index = autoResizeColumn();
        if(index >= 0)
        {
            adjustColumn(index);
            updateColumns();
            return;
        }
    }
}

void PlayListHeader::updateColumns()
{
    if(!isVisible())
        return;

    bool rtl = (layoutDirection() == Qt::RightToLeft);

    int sx = 5;

    if(m_model->count() == 1)
    {
        if(m_number_width)
            sx += m_number_width + 2 * m_pl_padding;
        m_model->setData(0, RECT, rtl ? QRect(5 + m_scrollbar_width, 0, width() - sx - 5 - m_scrollbar_width, height()) :
                                        QRect(sx, 0, width() - sx - 5 - m_scrollbar_width, height()));
        m_model->setData(0, NAME, m_model->name(0));
        update();
        return;
    }

    for(int i = 0; i < m_model->count(); ++i)
    {
        int size = m_model->data(i, SIZE).toInt();

        if(rtl)
            m_model->setData(i, RECT, QRect(width() - sx - size, 0, size, height()));
        else
            m_model->setData(i, RECT, QRect(sx, 0, size, height()));
        m_model->setData(i, NAME, m_model->name(i));
        sx += size;
    }
    update();
}

int PlayListHeader::requiredHeight() const
{
    return m_size_hint.height();
}

QList<int> PlayListHeader::sizes() const
{
    QList<int> sizeList;
    for(int i = 0; i < m_model->count(); ++i)
        sizeList.append(m_model->data(i, SIZE).toInt());
    return sizeList;
}

QList<int> PlayListHeader::alignment() const
{
    QList<int> alignmentList;
    for(int i = 0; i < m_model->count(); ++i)
        alignmentList.append(m_model->data(i, ALIGNMENT).toInt());
    return alignmentList;
}

int PlayListHeader::trackStateColumn() const
{
    for(int i = 0; i < m_model->count(); ++i)
    {
        if(m_model->data(i, TRACK_STATE).toBool())
        {
            return i;
        }
    }
    return -1;
}

int PlayListHeader::maxScrollValue() const
{
    if(m_model->count() == 1)
        return 0;

    int row_width = 0;
    foreach (int size, sizes())
    {
        row_width += size;
    }
    return qMax(0, row_width - width() + m_scrollbar_width + 10);
}

int PlayListHeader::offset() const
{
    return m_offset;
}

bool PlayListHeader::hasAutoResizeColumn() const
{
    return m_auto_resize;
}

void PlayListHeader::scroll(int offset)
{
    m_offset = offset;
    update();
}

void PlayListHeader::showSortIndicator(int column, bool reverted)
{
    if(m_sorting_column == column && m_reverted == reverted)
        return;

    m_sorting_column = column;
    m_reverted = reverted;
    updateColumns();
}

void PlayListHeader::hideSortIndicator()
{
    if(m_sorting_column != -1)
    {
        m_sorting_column = -1;
        updateColumns();
    }
}

void PlayListHeader::addColumn()
{
    int column = findColumn(m_pressed_pos);

    if(column < 0)
    {
        QRect firstRect = m_model->data(0, RECT).toRect();
        QRect lastRect = m_model->data(m_model->count() - 1, RECT).toRect();

        if(m_pressed_pos.x() > lastRect.right())
            column = m_model->count();
        else if(m_pressed_pos.x() < firstRect.x())
            column = 0;
    }

    if(column < 0)
        return;

    m_model->execInsert(column);
}

void PlayListHeader::editColumn()
{
    if(m_pressed_column < 0)
         return;

    m_model->execEdit(m_pressed_column);
}

void PlayListHeader::removeColumn()
{
    if(m_pressed_column < 0)
         return;

    m_model->remove(m_pressed_column);
}

void PlayListHeader::setAutoResize(bool on)
{
    if(m_pressed_column < 0)
        return;

    m_auto_resize = on;

    if(on)
    {
        for(int i = 0; i < m_model->count(); ++i)
            m_model->setData(i, AUTO_RESIZE, false);
    }

    m_model->setData(m_pressed_column, AUTO_RESIZE, on);

    if(on)
    {
        m_offset = 0;
        adjustColumn(m_pressed_column);
        updateColumns();
    }
    PlayListManager::instance()->selectedPlayList()->updateMetaData();
}

void PlayListHeader::showTrackState(bool yes)
{
    if(m_pressed_column < 0)
        return;

    if(yes)
    {
        for(int i = 0; i < m_model->count(); ++i)
            m_model->setData(i, TRACK_STATE, false);
    }

    m_model->setData(m_pressed_column, TRACK_STATE, yes);
    PlayListManager::instance()->selectedPlayList()->updateMetaData();
}

void PlayListHeader::setAlignment(QAction *action)
{
    if(m_pressed_column < 0)
         return;

    m_model->setData(m_pressed_column, ALIGNMENT, action->data().toInt());
    PlayListManager::instance()->selectedPlayList()->updateMetaData();
}

void PlayListHeader::onColumnAdded(int index)
{
    m_model->setData(index, SIZE, INITAL_SIZE);
    m_model->setData(index, ALIGNMENT, (layoutDirection() == Qt::RightToLeft) ?
                         ListWidgetRow::ALIGN_RIGHT : ListWidgetRow::ALIGN_LEFT);

    if(m_auto_resize && isVisible())
    {
        adjustColumn(autoResizeColumn());
    }
    updateColumns();
}

void PlayListHeader::onColumnRemoved()
{
    m_auto_resize = autoResizeColumn() >= 0;
    if(m_auto_resize && isVisible())
    {
        adjustColumn(autoResizeColumn());
    }
    updateColumns();
}

void PlayListHeader::mousePressEvent(QMouseEvent *e)
{
    bool rtl = layoutDirection() == Qt::RightToLeft;

    if(e->button() == Qt::LeftButton)
    {
        m_pressed_column = findColumn(e->pos());
        if(m_pressed_column >= 0)
        {
            m_pressed_pos = e->pos();
            m_mouse_pos = e->pos();
            m_pressed_pos.rx() += m_offset;
            m_mouse_pos.rx() += m_offset;

            if(rtl)
            {
                if(e->pos().x() < m_model->data(m_pressed_column, RECT).toRect().x() + m_metrics->width("9"))
                {
                    m_old_size = size(m_pressed_column);
                    m_task = RESIZE;
                }
                else
                {
                    m_press_offset = e->pos().x() - m_model->data(m_pressed_column, RECT).toRect().x();
                    m_task = SORT;
                }
            }
            else
            {
                if(e->pos().x() > m_model->data(m_pressed_column, RECT).toRect().right() - m_metrics->width("9"))
                {
                    m_old_size = size(m_pressed_column);
                    m_task = RESIZE;
                }
                else
                {
                    m_press_offset = e->pos().x() - m_model->data(m_pressed_column, RECT).toRect().x();
                    m_task = SORT;
                }
            }
        }
        else
        {
            m_task = NO_TASK;
            update();
        }
    }
}

void PlayListHeader::mouseReleaseEvent(QMouseEvent *)
{
    if(m_task == SORT)
        PlayListManager::instance()->selectedPlayList()->sortByColumn(m_pressed_column);

    m_task = NO_TASK;
    update();
}

void PlayListHeader::mouseMoveEvent(QMouseEvent *e)
{
    bool rtl = layoutDirection() == Qt::RightToLeft;

    if(m_task == SORT)
    {
        m_task = MOVE;
    }

    int x = e->pos().x() + m_offset;

    if(m_task == RESIZE && m_model->count() > 1)
    {
        int index = autoResizeColumn();

        if(index == -1 || m_pressed_column < m_model->count() - 1)
        {
            if(rtl)
                setSize(m_pressed_column, m_old_size - x + m_pressed_pos.x());
            else
                setSize(m_pressed_column, m_old_size + x - m_pressed_pos.x());
            setSize(m_pressed_column, qMax(size(m_pressed_column), MIN_SIZE));
        }

        if(m_pressed_column < index)
        {
            adjustColumn(index);
        }
        else if(index != -1 && m_pressed_column < m_model->count() - 1)
        {
            adjustColumn(m_pressed_column + 1);
        }
        m_offset = qMin(m_offset, maxScrollValue());
        updateColumns();
        PlayListManager::instance()->selectedPlayList()->updateMetaData();
    }
    else if(m_task == MOVE)
    {
        m_mouse_pos = e->pos();
        m_mouse_pos.rx() += m_offset;

        int dest = -1;
        for(int i = 0; i < m_model->count(); ++i)
        {
            QRect rect = m_model->data(i, RECT).toRect();
            int x_delta = m_mouse_pos.x() - rect.x();
            if(x_delta < 0 || x_delta > rect.width())
                continue;

            if(rtl)
            {
                if((x_delta > rect.width()/2 && m_pressed_column > i) ||
                        (x_delta < rect.width()/2 && m_pressed_column < i))
                {
                    dest = i;
                    break;
                }
            }
            else
            {
                if((x_delta > rect.width()/2 && m_pressed_column < i) ||
                        (x_delta < rect.width()/2 && m_pressed_column > i))
                {
                    dest = i;
                    break;
                }
            }
        }
        if(dest == -1 || dest == m_pressed_column)
        {
            update();
            QWidget::mouseMoveEvent(e);
            return;
        }
        m_model->move(m_pressed_column, dest);
        m_pressed_column = dest;
        update();
    }
    else if(e->button() == Qt::NoButton && m_model->count() > 1)
    {
        int column = findColumn(e->pos());

        if(!m_auto_resize || column < m_model->count() - 1)
        {
            if(rtl)
            {
                if(column >= 0 && x < m_model->data(column, RECT).toRect().x() + m_metrics->width("9"))
                    setCursor(Qt::SplitHCursor);
                else
                    setCursor(Qt::ArrowCursor);
            }
            else
            {
                if(column >= 0 && x > m_model->data(column, RECT).toRect().right() - m_metrics->width("9"))
                    setCursor(Qt::SplitHCursor);
                else
                    setCursor(Qt::ArrowCursor);
            }
        }
        else
        {
            setCursor(Qt::ArrowCursor);
        }
    }
}

void PlayListHeader::resizeEvent(QResizeEvent *e)
{
    if(m_model->count() == 1)
    {
        updateColumns();
        return;
    }

    if(!isVisible())
        return;

    if(m_auto_resize)
    {
        adjustColumn(autoResizeColumn());
        m_offset = qMin(m_offset, maxScrollValue());
        updateColumns();
        return;
    }

    if(m_offset > maxScrollValue())
    {
        m_offset = maxScrollValue();
        updateColumns();
        return;
    }


    if(layoutDirection() == Qt::RightToLeft || e->oldSize().height() != e->size().height())
    {
        updateColumns();
        return;
    }
}

void PlayListHeader::contextMenuEvent(QContextMenuEvent *e)
{
    m_pressed_pos = e->pos();
    m_pressed_column = findColumn(e->pos());
    if(m_pressed_column >= 0)
    {
        m_trackStateAction->setChecked(m_model->data(m_pressed_column, TRACK_STATE).toBool());
        m_autoResizeAction->setChecked(m_model->data(m_pressed_column, AUTO_RESIZE).toBool());

        int alignment = m_model->data(m_pressed_column, ALIGNMENT).toInt();
        foreach (QAction *action, m_alignmentMenu->actions())
        {
            if(action->data().toInt() == alignment)
            {
                action->setChecked(true);
                break;
            }
        }

        //hide unused actions
        foreach (QAction *action, m_menu->actions())
        {
            if(m_menu->actions().at(0) == action)
                action->setVisible(m_model->count() < MAX_COLUMNS);
            else if(m_menu->actions().at(1) == action)
                action->setVisible(true);
            else
                action->setVisible(m_model->count() != 1); //multicolumn mode
        }

    }
    else
    {
        foreach (QAction *action, m_menu->actions())
        {
            if(action == m_menu->actions().first())
                action->setVisible(m_model->count() < MAX_COLUMNS);
            else
                action->setVisible(false);
        }
    }
    m_menu->exec(e->globalPos());
}

void PlayListHeader::paintEvent(QPaintEvent *)
{
    bool rtl = (layoutDirection() == Qt::RightToLeft);

    QPainter painter(this);
    painter.translate(rtl ? m_offset : -m_offset, 0);

    {
        QStyleOption opt;
        opt.initFrom(this);
        opt.state |= QStyle::State_Horizontal;
        opt.rect = QRect(0,0,m_model->data(0, RECT).toRect().x(), height());
        style()->drawControl(QStyle::CE_HeaderEmptyArea, &opt, &painter, this);
        opt.rect = QRect(m_model->data(m_model->count() - 1, RECT).toRect().right(), 0,
                         width() - m_model->data(m_model->count() - 1, RECT).toRect().right(), height());
        style()->drawControl(QStyle::CE_HeaderEmptyArea, &opt, &painter, this);
    }

    for(int i = 0; i < m_model->count(); ++i)
    {
        QStyleOptionHeader opt;
        initStyleOption(&opt);
        opt.rect = m_model->data(i, RECT).toRect();
        opt.text = name(i);
        opt.section = i;
        opt.state |= QStyle::State_Active;
        if(i == 0)
            opt.position = QStyleOptionHeader::Beginning;
        else if(i < m_model->count() - 1)
            opt.position = QStyleOptionHeader::Middle;
        else if(i == m_model->count() - 1)
            opt.position = QStyleOptionHeader::End;

        if(i == m_sorting_column)
            opt.sortIndicator = m_reverted ? QStyleOptionHeader::SortUp : QStyleOptionHeader::SortDown;

        style()->drawControl(QStyle::CE_Header, &opt, &painter, this);
    }

    if(m_model->count() == 1)
        return;

    if(m_task == MOVE)
    {
        QStyleOptionHeader opt;
        initStyleOption(&opt);
        opt.rect = m_model->data(m_pressed_column, RECT).toRect();
        opt.text = name(m_pressed_column);
        opt.section = m_pressed_column;
        painter.setOpacity(0.75);
        opt.rect.moveTo(m_mouse_pos.x() - m_press_offset, opt.rect.y());
        style()->drawControl(QStyle::CE_Header, &opt, &painter, this);
    }
}

int PlayListHeader::findColumn(QPoint pos)
{
    for(int i = 0; i < m_model->count(); ++i)
    {
        if(m_model->data(i,RECT).toRect().contains(pos))
            return i;
    }
    return -1;
}

int PlayListHeader::size(int index) const
{
    return m_model->data(index, SIZE).toInt();
}

void PlayListHeader::setSize(int index, int size)
{
    m_model->setData(index, SIZE, size);
}

const QString PlayListHeader::name(int index) const
{
    return m_model->data(index, NAME).toString();
}

void PlayListHeader::adjustColumn(int index)
{
    int w = 0;

    for(int i = 0; i < m_model->count(); ++i)
    {
        if(i != index)
            w += size(i);
    }

    setSize(index, qMax(width() - 10 - m_scrollbar_width - w, MIN_SIZE));
}

int PlayListHeader::autoResizeColumn() const
{
    int index = -1;
    for(int i = 0; i < m_model->count(); ++i)
    {
        if(m_model->data(i, AUTO_RESIZE).toBool())
        {
            index = i;
            break;
        }
    }
    return index;
}

void PlayListHeader::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Simple");
    m_model->saveSettings(&settings);
    QList<QVariant> sizes, alignment;
    int autoResizeColumn = -1;
    int trackStateColumn = -1;
    for(int i = 0; i < m_model->count(); ++i)
    {
        sizes << m_model->data(i, SIZE).toInt();
        alignment << m_model->data(i, ALIGNMENT).toInt();
        if(m_model->data(i, AUTO_RESIZE).toBool())
            autoResizeColumn = i;
        if(m_model->data(i, TRACK_STATE).toBool())
            trackStateColumn = i;
    }
    settings.setValue("pl_column_sizes", sizes);
    settings.setValue("pl_column_alignment", alignment);
    settings.setValue("pl_autoresize_column", autoResizeColumn);
    settings.setValue("pl_track_state_column", trackStateColumn);
    settings.endGroup();
}

void PlayListHeader::showEvent(QShowEvent *)
{
    if(m_auto_resize)
    {
        adjustColumn(autoResizeColumn());
        m_offset = qMin(m_offset, maxScrollValue());
        updateColumns();
        PlayListManager::instance()->selectedPlayList()->updateMetaData();
    }
    else
        updateColumns();
}

void PlayListHeader::hideEvent(QHideEvent *)
{
    writeSettings();
}

void PlayListHeader::initStyleOption(QStyleOptionHeader *opt)
{
    opt->initFrom(this);
    opt->state = QStyle::State_None | QStyle::State_Raised | QStyle::State_Horizontal | QStyle::State_Enabled;
    opt->orientation = Qt::Horizontal;
    opt->iconAlignment = Qt::AlignVCenter;
    opt->textAlignment = Qt::AlignLeft | Qt::AlignVCenter;
}
