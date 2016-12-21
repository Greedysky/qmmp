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

#include <QSettings>
#include <QPainter>
#include <QApplication>
#include <qmmp/qmmp.h>
#include "skin.h"
#include "listwidgetdrawer.h"

// |= number=|=row1=|=row2=|=extra= duration=|

ListWidgetDrawer::ListWidgetDrawer()
{
    m_skin = Skin::instance();
    m_update = false;
    m_show_anchor = false;
    m_show_number = false;
    m_align_numbers = false;
    m_show_lengths = false;
    m_single_column = true;
    m_row_height = 0;
    m_number_width = 0;
    m_padding = 0;
    m_metrics = 0;
    m_extra_metrics = 0;
    readSettings();
    loadColors();
}

ListWidgetDrawer::~ListWidgetDrawer()
{
    if(m_metrics)
        delete m_metrics;
    if(m_extra_metrics)
        delete m_extra_metrics;
}

void ListWidgetDrawer::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Skinned");
    m_show_anchor = settings.value("pl_show_anchor", false).toBool();
    m_show_number = settings.value ("pl_show_numbers", true).toBool();
    m_show_lengths = settings.value ("pl_show_lengths", true).toBool();
    m_align_numbers = settings.value ("pl_align_numbers", false).toBool();
    m_font.fromString(settings.value ("pl_font", qApp->font().toString()).toString());
    m_extra_font = m_font;
    m_extra_font.setPointSize(m_font.pointSize() - 1);
    if (m_update)
    {
        delete m_metrics;
        delete m_extra_metrics;
    }
    m_update = true;
    m_metrics = new QFontMetrics(m_font);
    m_extra_metrics = new QFontMetrics(m_extra_font);
    m_padding = m_metrics->width("9")/2;
    m_row_height = m_metrics->lineSpacing() + 1;
}

void ListWidgetDrawer::loadColors()
{
    m_normal.setNamedColor(m_skin->getPLValue("normal"));
    m_current.setNamedColor(m_skin->getPLValue("current"));
    m_normal_bg.setNamedColor(m_skin->getPLValue("normalbg"));
    m_selected_bg.setNamedColor(m_skin->getPLValue("selectedbg"));
}

int ListWidgetDrawer::rowHeight() const
{
    return m_row_height;
}

int ListWidgetDrawer::numberWidth() const
{
    return m_number_width;
}

void ListWidgetDrawer::calculateNumberWidth(int count)
{
    //song numbers width
    if(m_show_number && m_align_numbers && count)
        m_number_width = m_metrics->width("9") * QString::number(count).size();
    else
        m_number_width = 0;
}

void ListWidgetDrawer::setSingleColumnMode(int enabled)
{
    m_single_column = enabled;
}

void ListWidgetDrawer::prepareRow(ListWidgetRow *row)
{
    if(m_number_width && m_single_column)
        row->numberColumnWidth = m_number_width + 2 * m_padding;
    else
        row->numberColumnWidth = 0;

    if(row->flags & ListWidgetRow::GROUP)
    {
        row->titles[0] = m_metrics->elidedText (row->titles[0], Qt::ElideRight,
                                            row->rect.width() - row->numberColumnWidth - 12 - 70);
        return;
    }

    if(row->titles.count() == 1)
    {
        if(m_show_number && !m_align_numbers)
            row->titles[0].prepend(QString("%1").arg(row->number)+". ");

        if((m_show_lengths && !row->length.isEmpty()) || !row->extraString.isEmpty())
            row->lengthColumnWidth = m_padding;
        else
            row->lengthColumnWidth = 0;

        if(m_show_lengths && !row->length.isEmpty())
            row->lengthColumnWidth += m_metrics->width(row->length) + m_padding;

        if(!row->extraString.isEmpty())
            row->lengthColumnWidth += m_extra_metrics->width(row->extraString) + m_padding;
    }

    //elide title
    int visible_width = row->rect.width() - row->lengthColumnWidth - row->numberColumnWidth;

    if(row->titles.count() == 1 && !row->lengthColumnWidth)
    {
        row->titles[0] = m_metrics->elidedText (row->titles[0], Qt::ElideRight, visible_width - 2 * m_padding);
        return;
    }
    else if(row->titles.count() == 1)
    {
        row->titles[0] = m_metrics->elidedText (row->titles[0], Qt::ElideRight, visible_width - m_padding);
        return;
    }

    for(int i = 0; i < row->titles.count(); ++i)
    {
        int size = row->sizes[i];
        if(i == row->trackStateColumn && !row->extraString.isEmpty())
        {
            int text_size = qMax(0, size - 3 * m_padding - m_extra_metrics->width(row->extraString));
            row->titles[i] = m_metrics->elidedText (row->titles[i], Qt::ElideRight, text_size);
            row->extraString = m_extra_metrics->elidedText(row->extraString, Qt::ElideRight,
                                                           size - 3 * m_padding - m_metrics->width(row->titles[i]));
        }
        else
        {
            row->titles[i] = m_metrics->elidedText (row->titles[i], Qt::ElideRight, size - 2 * m_padding);
        }
        visible_width -= size;
    }
}

void ListWidgetDrawer::fillBackground(QPainter *painter, int width, int height)
{
    painter->setBrush(m_normal_bg);
    painter->setPen(m_normal_bg);
    painter->drawRect(0,0,width,height);
}

void ListWidgetDrawer::drawBackground(QPainter *painter, ListWidgetRow *row)
{
    painter->setBrush((row->flags & ListWidgetRow::SELECTED) ? m_selected_bg : m_normal_bg);

    if(m_show_anchor && (row->flags & ListWidgetRow::ANCHOR))
    {
        painter->setPen(m_normal);
    }
    else if(row->flags & ListWidgetRow::SELECTED)
    {
        painter->setPen(m_selected_bg);
    }
    else
    {
        painter->setPen(m_normal_bg);
    }
    painter->drawRect(row->rect);
}

void ListWidgetDrawer::drawSeparator(QPainter *painter, ListWidgetRow *row, bool rtl)
{
    int sx = 50 + row->numberColumnWidth;  // row->rect.x() + 50;
    int sy = row->rect.y() + m_metrics->overlinePos() - 1;

    painter->setFont(m_font);
    painter->setPen(m_normal);

    if(rtl)
        sx = row->rect.right() - sx - m_metrics->width(row->titles[0]);
    else
        sx += row->rect.x();

    painter->drawText(sx, sy, row->titles[0]);

    sy -= (m_metrics->lineSpacing()/2 - 2);

    if(rtl)
    {
        painter->drawLine(row->rect.x() + 5, sy, sx - 5, sy);
        painter->drawLine(sx + m_metrics->width(row->titles[0]) + 5, sy,
                          row->rect.right() - row->numberColumnWidth - m_padding, sy);
        if(row->numberColumnWidth)
        {
            painter->drawLine(row->rect.right() - row->numberColumnWidth, row->rect.top(),
                              row->rect.right() - row->numberColumnWidth, row->rect.bottom() + 1);
        }
    }
    else
    {
        painter->drawLine(sx - 45, sy, sx - 5, sy);
        painter->drawLine(sx + m_metrics->width(row->titles[0]) + 5, sy,
                          row->rect.width(), sy);
        if(row->numberColumnWidth)
        {
            painter->drawLine(row->rect.left() + row->numberColumnWidth, row->rect.top(),
                              row->rect.left() + row->numberColumnWidth, row->rect.bottom() + 1);
        }
    }
}

void ListWidgetDrawer::drawTrack(QPainter *painter, ListWidgetRow *row, bool rtl)
{
    int sy = row->rect.y() + m_metrics->overlinePos() - 1;
    int sx = rtl ? row->rect.right() : row->rect.x();
    int title_x = 0, extra_x = 0;
    bool draw_extra = false;

    painter->setFont(m_font);

    if(rtl)
    {
        //|=duration=extra=|=  col1=|=number =|
        if(row->titles.count() == 1)
        {
            if(row->numberColumnWidth)
            {
                sx -= row->numberColumnWidth;
                painter->setPen(row->flags & ListWidgetRow::CURRENT ? m_current : m_normal);
                QString number = QString("%1").arg(row->number);
                painter->drawText(sx + m_padding, sy, number);
                painter->setPen(m_normal);
                painter->drawLine(sx, row->rect.top(), sx, row->rect.bottom() + 1);
            }

            sx -= m_metrics->width(row->titles[0]);
            painter->setPen(row->flags & ListWidgetRow::CURRENT ? m_current : m_normal);
            painter->drawText(sx - m_padding, sy, row->titles[0]);

            sx = row->rect.x() + m_padding;

            if(m_show_lengths && !row->length.isEmpty())
            {
                painter->drawText(sx, sy, row->length);
                sx += m_metrics->width(row->length);
                sx += m_padding;
            }

            if(!row->extraString.isEmpty())
            {
                painter->setFont(m_extra_font);
                painter->drawText(sx, sy, row->extraString);
            }
        }
        else //|=extra col1=|=  col2=|
        {
            for(int i = 0; i < row->sizes.count(); i++)
            {
                painter->setPen(row->flags & ListWidgetRow::CURRENT ? m_current : m_normal);
                draw_extra = (i == row->trackStateColumn && !row->extraString.isEmpty());

                if(row->alignment[i] == ListWidgetRow::ALIGN_LEFT)
                {
                    title_x = sx - row->sizes[i] + m_padding;
                    extra_x = draw_extra ? sx - m_padding - m_extra_metrics->width(row->extraString) : 0;
                }
                else if(row->alignment[i] == ListWidgetRow::ALIGN_RIGHT)
                {
                    title_x = sx - m_padding - m_metrics->width(row->titles[i]);
                    extra_x = draw_extra ? sx - row->sizes[i] + m_padding : 0;
                }
                else
                {
                    title_x = sx - row->sizes[i] / 2 - m_metrics->width(row->titles[i]) / 2 +
                            (draw_extra ? (m_extra_metrics->width(row->extraString) + m_padding) / 2 : 0);
                    extra_x = draw_extra ? title_x - m_metrics->width(row->extraString) - m_padding : 0;
                }

                painter->drawText(title_x, sy, row->titles[i]);

                if(draw_extra)
                {
                    painter->setFont(m_extra_font);
                    painter->drawText(extra_x, sy, row->extraString);
                    painter->setFont(m_font);
                }

                sx -= row->sizes[i];

                painter->setPen(m_normal);
                if(!row->autoResize || i < row->sizes.count() - 1) //do not draw last vertical line
                {
                    painter->drawLine(sx, row->rect.top(), sx, row->rect.bottom() + 1);
                }
            }
        }
    }
    else
    {
        //|= number=|=col  =|=extra=duration=|
        if(row->titles.count() == 1)
        {
            if(row->numberColumnWidth)
            {
                sx += row->numberColumnWidth;
                painter->setPen(row->flags & ListWidgetRow::CURRENT ? m_current : m_normal);
                QString number = QString("%1").arg(row->number);
                painter->drawText(sx - m_padding - m_metrics->width(number), sy, number);
                painter->setPen(m_normal);
                painter->drawLine(sx, row->rect.top(), sx, row->rect.bottom() + 1);
            }

            painter->setPen(row->flags & ListWidgetRow::CURRENT ? m_current : m_normal);
            painter->drawText(sx + m_padding, sy, row->titles[0]);

            sx = row->rect.right() - m_padding;

            if(m_show_lengths && !row->length.isEmpty())
            {
                sx -= m_metrics->width(row->length);
                painter->drawText(sx, sy, row->length);
                sx -= m_padding;
            }

            if(!row->extraString.isEmpty())
            {
                sx -= m_extra_metrics->width(row->extraString);
                painter->setFont(m_extra_font);
                painter->drawText(sx, sy, row->extraString);
            }
        }
        else //|=col1  extra=|=col2  =|
        {
            for(int i = 0; i < row->sizes.count(); i++)
            {
                painter->setPen(row->flags & ListWidgetRow::CURRENT ? m_current : m_normal);
                draw_extra = (i == row->trackStateColumn && !row->extraString.isEmpty());

                if(row->alignment[i] == ListWidgetRow::ALIGN_LEFT)
                {
                    title_x = sx + m_padding;
                    extra_x = draw_extra ? sx + row->sizes[i] - m_padding - m_extra_metrics->width(row->extraString) : 0;
                }
                else if(row->alignment[i] == ListWidgetRow::ALIGN_RIGHT)
                {
                    title_x = sx + row->sizes[i] - m_padding - m_metrics->width(row->titles[i]);
                    extra_x = draw_extra ? sx + m_padding : 0;
                }
                else
                {
                    title_x = sx + row->sizes[i] / 2 - m_metrics->width(row->titles[i]) / 2 -
                            (draw_extra ? (m_extra_metrics->width(row->extraString) + m_padding) / 2 : 0);
                    extra_x = draw_extra ? title_x + m_metrics->width(row->titles[i]) + m_padding : 0;
                }

                painter->drawText(title_x, sy, row->titles[i]);

                if(draw_extra)
                {
                    painter->setFont(m_extra_font);
                    painter->drawText(extra_x, sy, row->extraString);
                    painter->setFont(m_font);
                }

                sx += row->sizes[i];

                painter->setPen(m_normal);
                if(!row->autoResize || i < row->sizes.count() - 1) //do not draw last vertical line
                {
                    painter->drawLine(sx, row->rect.top(), sx, row->rect.bottom() + 1);
                }
            }
        }
    }
}

void ListWidgetDrawer::drawDropLine(QPainter *painter, int row_number, int width, int header_height)
{
    painter->setPen(m_current);
    painter->drawLine (5, header_height + row_number * m_row_height,
                       width - 5 , header_height + row_number * m_row_height);
}
