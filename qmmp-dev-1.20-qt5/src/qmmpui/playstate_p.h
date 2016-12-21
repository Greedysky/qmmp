/***************************************************************************
 *   Copyright (C) 2007-2013 by Ilya Kotov                                 *
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

#ifndef _PLAYSTATE_H
#define _PLAYSTATE_H

#include "playlistmodel.h"

/*! @internal
 * @brief Abstract class that represents data model playing states.
 * @author Vladimir Kuznetsov <vovanec@gmail.com>
 */
class PlayState
{
public:

    /*! Makes single step forward through songs list.
     *  If the step has done returns \b true, otherwise returns \b false
     */
    virtual bool next() = 0;

    /*! Makes single step back through songs list.
     *  If the step has done returns \b true, otherwise returns \b false
     */
    virtual bool previous() = 0;
    /*!
     * Returns next item index.
     */
    virtual int nextIndex() = 0;
    /*!
     * Service method, resets state to it's defaults.
     */
    virtual void resetState()
    {
        ;
    }
    /*!
     * Service method, can be used for state initializing.
     */
    virtual void prepare()
    {
        ;
    }
    virtual ~PlayState()
    {
        ;
    }
    PlayState(PlayListModel* model);

protected:
    /// Data model
    PlayListModel* m_model;
    QmmpUiSettings *m_ui_settings;
};

/*! @internal
 * @brief Represents normal playing state.
 * @author Vladimir Kuznetsov <vovanec@gmail.com>
 */
class NormalPlayState : public PlayState
{
public:
    virtual bool next();
    virtual bool previous();
    virtual int nextIndex();
    NormalPlayState(PlayListModel* model);
};

/*! @internal
 * @brief Represents shuffle playing state.
 * @author Vladimir Kuznetsov <vovanec@gmail.com>
 */
class ShufflePlayState : public PlayState
{
public:
    virtual bool next();
    virtual bool previous();
    virtual int nextIndex();
    virtual void prepare();
    ShufflePlayState(PlayListModel* model);
    virtual void resetState();
private:

    /// Current shuffled index.
    int m_shuffled_current;

    /// List of indexes used for shuffled playing.
    QList<int> m_shuffled_indexes;
};


#endif
