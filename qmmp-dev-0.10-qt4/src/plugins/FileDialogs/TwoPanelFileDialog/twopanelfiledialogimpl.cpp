/**************************************************************************
*   Copyright (C) 2016 by Ilya Kotov                                      *
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

#include <QDirModel>
#include <QApplication>
#include <QFileInfo>
#include <QStyle>
#include <QSettings>
#include <QMessageBox>
#include <QHeaderView>
#include <qmmp/qmmp.h>
#include "twopanelfiledialogimpl.h"

#define HISTORY_SIZE 8


/**
 *   This variable has been copied from Qt library
 */

const char *qt_file_dialog_filter_reg_exp =
    "([a-zA-Z0-9 -]*)\\(([a-zA-Z0-9_.*? +;#\\-\\[\\]@\\{\\}/!<>\\$%&=^~:\\|]*)\\)$";

/**
 *   This function has been copied from Qt library
 */

// Makes a list of filters from a normal filter string "Image Files (*.png *.jpg)"
static QStringList qt_clean_filter_list(const QString &filter)
{
    QRegExp regexp(QString::fromLatin1(qt_file_dialog_filter_reg_exp));
    QString f = filter;
    int i = regexp.indexIn(f);
    if (i >= 0)
        f = regexp.cap(2);
    return f.split(QLatin1Char(' '), QString::SkipEmptyParts);
}

TwoPanelFileDialogImpl::TwoPanelFileDialogImpl(QWidget * parent) : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    m_mode = 0;

    m_dirModel = new QFileSystemModel(this);
    m_dirModel->setReadOnly(true);
    m_ui.dirListView->setModel(m_dirModel);
    m_dirModel->setRootPath("");
#if (QT_VERSION >= 0x040700)
    m_dirModel->setFilter(QDir::AllDirs | QDir::NoDot);
#else
    m_dirModel->setFilter(QDir::AllDirs);
#endif
    m_dirModel->setNameFilterDisables (false);

    connect(m_ui.dirListView->selectionModel(),
            SIGNAL(selectionChanged (QItemSelection, QItemSelection)),
            SLOT(updateDirSelection (QItemSelection, QItemSelection)));

    connect(m_ui.fileListWidget, SIGNAL(itemSelectionChanged()), SLOT(updateFileSelection ()));

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    restoreGeometry(settings.value("TwoPanelFileDialog/geometry").toByteArray());
    m_history = settings.value("TwoPanelFileDialog/history").toStringList();
    m_ui.lookInComboBox->addItems(m_history);
    m_ui.lookInComboBox->setMaxCount(HISTORY_SIZE);
    QCompleter* dirCompleter = new QCompleter (m_dirModel, this);
    m_ui.lookInComboBox->setCompleter (dirCompleter);

    if(qApp->style()->styleHint(QStyle::SH_DialogButtonBox_ButtonsHaveIcons, 0, this))
    {
        m_ui.playButton->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
        m_ui.addButton->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
        m_ui.closeButton->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogCloseButton));
    }
}

TwoPanelFileDialogImpl::~TwoPanelFileDialogImpl()
{
}

QStringList TwoPanelFileDialogImpl::selectedFiles() const
{
    QStringList l;
    if (m_mode == FileDialog::SaveFile)
    {
        QModelIndexList indexes = m_ui.dirListView->selectionModel()->selectedRows(0);
        if(!indexes.isEmpty() && indexes.first().isValid())
        {
            l << m_dirModel->filePath(indexes.first()) + "/" + m_ui.fileNameLineEdit->text();
        }
    }
    else if(m_mode == FileDialog::AddDir || m_mode == FileDialog::AddDirs)
    {
        foreach (QModelIndex index, m_ui.dirListView->selectionModel()->selectedRows(0))
        {
            if(index.isValid() && (m_dirModel->fileName(index) != ".."))
            {
                l << m_dirModel->filePath(index);
            }
        }
    }
    else
    {
        foreach (QListWidgetItem *item, m_ui.fileListWidget->selectedItems())
        {
            l << item->data(Qt::UserRole).toString();
        }
    }
    return l;
}

void TwoPanelFileDialogImpl::updateDirSelection(const QItemSelection &s, const QItemSelection &)
{
    m_ui.fileListWidget->clear();

    if(m_mode == FileDialog::AddDir || m_mode == FileDialog::AddDirs)
    {
        m_ui.addButton->setEnabled(!selectedFiles().isEmpty());
        return;
    }

    if(s.indexes().isEmpty())
        return;

    QModelIndex index = s.indexes().first();

    if(index.isValid())
        updateFileList(m_dirModel->filePath(index));
}

void TwoPanelFileDialogImpl::updateFileSelection ()
{
    QStringList paths = selectedFiles();

    if(paths.isEmpty())
    {
        m_ui.fileNameLineEdit->clear();
        m_ui.addButton->setEnabled(false);
        m_ui.playButton->setEnabled(false);
        return;
    }

    QStringList names;

    foreach(QString path, paths)
    {
        QString name = QFileInfo(path).fileName();
        if (!names.contains(name))
            names << name;
    }

    QString str;
    if (names.size() == 1)
        str = names.at(0);
    else
    {
        str = names.join ("\" \"");
        str.append("\"");
        str.prepend("\"");
    }
    if (!m_ui.fileNameLineEdit->hasFocus())
        m_ui.fileNameLineEdit->setText(str);


    m_ui.addButton->setEnabled(true);
    m_ui.playButton->setEnabled(false);

    if(m_mode == FileDialog::AddFiles || m_mode == FileDialog::AddDirsFiles || m_mode == FileDialog::AddFile
            || m_mode == FileDialog::PlayDirsFiles)
    {
        foreach(str, paths)
        {
            if(QFileInfo(str).isFile())
            {
                m_ui.playButton->setEnabled(true);
                break;
            }
        }
    }
}

void TwoPanelFileDialogImpl::on_dirListView_doubleClicked(const QModelIndex &ind)
{
    if (!ind.isValid())
        return;

    QFileInfo info = m_dirModel->fileInfo(ind);
    QModelIndex rootIndex = m_dirModel->setRootPath(info.canonicalFilePath());

    if(rootIndex.isValid())
    {
        m_ui.dirListView->setRootIndex(rootIndex);
        m_ui.lookInComboBox->setEditText(m_dirModel->filePath(rootIndex));
        m_ui.fileListWidget->clear();
    }
}

void TwoPanelFileDialogImpl::on_lookInComboBox_activated(const QString &path)
{
    if (QDir(path).exists ())
    {
        m_ui.dirListView->setRootIndex(m_dirModel->index(path));
        m_dirModel->setRootPath(path);
        m_ui.fileListWidget->clear();
    }
}

void TwoPanelFileDialogImpl::on_fileListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QStringList l;
    l << item->data(Qt::UserRole).toString();
    addToHistory(l[0]);
    addFiles(l, false);
}

void TwoPanelFileDialogImpl::on_fileNameLineEdit_textChanged(const QString &text)
{
    if (m_mode == FileDialog::SaveFile)
    {
        m_ui.addButton->setEnabled(!text.isEmpty());
        m_ui.playButton->setEnabled(!text.isEmpty());
        return;
    }
    QString path;
    if (text.startsWith("/"))
        path = text;
    else
        path = m_dirModel->filePath(m_ui.dirListView->currentIndex()) + "/" + text;

    if (!QFileInfo(path).exists())
        return;

    for(int i = 0; i < m_ui.fileListWidget->count(); ++i)
    {
        if(path == m_ui.fileListWidget->item(i)->data(Qt::UserRole).toString())
        {
            m_ui.fileListWidget->setCurrentRow(i, QItemSelectionModel::Select);
            break;
        }
    }
}

void TwoPanelFileDialogImpl::on_addButton_clicked()
{
    QStringList l = selectedFiles();

    if(!l.isEmpty())
    {
        addToHistory(l.first());
        addFiles(l, false);
    }
}

void TwoPanelFileDialogImpl::on_playButton_clicked()
{
    QStringList l = selectedFiles();

    if(!l.isEmpty())
    {
        addToHistory(l.first());
        addFiles(l, true);
    }
}

void TwoPanelFileDialogImpl::setModeAndMask(const QString& path, FileDialog::Mode m, const QStringList& mask)
{
    m_mode = m;
    m_ui.dirListView->clearSelection();
    m_ui.fileTypeComboBox->clear();

    QFileInfo info(path);
    QString fileName;

    if(info.isFile())
    {
        fileName = info.fileName();
        info = QFileInfo(info.absolutePath());
    }

    if(!info.exists())
    {
        info = QFileInfo(QDir::homePath());
        fileName.clear();
    }

    if (m_dirModel->filePath(m_ui.dirListView->rootIndex()) != info.absolutePath())
    {
        m_dirModel->setRootPath(info.path());
        m_ui.dirListView->setRootIndex(m_dirModel->index(info.absolutePath()));
    }
    m_ui.dirListView->setCurrentIndex(m_dirModel->index(info.absoluteFilePath()));
    m_ui.lookInComboBox->setEditText(info.absolutePath());

    m_ui.fileNameLineEdit->setText(fileName);
    m_ui.addButton->setEnabled(!fileName.isEmpty());
    m_ui.playButton->setEnabled(false);
    m_ui.playButton->setVisible(m == FileDialog::PlayDirsFiles);


    switch (m)
    {
    case FileDialog::AddFile:
    case FileDialog::AddFiles:
    case FileDialog::AddDirsFiles:
    case FileDialog::PlayDirsFiles:
    {
        m_ui.fileListWidget->setVisible(true);
        m_ui.addButton->setText(tr("Add"));
        m_ui.fileTypeComboBox->clear();
        m_ui.fileTypeComboBox->addItems(mask);
        m_ui.fileTypeComboBox->setEnabled(true);
        on_fileTypeComboBox_activated(0);
        m_ui.dirListView->setSelectionMode(QAbstractItemView::SingleSelection);
        if(m == FileDialog::AddFile)
            m_ui.fileListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        else
            m_ui.fileListWidget->setSelectionMode (QAbstractItemView::ExtendedSelection);
        break;
    }
    case FileDialog::AddDir:
    case FileDialog::AddDirs:
    {
        m_ui.fileListWidget->setVisible(false);
        m_ui.addButton->setText(tr("Add"));
        m_ui.fileTypeComboBox->clear();
        m_ui.fileTypeComboBox->addItem(tr("Directories"));
        m_ui.fileTypeComboBox->setEnabled(false);
        if(m == FileDialog::AddDir)
            m_ui.dirListView->setSelectionMode(QAbstractItemView::SingleSelection);
        else
            m_ui.dirListView->setSelectionMode (QAbstractItemView::ExtendedSelection);
        m_ui.addButton->setEnabled(!m_ui.dirListView->selectionModel()->selectedRows().isEmpty());
        break;
    }
    case FileDialog::SaveFile:
    {
        m_ui.fileListWidget->setVisible(false);
        m_ui.addButton->setText(tr("Save"));
        m_ui.fileTypeComboBox->clear();
        m_ui.fileTypeComboBox->addItems(mask);
        m_ui.fileTypeComboBox->setEnabled(true);
        on_fileTypeComboBox_activated(0);
        m_ui.dirListView->setSelectionMode(QAbstractItemView::SingleSelection);
        m_ui.fileListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        break;
    }
    default:
        ;
    }
}

void TwoPanelFileDialogImpl::on_fileTypeComboBox_activated(int index)
{
    m_filters = qt_clean_filter_list(m_ui.fileTypeComboBox->itemText(index));

    m_ui.fileListWidget->clear();

    QModelIndexList indexes = m_ui.dirListView->selectionModel()->selectedRows(0);

    if(!indexes.isEmpty() && indexes.first().isValid())
        updateFileList(m_dirModel->filePath(indexes.first()));
}

void TwoPanelFileDialogImpl::updateFileList(const QString &path)
{
    m_ui.fileListWidget->clear();
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);

    foreach (QFileInfo info, dir.entryInfoList(m_filters))
    {
        QListWidgetItem *item = new QListWidgetItem(info.fileName());
        item->setIcon(m_dirModel->iconProvider()->icon(info));
        item->setData(Qt::UserRole, info.absoluteFilePath());
        m_ui.fileListWidget->addItem(item);
    }
}

void TwoPanelFileDialogImpl::hideEvent (QHideEvent *event)
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("TwoPanelFileDialog/geometry", saveGeometry());
    settings.setValue("TwoPanelFileDialog/history", m_history);
    QWidget::hideEvent(event);
}

void TwoPanelFileDialogImpl::addToHistory(const QString &path)
{
    QString path_copy = path;
    if (path_copy.endsWith('/'))
        path_copy.remove(path.size()-1, 1);
    QString dir_path = path.left(path_copy.lastIndexOf ('/'));

    m_history.removeAll(dir_path);
    m_history.prepend(dir_path);

    while ( m_history.size() > HISTORY_SIZE)
        m_history.removeLast();

    m_ui.lookInComboBox->clear();
    m_ui.lookInComboBox->addItems(m_history);
}

void TwoPanelFileDialogImpl::addFiles(const QStringList &list, bool play)
{
    if (!isModal())
    {
        emit filesSelected(list, play);
        accept();
    }
    else if (m_mode == FileDialog::SaveFile)
    {
        //check file extension
        QString f_name = m_ui.fileNameLineEdit->text();
        bool contains = false;
        foreach(QString str, qt_clean_filter_list(m_ui.fileTypeComboBox->currentText()))
        {
            QRegExp regExp(str);
            regExp.setPatternSyntax(QRegExp::Wildcard);
            if (f_name.contains(regExp))
            {
                contains = true;
                break;
            }
        }
        //add extensio to file name
        if (!contains)
        {
            QString ext = qt_clean_filter_list(m_ui.fileTypeComboBox->currentText())[0];
            ext.remove("*");
            if (!ext.isEmpty() && ext != ".")
            {
                f_name.append(ext);
                qDebug("TwoPanelFileDialogImpl: added file extension");
                m_ui.fileNameLineEdit->setText(f_name);
                return;
            }
        }
        QFileInfo info(list[0]);

        if (info.exists())
        {
            if (QMessageBox::question (this, windowTitle (),
                                       tr("%1 already exists.\nDo you want to replace it?")
                                       .arg(m_ui.fileNameLineEdit->text()),
                                       QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
                accept();
            else
                return;

        }
        accept();
    }
    else
        accept();
}
