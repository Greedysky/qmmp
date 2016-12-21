/***************************************************************************
 *   Copyright (C) 2008-2015 by Ilya Kotov                                 *
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

#include <QMessageBox>
#include <QTranslator>
#include "decoder_cue.h"
#include "cuemetadatamodel.h"
#include "cueparser.h"
#include "settingsdialog.h"
#include "decodercuefactory.h"

// DecoderCUEFactory

bool DecoderCUEFactory::canDecode(QIODevice *) const
{
    return false;
}

const DecoderProperties DecoderCUEFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("CUE Plugin");
    properties.shortName = "cue";
    properties.filters << "*.cue";
    properties.description = tr("CUE Files");
    //properties.contentType = "application/ogg;audio/x-vorbis+ogg";
    properties.protocols << "cue";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.noInput = true;
    properties.noOutput = true;
    return properties;
}

Decoder *DecoderCUEFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderCUE(path);
}

QList<FileInfo *> DecoderCUEFactory::createPlayList(const QString &fileName, bool useMetaData, QStringList *ignoredPaths)
{
    Q_UNUSED(useMetaData);
    CUEParser parser(fileName);
    if(fileName.contains("://"))
    {
        QList<FileInfo *> list;
        int track = fileName.section("#", -1).toInt();
        if (!parser.count() || track <= 0 || track > parser.count())
            return list;
        list = parser.createPlayList();
        FileInfo *info = list.takeAt(track - 1);
        qDeleteAll(list);
        return QList<FileInfo *>() << info;
    }
    else
    {
        ignoredPaths->append(parser.dataFiles());
        return parser.createPlayList();
    }
}

MetaDataModel* DecoderCUEFactory::createMetaDataModel(const QString &path, QObject *parent)
{
    return path.startsWith("cue://") ? new CUEMetaDataModel(path, parent) : 0;
}

void DecoderCUEFactory::showSettings(QWidget *parent)
{
     SettingsDialog *s = new SettingsDialog(parent);
     s->show();
}

void DecoderCUEFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About CUE Audio Plugin"),
                        tr("Qmmp CUE Audio Plugin")+"\n"+
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *DecoderCUEFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/cue_plugin_") + locale);
    return translator;
}
