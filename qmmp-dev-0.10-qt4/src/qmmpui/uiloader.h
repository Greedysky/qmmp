/***************************************************************************
 *   Copyright (C) 2011-2013 by Ilya Kotov                                 *
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
#ifndef UILOADER_H
#define UILOADER_H

#include <QString>
#include <QList>
#include <QObject>
#include "uifactory.h"

class QmmpUiPluginCache;

/*! @brief The UiLoader provides user interface plugins access
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class UiLoader
{
public:
    /*!
     * Returns a list of the loaded ui plugin factories.
     */
    static QList<UiFactory*> factories();
    /*!
     * Returns plugin file path.
     * @param factory User interface plugin factory.
     */
    static QString file(UiFactory *factory);
    /*!
     * Selects active user interface factory.
     * @param factory Ui plugin factory.
     */
    static void select(UiFactory* factory);
    /*!
     * Returns \b true if general plugin is enabled, otherwise returns \b false
     */
    static UiFactory *selected();

private:
    static void loadPlugins();
    static QList<QmmpUiPluginCache*> *m_cache;
};

#endif //UILOADER_H
