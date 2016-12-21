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

#include "metadatahelper_p.h"

MetaDataHelper *MetaDataHelper::m_instance = 0;

MetaDataHelper::MetaDataHelper()
{
    m_instance = this;
    m_group_formatter = new MetaDataFormatter();
    m_title_formatters << new MetaDataFormatter();
}

MetaDataHelper::~MetaDataHelper()
{
    m_instance = 0;
    delete m_group_formatter;
    qDeleteAll(m_title_formatters);
    m_title_formatters.clear();
}

void MetaDataHelper::setTitleFormats(const QStringList &titleFormats)
{
    while(m_title_formatters.count() > titleFormats.count())
        delete m_title_formatters.takeLast();

    while(m_title_formatters.count() < titleFormats.count())
        m_title_formatters << new MetaDataFormatter();

    for(int i = 0; i < m_title_formatters.count(); ++i)
        m_title_formatters.at(i)->setPattern(titleFormats.at(i));
}

void MetaDataHelper::setGroupFormat(const QString &groupFormat)
{
    m_group_formatter->setPattern(groupFormat);
}

int MetaDataHelper::columnCount() const
{
    return m_title_formatters.count();
}

const MetaDataFormatter *MetaDataHelper::titleFormatter(int index) const
{
    return m_title_formatters.at(index);
}

const MetaDataFormatter *MetaDataHelper::groupFormatter() const
{
    return m_group_formatter;
}

MetaDataHelper *MetaDataHelper::instance()
{
    return m_instance;
}
