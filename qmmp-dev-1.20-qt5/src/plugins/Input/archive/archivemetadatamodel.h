/***************************************************************************
 *   Copyright (C) 2016 by Ilya Kotov                                      *
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

#ifndef ARCHIVEMETADATAMODEL_H
#define ARCHIVEMETADATAMODEL_H

#include <QObject>
#include <qmmp/metadatamodel.h>

class ArchiveInputDevice;
class ArchiveTagReader;

class ArchiveMetaDataModel : public MetaDataModel
{
    Q_OBJECT
public:
    explicit ArchiveMetaDataModel(const QString &url, QObject *parent = 0);
    virtual ~ArchiveMetaDataModel();

    QHash<QString, QString> audioProperties();

private:
    ArchiveInputDevice *m_input;
    ArchiveTagReader *m_reader;
    QString m_url;

};

#endif // ARCHIVEMETADATAMODEL_H
