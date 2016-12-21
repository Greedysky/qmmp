/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
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
#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QObject>
#include "playlistheadermodel.h"
#include "playlistmodel.h"

class QTimer;
class QmmpUiSettings;

/*! @brief The PlayListManager class is used to handle multiple playlists.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class PlayListManager : public QObject
{
Q_OBJECT
public:
    /*!
     * Constructor.
     * @param parent Parent object.
     */
    PlayListManager(QObject *parent);
    /*!
     * Destructor.
     */
    ~PlayListManager();
    /*!
     * Returns a pointer to the object's instance.
     */
    static PlayListManager* instance();
    /*!
     * Returns a list of all playlists.
     */
    QList <PlayListModel *> playLists() const;
    /*!
     * Returns a names of all playlists.
     */
    QStringList playListNames() const;
    /*!
     * Returns selected playlist.
     */
    PlayListModel *selectedPlayList() const;
    /*!
     * Returns active playlist.
     */
    PlayListModel *currentPlayList() const;
    /*!
     * Returns selected playlist index.
     */
    int selectedPlayListIndex() const;
    /*!
     * Returns active playlist index.
     */
    int currentPlayListIndex() const;
    /*!
     * Returns a number of playlists.
     */
    int count() const;
    /*!
     * Returns the index position of the playlist \b model.
     */
    int indexOf(PlayListModel *model) const;
    /*!
     * Returns the playlist at index position \b i in the list.
     * \b i must be a valid index position in the list (i.e., 0 <= i < count()).
     */
    PlayListModel *playListAt(int i) const;
    /*!
     * Returns playlist header model.
     */
    PlayListHeaderModel *headerModel();

signals:
    /*!
     * Emitted when current playlist changes.
     * @param current Current playlist.
     * @param previous Previous playlist.
     */
    void currentPlayListChanged (PlayListModel *current, PlayListModel *previous);
    /*!
     * Emitted when selected playlist changes.
     * @param selected Selected playlist.
     * @param previous Previous selected playlist.
     */
    void selectedPlayListChanged (PlayListModel *selected, PlayListModel *previous);
    /*!
     * Emitted when the playlist with index \b index is added.
     */
    void playListAdded(int index);
    /*!
     * Emitted when the playlist with index \b index is removed.
     */
    void playListRemoved(int index);
    /*!
     * Emitted when playlist changes its position from \b i to  \b j.
     */
    void playListMoved(int i, int j);
    /*!
     * Emitted when the list of playlists is changed.
     */
    void playListsChanged();

public slots:
    /*!
     * Selects playlist \b model.
     */
    void selectPlayList(PlayListModel *model);
    /*!
     * Selects playlist with index \b index.
     */
    void selectPlayList(int index);
    /*!
     * Selects playlist with name \b name.
     */
    void selectPlayList(const QString &name);
    /*!
     * Selects next playlist if possible.
     */
    void selectNextPlayList();
    /*!
     * Selects previous playlist if possible.
     */
    void selectPreviousPlayList();
    /*!
     * Sets current playlist to \b model.
     */
    void activatePlayList(PlayListModel *model);
    /*!
     * Sets current playlist with index \b index.
     */
    void activatePlayList(int index);
    /*!
     * Creates and selects new playlist with the given name \b name.
     */
    PlayListModel *createPlayList(const QString &name = QString());
    /*!
     * Removes playlist \b model.
     */
    void removePlayList(PlayListModel *model);
    /*!
     * Removes playlist with index \b index.
     */
    void removePlayList(int index);
    /*!
     * Moves playlist with index \b i to index \b j.
     */
    void move(int i, int j);
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->clear()
     */
    void clear();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->clearSelection()
     */
    void clearSelection();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->removeSelected()
     */
    void removeSelected();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->removeUnselected()
     */
    void removeUnselected();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->removeAt(i)
     */
    void removeTrack (int i);
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->removeItem(item)
     */
    void removeTrack (PlayListTrack *item);
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->invertSelection()
     */
    void invertSelection();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->selectAll()
     */
    void selectAll();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->showDetails()
     */
    void showDetails();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->add(paths)
     */
    void add(const QStringList &paths);
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->randomizeList()
     */
    void randomizeList();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->reverseList()
     */
    void reverseList();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->sortSelection(mode)
     */
    void sortSelection(int mode);
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->sort(mode)
     */
    void sort(int mode);
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->addToQueue()
     */
    void addToQueue();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->clearInvalidItems()
     */
    void removeInvalidTracks();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->removeDuplicates()
     */
    void removeDuplicates();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->refresh()
     */
    void refresh();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->clearQueue()
     */
    void clearQueue();
    /*!
     * This is a convenience function and is the same as calling \b selectedPlayList()->stopAfterSelected()
     */
    void stopAfterSelected();

private slots:
    void writePlayLists();
    void onListChanged(int flags);

private:
    void readPlayLists();
    static PlayListManager* m_instance;
    QList <PlayListModel *> m_models;
    PlayListModel *m_current;
    PlayListModel *m_selected;
    QTimer *m_timer;
    PlayListHeaderModel *m_header;
    QmmpUiSettings *m_ui_settings;
};

#endif // PLAYLISTMANAGER_H
