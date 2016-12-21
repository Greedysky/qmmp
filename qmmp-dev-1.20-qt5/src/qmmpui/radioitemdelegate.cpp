/***************************************************************************
 *   Copyright (C) 2012-2016 by Ilya Kotov                                 *
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

#include <QApplication>
#include <QMouseEvent>
#include "radioitemdelegate_p.h"

RadioItemDelegate::RadioItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

void RadioItemDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    if (hasRadioButton(index))
    {
        QStyleOptionViewItem opt = option;

        initStyleOption(&opt, index);

        QStyleOptionButton buttonOption;
        buttonOption.rect = option.rect;
        buttonOption.text = index.data(Qt::DisplayRole).toString();
        buttonOption.state |= QStyle::State_Enabled;

        if (index.data(Qt::CheckStateRole) == Qt::Checked)
        {
            buttonOption.state |= QStyle::State_On;
            buttonOption.state &=~ QStyle::State_Off;
        }
        else
        {
            buttonOption.state |= QStyle::State_Off;
            buttonOption.state &=~ QStyle::State_On;
        }

        buttonOption.palette = opt.palette;

        if(opt.state.testFlag(QStyle::State_Selected))
        {
            buttonOption.palette.setBrush(QPalette::WindowText,
                                          opt.palette.brush(QPalette::HighlightedText));
            qApp->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);
        }
        qApp->style()->drawControl(QStyle::CE_RadioButton, &buttonOption, painter);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize RadioItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    if (hasRadioButton(index))
    {
        int buttonHeight = qApp->style()->pixelMetric(QStyle::PM_ExclusiveIndicatorHeight, &option);
        size.setHeight(qMax(size.height(), buttonHeight));
    }
    return size;
}

bool RadioItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                    const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::KeyPress)
    {
        if (hasRadioButton(index))
        {
            if(event->type() == QEvent::MouseButtonRelease)
            {
                QRect checkRect = qApp->style()->subElementRect(QStyle::SE_RadioButtonIndicator, &option);

                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

                if(!checkRect.contains(mouseEvent->pos()))
                    return true;
            }

            if (!index.data(Qt::CheckStateRole).toBool())
            {
                model->setData(index, Qt::Checked, Qt::CheckStateRole);
                QModelIndex parentItem = index.parent();
                for (int i = 0; i < model->rowCount(parentItem); ++i)
                {
                    QModelIndex childIndex = parentItem.child(i,0);
                    if (childIndex != index)
                        model->setData(childIndex, Qt::Unchecked, Qt::CheckStateRole);
                }
            }
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

bool RadioItemDelegate::hasRadioButton(const QModelIndex &index) const
{
    return index.flags().testFlag(Qt::ItemIsUserCheckable) && index.data(RadioButtonRole).toBool();
}
