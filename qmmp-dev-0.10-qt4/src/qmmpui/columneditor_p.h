/***************************************************************************
 *   Copyright (C) 2015 by Ilya Kotov                                      *
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

#ifndef COLUMNEDITOR_P_H
#define COLUMNEDITOR_P_H

#include <QDialog>

namespace Ui {
class ColumnEditor;
}

/*! @internal
   @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class ColumnEditor : public QDialog
{
    Q_OBJECT
public:
    explicit ColumnEditor(const QString &name, const QString &patt, QWidget *parent = 0);
    ~ColumnEditor();

    QString name() const;
    QString pattern() const;

private slots:
    void insertExpression(QAction *a);
    void on_comboBox_activated(int index);
    void on_formatLineEdit_textChanged(const QString &text);

private:
    void createMenu();
    void fillTypes();
    Ui::ColumnEditor *m_ui;
};

#endif // COLUMNEDITOR_P_H
