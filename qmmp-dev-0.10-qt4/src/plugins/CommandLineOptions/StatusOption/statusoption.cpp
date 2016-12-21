/***************************************************************************
 *   Copyright (C) 2010-2015 by Ilya Kotov                                 *
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

#include <QtPlugin>
#include <QTranslator>
#include <QLocale>
#include <QMap>
#include <qmmp/soundcore.h>
#include <qmmpui/metadataformatter.h>
#include "statusoption.h"

bool StatusOption::identify(const QString &str) const
{
    QStringList opts;
    opts << "--status" << "--nowplaying" << "--nowplaying-syntax";
    return opts.contains(str);
}

const QString StatusOption::helpString() const
{
    QString help;
    help += QString("--status                 ") + tr("Print playback status") + "\n";
    help += QString("--nowplaying <fmt>       ")
            + tr("Print formatted track name (example: qmmp --nowplaying \"%t - %a\")") + "\n";
    help += QString("--nowplaying-syntax      ") + tr("Print --nowplaying syntax") + "\n";
    return  help;
}

QString StatusOption::executeCommand(const QString &opt_str, const QStringList &args)
{
    SoundCore *core = SoundCore::instance();
    QString out;
    if(opt_str == "--status")
    {
        QMap<int, QString> state_names;
        state_names.insert(Qmmp::Playing, "[playing]");
        state_names.insert(Qmmp::Paused, "[paused]");
        state_names.insert(Qmmp::Stopped, "[stopped]");
        state_names.insert(Qmmp::Buffering, "[buffering]");
        state_names.insert(Qmmp::NormalError, "[error]");
        state_names.insert(Qmmp::FatalError, "[error]");
        out += state_names[core->state()];

        if(core->state() == Qmmp::Playing || core->state() == Qmmp::Paused)
        {
            out += " ";
            out += genProgressBar() + "\n";
            out += "ARTIST = %p\n";
            out += "ALBUMARTIST = %aa\n";
            out += "TITLE = %t\n";
            out += "ALBUM = %a\n";
            out += "COMMENT = %c\n";
            out += "GENRE = %g\n";
            out += "YEAR = %y\n";
            out += "TRACK = %n\n";
            out += "FILE = %f\n";
            MetaDataFormatter formatter(out);
            out = formatter.format(core->metaData(), core->totalTime());
        }
        else
            out += "\n";
    }
    else if(opt_str == "--nowplaying")
    {
        QString t = args.join(" ");
        MetaDataFormatter formatter(t);
        out = formatter.format(core->metaData(), core->totalTime());
        out += "\n";
    }
    else if(opt_str == "--nowplaying-syntax")
    {
        out += tr("Syntax:") + "\n";
        out += tr("%p - artist") + "\n";
        out += tr("%a - album") + "\n";
        out += tr("%aa - album artist") + "\n";
        out += tr("%t - title") + "\n";
        out += tr("%n - track") + "\n";
        out += tr("%NN - 2-digit track") + "\n";
        out += tr("%g - genre") + "\n";
        out += tr("%c - comment") + "\n";
        out += tr("%C - composer") + "\n";
        out += tr("%D - disc number") + "\n";
        out += tr("%f - file name") + "\n";
        out += tr("%F - full path") + "\n";
        out += tr("%y - year") + "\n";
        out += tr("%l - duration") + "\n";
        out += tr("%if(A&B&C,D,E) - condition") + "\n";
    }
    return out;
}

const QString StatusOption::name() const
{
    return "StatusOption";
}

QTranslator *StatusOption::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/status_plugin_") + locale);
    return translator;
}

QString StatusOption::genProgressBar()
{
    SoundCore *core = SoundCore::instance();
    QString totalTime = QString("%1:%2").arg(core->totalTime()/60000)
            .arg(core->totalTime()%60000/1000, 2, 10, QChar('0'));
    QString currentTime = QString("%1:%2").arg(core->elapsed()/60000)
            .arg(core->elapsed()%60000/1000, 2, 10, QChar('0'));
    QString out = currentTime;
    if(core->totalTime())
    {
        out.clear();
        int played_count = 22 * (double)core->elapsed()/core->totalTime();
        for(int i = 0; i < played_count; ++i)
            out += "=";
        out += "#";
        for(int i = played_count; i < 22; ++i)
            out += "-";
        out += " " + currentTime + "/" +totalTime;
    }
    return out;
}

Q_EXPORT_PLUGIN2(statusoption, StatusOption)
