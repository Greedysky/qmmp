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

#include <taglib/tiostream.h>
#include "archivetagreader.h"

class IODeviceStream : public TagLib::IOStream
{
public:
    IODeviceStream(QIODevice *input, const QString &url)
    {
        m_input = input;
        m_fileName = url.section("/", -1);
    }

    virtual ~IODeviceStream() {}

    virtual TagLib::FileName name() const
    {
        return QStringToFileName(m_fileName);
    }
    virtual TagLib::ByteVector readBlock(unsigned long length)
    {
        QByteArray data = m_input->read(length);
        return TagLib::ByteVector(data.constData(), data.size());
    }
    virtual void writeBlock(const TagLib::ByteVector &)
    {}
    virtual void insert(const TagLib::ByteVector &, unsigned long, unsigned long)
    {}
    virtual void removeBlock(unsigned long, unsigned long)
    {}
    virtual bool readOnly() const
    {
        return true;
    }
    virtual bool isOpen() const
    {
        return m_input->isOpen();
    }
    virtual void seek(long offset, Position p = Beginning)
    {
        switch (p)
        {
        case Beginning:
            m_input->seek(offset);
            break;
        case Current:
            m_input->seek(m_input->pos() + offset);
            break;
        case End:
            m_input->seek(m_input->size() + offset);
            break;
        }
    }
    virtual void clear()
    {
        m_input->seek(0);
        TagLib::IOStream::clear();
    }
    virtual long tell() const
    {
        return m_input->pos();
    }
    virtual long length()
    {
        return m_input->size();
    }
    virtual void truncate(long)
    {}

private:
    QString m_fileName;
    QIODevice *m_input;

};

ArchiveTagReader::ArchiveTagReader(QIODevice *input, const QString &url)
{
    m_stream = new IODeviceStream(input, url);
    m_file = new TagLib::FileRef(m_stream);
    m_url = url;
}

ArchiveTagReader::~ArchiveTagReader()
{
    delete m_file;
    delete m_stream;
}

const QMap<Qmmp::MetaData, QString> ArchiveTagReader::metaData() const
{
    QMap<Qmmp::MetaData, QString> m;

    TagLib::Tag *tag = m_file->tag();
    if(tag)
    {
        m[Qmmp::ALBUM] = TStringToQString(tag->album()).trimmed();
        m[Qmmp::ARTIST] = TStringToQString(tag->artist()).trimmed();
        m[Qmmp::COMMENT] = TStringToQString(tag->comment()).trimmed();
        m[Qmmp::GENRE] = TStringToQString(tag->genre()).trimmed();
        m[Qmmp::TITLE] = TStringToQString(tag->title()).trimmed();
        m[Qmmp::YEAR] = QString::number(tag->year());
        m[Qmmp::TRACK] = QString::number(tag->track());
    }
    m[Qmmp::URL] = m_url;
    return m;
}

TagLib::AudioProperties *ArchiveTagReader::audioProperties() const
{
    return m_file->audioProperties();
}
