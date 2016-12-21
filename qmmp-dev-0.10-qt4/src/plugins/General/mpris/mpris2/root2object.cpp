/***************************************************************************
 *   Copyright (C) 2010-2012 by Ilya Kotov                                 *
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

#include <qmmp/qmmp.h>
#include <qmmp/decoder.h>
#include <qmmp/decoderfactory.h>
#include <qmmp/abstractengine.h>
#include <qmmp/enginefactory.h>
#include <qmmp/enginefactory.h>
#include <qmmp/inputsource.h>
#include <qmmp/inputsourcefactory.h>
#include <qmmp/metadatamanager.h>
#include <qmmpui/uihelper.h>
#include "root2object.h"

Root2Object::Root2Object(QObject *parent) : QDBusAbstractAdaptor(parent)
{}


Root2Object::~Root2Object()
{}

bool Root2Object::canQuit() const
{
    return true;
}

bool Root2Object::canRaise() const
{
    return false;
}

QString Root2Object::desktopEntry() const
{
    return "qmmp";
}

bool Root2Object::hasTrackList() const
{
    return false;
}
QString Root2Object::identity() const
{
    return "Qmmp";
}

QStringList Root2Object::supportedMimeTypes() const
{
    QStringList mimeTypes;
    foreach(DecoderFactory *factory, Decoder::enabledFactories())
        mimeTypes << factory->properties().contentTypes;
    foreach(EngineFactory *factory, AbstractEngine::enabledFactories())
        mimeTypes << factory->properties().contentTypes;
    mimeTypes.removeDuplicates();
    return mimeTypes;
}

QStringList Root2Object::supportedUriSchemes() const
{
    QStringList protocols = MetaDataManager::instance()->protocols();
    if(!protocols.contains("file")) //append file if needed
        protocols.append("file");
    return protocols;
}

void Root2Object::Quit()
{
    UiHelper::instance()->exit();
}

void Root2Object::Raise(){}
