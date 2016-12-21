/***************************************************************************
 *   Copyright (C) 2008-2014 by Ilya Kotov                                 *
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
#ifndef BUILTINCOMMANDLINEOPTION_H
#define BUILTINCOMMANDLINEOPTION_H

#include <QObject>
#include <QHash>
#include <QStringList>
#include <QTimer>

class PlayListModel;

/**
    @author Vladimir Kuznetsov <vovanec@gmail.ru>
*/

/*!
 * Represens command line option handling for standard operations.
 */
class BuiltinCommandLineOption : public QObject
{
    Q_OBJECT
public:
    BuiltinCommandLineOption(QObject *parent = 0);

    ~BuiltinCommandLineOption();

    bool identify(const QString& str)const;
    const QString helpString()const;
    void executeCommand(const QString& option, const QStringList &args,
                        const QString &cwd);
    QHash <QString, QStringList> splitArgs(const QStringList &args) const;

private slots:
    void disconnectPl();
    void addPendingPaths();

private:
    QStringList m_options;
    PlayListModel *m_model;
    QStringList m_pending_path_list;
#ifdef Q_OS_WIN
    QTimer *m_timer;
#endif
};

#endif
