/***************************************************************************
 *   Copyright (C) 2008-2013 by Ilya Kotov                                 *
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
#ifndef GENERAL_H
#define GENERAL_H

#include <QObject>
#include <QStringList>
#include <QHash>
#include "generalfactory.h"

class QmmpUiPluginCache;

/*! @brief The General class provides simple access to general plugins
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class General
{
public:
    /*!
     * Prepares general plugins for usage.
     * @param parent Parent object.
     */
    static void create(QObject *parent);
    /*!
     * Returns a list of the general plugin factories.
     */
    static QList<GeneralFactory *> factories();
    /*!
     * Returns a list of the enabled general plugin factories.
     */
    static QList<GeneralFactory *> enabledFactories();
    /*!
     * Returns plugin file path.
     * @param factory General plugin factory.
     */
    static QString file(GeneralFactory *factory);
    /*!
     * Sets whether the general plugin is enabled.
     * @param factory General plugin factory.
     * @param enable Plugin enable state (\b true - enable, \b false - disable)
     */
    static void setEnabled(GeneralFactory* factory, bool enable = true);
    /*!
     * Shows configuration dialog and updates settings automatically.
     * @param factory General plugin factory.
     * @param parentWidget Parent widget.
     */
    static void showSettings(GeneralFactory* factory, QWidget* parentWidget);
    /*!
     * Returns \b true if general plugin is enabled, otherwise returns \b false
     * @param factory General plugin factory.
     */
    static bool isEnabled(GeneralFactory* factory);

private:
    static void loadPlugins();
    static QHash <GeneralFactory*, QObject*> *m_generals;
    static QObject *m_parent;
    static QList<QmmpUiPluginCache*> *m_cache;
    static QStringList m_enabledNames;
};

#endif
