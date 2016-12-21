/***************************************************************************
 *   Copyright (C) 2012-2015 by Ilya Kotov                                 *
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

#ifndef QMMPUISETTINGS_H
#define QMMPUISETTINGS_H

#include <QObject>
#include <QStringList>
#include "playlistheadermodel.h"
#include "metadataformatter.h"

class QTimer;
class MetaDataHelper;

/*! @brief The QmmpUiSettings class provides access to global libqmmpui library settings.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class QmmpUiSettings : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructor.
     * @param parent Parent object.
     * This functions is for internal usage only, use QmmpSettings::instance() instead.
     */
    explicit QmmpUiSettings(QObject *parent = 0);
    /*!
     * Destructor.
     */
    virtual ~QmmpUiSettings();
    /*!
     * Returns state of the "Convert underscores to blanks" option (\b true - enabled, \b false - disabled).
     */
    bool convertUnderscore() const;
    /*!
     * Returns state of the "Convert %20 to blanks" option (\b true - enabled, \b false - disabled).
     */
    bool convertTwenty() const;
    /*!
     * Returns the state of metadata usage (\b true - use, \b false - not use).
     */
    bool useMetadata() const;
    /*!
     * Returns playlist group format.
     */
    const QString groupFormat() const;
    /*!
     * Returns state of "Repeat All" option.
     */
    bool isRepeatableList() const;
    /*!
     * Returns state of "Shuffle" option.
     */
    bool isShuffle() const;
    /*!
     * Returns \b true if the playlist groups are enabled. Otherwise returns \b false.
     */
    bool isGroupsEnabled() const;
    /*!
     * Returns \b true if "Repeate Track" option is enabled, otherwise returns \b false
     */
    bool isRepeatableTrack() const;
    /*!
     * Returns \b true if "No playlist advance" option is enabled, otherwise returns \b false
     */
    bool isNoPlayListAdvance() const;
    /*!
     * Sets the "Convert underscores to blanks" option state to \b enabled
     * @param enabled Option state (\b true - enabled, \b false - disabled)
     */
    void setConvertUnderscore(bool enabled);
    /*!
     * Sets the "Convert %20 to blanks" option state to \b enabled
     * @param enabled Option state (\b true - enabled, \b false - disabled)
     */
    void setConvertTwenty(bool enabled);
    /*!
     * Sets group format.
     * \param groupFormat group format. \sa MetaDataFormatter
     */
    void setGroupFormat(const QString &groupFormat);
    /*!
     * Sets metadata usage option state to \b enabled
     * @param enabled Option state (\b true - enabled, \b false - disabled)
     */
    void setUseMetadata(bool enabled);
    /*!
     * Returns \b true if option "Resume On Startup" is enabled, otherwise returns \b false
     */
    bool resumeOnStartup() const;
    /*!
     * Sets the "Resume on startup" option state to \b enabled
     * @param enabled Option state (\b true - enabled, \b false - disabled)
     */
    void setResumeOnStartup(bool enabled);
    /*!
     * Sets the "Use clipboard in the URL dialog" option state to \b enabled
     * @param enabled Option state (\b true - enabled, \b false - disabled)
     */
    void setUseClipboard(bool enabled);
    /*!
     * Returns \b true if option "Use clipboard in the URL dialog" is enabled, otherwise returns \b false
     */
    bool useClipboard() const;
    /*!
     * Returns a list of filters which should be used for directory scanning.
     */
    QStringList restrictFilters() const;
    /*!
     * Disables all builtin directory scanning filters and sets them to \b filters (Example: *.mp3,*.ogg).
     */
    void setRestrictFilters(const QString &filters);
    /*!
     * Returns a list of filters which should be excluded from incoming files while directory scanning.
     */
    QStringList excludeFilters() const;
    /*!
     * Sets a list of filters which should be excluded from incoming files while directory scanning.
     * @param filters A set of filters separated by semicolon (Example: *.cue,*.ogg).
     */
    void setExcludeFilters(const QString &filters);
    /*!
     * Returns \b true if defaut playlist is enabled, othewise returns \b false.
     * Default playlist receives files from command line.
     */
    bool useDefaultPlayList() const;
    /*!
     * Returns default playlist name.
     */
    QString defaultPlayListName() const;
    /*!
     * Sets default playlist options.
     * @param name Default playlist name.
     * @param enabled Default playlist state (\b true - enabled, \b false - disabled).
     */
    void setDefaultPlayList(const QString &name, bool enabled = true);
    /*!
     * Enable/disable playlist auto-saving.
     * @param enabled New setting for the auto-save option (\b true to enable)
     */
    void setAutoSavePlayList(bool enabled);
    /*!
     * Return the current setting for the playlist auto-save option.
     * @return \b true if the playlist should be saved automatically, \b false
     *         otherwise.
     */
    bool autoSavePlayList() const;
    /*!
     * Enable/disable feature to clear previous playlist when opening new one.
     * @param enabled New setting for this option (\b true to enable)
     */
    void setClearPreviousPlayList(bool enabled);
    /*!
     * Return the current setting for the option to clear playlist  when opening new one.
     * @return \b true if the playlist should be saved automatically, \b false
     *         otherwise.
     */
    bool clearPreviousPlayList() const;
    /*!
     * Returns a pointer to the QmmpUiSettings instance.
     */
    static QmmpUiSettings* instance();

signals:
    /*!
     * Emitted when state of the "Repeat All" option has changed.
     * @param state New state of the "Repeat All" option (\b true - enabled, \b false disabled)
     */
    void repeatableListChanged(bool state);
    /*!
     * Emitted when state of the "Shuffle" option has changed.
     * @param state New state of the "Shuffle" option (\b true - enabled, \b false disabled)
     */
    void shuffleChanged(bool state);
    /*!
     * Emitted when state of the "Group tracks" option has changed.
     * @param state New state of the "Group tracks" option (\b true - enabled, \b false disabled
     */
    void groupsChanged(bool state);
    /*!
     * Tracks current track repeat state;
     * @param enabled New repeate state of the current track (\b true - enabled, \b false - disabled)
     */
    void repeatableTrackChanged(bool enabled);
    /*!
     * Emitted when state of the "No playlist advance" option changes.
     * @param enabled New state of this option (\b true - no playlist advance,
     * \b false - normal playlist behaviour)
     */
    void noPlayListAdvanceChanged(bool enabled);

public slots:
    /*!
     * Prepares all playlists for repeatable playing (loop mode).
     * @param r State of the repeatable mode (\b true - enabled, \b false - disabled)
     */
    void setRepeatableList(bool r);
    /*!
     * Prepares all playlists for shuffle playing.
     * @param s State of the shuffle mode (\b true - enabled, \b false - disabled)
     */
    void setShuffle(bool s);
    /*!
     * Enables or disables playlist groups
     * * @param enabled State of the groups (\b true - enabled, \b false - disabled)
     */
    void setGroupsEnabled(bool enabled);
    /*!
     * Toggles the current track repeat.
     * @param enabled Repeate state of the current track (\b true - to repeat, \b false - to stop repeating)
     */
    void setRepeatableTrack(bool enabled);
    /*!
     * When finished playing a song, don't automatically advance to the next
     * @param enabled State of the 'No playlist advance' option
     * (\b true - enabled, \b false - normal playback)
     */
    void setNoPlayListAdvance(bool enabled);

private slots:
    /*!
     * Writes all unsaved settings to configuration file
     */
    void sync();


private:
    static QmmpUiSettings* m_instance;
    //playlist
    QString m_group_format;
    bool m_convertUnderscore, m_convertTwenty;
    bool m_useMetadata;
    bool m_autosave_playlist;
    bool m_repeate_list;
    bool m_shuffle;
    bool m_groups_enabled;
    bool m_repeat_track;
    bool m_no_pl_advance;
    bool m_clear_prev_playlist;
    //general
    bool m_resume_on_startup;
    QStringList m_exclude_filters, m_restrict_filters;
    //default playlist
    bool m_use_default_pl;
    QString m_default_pl_name;
    //url dialog
    bool m_use_clipboard;
    //timer
    QTimer *m_timer;
    //formatters
    MetaDataHelper *m_helper;
};

#endif // QMMPUISETTINGS_H
