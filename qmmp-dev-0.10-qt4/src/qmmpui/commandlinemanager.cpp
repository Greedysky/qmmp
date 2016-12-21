/***************************************************************************
 *   Copyright (C) 2008-2016 by Ilya Kotov                                 *
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

#include <QObject>
#include <QList>
#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include <cstdlib>
#include <iostream>
#include <qmmp/qmmp.h>
#include <qmmp/soundcore.h>
#include <qmmpui/uihelper.h>
#include <qmmpui/mediaplayer.h>
#include "commandlinemanager.h"

using namespace std;

QList<CommandLineOption *> *CommandLineManager::m_options = 0;
QHash<CommandLineOption*, QString> *CommandLineManager::m_files = 0;

void CommandLineManager::checkOptions()
{
    if (!m_options)
    {
        m_options = new QList<CommandLineOption *>;
        m_files = new QHash<CommandLineOption*, QString>;

        QDir pluginsDir (Qmmp::pluginsPath());
        pluginsDir.cd("CommandLineOptions");
        QStringList filters;
        filters << "*.dll" << "*.so";
        foreach (QString fileName, pluginsDir.entryList(filters, QDir::Files))
        {
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = loader.instance();
            if (loader.isLoaded())
                /*qDebug("CommandLineManager: loaded plugin %s", qPrintable(fileName))*/;
            else
                qWarning("CommandLineManager: %s", qPrintable(loader.errorString ()));

            CommandLineOption *option = 0;
            if (plugin)
                option = qobject_cast<CommandLineOption *>(plugin);

            if (option)
            {
                m_options->append(option);
                m_files->insert(option, pluginsDir.absoluteFilePath(fileName));
                qApp->installTranslator(option->createTranslator(qApp));
            }
        }
    }
}

QString CommandLineManager::executeCommand(const QString& opt_str, const QStringList &args)
{
    checkOptions();
    if(!UiHelper::instance() || !SoundCore::instance() || !MediaPlayer::instance())
    {
        qWarning("CommandLineManager: player objects are not created");
        return QString();
    }
    foreach(CommandLineOption *opt, *m_options)
    {
        if (opt->identify(opt_str))
        {
            return opt->executeCommand(opt_str, args);
        }
    }
    return QString();
}

bool CommandLineManager::hasOption(const QString &opt_str)
{
    checkOptions();
    foreach(CommandLineOption *opt, *m_options)
    {
        if (opt->identify(opt_str))
            return true;
    }
    return false;
}

void CommandLineManager::printUsage()
{
    checkOptions();
    foreach(CommandLineOption *opt, *m_options)
        cout << qPrintable(opt->helpString());
}
