/***************************************************************************
 *   Copyright (C) 2015 by Ilya Kotov                                      *
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

#ifndef PLAYLISTHEADERMODEL_H
#define PLAYLISTHEADERMODEL_H

#include <QObject>
#include <QWidget>
#include <QSettings>
#include <QHash>
#include <QVariant>
#include "metadataformatter.h"

class MetaDataHelper;

/*!
 * @brief Helper class that provides access to playlist column configuration.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class PlayListHeaderModel : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a playlist model.
     * @param parent QObject parent.
     */
    explicit PlayListHeaderModel(QObject *parent = 0);
    /*!
     * Object destructor.
     */
    ~PlayListHeaderModel();
    /*!
     * Restores column settings using internal configuration file.
     * @param groupName Name of the settings group (should be different for each UI plugin)
     */
    void restoreSettings(const QString &groupName);
    /*!
     * Restores column settings using provided QSettings object.
     * @param settings Pointer to QSettings object.
     */
    void restoreSettings(QSettings *settings);
    /*!
     * Saves column settings using internal configuration file.
     * @param groupName Name of the settings group (should be different for each UI plugin)
     */
    void saveSettings(const QString &groupName);
    /*!
     * Saves column settings using provided QSettings object.
     * @param settings Pointer to QSettings object.
     */
    void saveSettings(QSettings *settings);
    /*!
     * Indicates that the settings is loaded.
     */
    bool isSettingsLoaded() const;
    /*!
     * Inserts the new column.
     * @param index Index of the column.
     * @param name Name of the column.
     * @param pattern Column template. See \b MetaDataFormatter for details.
     */
    void insert(int index, const QString &name, const QString &pattern);
    /*!
     * Removes column.
     * @param index Index of the column.
     */
    void remove(int index);
    /*!
     * Moves the column at index position \b from to index position \b to.
     */
    void move(int from, int to);
    /*!
     * Opens internal column editor.
     * @param index Index of the column.
     * @param parent Parent widget,
     */
    void execEdit(int index, QWidget *parent = 0);
    /*!
     * Opens internal column editor and inserts new column if user press "ok".
     * @param index Required index of the column.
     * @param parent Parent widget.
     */
    void execInsert(int index, QWidget *parent = 0);
    /*!
     * Returns number of the columns.
     */
    int count();
    /*!
     * Returns name of the column with index \b index.
     */
    const QString name(int index) const;
    /*!
     * Returns template of the column with index \b index.
     */
    const QString pattern(int index) const;
    /*!
     * Adds extra data to column (size, geometry, etc).
     * @param index Index of the column.
     * @param key Key of the extra field.
     * @param data Extra data value.
     */
    void setData(int index, int key, const QVariant &data);
    /*!
     * Returns extra data of the column.
     * @param index Index of the column.
     * @param key Key of the extra field.
     */
    const QVariant data(int index, int key) const;

signals:
    /*!
     * Emitted when a new column is added.
     * @param index Index of the column.
     */
    void columnAdded(int index);
    /*!
     * Emitted when a column is removed.
     * @param index Index of the column.
     */
    void columnRemoved(int index);
    /*!
     * Emitted when a column is changed.
     * @param index Index of the column.
     */
    void columnChanged(int index);
    /*!
     * Emitted when the column at index position \b from is moved to index position \b to.
     */
    void columnMoved(int from, int to);
    /*!
     * Emitted when columns have changed.
     */
    void headerChanged();

private:
    void updatePlayLists();

    struct ColumnHeader
    {
        QString name;
        QString pattern;
        QHash<int, QVariant> data;
    };
    QList<ColumnHeader> m_columns;
    bool m_settings_loaded;
    MetaDataHelper *m_helper;
};

#endif // COLUMNMANAGER_H
