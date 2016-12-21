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

#ifndef METADATAHELPER_P_H
#define METADATAHELPER_P_H

#include <QList>
#include <QStringList>
#include "metadataformatter.h"

/*! @internal
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class MetaDataHelper
{
public:
    MetaDataHelper();
    ~MetaDataHelper();

    void setTitleFormats(const QStringList &titleFormats);
    void setGroupFormat(const QString &groupFormat);

    int columnCount() const;
    const MetaDataFormatter *titleFormatter(int index) const;
    const MetaDataFormatter *groupFormatter() const;

    static MetaDataHelper *instance();

private:
    static MetaDataHelper *m_instance;

    MetaDataFormatter *m_group_formatter;
    QList <MetaDataFormatter*> m_title_formatters;


};

#endif // METADATAHELPER_P_H
