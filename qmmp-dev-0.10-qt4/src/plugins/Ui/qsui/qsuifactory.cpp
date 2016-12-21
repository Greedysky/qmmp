/***************************************************************************
 *   Copyright (C) 2011-2012 by Ilya Kotov                                 *
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
#include <QMessageBox>
#include <qmmp/qmmpsettings.h>
#include "mainwindow.h"
#include "aboutqsuidialog.h"
#include "qsuifactory.h"

const UiProperties QSUIFactory::properties() const
{
    UiProperties props;
    props.hasAbout = true;
    props.name = tr("Simple User Interface");
    props.shortName = "qsui";
    return props;
}

QObject *QSUIFactory::QSUIFactory::create()
{
    QmmpSettings::instance()->readEqSettings(EqSettings::EQ_BANDS_15);
    return new MainWindow();
}

void QSUIFactory::showAbout(QWidget *parent)
{
    AboutQSUIDialog about(parent);
    about.exec();
}

QTranslator *QSUIFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/qsui_plugin_") + locale);
    return translator;
}

Q_EXPORT_PLUGIN2(qsui, QSUIFactory)
