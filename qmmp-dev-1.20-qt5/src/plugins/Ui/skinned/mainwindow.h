/***************************************************************************
 *   Copyright (C) 2006-2016 by Ilya Kotov                                 *
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <qmmp/output.h>
#include <qmmp/decoder.h>
#include <qmmp/decoderfactory.h>
#include <qmmpui/playlistitem.h>
#include <qmmpui/metadataformatter.h>
#include "display.h"
#include "titlebar.h"

class PlayList;
class PlayListManager;
class EqWidget;
class MainVisual;
class Skin;
class SoundCore;
class VisualMenu;
class UiHelper;
class MediaPlayer;
class QMenu;
class QKeyEvent;
class QmmpUiSettings;

/**
   @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

    ~MainWindow();

    PlayList *playlist(){return m_playlist;}

    QMenu* menu();
    MainDisplay* mainDisplay()const;

public slots:
    void previous();
    void play();
    void pause();
    void playPause();
    void stop();
    void next();
    void replay();
    void jumpToTrack();
    void toggleVisibility();
    void showAndRaise();

    void addDir();
    void addFile();
    void playFiles();
    void addUrl();

    void loadPlaylist();
    void savePlaylist();
    void about();
    void updateSettings();

private slots:
    void showState(Qmmp::State state);
    void showMetaData();
    void showSettings();
    void forward();
    void backward();

private:
    //events
    void closeEvent (QCloseEvent *);
    void hideEvent (QHideEvent *);
    void changeEvent (QEvent *event);
    void keyPressEvent (QKeyEvent* );


    void readSettings();
    void writeSettings();
    void createActions();
    SoundCore *m_core;
    QMenu *m_mainMenu;
    MainDisplay *m_display;
    PlayList *m_playlist;
    PlayListManager *m_pl_manager;
    EqWidget *m_equalizer;
    MainVisual *m_vis;
    bool m_update;
    Skin *m_skin;
    bool m_hideOnClose, m_startHidden;
    VisualMenu *m_visMenu;
    UiHelper *m_uiHelper;
    QmmpUiSettings *m_ui_settings;
    MediaPlayer *m_player;
    MetaDataFormatter m_titleFormatter;
};

#endif
