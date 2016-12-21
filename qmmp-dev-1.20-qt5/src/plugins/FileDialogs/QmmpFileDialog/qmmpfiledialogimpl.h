/**************************************************************************
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

#ifndef QMMPFILEDIALOGIMPL_H
#define QMMPFILEDIALOGIMPL_H

#include "ui_qmmpfiledialog.h"
#include <QDialog>
#include <QCompleter>
#include <QAbstractItemView>
#include <qmmpui/filedialog.h>
#include <QFileSystemModel>


class QmmpFileDialogImpl : public QDialog , private Ui::QmmpFileDialog
{
    Q_OBJECT
public:
    QmmpFileDialogImpl(QWidget *parent = 0, Qt::WindowFlags f = 0);

    ~QmmpFileDialogImpl();

    void setModeAndMask(const QString&,FileDialog::Mode m, const QStringList& mask = QStringList());
    void loadMountedVolumes();
    QStringList selectedFiles ();

protected slots:
    void on_mountPointsListWidget_itemClicked(QListWidgetItem *item);
    void on_lookInComboBox_activated(const QString&);
    void on_upToolButton_clicked();
    void on_fileListView_doubleClicked(const QModelIndex&);
    void on_treeView_doubleClicked(const QModelIndex&);
    void on_fileNameLineEdit_returnPressed();
    void on_fileNameLineEdit_textChanged (const QString &text);
    void on_addPushButton_clicked();
    void on_listToolButton_toggled(bool);
    void on_detailsToolButton_toggled(bool);
    void on_fileTypeComboBox_activated(int);

signals:
    void filesSelected(const QStringList&, bool play = false);

protected:
    virtual void hideEvent (QHideEvent *event);

private slots:
    void updateSelection ();

private:
    int m_mode;
    QFileSystemModel* m_model;
    void addToHistory(const QString &path);
    void addFiles(const QStringList &list);
    QStringList m_history;

};

class PathCompleter : public QCompleter
{
    Q_OBJECT
public:
    PathCompleter(QAbstractItemModel *model, QAbstractItemView *itemView, QObject *parent = 0) : QCompleter(model, parent)
    {
        m_itemView = itemView;
    };


    QString pathFromIndex(const QModelIndex &index) const
    {
        const QFileSystemModel *dirModel = static_cast<const QFileSystemModel *>(model());
        QString currentLocation = dirModel->filePath(m_itemView->rootIndex());
        QString path = dirModel->filePath(index);
        if (path.startsWith(currentLocation))
        {
            path = path.mid(currentLocation.length() + 1);
        }
        return path;
    }


    QStringList splitPath(const QString &path) const
    {
        if (path.isEmpty())
            return QStringList(completionPrefix());
        QStringList parts;
        if (!path.startsWith(QDir::separator()))
        {
            const QFileSystemModel *dirModel = static_cast<const QFileSystemModel *>(model());
            QString currentLocation = QDir::toNativeSeparators(dirModel->filePath(m_itemView->rootIndex()));
            parts = QCompleter::splitPath(currentLocation);
        }
        parts << QCompleter::splitPath(path);
        return parts;
    }
private:
    QAbstractItemView *m_itemView;
};


#endif //QMMPFILEDIALOGIMPL_H
