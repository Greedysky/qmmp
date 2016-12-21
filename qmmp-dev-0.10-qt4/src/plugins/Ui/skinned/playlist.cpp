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
#include <QPainter>
#include <QResizeEvent>
#include <QSettings>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QCloseEvent>
#include <QInputDialog>
#include <qmmpui/playlistitem.h>
#include <qmmpui/playlistmodel.h>
#include <qmmpui/playlistmanager.h>
#include <qmmpui/uihelper.h>
#include <qmmpui/qmmpuisettings.h>
#include <qmmp/soundcore.h>
#include "dock.h"
#include "skin.h"
#include "listwidget.h"
#include "button.h"
#include "playlisttitlebar.h"
#include "playlistslider.h"
#include "pixmapwidget.h"
#include "symboldisplay.h"
#include "playlistcontrol.h"
#include "keyboardmanager.h"
#include "playlistbrowser.h"
#include "playlistselector.h"
#include "windowsystem.h"
#include "actionmanager.h"
#include "playlistheader.h"
#include "playlist.h"

PlayList::PlayList (PlayListManager *manager, QWidget *parent)
        : QWidget (parent)
{
    setAttribute(Qt::WA_AlwaysShowToolTips,true);
    setWindowTitle(tr("Playlist"));
    m_pl_manager = manager;
    m_ui_settings = QmmpUiSettings::instance();
    m_update = false;
    m_resize = false;
    m_skin = Skin::instance();
    m_ratio = m_skin->ratio();
    m_shaded = false;
    m_pl_browser = 0;
    m_pl_selector = 0;

#ifdef Q_WS_X11
    QString wm_name = WindowSystem::netWindowManagerName();
    m_compiz = wm_name.contains("compiz", Qt::CaseInsensitive);
    if(wm_name.contains("metacity", Qt::CaseInsensitive) ||
       wm_name.contains("openbox", Qt::CaseInsensitive))
        setWindowFlags (Qt::Tool | Qt::FramelessWindowHint);
    else
#endif
        setWindowFlags (Qt::Dialog | Qt::FramelessWindowHint);

#ifdef Q_WS_X11
    if(m_compiz)
    {
        setFixedSize(275*m_ratio, 116*m_ratio);
    }
    else
#endif
    {
        resize (275*m_ratio, 116*m_ratio);
        setSizeIncrement (25*m_ratio, 29*m_ratio);
        setMinimumSize(275*m_ratio, 116*m_ratio);
    }

    m_listWidget = new ListWidget (this);
    m_plslider = new PlayListSlider (this);
    m_buttonAdd = new Button (this,Skin::PL_BT_ADD,Skin::PL_BT_ADD, Skin::CUR_PNORMAL);
    m_buttonSub = new Button (this,Skin::PL_BT_SUB,Skin::PL_BT_SUB, Skin::CUR_PNORMAL);
    m_selectButton  = new Button (this,Skin::PL_BT_SEL,Skin::PL_BT_SEL, Skin::CUR_PNORMAL);
    m_sortButton= new Button (this,Skin::PL_BT_SORT,Skin::PL_BT_SORT, Skin::CUR_PNORMAL);
    m_playlistButton = new Button (this,Skin::PL_BT_LST,Skin::PL_BT_LST, Skin::CUR_PNORMAL);
    m_resizeWidget = new QWidget(this);
    m_resizeWidget->resize(25,25);
    m_resizeWidget->setCursor(m_skin->getCursor (Skin::CUR_PSIZE));
    m_pl_control = new PlaylistControl (this);

    m_length_totalLength = new SymbolDisplay (this,17);
    m_length_totalLength->setAlignment (Qt::AlignLeft);

    m_current_time = new SymbolDisplay (this,6);
    m_keyboardManager = new KeyboardManager (m_listWidget);

    connect (m_listWidget, SIGNAL (selectionChanged()), parent, SLOT (replay()));

    connect (m_plslider, SIGNAL (sliderMoved (int)), m_listWidget, SLOT (scroll (int)));
    connect (m_listWidget, SIGNAL (positionChanged (int, int)), m_plslider,
             SLOT (setPos (int, int)));
    connect (m_skin, SIGNAL (skinChanged()), this, SLOT (updateSkin()));
    connect (m_buttonAdd, SIGNAL (clicked()), SLOT (showAddMenu()));
    connect (m_buttonSub, SIGNAL (clicked()), SLOT (showSubMenu()));
    connect (m_selectButton, SIGNAL (clicked()), SLOT (showSelectMenu()));
    connect (m_sortButton, SIGNAL (clicked()), SLOT (showSortMenu()));
    connect (m_playlistButton, SIGNAL (clicked()), SLOT (showPlaylistMenu()));

    connect (m_pl_control, SIGNAL (nextClicked()), SIGNAL (next()));
    connect (m_pl_control, SIGNAL (previousClicked()), SIGNAL (prev()));
    connect (m_pl_control, SIGNAL (playClicked()), SIGNAL (play()));
    connect (m_pl_control, SIGNAL (pauseClicked()), SIGNAL (pause()));
    connect (m_pl_control, SIGNAL (stopClicked()), SIGNAL (stop()));
    connect (m_pl_control, SIGNAL (ejectClicked()), SIGNAL (eject()));

    connect (m_pl_manager, SIGNAL (selectedPlayListChanged(PlayListModel *, PlayListModel *)),
             m_listWidget, SLOT(setModel(PlayListModel*, PlayListModel*)));
    m_listWidget->setModel(m_pl_manager->selectedPlayList());

    createMenus();
    createActions();
    readSettings();

    m_titleBar = new PlayListTitleBar (this);
    m_titleBar->setMinimumSize(0,0);
    m_titleBar->move (0,0);
    connect (m_pl_manager, SIGNAL (currentPlayListChanged(PlayListModel *, PlayListModel *)),
             m_titleBar, SLOT(setModel(PlayListModel*)));
    m_titleBar->setModel(m_pl_manager->currentPlayList());

    setCursor(m_skin->getCursor(Skin::CUR_PNORMAL));
    updatePositions();
}

PlayList::~PlayList()
{
    delete m_keyboardManager;
}

void PlayList::updatePositions()
{
    int sx = (width()-275*m_ratio)/25;
    int sy = (height()-116*m_ratio)/29;
    if (sx < 0 || sy < 0 || m_shaded) //skip shaded mode
        return;

    m_titleBar->resize (275*m_ratio+25*sx, 20*m_ratio);
    m_plslider->resize (20*m_ratio, 58*m_ratio+sy*29);

    int pl_x = 12*m_ratio;
    int pl_y = 20*m_ratio;
    int pl_w = 243*m_ratio+25*sx;
    int pl_h = 58*m_ratio+29*sy;

    if(m_pl_selector)
    {
        m_pl_selector->resize(243*m_ratio+25*sx, m_pl_selector->height());
        m_pl_selector->move(12*m_ratio, 20*m_ratio + 58*m_ratio+29*sy - m_pl_selector->height());
        pl_h -= m_pl_selector->height();
    }

    m_listWidget->resize (pl_w, pl_h);
    m_listWidget->move (pl_x, pl_y);

    m_buttonAdd->move (11*m_ratio, 86*m_ratio+29*sy);
    m_buttonSub->move (40*m_ratio, 86*m_ratio+29*sy);
    m_selectButton->move (70*m_ratio, 86*m_ratio+29*sy);
    m_sortButton->move (99*m_ratio, 86*m_ratio+29*sy);

    m_pl_control->move (128*m_ratio+sx*25, 100*m_ratio+29*sy);
    m_playlistButton->move (228*m_ratio+sx*25, 86*m_ratio+29*sy);

    m_length_totalLength -> move (132*m_ratio+sx*25, 88*m_ratio+29*sy);
    m_current_time->move (191*m_ratio+sx*25,101*m_ratio+29*sy);

    m_plslider->move (255*m_ratio+sx*25,20*m_ratio);
    m_resizeWidget->move(width() - 25, height() - 29);
}

void PlayList::createMenus()
{
    m_addMenu = new QMenu (this);
    m_subMenu = new QMenu (this);
    m_selectMenu = new QMenu (this);
    m_sortMenu = new QMenu (this);
    m_playlistMenu = new QMenu (this);
    m_copySelectedMenu = new QMenu (tr("&Copy Selection To"), m_listWidget->menu());
    m_copySelectedMenu->setIcon(QIcon::fromTheme("edit-copy"));
    connect(m_copySelectedMenu, SIGNAL(aboutToShow()),
            SLOT(generateCopySelectedMenu()));
    connect(m_copySelectedMenu, SIGNAL(triggered(QAction *)),
            SLOT(copySelectedMenuActionTriggered(QAction *)));
}

void PlayList::createActions()
{
    //add menu
    m_addMenu->addAction(SET_ACTION(ActionManager::PL_ADD_FILE, parent(), SLOT(addFile())));
    m_addMenu->addAction(SET_ACTION(ActionManager::PL_ADD_DIRECTORY, parent(), SLOT(addDir())));
    m_addMenu->addAction(SET_ACTION(ActionManager::PL_ADD_URL, parent(), SLOT(addUrl())));
    //sub menu
    m_subMenu->addAction(SET_ACTION(ActionManager::PL_REMOVE_SELECTED, m_pl_manager, SLOT(removeSelected())));
    m_subMenu->addAction(SET_ACTION(ActionManager::PL_REMOVE_ALL, m_pl_manager, SLOT(clear())));
    m_subMenu->addAction(SET_ACTION(ActionManager::PL_REMOVE_UNSELECTED, m_pl_manager,
                                SLOT(removeUnselected())));
    m_subMenu->addSeparator();
    m_subMenu->addAction(SET_ACTION(ActionManager::PL_REFRESH, m_pl_manager,
                                SLOT(refresh())));
    m_subMenu->addAction(SET_ACTION(ActionManager::PL_REMOVE_INVALID, m_pl_manager,
                                SLOT(removeInvalidTracks())));
    m_subMenu->addAction(SET_ACTION(ActionManager::PL_REMOVE_DUPLICATES, m_pl_manager,
                                SLOT(removeDuplicates())));
    //sort menu
    m_sortMenu->addAction(SET_ACTION(ActionManager::PL_SHOW_INFO, m_pl_manager, SLOT (showDetails ())));
    m_sortMenu->addSeparator();

    QMenu* sort_mode_menu = new QMenu (tr("Sort List"), m_sortMenu);
    sort_mode_menu->setIcon(QIcon::fromTheme("view-sort-ascending"));
    QSignalMapper* signalMapper = new QSignalMapper (this);
    QAction* titleAct = sort_mode_menu->addAction (tr ("By Title"));
    connect (titleAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (titleAct, PlayListModel::TITLE);

    QAction* albumAct = sort_mode_menu->addAction (tr ("By Album"));
    connect (albumAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (albumAct, PlayListModel::ALBUM);

    QAction* discnumberAct = sort_mode_menu->addAction (tr ("By Disc Number"));
    connect (discnumberAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (discnumberAct, PlayListModel::DISCNUMBER);

    QAction* artistAct = sort_mode_menu->addAction (tr ("By Artist"));
    connect (artistAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (artistAct, PlayListModel::ARTIST);

    QAction* albumArtistAct = sort_mode_menu->addAction (tr ("By Album Artist"));
    connect (albumArtistAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (albumArtistAct, PlayListModel::ALBUMARTIST);

    QAction* nameAct = sort_mode_menu->addAction (tr ("By Filename"));
    connect (nameAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (nameAct, PlayListModel::FILENAME);

    QAction* pathnameAct = sort_mode_menu->addAction (tr ("By Path + Filename"));
    connect (pathnameAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (pathnameAct, PlayListModel::PATH_AND_FILENAME);

    QAction* dateAct = sort_mode_menu->addAction (tr ("By Date"));
    connect (dateAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (dateAct, PlayListModel::DATE);

    QAction* trackAct = sort_mode_menu->addAction (tr("By Track Number"));
    connect (trackAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (trackAct, PlayListModel::TRACK);

    QAction* fileCreationDateAct = sort_mode_menu->addAction (tr("By File Creation Date"));
    connect (fileCreationDateAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (fileCreationDateAct, PlayListModel::FILE_CREATION_DATE);

    QAction* fileModificationDateAct = sort_mode_menu->addAction (tr("By File Modification Date"));
    connect (fileModificationDateAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (fileModificationDateAct, PlayListModel::FILE_MODIFICATION_DATE);

    QAction* groupAct = sort_mode_menu->addAction (tr("By Group"));
    connect (groupAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (groupAct, PlayListModel::GROUP);

    connect (signalMapper, SIGNAL (mapped (int)), m_pl_manager, SLOT (sort (int)));

    m_sortMenu->addMenu (sort_mode_menu);

    sort_mode_menu = new QMenu (tr("Sort Selection"), m_sortMenu);
    sort_mode_menu->setIcon(QIcon::fromTheme("view-sort-ascending"));
    signalMapper = new QSignalMapper (this);
    titleAct = sort_mode_menu->addAction (tr ("By Title"));
    connect (titleAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (titleAct, PlayListModel::TITLE);

    albumAct = sort_mode_menu->addAction (tr ("By Album"));
    connect (albumAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (albumAct, PlayListModel::ALBUM);

    discnumberAct = sort_mode_menu->addAction (tr ("By Disc Number"));
    connect (discnumberAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (discnumberAct, PlayListModel::DISCNUMBER);

    artistAct = sort_mode_menu->addAction (tr ("By Artist"));
    connect (artistAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (artistAct, PlayListModel::ARTIST);

    albumArtistAct = sort_mode_menu->addAction (tr ("By Album Artist"));
    connect (albumArtistAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (albumArtistAct, PlayListModel::ALBUMARTIST);

    nameAct = sort_mode_menu->addAction (tr ("By Filename"));
    connect (nameAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (nameAct, PlayListModel::FILENAME);

    pathnameAct = sort_mode_menu->addAction (tr ("By Path + Filename"));
    connect (pathnameAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (pathnameAct, PlayListModel::PATH_AND_FILENAME);

    dateAct = sort_mode_menu->addAction (tr ("By Date"));
    connect (dateAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (dateAct, PlayListModel::DATE);

    trackAct = sort_mode_menu->addAction (tr ("By Track Number"));
    connect (trackAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (trackAct, PlayListModel::TRACK);

    fileCreationDateAct = sort_mode_menu->addAction (tr("By File Creation Date"));
    connect (fileCreationDateAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (fileCreationDateAct, PlayListModel::FILE_CREATION_DATE);

    fileModificationDateAct = sort_mode_menu->addAction (tr("By File Modification Date"));
    connect (fileModificationDateAct, SIGNAL (triggered (bool)), signalMapper, SLOT (map()));
    signalMapper->setMapping (fileModificationDateAct, PlayListModel::FILE_MODIFICATION_DATE);

    connect (signalMapper, SIGNAL (mapped (int)), m_pl_manager, SLOT (sortSelection (int)));

    m_sortMenu->addMenu (sort_mode_menu);
    m_sortMenu->addSeparator();
    m_sortMenu->addAction (QIcon::fromTheme("media-playlist-shuffle"), tr("Randomize List"),
                           m_pl_manager, SLOT(randomizeList()));
    m_sortMenu->addAction (QIcon::fromTheme("view-sort-descending"), tr("Reverse List"),
                           m_pl_manager, SLOT(reverseList()));

    m_sortMenu->addAction(SET_ACTION(ActionManager::PL_GROUP_TRACKS, m_ui_settings, SLOT(setGroupsEnabled(bool))));
    m_sortMenu->addAction(ACTION(ActionManager::PL_SHOW_HEADER));
    ACTION(ActionManager::PL_GROUP_TRACKS)->setChecked(m_ui_settings->isGroupsEnabled());
    //playlist context menu
    m_listWidget->menu()->addAction(ActionManager::instance()->action(ActionManager::PL_SHOW_INFO));
    m_listWidget->menu()->addSeparator();
    m_listWidget->menu()->addActions (m_subMenu->actions().mid(0,3)); //use 3 first actions
    m_listWidget->menu()->addMenu(UiHelper::instance()->createMenu(UiHelper::PLAYLIST_MENU,
                                  tr("Actions"), this));
    m_listWidget->menu()->addSeparator();
    m_listWidget->menu()->addAction(SET_ACTION(ActionManager::PL_ENQUEUE, m_pl_manager, SLOT(addToQueue())));
    //select menu
    m_selectMenu->addAction(SET_ACTION(ActionManager::PL_INVERT_SELECTION, m_pl_manager,
                                   SLOT(invertSelection ())));
    m_selectMenu->addSeparator();
    m_selectMenu->addAction(SET_ACTION(ActionManager::PL_CLEAR_SELECTION, m_pl_manager,
                                   SLOT(clearSelection ())));
    m_selectMenu->addAction(SET_ACTION(ActionManager::PL_SELECT_ALL, m_pl_manager, SLOT(selectAll())));
    //Playlist Menu
    m_playlistMenu->addAction(SET_ACTION(ActionManager::PL_NEW, m_pl_manager, SLOT(createPlayList())));
    m_playlistMenu->addAction(SET_ACTION(ActionManager::PL_CLOSE, this, SLOT(deletePlaylist())));
    m_playlistMenu->addAction(SET_ACTION(ActionManager::PL_RENAME, this, SLOT(renamePlaylist())));
    m_playlistMenu->addSeparator();
    m_playlistMenu->addAction(SET_ACTION(ActionManager::PL_LOAD, this, SIGNAL(loadPlaylist())));
    m_playlistMenu->addAction(SET_ACTION(ActionManager::PL_SAVE, this, SIGNAL(savePlaylist())));
    m_playlistMenu->addSeparator();
    m_playlistMenu->addAction(SET_ACTION(ActionManager::PL_SELECT_NEXT, m_pl_manager,
                                     SLOT(selectNextPlayList())));
    m_playlistMenu->addAction(SET_ACTION(ActionManager::PL_SELECT_PREVIOUS, m_pl_manager,
                                     SLOT(selectPreviousPlayList())));
    m_playlistMenu->addAction(SET_ACTION(ActionManager::PL_SHOW_MANAGER, this, SLOT(showPlayLists())));
}

void PlayList::closeEvent (QCloseEvent *e)
{
    if (e->spontaneous ())
        parentWidget()->close();
}

void PlayList::paintEvent (QPaintEvent *)
{
    int sx = (width()-275*m_ratio) /25;
    int sy = (height()-116*m_ratio) /29;

    QPainter paint(this);
    drawPixmap (&paint, 0, 20*m_ratio, m_skin->getPlPart (Skin::PL_LFILL));
    for (int i = 1; i<sy+2*m_ratio; i++)
    {
        drawPixmap (&paint, 0, 20*m_ratio+29*i, m_skin->getPlPart (Skin::PL_LFILL));
    }
    drawPixmap (&paint, 0, 78*m_ratio+29*sy, m_skin->getPlPart (Skin::PL_LSBAR));

    for (int i = 0; i<sx; i++)
    {
        drawPixmap (&paint, 125*m_ratio+i*25,78*m_ratio+sy*29,m_skin->getPlPart (Skin::PL_SFILL1));
    }
    drawPixmap (&paint,125*m_ratio+sx*25,78*m_ratio+sy*29,m_skin->getPlPart (Skin::PL_RSBAR));

}

void PlayList::drawPixmap (QPainter *painter, int x, int y, const QPixmap &pix)
{
    style()->drawItemPixmap(painter, QRect(x, y, pix.width(), pix.height()), Qt::AlignCenter, pix);
}

void PlayList::resizeEvent (QResizeEvent *)
{
    updatePositions();
}

void PlayList::mousePressEvent (QMouseEvent *e)
{
    Q_UNUSED(e);
    if (m_resizeWidget->underMouse())
    {
        m_resize = true;
        setCursor (m_skin->getCursor (Skin::CUR_PSIZE));
    }
    else
        m_resize = false;
}

void PlayList::mouseMoveEvent (QMouseEvent *e)
{
    if (m_resize)
    {
#ifdef Q_WS_X11
        if(m_compiz)
#endif
        {
            int dx = m_ratio * 25;
            int dy = m_ratio * 29;

            int sx = ((e->x() - 275 * m_ratio) + 14) / dx;
            int sy = ((e->y() - 116 * m_ratio) + 14) / dy;

            sx = qMax(sx, 0);
            sy = qMax(sy, 0);

#ifdef Q_OS_WIN
            resize(275 * m_ratio + dx * sx, 116 * m_ratio + dy * sy);
#else
            setFixedSize(275 * m_ratio + dx * sx, 116 * m_ratio + dy * sy);
#endif
        }
#ifdef Q_WS_X11
        else
            resize (e->x() +25, e->y() +25);
#endif


#ifdef Q_WS_X11
        //avoid right corner moving during resize
        if(layoutDirection() == Qt::RightToLeft)
            WindowSystem::revertGravity(winId());
#endif

    }
}

void PlayList::mouseReleaseEvent (QMouseEvent *)
{
    setCursor (m_skin->getCursor (Skin::CUR_PNORMAL));
    /*if (m_resize)
        m_listWidget->updateList();*/
    m_resize = false;
    Dock::instance()->updateDock();
}

void PlayList::changeEvent (QEvent * event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        m_titleBar->setActive (isActiveWindow());
    }
}

void PlayList::readSettings()
{
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    if (settings.value("Skinned/pl_show_plalists", false).toBool())
    {
        if(!m_pl_selector)
            m_pl_selector = new PlayListSelector(m_pl_manager, this);
        m_pl_selector->show();
        m_listWidget->menu()->insertMenu(m_listWidget->menu()->actions().at(2),m_copySelectedMenu);
    }
    else
    {
        if(m_pl_selector)
        {
            m_pl_selector->deleteLater();
            m_listWidget->menu()->removeAction(m_copySelectedMenu->menuAction());
        }
        m_pl_selector = 0;
    }

    if (m_update)
    {
        m_listWidget->readSettings();
        m_titleBar->readSettings();
        if(m_pl_selector)
            m_pl_selector->readSettings();
        updatePositions();
    }
    else
    {
        move (settings.value ("Skinned/pl_pos", QPoint (100, 332)).toPoint());  //position
        m_update = true;
    }
}

#ifdef Q_WS_X11
bool PlayList::event (QEvent *event)
{
    if(event->type() == QEvent::WinIdChange || event->type() == QEvent::Show)
    {
        WindowSystem::ghostWindow(winId());
        WindowSystem::setWinHint(winId(), "playlist", "Qmmp");
    }
    return QWidget::event(event);
}
#endif

void PlayList::writeSettings()
{
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    //position
    settings.setValue ("Skinned/pl_pos", this->pos());
}

#ifdef Q_WS_X11
bool PlayList::useCompiz() const
{
    return m_compiz;
}
#endif

void PlayList::showAddMenu()
{
    m_addMenu->exec (m_buttonAdd->mapToGlobal (QPoint (0,0)));
}

void PlayList::showSubMenu()
{
    m_subMenu->exec (m_buttonSub->mapToGlobal (QPoint (0,0)));
}

void PlayList::showSelectMenu()
{
    m_selectMenu->exec (m_selectButton->mapToGlobal (QPoint (0,0)));
}

void PlayList::showSortMenu()
{
    m_sortMenu->exec (m_sortButton->mapToGlobal (QPoint (0,0)));
}

QString PlayList::formatTime (int sec)
{
    if(sec >= 3600)
        sec /= 60;
    return QString("%1:%2").arg(sec/60, 2, 10, QChar('0')).arg(sec%60, 2, 10, QChar('0'));
}

void PlayList::setTime(qint64 time)
{
    if (time < 0)
        m_current_time->display ("--:--");
    else
        m_current_time->display (formatTime (time/1000));
    m_current_time->update();

    if (SoundCore::instance())
    {
        QString str_length = formatTime (m_pl_manager->currentPlayList()->totalLength()) +
                             "/" + formatTime (SoundCore::instance()->totalTime()/1000);
        m_length_totalLength->display (str_length);
        m_length_totalLength->update();
    }
}

void PlayList::showPlaylistMenu()
{
    m_playlistMenu->exec (m_playlistButton->mapToGlobal (QPoint (0,0)));
}

void PlayList::keyPressEvent (QKeyEvent *ke)
{
    m_keyboardManager->handleKeyPress (ke);
}

void PlayList::updateSkin()
{
    setCursor(m_skin->getCursor(Skin::CUR_PNORMAL)); // TODO shaded
    m_resizeWidget->setCursor(m_skin->getCursor (Skin::CUR_PSIZE));
    m_ratio = m_skin->ratio();
    setMinimalMode(m_shaded);
}

void PlayList::deletePlaylist()
{
    m_pl_manager->removePlayList(m_pl_manager->selectedPlayList());
}

void PlayList::renamePlaylist()
{
    bool ok = false;
    QString name = QInputDialog::getText (this,
                                          tr("Rename Playlist"), tr("Playlist name:"),
                                          QLineEdit::Normal,
                                          m_pl_manager->selectedPlayList()->name(), &ok);
    if(ok)
        m_pl_manager->selectedPlayList()->setName(name);
}

void PlayList::showPlayLists()
{
    if(!m_pl_browser)
    {
        m_pl_browser = new PlayListBrowser(m_pl_manager, this);
        m_pl_browser->show();
    }
    else
        m_pl_browser->show();
}

void PlayList::generateCopySelectedMenu()
{
    m_copySelectedMenu->clear();
    QAction* action = m_copySelectedMenu->addAction (tr ("&New PlayList"));
    action->setIcon(QIcon::fromTheme("document-new"));
    m_copySelectedMenu->addSeparator();

    foreach(QString name, m_pl_manager->playListNames())
    {
        m_copySelectedMenu->addAction("&"+name.replace("&", "&&"));
    }
}

void PlayList::copySelectedMenuActionTriggered(QAction *action)
{
    PlayListModel *targetPlayList = 0;
    QString actionText=action->text();
    if(action == m_copySelectedMenu->actions().at(0))//actionText == tr ("&New PlayList"))
    {
        targetPlayList = m_pl_manager->createPlayList(m_pl_manager->selectedPlayList()->name());
    }
    else
    {
        actionText.remove(0,1).replace("&&", "&");
        foreach(PlayListModel *model, m_pl_manager->playLists())
        {
            //if("&" + model->name().replace("&", "&&") == actionText)
            if(model->name() == actionText)
            {
                targetPlayList = model;
                break;
            }
        }
    }
    if(!targetPlayList)
    {
        qWarning("Error: Cannot find target playlist '%s'",qPrintable(actionText));
        return;
    }
    QList <PlayListTrack *> theCopy;
    foreach(PlayListTrack *track, m_pl_manager->selectedPlayList()->selectedTracks())
    {
        PlayListTrack *newItem = new PlayListTrack(*track);
        theCopy << newItem;
    }
    targetPlayList->add(theCopy);
}

void PlayList::setMinimalMode(bool b)
{
    if(!m_shaded)
        m_height = height();
    m_shaded = b;

#ifdef Q_WS_X11
    if(m_compiz)
    {
        if(m_shaded)
        {
            m_height = height();
            setFixedSize(qMax(width(), 275 * m_ratio), 14*m_ratio);
        }
        else
        {
            setFixedSize(qMax(width(), 275 * m_ratio), qMax(m_height, 116*m_ratio));
        }
    }
    else
#endif
    {
        if(m_shaded)
        {
            m_height = height();
            setSizeIncrement (25*m_ratio, 1);
            setMinimumSize (275*m_ratio, 14*m_ratio);
            resize(width(), 14*m_ratio);
        }
        else
        {
            setMinimumSize(275*m_ratio, 116*m_ratio);
            resize (width(), m_height);
            setSizeIncrement (25*m_ratio, 29*m_ratio);
        }
    }
    updatePositions();
    update();
}
