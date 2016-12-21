/***************************************************************************
 *   Copyright (C) 2011-2015 by Ilya Kotov                                 *
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

#ifndef LOGO_H
#define LOGO_H

#include <QWidget>
#include <QHash>
#include <QChar>
#include <QStringList>
#include <QQueue>
#include <QPixmap>
#include <qmmp/visual.h>

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class Logo : public Visual
{
    Q_OBJECT
public:
    explicit Logo(QWidget *parent = 0);
    virtual ~Logo();

    void add(float *data, size_t samples, int chan);
    void clear();

private slots:
    void updateLetters();

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void processPreset1();
    void processPreset2();
    void processPreset3();
    void processPreset4();
    QHash <QChar, QPixmap> m_letters;
    QStringList m_lines;
    QStringList m_source_lines;
    float *m_buffer;
    int m_buffer_at;
    int m_value;
    qint64 m_elapsed;

};

#endif // LOGO_H
