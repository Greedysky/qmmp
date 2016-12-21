/***************************************************************************
 *   Copyright (C) 2008-2012 by Ilya Kotov                                 *
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
#ifndef COMMANDLINEMANAGER_H
#define COMMANDLINEMANAGER_H

#include <QHash>
#include "general.h"
#include "commandlineoption.h"


/*! @brief Helper class used for handle command line plugins
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class CommandLineManager
{
public:
    /*!
     * Executes command \b opt_str
     * @param opt_str Command line option string
     * @param args Command arguments
     * @return Command output result
     */
    static QString executeCommand(const QString& opt_str, const QStringList &args = QStringList());
    /*!
     * Return \b true if command \b opt_str is supported, otherwise returns \b false
     */
    static bool hasOption(const QString &opt_str);
    /*!
     * Prints usage to stdout
     */
    static void printUsage();

private:
    static void checkOptions();
    static QList<CommandLineOption *> *m_options;
    static QHash<CommandLineOption*, QString> *m_files;
};

#endif
