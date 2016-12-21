/***************************************************************************
 *   Copyright (C) 2008-2012 by Ilya Kotov                                 *
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
#ifndef GENERALFACTORY_H
#define GENERALFACTORY_H

class QObject;
class QTranslator;
class QDialog;
class QString;
class QWidget;
class Control;
class General;

/*! @brief Helper class to store general plugin properies.
 */
class GeneralProperties
{
public:
    /*!
     * Constructor
     */
    GeneralProperties()
    {
        hasAbout = false;
        hasSettings = false;
        visibilityControl = false;
    }
    QString name;           /*!< File dialog plugin full name */
    QString shortName;      /*!< File dialog short name for internal usage */
    bool hasAbout;          /*!< Should be \b true if plugin has about dialog, otherwise returns \b false */
    bool hasSettings;       /*!< Should be \b true if plugin has settings dialog, otherwise returns \b false */
    bool visibilityControl; /*!< Should be \b true if plugin can show/hide main window of the player,
                             * otherwise returns \b false */
};
/*! @brief %General plugin interface.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class GeneralFactory
{
public:
    /*!
     * Object destructor.
     */
    virtual ~GeneralFactory() {}
    /*!
     * Returns general plugin properties.
     */
    virtual const GeneralProperties properties() const = 0;
    /*!
     * Creates object of the General class.
     */
    virtual QObject *create(QObject *parent) = 0;
    /*!
     * Creates configuration dialog.
     * @param parent Parent widget.
     * @return Configuration dialog pointer.
     */
    virtual QDialog *createConfigDialog(QWidget *parent) = 0;
    /*!
     * Shows about dialog.
     * @param parent Parent widget.
     */
    virtual void showAbout(QWidget *parent) = 0;
    /*!
     * Creates QTranslator object of the system locale. Should return \b 0 if translation doesn't exist.
     * @param parent Parent object.
     */
    virtual QTranslator *createTranslator(QObject *parent) = 0;
};

Q_DECLARE_INTERFACE(GeneralFactory, "GeneralFactory/1.0")
#endif
