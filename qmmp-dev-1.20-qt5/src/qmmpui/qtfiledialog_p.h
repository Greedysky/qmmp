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
#ifndef QTFILEDIALOG_H
#define QTFILEDIALOG_H

#include <QObject>
#include "filedialog.h"

class QTranslator;

/*! @internal
 * @author Vladimir Kuznetsov <vovanec@gmail.com>
 */

class QtFileDialogFactory : public QObject, public FileDialogFactory
{
    Q_OBJECT
    Q_INTERFACES(FileDialogFactory)
public:
    QtFileDialogFactory(){}
    FileDialog* create();
    const FileDialogProperties properties() const;
    void showAbout(QWidget*);
    QTranslator *createTranslator(QObject*);
    virtual ~QtFileDialogFactory()
    {
        ;
    }
};

class QtFileDialog : public FileDialog
{
public:
    virtual ~QtFileDialog();

protected:
    QStringList exec(QWidget *parent, const QString &dir, Mode mode,
                     const QString &caption, const QString &filter,
                     QString *selectedFilter);
};

#endif
