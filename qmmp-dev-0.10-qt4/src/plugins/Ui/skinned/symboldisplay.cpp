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
#include <QPainter>
#include <math.h>
#include "skin.h"
#include "symboldisplay.h"

SymbolDisplay::SymbolDisplay (QWidget *parent, int digits)
        : PixmapWidget (parent)
{
    m_alignment = Qt::AlignRight;
    m_skin = Skin::instance();
    m_digits  = digits;
    m_max = 0;
    connect (m_skin, SIGNAL (skinChanged()), this, SLOT (draw()));
    draw();
    for (int i=0; i<m_digits; ++i)
#if defined(Q_OS_FREEBSD) || defined(Q_OS_WIN) || defined (Q_OS_MAC)
        m_max += 9 * (int) pow(10,i);
#else
        m_max += 9 * (int) exp10(i);
#endif
}

SymbolDisplay::~SymbolDisplay()
{}

void SymbolDisplay::setAlignment(Qt::Alignment a)
{
    m_alignment = a;
}

Qt::Alignment SymbolDisplay::alignment() const
{
    return m_alignment;
}

void SymbolDisplay::display (const QString& str)
{
    m_text = str;
    if (!str.isEmpty())
        draw();
}

void SymbolDisplay::draw()
{
    QString str = m_text;
    QPixmap bg = m_skin->getLetter (' ');
    int w = bg.size().width();
    int h = bg.size().height();
    QPixmap tmp (m_digits*w,h);
    QPainter paint (&tmp);
    int j;
    for (int i = 0; i < m_digits; ++i)
    {
        if (m_alignment == Qt::AlignRight) // TODO: add align Center
        {
            j = str.size() -1 - i;
            if (j >= 0)
                paint.drawPixmap ((m_digits-1-i) *w,0,m_skin->getLetter (str.at (j)));
            else
                paint.drawPixmap ((m_digits-1-i) *w,0,m_skin->getLetter (' '));
        }
        else
        {
            if (i < str.size())
                paint.drawPixmap (i * w,0,m_skin->getLetter (str.at (i)));
            else
                paint.drawPixmap (i * w,0,m_skin->getLetter (' '));
            ;
        }
    }
    setPixmap(tmp);
}

void SymbolDisplay::display(int val)
{
    if (val < m_max)
        display(QString::number(val));
    else
        display(QString("%1h").arg(val/100));
}
