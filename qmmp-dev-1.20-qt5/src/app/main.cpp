/***************************************************************************
 *   Copyright (C) 2006-2016 by Ilya Kotov                                 *
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

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QIcon>
#include <stdio.h>
#include <stdlib.h>
#ifdef Q_OS_WIN
#include <QtGlobal>
#include <windows.h>
#include <winuser.h>
#endif
#include <qmmp/qmmp.h>
#include "qmmpstarter.h"

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    //allows to activate main window from other instances
    AllowSetForegroundWindow(ASFW_ANY);
#endif
    QApplication a (argc, argv );
    a.setApplicationName("qmmp");
    QIcon icon;
    icon.addFile(":/16x16/qmmp.png");
    icon.addFile(":/32x32/qmmp.png");
    icon.addFile(":/48x48/qmmp.png");
    icon.addFile(":/56x56/qmmp.png");
    icon.addFile(":/scalable/qmmp.svgz");
    a.setWindowIcon(icon);
#ifdef Q_OS_WIN
    if(Qmmp::isPortable())
        qputenv("HOMEPATH", Qmmp::configDir().toLatin1()); //librcc database path
#endif
    QTranslator translator;
    QString locale = Qmmp::systemLanguageID();
    translator.load(QString(":/qmmp_") + locale);
    a.installTranslator(&translator);

    QTranslator qt_translator;
    qt_translator.load(QLibraryInfo::location (QLibraryInfo::TranslationsPath) + "/qtbase_" + locale);
    a.installTranslator(&qt_translator);

    QMMPStarter starter;

    if(starter.isFinished())
        return starter.exitCode();

    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
