/***************************************************************************
 *   Copyright (C) 2010 by Ilya Kotov                                      *
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

#ifndef TEMPLATEEDITOR_H
#define TEMPLATEEDITOR_H

#include <QDialog>

namespace Ui {
    class TemplateEditor;
}
class QAction;


/*! @brief The TemplateEditor class provides simple template editor dialog.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class TemplateEditor : public QDialog
{
Q_OBJECT
public:
    /*!
     * Constructor.
     * @param parent Parent widget.
     */
    explicit TemplateEditor(QWidget *parent = 0);
    /*!
     * Returns current template from editor.
     */
    QString currentTemplate() const;
    /*!
     * Sets template which is placed in the text edit to \b text.
     */
    void setTemplate(const QString &text = QString());
    /*!
     * Sets default template to \b text.
     * This template will be placed in the text edit if the user pressed Reset
     */
    void setDefaultTemplate(const QString &text);
    /*!
     * Static convenience function to get a template from the user.
     * @param parent Parent widget.
     * @param title Window title.
     * @param text Template which is placed in the text edit.
     * @param default_template This template will be placed in the text edit if the user pressed Reset.
     * @param ok This varible will be set to \b true (\b false) if the user pressed OK (Cancel).
     */
    static QString getTemplate (QWidget *parent, const QString &title, const QString &text = QString(),
                                const QString &default_template = QString(), bool *ok = 0);

private slots:
    void insertExpression(QAction *a);
    void on_resetButton_clicked();

private:
    void createMenu();
    Ui::TemplateEditor *m_ui;
    QString m_defaultTemplate;

};

#endif // TEMPLATEEDITOR_H
