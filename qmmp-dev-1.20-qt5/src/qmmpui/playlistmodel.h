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
#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QQueue>
#include <QPointer>
#include <QVector>
#include "playlistitem.h"
#include "playlisttrack.h"
#include "playlistgroup.h"

class FileLoader;
class PlayState;
class PlayListFormat;
class PlayListModel;
class PlayListContainer;
class QmmpUiSettings;
class PlayListTask;

/*! @brief Helper class that keeps track of a view's selected items.
 *
 * @author Vladimir Kuznetsov <vovanec@gmail.com>
 */
struct SimpleSelection
{
    /*!
     * Constructs an empty selection model.
     */
    SimpleSelection()
    {
        m_bottom = -1;
        m_top = 1;
    }
    /*!
     * Returns \p true if this selection is valid; otherwise returns returns \p false.
     */
    inline bool isValid()const
    {
        return (m_bottom != -1) && (m_top != -1);
    }
    /*!
     * Prints object state.
     */
    inline void dump()const
    {
        qWarning("top: %d\tbotom: %d", m_top, m_bottom);
    }
    /*!
     * Returns number of selected items.
     */
    inline int count()const
    {
        return m_bottom - m_top + 1;
    }
    int m_bottom;               /*!< Bottom of the selection */
    int m_top;                  /*!< Top of the selection   */
    QList<int> m_selected_indexes;  /*!< Selected items numbers */
};
/*! @brief The PlayListModel class provides a data model for the playlist.
 *
 * @author Vladimir Kuznetsov <vovanec@gmail.com>
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 *
 *         Playlist Structure
 *      -----------------------
 *     | item  | track | item  |
 *     | index | index | type  |
 *      -----------------------
 *     |  0    |       | group |
 *     |  1    |  0    | track |
 *     |  2    |  1    | track |
 *     |  3    |  2    | track |
 *     |  4    |  3    | track |
 *     |  5    |       | group |
 *     |  6    |  4    | track |
 *     |  7    |  5    | track |
 *     |  8    |  6    | track |
 */
class PlayListModel : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a playlist model.
     * @param name Playlist name.
     * @param parent QObject parent
     */
    PlayListModel(const QString &name, QObject *parent = 0);
    /*!
     * Object destructor.
     */
    ~PlayListModel();
    /*!
     * Returns playlist name.
     */
    QString name() const;
    /*!
     * Sets the name of the playlist to \b name.
     */
    void setName(const QString &name);
    /*!
     * Returns number of items.
     */
    int count() const;
    /*!
     * Returns number of tracks.
     */
    int trackCount() const;
    /*!
     * Returns \b true if the model contains no tracks; otherwise returns \b false.
     */
    bool isEmpty() const;
    /*!
     * Returns number of columns.
     */
    int columnCount() const;
    /*!
     * Returns the current track.
     */
    PlayListTrack* currentTrack() const;
    /*!
     * Returns the next playing track or 0 if next track is unknown.
     */
    PlayListTrack* nextTrack() const;
    /*!
     * Returns the row of the \b item
     */
    int indexOf(PlayListItem* item) const;
    /*!
     * Returns the item with the index \b index or 0 if item doesn't exist.
     */
    PlayListItem* item(int index) const;
    /*!
     * Returns the track with the index \b index or 0 if track doesn't exist.
     */
    PlayListTrack* track(int index) const;
    /*!
     * Returns the group with the index \b index or 0 if group doesn't exist.
     */
    PlayListGroup* group(int index) const;
    /*!
     * Returns index of the current item.
     */
    int currentIndex() const;
    /*!
     * Sets current index.
     * Returns \b false if item with this index doesn't exist, otherwise returns \b true
     * @param index Number of item.
     */
    bool setCurrent (int index);
    /*!
     * Sets current item to \b item.
     * Returns \b true if success, otherwise returns \b false
     */
    bool setCurrent(PlayListTrack *item);
    /*!
     * Returns \b true if a playlist contains track with the index \b index.
     * Otherwise returns \b false.
     */
    bool isTrack(int index) const;
    /*!
     * Returns \b true if a playlist contains group separator with the index \b index.
     * Otherwise returns \b false.
     */
    bool isGroup(int index) const;
    /*!
     * Returns \b true if item with \b index is selected, otherwise returns \b false
     */
    bool isSelected(int index) const;
    /*!
     * Sets the selected state of the item to \b select
     * @param index Number of item.
     * @param selected Selection state (\b true - select, \b false - unselect)
     */
    void setSelected(int index, bool selected = true);
    /*!
     * Sets the selected state of the list of tracks to \b select
     * @param tracks List of tracks.
     * @param selected Selection state (\b true - select, \b false - unselect)
     */
    void setSelected(QList<PlayListTrack *> tracks, bool selected = true);
    /*!
     * Sets the selected state of the list of items to \b select
     * @param items List of items.
     * @param selected Selection state (\b true - select, \b false - unselect)
     */
    void setSelected(QList<PlayListItem *> items, bool selected = true);
    /*!
     * Sets the selected state of the list of items range.
     * @param first Firts item in the range.
     * @param last Last item in the range.
     * @param selected Selection state (\b true - select, \b false - unselect).
     */
    void setSelected(int first, int last, bool selected = true);
    /*!
     * Advances to the next item. Returns \b false if next iten doesn't exist,
     * otherwise returns \b true
     */
    bool next();
    /*!
     * Goes back to the previous item. Returns \b false if previous iten doesn't exist,
     * otherwise returns \b true
     */
    bool previous();
    /*!
     * Returns a list of the items, starting at position \b pos
     * \param pos First item position.
     * \param count A number of items. If \b count is -1 (the default), all items from pos are returned.
     */
    QList<PlayListItem *> mid(int pos, int count = -1) const;
    /*!
     *  Moves the item at index position \b from to index position \b to.
     */
    void moveItems(int from, int to);
    /*!
     * Returns \b true if \b f file is in play queue, otherwise returns \b false.
     */
    bool isQueued(PlayListTrack* item) const;
    /*!
     * Returns \b true if play queue is empty,otherwise returns - \b false.
     */
    bool isEmptyQueue()const;
    /*!
     * Returns index of \b f file in queue.e
     */
    int queuedIndex(PlayListTrack* track) const;
    /*!
     * Returns the number of items in the queue
     */
    int queueSize() const;
    /*!
     * Returns \b true if playback stops after \b item, otherwise returns \b false.
     */
    bool isStopAfter(PlayListItem* item) const;
    /*!
     * Returns current selection(playlist can contain a lot of selections,
     * this method returns selection which \b row belongs to)
     */
    const SimpleSelection& getSelection(int row);
    /*!
     * Returns list with selected items indexes.
     */
    QList<int> selectedIndexes() const;
    /*!
     * Returns list of \b PlayListItem pointers that are selected.
     */
    QList<PlayListTrack *> selectedTracks() const;
    /*!
     * Returns list of all \b PlayListItem pointers.
     */
    QList<PlayListItem*> items() const;
    /*!
     * Returns number of first item that selected upper the \b row item.
     */
    int firstSelectedUpper(int row);
    /*!
     * Returns number of first item that selected lower the \b row item.
     */
    int firstSelectedLower(int row);
    /*!
     * Returns total lenght in seconds of all songs.
     */
    int totalLength() const;
    /*!
     * Loads playlist with \b f_name name.
     */
    void loadPlaylist(const QString& f_name);
    /*!
     * Saves current songs to the playlist with \b f_name name.
     */
    void savePlaylist(const QString& f_name);
    /*!
     * Returns \b true if the file loader thread is active; otherwise returns \b false.
     */
    bool isLoaderRunning() const;
    /*!
     * Returns \b true if the playlist contains an item with URL \b url; otherwise returns \b false.
     */
    bool contains(const QString &url);
    /*!
     * Enum of the available sort modes.
     */
    enum SortMode
    {
        TITLE = 0,              /*!< by title */
        ALBUM,                  /*!< by album */
        DISCNUMBER,             /*!< by discnumber */
        ARTIST,                 /*!< by artist */
        ALBUMARTIST,            /*!< by album artist */
        FILENAME,               /*!< by file name */
        PATH_AND_FILENAME,      /*!< by path and file name */
        DATE,                   /*!< by date */
        TRACK,                  /*!< by track */
        FILE_CREATION_DATE,     /*!< by file creation date */
        FILE_MODIFICATION_DATE, /*!< by file modification date */
        GROUP                   /*!< by group name */
    };
    /*!
     * Converts item index \b index to track index
     * Returns \b -1 if item with index \b index is not a track or does not exist.
     */
    int indexOfTrack(int index) const;
    /*!
     * Finds track with index \b track_index.
     * Return null pointer if playlist does not contain track with index \b track_index.
     */
    PlayListTrack *findTrack(int track_index) const;
    /*!
     * Enum of the playlist update flags.
     */
    enum UpdateFlags
    {
        STRUCTURE = 0x01,  /*!< Structure of the playlist has been changed */
        SELECTION = 0x02,  /*!< Current selection has been changed */
        QUEUE = 0x04,      /*!< Track queue has been changed */
        CURRENT = 0x08,    /*!< Current track has been changed */
        STOP_AFTER = 0x10, /*!< Stop track has been changed */
        METADATA = 0x20    /*!< Metadata has been changed */
    };

signals:
    /*!
     * Emitted when the state of PlayListModel has changed.
     * @param flags Playlist updated flags. See \b UpdateFlags enum for details.
     */
    void listChanged(int flags);
    /*!
     * Emitted when new track has added.
     * @param track Pointer of the new playlist track.
     */
    void trackAdded(PlayListTrack *track);
    /*!
     * Emitted when playlist name has chanded.
     * @param name New playlist name.
     */
    void nameChanged(const QString& name);
    /*!
     * Emitted when playlist loader thread has finished.
     */
    void loaderFinished();
    /*!
     * Emitted to ensure that the current track is visible.
     */
    void currentVisibleRequest();
    /*!
     * Emitted when sorting by column is finished.
     * @param column Column index.
     * @param reverted Sort direction.
     */
    void sortingByColumnFinished(int column, bool reverted);

public slots:
    /*!
     * Adds \b track to the playlist.
     */
    void add(PlayListTrack *track);
    /*!
     * Adds a list of tracks to the playlist.
     * @param tracks List of tracks.
     */
    void add(QList <PlayListTrack *> tracks);
    /*!
     * Adds a list of files and directories to the playlist
     * @param path Full path of file or directory.
     */
    void add(const QString &path);
    /*!
     * Adds a list of files and directories to the playlist
     * @param paths Full paths of files and directories.
     */
    void add(const QStringList &paths);
    /*!
     * Inserts \b track at index position \b index in the playlist.
     */
    void insert(int index, PlayListTrack *track);
    /*!
     * Inserts \b track before playlist item \b before.
     */
    void insert(PlayListItem *before, PlayListTrack *track);
    /*!
     * Inserts \b tracks at index position \b index in the playlist.
     */
    void insert(int index, QList <PlayListTrack *> tracks);
    /*!
     * Inserts \b tracks after item \b before in the playlist.
     */
    void insert(PlayListItem *before, QList <PlayListTrack *> tracks);
    /*!
     * Inserts file or directory at index position \b index in the playlist.
     * @param path Full path of file or directory.
     * @param index Position in the playlist.
     */
    void insert(int index, const QString &path);
    /*!
     * Adds a list of files and directories at index position \b index in the playlist.
     * @param paths Full paths of files and directories.
     * @param index Position in the playlist.
     */
    void insert(int index, const QStringList &paths);
    /*!
     * Adds a list of URLs at index position \b index in the playlist.
     * @param urls A list of URLs.
     * @param index Position in the playlist.
     */
    void insert(int index, const QList<QUrl> &urls);
    /*!
     * Removes all items.
     */
    void clear();
    /*!
     * Clears selection.
     */
    void clearSelection();
    /*!
     * Removes selected items.
     */
    void removeSelected();
    /*!
     * Removes unselected items.
     */
    void removeUnselected();
    /*!
     * Removes items with \b i index.
     */
    void removeTrack (int i);
    /*!
     * Removes item \b item from playlist
     */
    void removeTrack (PlayListItem *track);
    /*!
     * Inverts selection (selects unselected items and unselects selected items)
     */
    void invertSelection();
    /*!
     * Selects all items.
     */
    void selectAll();
    /*!
     * Shows details for the first selected item.
     * @param parent parent Widget.
     */
    void showDetails(QWidget *parent = 0);
    /*!
     * Shows the details for the current song (if any).
     * @param parent parent widget.
     */
    void showDetailsForCurrent(QWidget *parent = 0);
    /*!
     * Ensures that the current track is visible.
     */
    void doCurrentVisibleRequest();
    /*!
     * Randomly changes items order.
     */
    void randomizeList();
    /*!
     * Reverces items order.
     */
    void reverseList();
    /*!
     * Sorts selected items in \b mode sort mode.
     */
    void sortSelection(int mode);
    /*!
     * Sorts items in \b mode sort mode.
     */
    void sort(int mode);
    /*!
     * Sorts tracks by the column with index \b column.
     */
    void sortByColumn(int column);
    /*!
     * Adds/removes selected items to/from playback queue.
     */
    void addToQueue();
    /*!
     * Adds/removes item \b f to/from playback queue.
     */
    void setQueued(PlayListTrack* item);
    /*!
     * Removes invalid tracks from playlist
     */
    void removeInvalidTracks();
    /*!
     * Removes duplicate tracks by URL.
     */
    void removeDuplicates();
    /*!
     * Removes invalid tracks and scans parent directories for the new files
     */
    void refresh();
    /*!
     * Removes all items from queue.
     */
    void clearQueue();
    /*!
     * Toggles 'stop after selected' feature.
     */
    void stopAfterSelected();
    /*!
     * Rebuilds groups
     */
    void rebuildGroups();
    /*!
     * Requires to update metadata.
     */
    void updateMetaData();

private:
    /*!
     * Returns topmost row in current selection
     */
    int topmostInSelection(int);
    /*!
     * Returns bottommost row in current selection
     */
    int bottommostInSelection(int);
    /*!
     * Removes items from model. If \b inverted is \b false -
     * selected items will be removed, else - unselected.
     */
    void removeSelection(bool inverted = false);

    int removeTrackInternal(int i);

private slots:
    /*!
     * Prepares play state object
     */
    void preparePlayState();
    /*!
     * Prepares model for shuffle playing. \b yes parameter is \b true - model iterates in shuffle mode.
     */
    void prepareForShufflePlaying(bool yes);
    /*!
     * Enabled/Disabled groped mode
     * @param enabled State of the groups (\b true - enabled, \b false - disabled)
     */
    void prepareGroups(bool enabled);

    void onTaskFinished();

private:
    PlayListTrack* m_current_track;
    PlayListTrack* m_stop_track;
    int m_current;
    SimpleSelection m_selection;  /*!< This flyweight object represents current selection. */
    QQueue <PlayListTrack*> m_queued_songs; /*!< Songs in play queue. */
    PlayState* m_play_state; /*!< Current playing state (Normal or Shuffle) */
    int m_total_length;
    FileLoader *m_loader;
    QString m_name;
    PlayListContainer *m_container;
    QmmpUiSettings *m_ui_settings;
    PlayListTask *m_task;
};

#endif
