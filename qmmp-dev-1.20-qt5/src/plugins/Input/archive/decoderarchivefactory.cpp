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

#include <QRegExp>
#include <QMessageBox>
#include <QTranslator>
#include <archive.h>
#include <archive_entry.h>
#include "decoder_archive.h"
#include "archivetagreader.h"
#include "archiveinputdevice.h"
#include "archivemetadatamodel.h"
#include "decoderarchivefactory.h"

// DecoderArchiveFactory
bool DecoderArchiveFactory::canDecode(QIODevice *) const
{
    return false;
}

const DecoderProperties DecoderArchiveFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Archive Plugin");
    properties.filters << "*.rar" << "*.zip";
    properties.description = tr("Archives");
    properties.contentTypes << "application/zip" << "application/x-rar-compressed";
    properties.shortName = "archive";
    properties.hasAbout = true;
    properties.hasSettings = false;
    properties.noInput = true;
    properties.protocols << "rar" << "zip";
    return properties;
}

Decoder *DecoderArchiveFactory::create(const QString &url, QIODevice *)
{
    return new DecoderArchive(url);
}

QList<FileInfo *> DecoderArchiveFactory::createPlayList(const QString &path, bool useMetaData, QStringList *)
{
    QList <FileInfo *> list;
    struct archive_entry *entry = 0;

    struct archive *a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    QString archivePath, requiredFilePath;
    if(path.contains("://"))
    {
        requiredFilePath = path.section("#", -1);
        archivePath = path;
        archivePath.remove(QRegExp("^.+://"));
        archivePath.remove(QRegExp("#.+$"));
    }
    else
    {
        archivePath = path;
    }

    if(archive_read_open_filename(a, archivePath.toLocal8Bit().constData(), 10240) != ARCHIVE_OK)
    {
        qWarning("DecoderArchiveFactory: unable to open archive; libarchive error: %s", archive_error_string(a));
        return list;
    }
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
    {
        if(archive_entry_filetype(entry) == AE_IFREG)
        {
            QString filePath = QString::fromLocal8Bit(archive_entry_pathname(entry));
            if(!filePath.startsWith("/"))
                filePath.prepend("/");

            if(!requiredFilePath.isEmpty() && filePath != requiredFilePath)
            {
                archive_read_data_skip(a);
                continue;
            }

            //is this file supported by qmmp?
            QList<DecoderFactory *> filtered = Decoder::findByFileExtension(filePath);
            foreach (DecoderFactory *f, filtered)
            {
                if(f->properties().noInput)
                    filtered.removeAll(f); //remove all factories without streaming input
            }

            if(!filtered.isEmpty())
            {
                list << new FileInfo(QString("%1://%2#%3")
                                     .arg(archivePath.section(".", -1)).toLower()
                                     .arg(archivePath)
                                     .arg(filePath));

                ArchiveInputDevice dev(a, entry, 0);
                ArchiveTagReader reader(&dev, list.last()->path());

                if(useMetaData)
                    list.last()->setMetaData(reader.metaData());
                if(reader.audioProperties())
                    list.last()->setLength(reader.audioProperties()->length());
            }
        }
        archive_read_data_skip(a);
    }
    archive_read_free(a);

    return list;
}

MetaDataModel* DecoderArchiveFactory::createMetaDataModel(const QString &path, QObject *parent)
{
    return new ArchiveMetaDataModel(path, parent);
}

void DecoderArchiveFactory::showSettings(QWidget *)
{}

void DecoderArchiveFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About Archive Reader Plugin"),
                        tr("Qmmp Archive Reader Plugin")+"\n"+
                        tr("Compiled against %1").arg(ARCHIVE_VERSION_STRING)+"\n" +
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *DecoderArchiveFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/archive_plugin_") + locale);
    return translator;
}
