/***************************************************************************
 *   Copyright (C) 2011 by Ilya Kotov                                      *
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

#ifndef PRESETEDITOR_H
#define PRESETEDITOR_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
    class PresetEditor;
}

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class PresetEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PresetEditor(const QVariantMap &data, QWidget *parent = 0);
    virtual ~PresetEditor();

    const QVariantMap data() const;

private slots:
    void addCommandString(QAction *);

private:
    void createMenus();
    Ui::PresetEditor *m_ui;
};

#endif // PRESETEDITOR_H
