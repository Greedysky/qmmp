/***************************************************************************
 *   Copyright (C) 2008-2014 by Ilya Kotov                                 *
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

#include <QApplication>
#include <QFileInfo>
#include <qmmp/soundcore.h>
#include <qmmpui/mediaplayer.h>
#include <qmmpui/uihelper.h>
#include <qmmpui/filedialog.h>
#include <qmmpui/uihelper.h>
#include <qmmpui/qmmpuisettings.h>
#include <qmmpui/playlistdownloader.h>
#include <qmmpui/playlistparser.h>
#include <qmmp/metadatamanager.h>
#include "builtincommandlineoption.h"

BuiltinCommandLineOption::BuiltinCommandLineOption(QObject *parent) : QObject(parent)
{
    m_model = 0;
    m_options << "--enqueue" << "-e"
              << "--play" << "-p"
              << "--pause" << "-u"
              << "--play-pause" << "-t"
              << "--stop" << "-s"
              << "--jump-to-file" << "-j"
              << "--quit" << "-q"
              << "--volume"
              << "--toggle-mute"
              << "--next" << "--previous"
              << "--toggle-visibility"
              << "--show-mw"
              << "--add-file" << "--add-dir";
#ifdef Q_OS_WIN
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(500);
    connect(m_timer, SIGNAL(timeout()), SLOT(addPendingPaths()));
#endif
}

BuiltinCommandLineOption::~BuiltinCommandLineOption()
{
}

// BuiltinCommandLineOption methods implementation
bool BuiltinCommandLineOption::identify(const QString &str) const
{
    return m_options.contains(str);
}

const QString BuiltinCommandLineOption::helpString() const
{
    return QString(
               "-e, --enqueue            "+tr("Don't clear the playlist") + "\n" +
               "-p, --play               "+tr("Start playing current song")+"\n" +
               "-u, --pause              "+tr("Pause current song")+ "\n"
               "-t, --play-pause         "+tr("Pause if playing, play otherwise")+ "\n"
               "-s, --stop               "+tr("Stop current song")+ "\n" +
               "-j, --jump-to-file       "+tr("Display Jump to File dialog")+ "\n" +
               "-q, --quit               "+tr("Quit application") + "\n" +
               "--volume <0..100>        "+tr("Set playback volume (example: qmmp --volume 20)") + "\n"
               "--toggle-mute            "+tr("Mute/Restore volume") + "\n"
               "--next                   "+tr("Skip forward in playlist")+ "\n" +
               "--previous               "+tr("Skip backwards in playlist")+"\n" +
               "--toggle-visibility      "+tr("Show/hide application")+ "\n" +
               "--show-mw                "+tr("Show main window") + "\n" +
               "--add-file               "+tr("Display Add File dialog")+ "\n" +
               "--add-dir                "+tr("Display Add Directory dialog")
           );
}

void BuiltinCommandLineOption::executeCommand(const QString &option_string,
                                              const QStringList &args,
                                              const QString &cwd)
{
    SoundCore *core = SoundCore::instance();
    MediaPlayer *player = MediaPlayer::instance();
    PlayListManager *pl_manager = PlayListManager::instance();
    QmmpUiSettings *settings = QmmpUiSettings::instance();
    if(!core || !player)
        return;
    if(option_string == "--enqueue" || option_string == "-e" || option_string.isEmpty())
    {
        if(args.isEmpty())
            return;
        QStringList full_path_list, remote_pls_list;
        foreach(QString s, args)
        {
#ifdef Q_OS_WIN
            s.replace("\\","/");
#endif
            if (QFileInfo(s).isAbsolute()) //absolute path
                    full_path_list << s;
            else if(s.contains("://")) //url
            {
                if(PlayListParser::findByUrl(s)) //remote playlist
                    remote_pls_list << s;
                else
                    full_path_list << s; //url
            }
            else //relative path
                full_path_list << cwd + "/" + s;
        }
        //default playlist
        if(settings->useDefaultPlayList())
        {
            if(!pl_manager->playListNames().contains(settings->defaultPlayListName()))
                pl_manager->createPlayList(settings->defaultPlayListName());
            pl_manager->selectPlayList(settings->defaultPlayListName());
        }
        pl_manager->activatePlayList(pl_manager->selectedPlayList());
        m_model = pl_manager->selectedPlayList();

        if(option_string.isEmpty())
        {
            m_model->clear(); //clear playlist if option is empty
            m_pending_path_list << full_path_list;
#ifdef Q_OS_WIN
            //windows starts instance for each selected file,
            //so we should wait paths from all started qmmp instances
            m_timer->start();
#else
            addPendingPaths();
#endif
        }
        else
            m_model->add(full_path_list);
        if(!remote_pls_list.isEmpty())
        {
            PlayListDownloader *downloader = new PlayListDownloader(this);
            connect(downloader, SIGNAL(done(QStringList)), m_model, SLOT(add(QStringList)));
            connect(downloader, SIGNAL(done(QStringList)), downloader, SLOT(deleteLater()));
            connect(downloader, SIGNAL(error(QString)), downloader, SLOT(deleteLater()));
            downloader->start(remote_pls_list.at(0));
        }
    }
    else if (option_string == "--play" || option_string == "-p")
    {
        player->play();
    }
    else if (option_string == "--stop" || option_string == "-s")
    {
        core->stop();
    }
    else if (option_string == "--pause" || option_string == "-u")
    {
        if(core->state() == Qmmp::Playing)
            core->pause();
    }
    else if (option_string == "--next")
    {
        player->next();
        if (core->state() == Qmmp::Stopped)
            player->play();
    }
    else if (option_string == "--previous")
    {
        player->previous();
        if (core->state() == Qmmp::Stopped)
            player->play();
    }
    else if (option_string == "--play-pause"  || option_string == "-t")
    {
        if (core->state() == Qmmp::Playing)
            core->pause();
        else
            player->play();
    }
    else if (option_string == "--jump-to-file" || option_string == "-j")
    {
        UiHelper::instance()->jumpToTrack();
    }
    else if (option_string == "--quit" || option_string == "-q")
    {
        qApp->closeAllWindows();
        qApp->quit();
    }
    else if (option_string == "--toggle-visibility")
    {
        UiHelper::instance()->toggleVisibility();
    }
    else if (option_string == "--show-mw")
    {
        UiHelper::instance()->showMainWindow();
    }
    else if (option_string == "--add-file")
    {
        UiHelper::instance()->addFiles();
    }
    else if (option_string == "--add-dir")
    {
        UiHelper::instance()->addDirectory();
    }
    else if (option_string == "--volume" && !args.isEmpty())
    {
        bool ok = false;
        int volume = args.at(0).toInt(&ok);
        if (ok)
            core->setVolume(volume,volume);
    }
    else if (option_string == "--toggle-mute")
    {
        core->setMuted(!core->isMuted());
    }
}

QHash <QString, QStringList> BuiltinCommandLineOption::splitArgs(const QStringList &args) const
{
    QHash <QString, QStringList> commands;
    QString lastCmd;
    foreach(QString arg, args)
    {
        QString cmd = arg.trimmed();
        if(cmd.startsWith("-") || cmd.startsWith("--"))
        {
            commands.insert(cmd, QStringList());
            lastCmd = cmd;
        }
        else if(!commands.isEmpty() && !lastCmd.isEmpty())
            commands[lastCmd] << arg;
    }
    return commands;
}

void BuiltinCommandLineOption::disconnectPl()
{
    if(m_model)
    {
        disconnect(m_model, SIGNAL(trackAdded(PlayListTrack*)), MediaPlayer::instance(), SLOT(play()));
        disconnect(m_model, SIGNAL(trackAdded(PlayListTrack*)), this, SLOT(disconnectPl()));
        disconnect(m_model, SIGNAL(loaderFinished()), this, SLOT(disconnectPl()));
        m_model = 0;
    }
}

void BuiltinCommandLineOption::addPendingPaths()
{
    if(m_pending_path_list.isEmpty())
        return;

    SoundCore *core = SoundCore::instance();
    MediaPlayer *player = MediaPlayer::instance();
    PlayListManager *pl_manager = PlayListManager::instance();

    if (core->state() != Qmmp::Stopped)
    {
        core->stop();
        qApp->processEvents(); //receive stop signal
    }
    m_model = pl_manager->selectedPlayList();
    m_model->clear();

    connect(m_model, SIGNAL(trackAdded(PlayListTrack*)), player, SLOT(play()));
    connect(m_model, SIGNAL(trackAdded(PlayListTrack*)), SLOT(disconnectPl()));
    connect(m_model, SIGNAL(loaderFinished()), SLOT(disconnectPl()));

    m_model->add(m_pending_path_list);
    m_pending_path_list.clear();
}
