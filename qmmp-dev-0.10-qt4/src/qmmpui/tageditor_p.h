/***************************************************************************
 *   Copyright (C) 2009-2016 by Ilya Kotov                                 *
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

#ifndef TAGEDITOR_H
#define TAGEDITOR_H

#include <QWidget>

namespace Ui {
    class TagEditor;
}

class TagModel;

/*! @internal
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class TagEditor : public QWidget
{
Q_OBJECT
public:
    TagEditor(TagModel *tagModel, QWidget *parent = 0);
    ~TagEditor();

    void save();

private:
    void readTag();
    Ui::TagEditor *m_ui;
    TagModel *m_tagModel;
    int m_discs; //Number of discs

};

#endif // TAGEDITOR_H
