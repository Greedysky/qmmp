/***************************************************************************
 *   Copyright (C) 2013-2016 by Ilya Kotov                                 *
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

#ifndef RGSCANDIALOG_H
#define RGSCANDIALOG_H

#include <QDialog>
#include <QStringList>
#include <qmmp/qmmp.h>
#include <taglib/taglib.h>
#include <taglib/tstring.h>
#include <taglib/tstringlist.h>
#include "ui_rgscandialog.h"

class QAction;
class PlayListTrack;
class ConverterPreset;
class RGScanner;
struct ReplayGainInfoItem;

namespace TagLib
{
    namespace ID3v2
    {
        class Tag;
    }
    namespace APE
    {
        class Tag;
    }
    namespace Ogg
    {
        class XiphComment;
    }
#if (TAGLIB_MAJOR_VERSION > 1) || ((TAGLIB_MAJOR_VERSION == 1) && (TAGLIB_MINOR_VERSION >= 10))
    namespace MP4
    {
        class Tag;
    }
#endif
}

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class RGScanDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RGScanDialog(QList <PlayListTrack *> tracks,  QWidget *parent = 0);
    virtual ~RGScanDialog();

private slots:
    void on_calculateButton_clicked();
    void onScanFinished(QString url);
    void reject();
    void on_writeButton_clicked();

private:
    void stop();
    RGScanner *findScannerByUrl(const QString &url);
    QString getAlbumName(const QString &url);
    TagLib::String gainToString(double value);
    TagLib::String peakToString(double value);
    TagLib::StringList gainToStringList(double value);
    TagLib::StringList peakToStringList(double value);
    void writeAPETag(TagLib::APE::Tag *tag, ReplayGainInfoItem *item);
    void writeID3v2Tag(TagLib::ID3v2::Tag *tag, ReplayGainInfoItem *item);
    void writeVorbisComment(TagLib::Ogg::XiphComment *tag, ReplayGainInfoItem *item);
#if (TAGLIB_MAJOR_VERSION > 1) || ((TAGLIB_MAJOR_VERSION == 1) && (TAGLIB_MINOR_VERSION >= 10))
    void writeMP4Tag(TagLib::MP4::Tag *tag, ReplayGainInfoItem *item);
#endif
    Ui::RGScanDialog m_ui;
    QList<RGScanner *> m_scanners;
    QList<ReplayGainInfoItem*> m_replayGainItemList;
};

#endif // RGSCANDIALOG_H
