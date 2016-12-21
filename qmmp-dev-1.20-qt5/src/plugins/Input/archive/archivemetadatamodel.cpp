/***************************************************************************
 *   Copyright (C) 2016 by Ilya Kotov                                      *
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

#include "archiveinputdevice.h"
#include "archivetagreader.h"
#include "archivemetadatamodel.h"

ArchiveMetaDataModel::ArchiveMetaDataModel(const QString &url, QObject *parent) :
    MetaDataModel(parent)
{
    m_reader = 0;
    m_input = 0;
    m_url = url;
}

ArchiveMetaDataModel::~ArchiveMetaDataModel()
{
    if(m_reader)
        delete m_reader;
    if(m_input)
        delete m_input;
}

QHash<QString, QString> ArchiveMetaDataModel::audioProperties()
{
    if(!m_input)
    {
        m_input = new ArchiveInputDevice(m_url);
        if(m_input->isOpen())
            m_reader = new ArchiveTagReader(m_input, m_url);
    }

    QHash <QString, QString> ap;
    if(m_reader && m_reader->audioProperties())
    {
        TagLib::AudioProperties *p = m_reader->audioProperties();
        ap.insert(tr("Length"), QString("%1:%2").arg(p->length()/60).arg(p->length()%60, 2, 10, QChar('0')));
        ap.insert(tr("Sample rate"), QString("%1 " + tr("Hz")).arg(p->sampleRate()));
        ap.insert(tr("Channels"), QString("%1").arg(p->channels()));
        ap.insert(tr("Bitrate"), QString("%1 " + tr("kbps")).arg(p->bitrate()));
        ap.insert(tr("File size"), QString("%1 "+tr("KB")).arg(m_input->size()/1024));
    }
    return ap;
}
