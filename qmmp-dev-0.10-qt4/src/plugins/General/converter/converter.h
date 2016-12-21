/***************************************************************************
 *   Copyright (C) 2011-2016 by Ilya Kotov                                 *
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

#ifndef CONVERTER_H
#define CONVERTER_H

#include <QRunnable>
#include <QObject>
#include <QQueue>
#include <QHash>
#include <QVariantMap>
#include <QMutex>
#include <stdio.h>
#include <qmmp/decoder.h>
#include <qmmp/inputsource.h>

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class Converter : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Converter(QObject *parent = 0);
    virtual ~Converter();

    bool prepare(const QString &url, int row, const QVariantMap &preset);

public slots:
    void stop();

signals:
    void progress(int percent);
    void finished(Converter *converter);
    void message(int row, QString message);

private:
    void run();
    bool convert(Decoder *decoder, FILE *file, bool use16bit);
    Decoder *m_decoder;
    InputSource *m_input;
    QVariantMap m_preset;
    QMutex m_mutex;
    bool m_user_stop;
    int m_row;

};

#endif // CONVERTER_H
