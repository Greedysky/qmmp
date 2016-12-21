/***************************************************************************
 *   Copyright (C) 2009-2013 by Ilya Kotov                                 *
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

#include "httpstreamreader.h"
#include "httpinputsource.h"

HTTPInputSource::HTTPInputSource(const QString &url, QObject *parent) : InputSource(url,parent)
{
    m_reader = new HttpStreamReader(url, this);
    connect(m_reader, SIGNAL(ready()),SIGNAL(ready()));
    connect(m_reader, SIGNAL(error()),SIGNAL(error()));
}

QIODevice *HTTPInputSource::ioDevice()
{
    return m_reader;
}

bool HTTPInputSource::initialize()
{
    m_reader->downloadFile();
    return true;
}

bool HTTPInputSource::isReady()
{
    return m_reader->isOpen();
}

bool HTTPInputSource::isWaiting()
{
    return (!m_reader->bytesAvailable() && m_reader->isOpen());
}

QString HTTPInputSource::contentType() const
{
    return m_reader->contentType();
}
