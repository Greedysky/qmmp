/***************************************************************************
 *   Copyright (C) 2006-2013 by Ilya Kotov                                 *
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

#include "qmmpuisettings.h"
#include "playstate_p.h"

PlayState::PlayState(PlayListModel *model) : m_model(model)
{
    m_ui_settings = QmmpUiSettings::instance();
}

ShufflePlayState::ShufflePlayState(PlayListModel * model) : PlayState(model)
{
    prepare();
}

bool ShufflePlayState::next()
{
    if(!m_model->count())
        return false;

    if (m_shuffled_current >= m_shuffled_indexes.count() - 1)
    {
        if (!m_ui_settings->isRepeatableList())
            return false;
        else
            prepare();
    }
    else
        m_shuffled_current = (m_shuffled_current + 1) % m_shuffled_indexes.count();

    return m_model->setCurrent(m_shuffled_indexes.at(m_shuffled_current));

}

int ShufflePlayState::nextIndex()
{
    if(!m_model->count())
        return -1;

    if (m_shuffled_current >= m_shuffled_indexes.count() - 1)
    {
        if (!m_ui_settings->isRepeatableList())
            return -1;
        else
            prepare();
    }
    return m_shuffled_indexes.at((m_shuffled_current + 1) % m_shuffled_indexes.count());
}

bool ShufflePlayState::previous()
{
    if(!m_model->count())
        return false;

    if (m_shuffled_current <= 0)
    {
        if (!m_ui_settings->isRepeatableList())
            return false;
        else
        {
            prepare();
            m_shuffled_current = m_shuffled_indexes.count() - 1;
        }
    }

    if (m_model->count() > 1)
        m_shuffled_current--;

    return m_model->setCurrent(m_shuffled_indexes.at(m_shuffled_current));
}

void ShufflePlayState::prepare()
{
    resetState();
    for(int i = 0; i < m_model->count(); i++)
    {
        if (i != m_model->currentIndex() && m_model->isTrack(i))
            m_shuffled_indexes << i;
    }

    for (int i = 0; i < m_shuffled_indexes.count(); i++)
        m_shuffled_indexes.swap(i, qrand()%m_shuffled_indexes.size());

    m_shuffled_indexes.prepend(m_model->currentIndex());
}

void ShufflePlayState::resetState()
{
    m_shuffled_indexes.clear();
    m_shuffled_current = 0;
}

NormalPlayState::NormalPlayState(PlayListModel * model) : PlayState(model)
{}

bool NormalPlayState::next()
{
    if(!m_model->count())
        return false;

    if (m_ui_settings->isRepeatableList() && m_model->currentIndex() == m_model->count() - 1)
    {
        if(m_model->track(0))
            return m_model->setCurrent(0);
        else if(m_model->count() >= 2 && m_model->track(1))
            return m_model->setCurrent(1);
        else
            return false;
    }

    if(m_model->currentIndex() + 1 >= m_model->count())
        return false;
    else if(m_model->track((m_model->currentIndex() + 1)))
        return m_model->setCurrent(m_model->currentIndex() + 1);
    else if(m_model->currentIndex() + 2 > m_model->count() - 1)
        return false;
    else if(m_model->track(m_model->currentIndex() + 2))
        return m_model->setCurrent(m_model->currentIndex() + 2);
    return false;
}

bool NormalPlayState::previous()
{
    if(!m_model->count())
        return false;

    if(m_ui_settings->isRepeatableList())
    {
        if(m_model->currentIndex() == 1 && !m_model->isTrack(0))
            return (m_model->setCurrent(m_model->count() - 1));
        else if(m_model->currentIndex() == 0)
            return m_model->setCurrent(m_model->count() - 1);
    }

    if(m_model->currentIndex() == 1 && !m_model->isTrack(0))
        return false;
    else if(m_model->currentIndex() == 0)
        return false;
    else if(m_model->isTrack(m_model->currentIndex() - 1))
        return m_model->setCurrent(m_model->currentIndex() - 1);
    else if(m_model->currentIndex() >= 2 && m_model->isTrack(m_model->currentIndex() - 2))
        return m_model->setCurrent(m_model->currentIndex() - 2);
    return false;
}

int NormalPlayState::nextIndex()
{
    if(!m_model->count())
        return -1;

    if (m_model->currentIndex() == m_model->count() - 1)
    {
        if (m_ui_settings->isRepeatableList())
        {
            if(m_model->isTrack(0))
                return 0;
            else if(m_model->isTrack(1))
                return 1;
        }
        return -1;
    }
    if(m_model->isTrack(m_model->currentIndex() + 1))
        return m_model->currentIndex() + 1;
    else if(m_model->currentIndex() + 2 > m_model->count() - 1)
        return -1;
    else if(m_model->isTrack(m_model->currentIndex() + 2))
        return m_model->currentIndex() + 2;
    return -1;
}
