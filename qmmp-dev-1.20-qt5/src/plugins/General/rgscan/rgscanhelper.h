/***************************************************************************
 *   Copyright (C) 2013 by Ilya Kotov                                      *
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
#ifndef RGSCANHELPER_H
#define RGSCANHELPER_H

#include <qmmpui/general.h>
#include <qmmp/qmmp.h>

class QAction;
class RGScanner;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class RGScanHelper : public QObject
{
Q_OBJECT
public:
    RGScanHelper(QObject *parent = 0);
    ~RGScanHelper();

private slots:
    void openRGScaner();

};

#endif //RGSCANHELPER_H
