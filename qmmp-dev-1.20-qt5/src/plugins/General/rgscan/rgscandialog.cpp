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

#include <QSettings>
#include <QProgressBar>
#include <QThreadPool>
#include <QSettings>
#include <qmmpui/playlisttrack.h>
#include <qmmpui/metadataformatter.h>
#include <qmmpui/filedialog.h>
#include <qmmp/metadatamanager.h>
#include <taglib/mpegfile.h>
#include <taglib/apetag.h>
#include <taglib/flacfile.h>
#include <taglib/xiphcomment.h>
#include <taglib/oggflacfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/wavpackfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/mp4file.h>
#include "rgscanner.h"
#include "gain_analysis.h"
#include "rgscandialog.h"

#define FILE_SKIPPED (Qt::UserRole + 1)

struct ReplayGainInfoItem
{
    QMap<Qmmp::ReplayGainKey, double> info;
    QString url;
    GainHandle_t *handle;
};

RGScanDialog::RGScanDialog(QList <PlayListTrack *> tracks,  QWidget *parent) : QDialog(parent)
{
    m_ui.setupUi(this);
    m_ui.tableWidget->verticalHeader()->setDefaultSectionSize(fontMetrics().height() + 3);
    m_ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    QStringList paths;
    MetaDataFormatter formatter("%if(%p&%t,%p - %t,%f) - %l");
    foreach(PlayListTrack *track, tracks)
    {
        //skip streams
        if(track->length() == 0 || track->url().contains("://"))
            continue;
        //skip duplicates
        if(paths.contains(track->url()))
            continue;
        //skip unsupported files
        if(!MetaDataManager::instance()->supports(track->url()))
            continue;

        QString ext = track->url().section(".", -1).toLower();

        if((ext == "mp3") || //mpeg 1 layer 3
                ext == "flac" || //native flac
                ext == "oga" || //ogg flac
                ext == "ogg" ||  //ogg vorbis
                ext == "wv" || //wavpack
                ext == "m4a") //aac (mp4 container)
        {
            paths.append(track->url());
            QString name = formatter.format(track);
            QTableWidgetItem *item = new QTableWidgetItem(name);
            item->setData(Qt::UserRole, track->url());
            item->setData(Qt::ToolTipRole, track->url());
            m_ui.tableWidget->insertRow(m_ui.tableWidget->rowCount());
            m_ui.tableWidget->setItem(m_ui.tableWidget->rowCount() - 1, 0, item);
            QProgressBar *progressBar = new QProgressBar(this);
            progressBar->setRange(0, 100);
            m_ui.tableWidget->setCellWidget(m_ui.tableWidget->rowCount() - 1, 1, progressBar);
        }
    }

    m_ui.tableWidget->resizeColumnsToContents();
    m_ui.writeButton->setEnabled(false);
    //read settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    restoreGeometry(settings.value("RGScanner/geometry").toByteArray());
    m_ui.trackCheckBox->setChecked(settings.value("RGScanner/write_track",true).toBool());
    m_ui.albumCheckBox->setChecked(settings.value("RGScanner/write_album",true).toBool());
    m_ui.skipScannedCheckBox->setChecked(settings.value("RGScanner/skip_scanned",true).toBool());
}

RGScanDialog::~RGScanDialog()
{
    stop();
    qDeleteAll(m_replayGainItemList);
    m_replayGainItemList.clear();
}

void RGScanDialog::on_calculateButton_clicked()
{
    m_ui.writeButton->setEnabled(false);
    for(int i = 0; i < m_ui.tableWidget->rowCount(); ++i)
    {
        QString url = m_ui.tableWidget->item(i, 0)->data(Qt::UserRole).toString();
        RGScanner *scanner = new RGScanner();
        m_ui.tableWidget->item(i, 0)->setData(FILE_SKIPPED, false);

        if(!scanner->prepare(url))
        {
            m_ui.tableWidget->setItem(i, 2, new QTableWidgetItem(tr("Error")));
            delete scanner;
            continue;
        }

        if(m_ui.skipScannedCheckBox->isChecked() && !scanner->oldReplayGainInfo().isEmpty())
        {
            qDebug("RGScanDialog: skipping scanned file..");
            m_ui.tableWidget->item(i, 0)->setData(FILE_SKIPPED, true);
            QMap<Qmmp::ReplayGainKey, double> rg = scanner->oldReplayGainInfo();
            m_ui.tableWidget->setItem(i, 2, new QTableWidgetItem(tr("%1 dB").arg(rg.value(Qmmp::REPLAYGAIN_TRACK_GAIN))));
            m_ui.tableWidget->setItem(i, 3, new QTableWidgetItem(tr("%1 dB").arg(rg.value(Qmmp::REPLAYGAIN_ALBUM_GAIN))));
            m_ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(rg.value(Qmmp::REPLAYGAIN_TRACK_PEAK))));
            m_ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(rg.value(Qmmp::REPLAYGAIN_ALBUM_PEAK))));
            delete scanner;
            continue;
        }

        scanner->setAutoDelete(false);
        m_scanners.append(scanner);
        connect(scanner, SIGNAL(progress(int)), m_ui.tableWidget->cellWidget(i, 1), SLOT(setValue(int)));
        connect(scanner, SIGNAL(finished(QString)), SLOT(onScanFinished(QString)));
        QThreadPool::globalInstance()->start(scanner);
    }
}

void RGScanDialog::onScanFinished(QString url)
{
    for(int i = 0; i < m_ui.tableWidget->rowCount(); ++i)
    {
        if(url != m_ui.tableWidget->item(i, 0)->data(Qt::UserRole).toString())
            continue;
        RGScanner *scanner = findScannerByUrl(url);
        if(!scanner)
            qFatal("RGScanDialog: unable to find scanner by URL!");
        m_ui.tableWidget->setItem(i, 2, new QTableWidgetItem(tr("%1 dB").arg(scanner->gain())));
        m_ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(scanner->peak())));
        break;
    }

    bool stopped = true;

    foreach (RGScanner *scanner, m_scanners)
    {
        if(scanner->isRunning() || scanner->isPending())
            stopped = false;
    }

    if(stopped)
    {
        qDebug("RGScanDialog: all threads are finished");
        QThreadPool::globalInstance()->waitForDone();

        QMultiMap<QString, ReplayGainInfoItem*> itemGroupMap; //items grouped  by album

        //group by album name
        foreach (RGScanner *scanner, m_scanners)
        {
            if(!scanner->hasValues())
                continue;
            ReplayGainInfoItem *item = new ReplayGainInfoItem;
            item->info[Qmmp::REPLAYGAIN_TRACK_GAIN] = scanner->gain();
            item->info[Qmmp::REPLAYGAIN_TRACK_PEAK] = scanner->peak();
            item->url = scanner->url();
            item->handle = scanner->handle();
            QString album = getAlbumName(item->url);
            itemGroupMap.insert(album, item);
        }
        //calculate album peak and gain
        foreach (QString album, itemGroupMap.keys())
        {
            QList<ReplayGainInfoItem*> items = itemGroupMap.values(album);
            GainHandle_t **a = (GainHandle_t **) malloc(items.count()*sizeof(GainHandle_t *));
            double album_peak = 0;
            for(int i = 0; i < items.count(); ++i)
            {
                a[i] = items[i]->handle;
                album_peak = qMax(items[i]->info[Qmmp::REPLAYGAIN_TRACK_PEAK], album_peak);
            }
            double album_gain = GetAlbumGain(a, items.count());
            free(a);
            foreach (ReplayGainInfoItem *item, items)
            {
                item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK] = album_peak;
                item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN] = album_gain;
            }
        }
        //clear scanners
        qDeleteAll(m_scanners);
        m_scanners.clear();
        //clear previous replaygain information
        qDeleteAll(m_replayGainItemList);
        m_replayGainItemList.clear();
        //update table
        m_replayGainItemList = itemGroupMap.values();
        for(int i = 0; i < m_ui.tableWidget->rowCount(); ++i)
        {
            QString url = m_ui.tableWidget->item(i, 0)->data(Qt::UserRole).toString();
            bool found = false;
            foreach (ReplayGainInfoItem *item, m_replayGainItemList)
            {
                if(item->url == url)
                {
                    found = true;
                    double album_gain = item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN];
                    double album_peak = item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK];
                    m_ui.tableWidget->setItem(i, 3, new QTableWidgetItem(tr("%1 dB").arg(album_gain)));
                    m_ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(album_peak)));
                }
            }
            if(!found && !m_ui.tableWidget->item(i, 0)->data(FILE_SKIPPED).toBool())
                m_ui.tableWidget->setItem(i, 3, new QTableWidgetItem(tr("Error")));
        }

        //clear items
        itemGroupMap.clear();

        m_ui.writeButton->setEnabled(true);
    }
}

void RGScanDialog::reject()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("RGScanner/geometry", saveGeometry());
    settings.setValue("RGScanner/write_track", m_ui.trackCheckBox->isChecked());
    settings.setValue("RGScanner/write_album", m_ui.albumCheckBox->isChecked());
    settings.setValue("RGScanner/skip_scanned", m_ui.skipScannedCheckBox->isChecked());
    QDialog::reject();
}

void RGScanDialog::stop()
{
    if(m_scanners.isEmpty())
        return;
    foreach (RGScanner *scaner, m_scanners)
    {
        scaner->stop();
    }
    QThreadPool::globalInstance()->waitForDone();
    qDeleteAll(m_scanners);
    m_scanners.clear();
}

RGScanner *RGScanDialog::findScannerByUrl(const QString &url)
{
    foreach (RGScanner *scanner, m_scanners)
    {
        if(scanner->url() == url)
            return scanner;
    }
    return 0;
}

QString RGScanDialog::getAlbumName(const QString &url)
{
    QList <FileInfo *> infoList = MetaDataManager::instance()->createPlayList(url);
    if(infoList.isEmpty())
        return QString();
    QString album = infoList.first()->metaData(Qmmp::ALBUM);
    qDeleteAll(infoList);
    return album;
}

TagLib::String RGScanDialog::gainToString(double value)
{
    return QStringToTString(QString("%1 dB").arg(value, 0, 'f', 2));
}

TagLib::String RGScanDialog::peakToString(double value)
{
    return QStringToTString(QString("%1").arg(value, 0, 'f', 6));
}

TagLib::StringList RGScanDialog::gainToStringList(double value)
{
   return TagLib::StringList (gainToString(value));
}

TagLib::StringList RGScanDialog::peakToStringList(double value)
{
    return TagLib::StringList (peakToString(value));
}

void RGScanDialog::writeAPETag(TagLib::APE::Tag *tag, ReplayGainInfoItem *item)
{
    if(m_ui.trackCheckBox->isChecked())
    {
        tag->addValue("REPLAYGAIN_TRACK_GAIN", gainToString(item->info[Qmmp::REPLAYGAIN_TRACK_GAIN]));
        tag->addValue("REPLAYGAIN_TRACK_PEAK", peakToString(item->info[Qmmp::REPLAYGAIN_TRACK_PEAK]));
    }
    if(m_ui.albumCheckBox->isChecked())
    {
        tag->addValue("REPLAYGAIN_ALBUM_GAIN", gainToString(item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN]));
        tag->addValue("REPLAYGAIN_ALBUM_PEAK", peakToString(item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK]));
    }
}

void RGScanDialog::writeID3v2Tag(TagLib::ID3v2::Tag *tag, ReplayGainInfoItem *item)
{
    tag->removeFrames("TXXX");
    if(m_ui.trackCheckBox->isChecked())
    {
        TagLib::ID3v2::UserTextIdentificationFrame *frame = new TagLib::ID3v2::UserTextIdentificationFrame();
        TagLib::StringList fields;
        fields.append("REPLAYGAIN_TRACK_GAIN");
        fields.append(gainToString(item->info[Qmmp::REPLAYGAIN_TRACK_GAIN]));
        frame->setText(fields);
        tag->addFrame(frame);

        fields.clear();
        frame = new TagLib::ID3v2::UserTextIdentificationFrame();
        fields.append("REPLAYGAIN_TRACK_PEAK");
        fields.append(peakToString(item->info[Qmmp::REPLAYGAIN_TRACK_PEAK]));
        frame->setText(fields);
        tag->addFrame(frame);
    }
    if(m_ui.albumCheckBox->isChecked())
    {
        TagLib::ID3v2::UserTextIdentificationFrame *frame = new TagLib::ID3v2::UserTextIdentificationFrame();
        TagLib::StringList fields;
        fields.append("REPLAYGAIN_ALBUM_GAIN");
        fields.append(gainToString(item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN]));
        frame->setText(fields);
        tag->addFrame(frame);

        fields.clear();
        frame = new TagLib::ID3v2::UserTextIdentificationFrame();
        fields.append("REPLAYGAIN_ALBUM_PEAK");
        fields.append(peakToString(item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK]));
        frame->setText(fields);
        tag->addFrame(frame);
    }
}

void RGScanDialog::writeVorbisComment(TagLib::Ogg::XiphComment *tag, ReplayGainInfoItem *item)
{
    if(m_ui.trackCheckBox->isChecked())
    {
        tag->addField("REPLAYGAIN_TRACK_GAIN", gainToString(item->info[Qmmp::REPLAYGAIN_TRACK_GAIN]));
        tag->addField("REPLAYGAIN_TRACK_PEAK", peakToString(item->info[Qmmp::REPLAYGAIN_TRACK_PEAK]));
    }
    if(m_ui.albumCheckBox->isChecked())
    {
        tag->addField("REPLAYGAIN_ALBUM_GAIN", gainToString(item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN]));
        tag->addField("REPLAYGAIN_ALBUM_PEAK", peakToString(item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK]));
    }
}

void RGScanDialog::writeMP4Tag(TagLib::MP4::Tag *tag, ReplayGainInfoItem *item)
{
    if(m_ui.trackCheckBox->isChecked())
    {
        tag->setItem("----:com.apple.iTunes:replaygain_track_gain",
                     gainToStringList(item->info[Qmmp::REPLAYGAIN_TRACK_GAIN]));
        tag->setItem("----:com.apple.iTunes:replaygain_track_peak",
                     gainToStringList(item->info[Qmmp::REPLAYGAIN_TRACK_PEAK]));
    }
    if(m_ui.albumCheckBox->isChecked())
    {
        tag->setItem("----:com.apple.iTunes:replaygain_album_gain",
                     gainToStringList(item->info[Qmmp::REPLAYGAIN_ALBUM_GAIN]));
        tag->setItem("----:com.apple.iTunes:replaygain_album_peak",
                     gainToStringList(item->info[Qmmp::REPLAYGAIN_ALBUM_PEAK]));
    }
}

void RGScanDialog::on_writeButton_clicked()
{
    if(m_replayGainItemList.isEmpty())
        return;

    qDebug("RGScanDialog: writing ReplayGain values...");

    foreach (ReplayGainInfoItem *item, m_replayGainItemList)
    {
        QString ext = item->url.section(".", -1).toLower();

        if(ext == "mp3") //mpeg 1 layer 3
        {
            TagLib::MPEG::File file(qPrintable(item->url));
            writeAPETag(file.APETag(true), item);
            writeID3v2Tag(file.ID3v2Tag(true), item);
            file.save(TagLib::MPEG::File::APE | TagLib::MPEG::File::ID3v2, false);
        }
        else if(ext == "flac") //flac
        {
            TagLib::FLAC::File file(qPrintable(item->url));
            writeVorbisComment(file.xiphComment(true), item);
            file.save();
        }
        else if(ext == "oga") //ogg flac
        {
            TagLib::Ogg::FLAC::File file(qPrintable(item->url));
            writeVorbisComment(file.tag(), item);
            file.save();
        }
        else if(ext == "ogg") //ogg vorbis
        {
            TagLib::Ogg::Vorbis::File file(qPrintable(item->url));
            writeVorbisComment(file.tag(), item);
            file.save();
        }
        else if(ext == "wv") //wavpack
        {
            TagLib::WavPack::File file(qPrintable(item->url));
            writeAPETag(file.APETag(true), item);
            file.save();
        }
        else if(ext == "m4a") //MPEG-4 Part 14
        {
            TagLib::MP4::File file(qPrintable(item->url));
            writeMP4Tag(file.tag(), item);
            file.save();
        }
    }
}
