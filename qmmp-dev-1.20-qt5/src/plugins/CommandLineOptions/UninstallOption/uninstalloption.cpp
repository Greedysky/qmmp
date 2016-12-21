/***************************************************************************
 *   Copyright (C) 2014 by Ilya Kotov                                      *
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

#include <QtPlugin>
#include <QTranslator>
#include <QLocale>
#include <QApplication>
#include <qmmpui/winfileassoc.h>
#include <qmmp/metadatamanager.h>
#include "uninstalloption.h"

bool UninstallOption::identify(const QString &str) const
{
    QStringList opts;
    opts << "--uninstall";
    return opts.contains(str);
}

const QString UninstallOption::helpString() const
{
    QString help;
    help += QString("--uninstall              ") + tr("Restore the old file associations and clean up the registry")
            + "\n";
    return  help;
}

QString UninstallOption::executeCommand(const QString &opt_str, const QStringList &args)
{
    Q_UNUSED(args);
    if(opt_str == "--uninstall")
    {
        WinFileAssoc assoc;
        QStringList regExts, extsToCheck;
        foreach (QString ext, MetaDataManager::instance()->nameFilters())
        {
            ext.remove("*.");
            extsToCheck.append(ext);
        }
        assoc.GetRegisteredExtensions(extsToCheck, regExts);
        assoc.RestoreFileAssociations(regExts);
        assoc.RemoveClassId();
        QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
    }
    return QString();
}

const QString UninstallOption::name() const
{
    return "UninstallOption";
}

QTranslator *UninstallOption::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/uninstall_plugin_") + locale);
    return translator;
}
