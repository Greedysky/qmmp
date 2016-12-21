/***************************************************************************
 *   Copyright (C) 2009-2016 by Ilya Kotov                                 *
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
#include <qmmp/qmmp.h>
#include <qmmpui/mediaplayer.h>
#include "ui_mainwindow.h"

class QSlider;
class QLabel;
class QMenu;
class QToolButton;
class UiHelper;
class PlayListModel;
class MediaPlayer;
class SoundCore;
class QmmpUiSettings;
class PlayListManager;
class GeneralHandler;
class VisualMenu;
class PositionSlider;
class KeyboardManager;
class QSUiAnalyzer;
class ListWidget;
class QSUiTabWidget;


/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void addDir();
    void addFiles();
    void playFiles();
    void addUrl();
    void updatePosition(qint64 pos);
    void seek();
    void showState(Qmmp::State);
    void updateStatus();
    void updateTabs();
    void removePlaylist();
    void removePlaylistWithIndex(int);
    void addTab(int);
    void removeTab(int);
    void renameTab();
    void aboutUi();
    void about();
    void toggleVisibility();
    void showAndRaise();
    void showSettings();
    void updateVolumeIcon();
    void jumpTo();
    void playPause();
    void readSettings();
    void showTabMenu(QPoint pos);
    void savePlayList();
    void loadPlayList();
    void showBuffering(int percent);
    void showEqualizer();
    void forward();
    void backward();
    void showMetaData();
    void setTitleBarsVisible(bool visible);
    void setToolBarsBlocked(bool blocked);
    void editToolBar();

private:
    void closeEvent(QCloseEvent *);
    void hideEvent(QHideEvent *);
    void createWidgets();
    void createActions();
    void writeSettings();
    bool m_wasMaximized;
    PlayListManager *m_pl_manager;
    Ui::MainWindow m_ui;
    MediaPlayer *m_player;
    QmmpUiSettings *m_ui_settings;
    PositionSlider *m_positionSlider;
    QLabel *m_timeLabel;
    SoundCore *m_core;
    QLabel *m_statusLabel;
    VisualMenu *m_visMenu;
    UiHelper *m_uiHelper;
    QMenu *m_pl_menu;
    QMenu *m_tab_menu;
    QSlider *m_volumeSlider;
    KeyboardManager *m_key_manager;
    QSUiAnalyzer *m_analyzer;
    QToolButton *m_addListButton, *m_tabListMenuButton;
    ListWidget *m_listWidget;
    MetaDataFormatter m_titleFormatter;
    bool m_update;
    bool m_hideOnClose;
    QSUiTabWidget *m_tabWidget;

};

#endif
