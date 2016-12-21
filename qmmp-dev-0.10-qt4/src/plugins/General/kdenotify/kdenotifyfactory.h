/***************************************************************************
 *   Copyright (C) 2009 by Artur Guzik                                     *
 *   a.guzik88@gmail.com                                                   *
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

#ifndef KDENOTIFYFACTORY_H
#define KDENOTIFYFACTORY_H

#include <QObject>
#include <QTranslator>
#include <QDialog>

#include <qmmpui/general.h>
#include <qmmpui/generalfactory.h>

class KdeNotifyFactory : public QObject, public GeneralFactory
{
    Q_OBJECT
    Q_INTERFACES(GeneralFactory)
public:
    const GeneralProperties properties() const;
    QObject *create(QObject *parent);
    QDialog *createConfigDialog(QWidget *parent);
    void showAbout(QWidget *parent);
    QTranslator *createTranslator(QObject *parent);
};

#endif // KDENOTIFYFACTORY_H
