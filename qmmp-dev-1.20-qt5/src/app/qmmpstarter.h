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

#ifndef _QMMPSTARTER_H
#define _QMMPSTARTER_H

#include <QObject>
#include <QAbstractSocket>
#include <QStringList>
#include <QSessionManager>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

class QLocalServer;
class QLocalSocket;
class MediaPlayer;
class SoundCore;
class BuiltinCommandLineOption;

/*!
 *  QMMPStarter represents wrapper object that is responsible
 * for proper QMMP initialization(only one instance of running
 * MainWindow) and passing command line args to application.
 * @author Vladimir Kuznetsov <vovanec@gmail.com>
 */
class QMMPStarter : public QObject
{
    Q_OBJECT
public:
    QMMPStarter();
    ~QMMPStarter();

    bool isFinished() const;
    int exitCode() const;


private slots:
    /*!
     * Passes command args to the running application
     */
    void writeCommand();
    void readCommand();
    void savePosition();
    void commitData(QSessionManager& manager);

private:
    QString processCommandArgs(const QStringList &list,const QString& cwd);
    /*!
     * Prints usage
     */
    void printUsage();
    /*!
     * Prints version of program
     */
    void printVersion();
    void startPlayer();

private:
    MediaPlayer *m_player;
    SoundCore *m_core;
    QObject *m_ui;
    QString argString;
    BuiltinCommandLineOption* m_option_manager;
    QLocalServer *m_server;
    QLocalSocket *m_socket;
    bool m_finished;
    bool m_exit_code;
#ifdef Q_OS_WIN
    HANDLE m_named_mutex;
#endif
};

#endif
