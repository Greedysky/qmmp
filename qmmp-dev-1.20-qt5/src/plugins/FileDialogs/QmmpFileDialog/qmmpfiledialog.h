/**************************************************************************
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

#ifndef QMMPFILEDIALOG_H
#define QMMPFILEDIALOG_H

#include <qmmpui/filedialog.h>

class QmmpFileDialogImpl;

class QmmpFileDialog : public FileDialog
{
    Q_OBJECT
public:
    QmmpFileDialog();
    virtual ~QmmpFileDialog();

protected:
    void raise(const QString &dir, Mode mode, const QString &caption,
               const QStringList &mask);

    QStringList exec(QWidget *parent, const QString &dir, Mode mode,
                     const QString &caption, const QString &filter, QString *);

private:
    QmmpFileDialogImpl *m_dialog;
};


class QmmpFileDialogFactory : public QObject, public FileDialogFactory
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.qmmp.qmmpui.FileDialogFactoryInterface.1.0")
Q_INTERFACES(FileDialogFactory)
public:
    virtual FileDialog* create();
    virtual const FileDialogProperties properties() const;
    virtual void showAbout(QWidget*);
    virtual QTranslator *createTranslator(QObject *parent);
    virtual ~QmmpFileDialogFactory()
    {
        ;
    }
};


#endif
