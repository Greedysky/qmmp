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

#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QHash>
#include "filedialogfactory.h"

class QmmpUiPluginCache;

/*! @brief The FileDialog class is the base interface class of the file dialogs.
 * @author Vladimir Kuznetsov <vovanec@gmail.com>
 */
class FileDialog : public QObject
{
    Q_OBJECT
public:
    /*!
     * Returns a list of registered file dialog factories.
     */
    static QList <FileDialogFactory*> factories();
    /*!
     * Selects current file dialog factory.
     */
    static void setEnabled(FileDialogFactory *factory);
    /*!
     * Returns \b true if file dialog \b factory is used by default, otherwise returns \b false
     */
    static bool isEnabled(FileDialogFactory *factory);
    /*!
     * Returns plugin file path.
     * @param factory File dialog plugin factory.
     */
    static QString file(FileDialogFactory *factory);
    /*!
     * Enum of available file dialog modes
     */
    enum Mode
    {
        AddFile = 0,   /*!< Get existing file */
        AddDir,        /*!< Get existing directory */
        AddFiles,      /*!< Get existing files */
        AddDirs,       /*!< Get existing directories */
        AddDirsFiles,  /*!< Get existing files and directories */
        PlayDirsFiles, /*!< Get existing files and directories with play button (if supported) */
        SaveFile       /*!< Get existing or non-existing file */
    };
    /*!
     * This is a static function that will open modal file dialog and
     * return an existing directory selected by the user.
     * @param parent Parent widget.
     * @param caption Dialog title.
     * @param dir Default directory.
     */
    static QString getExistingDirectory(QWidget *parent = 0,
                                        const QString &caption = QString(),
                                        const QString &dir = QString());
    /*!
     * This is a static function that will open modal file dialog and
     * return an existing file selected by the user.
     * @param parent Parent widget.
     * @param caption Dialog title.
     * @param dir Default directory.
     * @param filter Filer used by file dialog
     * (example: "Audio (*.mp3 *.ogg);;Text files (*.txt);;XML files (*.xml)").
     * @param selectedFilter Default selected filter
     */
    static QString getOpenFileName(QWidget *parent = 0,
                                   const QString &caption = QString(),
                                   const QString &dir = QString(),
                                   const QString &filter = QString(),
                                   QString *selectedFilter = 0);
    /*!
     * This is a static function that will open modal file dialog and
     * return an existing file selected by the user.
     * @param parent Parent widget.
     * @param caption Dialog title.
     * @param dir Default directory.
     * @param filter Filer used by file dialog
     * (example: "Audio (*.mp3 *.ogg);;Text files (*.txt);;XML files (*.xml)").
     * @param selectedFilter Default selected filter.
     */
    static QStringList getOpenFileNames(QWidget *parent = 0,
                                        const QString &caption = QString(),
                                        const QString &dir = QString(),
                                        const QString &filter = QString(),
                                        QString *selectedFilter = 0);
    /*!
     * This is a static function that will open modal file dialog and
     * return a file name selected by the user. The file does not have to exist.
     * @param parent Parent widget.
     * @param caption Dialog title.
     * @param dir Default directory.
     * @param filter Filer used by file dialog
     * (example: "Audio (*.mp3 *.ogg);;Text files (*.txt);;XML files (*.xml)").
     * @param selectedFilter Default selected filter.
     */
    static QString getSaveFileName (QWidget *parent = 0,
                                    const QString &caption = QString(),
                                    const QString &dir = QString(),
                                    const QString &filter = QString(),
                                    QString *selectedFilter = 0);
    /*!
     * Opens nonmodal file dialog. Selected file dialog should support nonmodal mode.
     * Otherwise this function creates modal dialog.
     * @param parent Parent widget.
     * @param mode File dialog mode.
     * @param dir Default directory.
     * @param receiver Receiver QObject.
     * @param member Receiver slot.
     * @param caption Dialog title.
     * @param filters Filer used by file dialog
     * (example: "Audio (*.mp3 *.ogg);;Text files (*.txt);;XML files (*.xml)").
     *
     * Usage: FileDialog::popup(this, FileDialog::AddDirs, &m_lastDir,
     *                 m_playListModel, SLOT(addFileList(QStringList)),
     *                 tr("Choose a directory"));
     */
    static void popup(QWidget *parent = 0,
                      Mode mode = AddFiles,
                      QString *dir = 0,
                      QObject *receiver = 0,
                      const char *member = 0,
                      const QString &caption = QString(),
                      const QString &filters = QString());

signals:
    /*!
     * Emitted when the add/open button has pressed. Subclass should emit this signal.
     */
    void filesSelected(const QStringList &selected, bool play = false);

protected:
    /*!
     * Object constructor.
     */
    FileDialog();
    /*!
     * Object destructor
     */
    virtual ~FileDialog();
    /*!
     * Opens nonmodal file dialog. Selected file dialog should support nonmodal mode.
     * Otherwise this function does nothing.
     * Nonmodal dialog subclass should reimplement this function.
     * @param dir Default directory.
     * @param mode File dialog mode.
     * @param caption Dialog title.
     * @param mask Filer used by file dialog
     */
    virtual void raise(const QString &dir, Mode mode,
                       const QString &caption, const QStringList &mask);
    /*!
    * This is a function that will open modal file dialog and
    * return a file(s) or directory name selected by the user.
    * Subclass should reimplement this function.
    * @param parent Parent widget.
    * @param dir Default directory.
    * @param mode File dialog mode.
    * @param caption Dialog title.
    * @param filter Filer used by file dialog
    * (example: "Audio (*.mp3 *.ogg);;Text files (*.txt);;XML files (*.xml)").
    * @param selectedFilter Default selected filter.
    */
    virtual QStringList exec(QWidget *parent, const QString &dir, Mode mode,
                             const QString &caption, const QString &filter = QString(),
                             QString *selectedFilter = 0) = 0;

private slots:
    void updateLastDir(const QStringList&);

private:
    static void loadPlugins();
    static FileDialog* instance();
    static FileDialogFactory *m_currentFactory;
    static FileDialog* m_instance;
    static QList<QmmpUiPluginCache*> *m_cache;

    void init(QObject* receiver, const char* member, QString *dir);
    bool m_initialized;
    QString *m_lastDir;
};

#endif
