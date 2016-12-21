/***************************************************************************
 *   Copyright (C) 2011-2016 by Ilya Kotov                                 *
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

#include <QSettings>
#include <QDesktopServices>
#include <QMenu>
#include <QFile>
#include <QDir>
#include <QProgressBar>
#include <QThreadPool>
#include <QProcess>
#include <QMessageBox>
#include <qmmpui/playlistitem.h>
#include <qmmpui/metadataformatter.h>
#include <qmmpui/filedialog.h>
#include <qmmp/metadatamanager.h>
#include "converter.h"
#include "preseteditor.h"
#include "converterdialog.h"

ConverterDialog::ConverterDialog(QList <PlayListTrack *> tracks,  QWidget *parent) : QDialog(parent)
{
    m_ui.setupUi(this);
    m_ui.tableWidget->verticalHeader()->setDefaultSectionSize(fontMetrics().height() + 3);
    m_ui.tableWidget->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    QStringList paths;
    MetaDataFormatter formatter("%if(%p&%t,%p - %t,%f) - %l");
    foreach(PlayListTrack *track, tracks)
    {
        //skip streams
        if(track->length() == 0 || track->url().contains("://"))
            continue;
        //skip duplicates
        if(paths.contains(track->url()))
            continue;
        //skip unsupported files
        if(!MetaDataManager::instance()->supports(track->url()))
            continue;

        paths.append(track->url());
        QString name = formatter.format(track);
        QTableWidgetItem *item = new QTableWidgetItem(name);
        item->setData(Qt::UserRole, track->url());
        item->setData(Qt::ToolTipRole, track->url());
        m_ui.tableWidget->insertRow(m_ui.tableWidget->rowCount());
        m_ui.tableWidget->setItem(m_ui.tableWidget->rowCount() - 1, 0, item);
        QProgressBar *progressBar = new QProgressBar(this);
        progressBar->setRange(0, 100);
        m_ui.tableWidget->setCellWidget(m_ui.tableWidget->rowCount() - 1, 1, progressBar);
        m_ui.tableWidget->setItem(m_ui.tableWidget->rowCount() - 1, 2, new QTableWidgetItem());
    }
    m_ui.tableWidget->resizeColumnsToContents();

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Converter");
    QString music_path = QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
    m_ui.outDirEdit->setText(settings.value("out_dir", music_path).toString());
    m_ui.outFileEdit->setText(settings.value("file_name","%p - %t").toString());
    m_ui.overwriteCheckBox->setChecked(settings.value("overwrite",false).toBool());
    restoreGeometry(settings.value("geometry").toByteArray());
    settings.endGroup();
    createMenus();

    readPresets(":/converter/presets.conf");
    readPresets(Qmmp::configDir() + "converter/presets.conf");
}

ConverterDialog::~ConverterDialog()
{
    savePresets();
    on_stopButton_clicked();
}

QVariantMap ConverterDialog::preset() const
{
    if(m_ui.presetComboBox->currentIndex() == -1)
        return QVariantMap();
    int index = m_ui.presetComboBox->currentIndex();
    //aditional parameters
    QVariantMap preset = m_ui.presetComboBox->itemData(index).toMap();
    preset["out_dir"] = m_ui.outDirEdit->text();
    preset["file_name"] = m_ui.outFileEdit->text();
    preset["overwrite"] = m_ui.overwriteCheckBox->isChecked();
    return preset;
}

void ConverterDialog::on_dirButton_clicked()
{
    QString dir = FileDialog::getExistingDirectory(this, tr("Choose a directory"),
                                        m_ui.outDirEdit->text());
    if(!dir.isEmpty())
        m_ui.outDirEdit->setText(dir);
}

void ConverterDialog::on_convertButton_clicked()
{
    if(!checkPreset(preset()))
        return;

    m_ui.convertButton->setEnabled(false);
    m_converters.clear();
    for(int i = 0; i < m_ui.tableWidget->rowCount(); ++i)
    {
        QString url = m_ui.tableWidget->item(i, 0)->data(Qt::UserRole).toString();
        Converter *converter = new Converter();


        if(!converter->prepare(url, i, preset()))
        {
            m_ui.tableWidget->item(i, 2)->setText(tr("Error"));
            delete converter;
            continue;
        }
        else
            m_ui.tableWidget->item(i, 2)->setText(tr("Waiting"));

        converter->setAutoDelete(false);
        m_converters.append(converter);
        connect(converter, SIGNAL(progress(int)), m_ui.tableWidget->cellWidget(i, 1), SLOT(setValue(int)));
        connect(converter, SIGNAL(finished(Converter *)), SLOT(onConvertFinished(Converter *)));
        connect(converter, SIGNAL(message(int, QString)), SLOT(onStateChanged(int, QString)));
        QThreadPool::globalInstance()->start(converter);
    }
    m_ui.tableWidget->resizeColumnsToContents();
}

void ConverterDialog::on_stopButton_clicked()
{
    if(m_converters.isEmpty())
        return;

    foreach(Converter *c, m_converters)
        c->stop();
    QThreadPool::globalInstance()->waitForDone();
    qDeleteAll(m_converters);
    m_converters.clear();
    m_ui.convertButton->setEnabled(true);
}

void ConverterDialog::onStateChanged(int row, QString message)
{
    m_ui.tableWidget->item(row, 2)->setText(message);
    m_ui.tableWidget->resizeColumnsToContents();
}

void ConverterDialog::onConvertFinished(Converter *c)
{
    if(m_converters.contains(c))
    {
        m_converters.removeAll(c);
        delete c;
    }
    if(m_converters.isEmpty())
        m_ui.convertButton->setEnabled(true);
}

void ConverterDialog::reject()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Converter");
    settings.setValue("out_dir", m_ui.outDirEdit->text());
    settings.value("file_name", m_ui.outFileEdit->text());
    settings.setValue("overwrite", m_ui.overwriteCheckBox->isChecked());
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();
    QDialog::reject();
}

void ConverterDialog::createMenus()
{
    QMenu *fileNameMenu = new QMenu(this);
    fileNameMenu->addAction(tr("Artist"))->setData("%p");
    fileNameMenu->addAction(tr("Album"))->setData("%a");
    fileNameMenu->addAction(tr("Album Artist"))->setData("%aa");
    fileNameMenu->addAction(tr("Title"))->setData("%t");
    fileNameMenu->addAction(tr("Track Number"))->setData("%n");
    fileNameMenu->addAction(tr("Two-digit Track Number"))->setData("%NN");
    fileNameMenu->addAction(tr("Genre"))->setData("%g");
    fileNameMenu->addAction(tr("Comment"))->setData("%c");
    fileNameMenu->addAction(tr("Composer"))->setData("%C");
    fileNameMenu->addAction(tr("Duration"))->setData("%l");
    fileNameMenu->addAction(tr("Disc Number"))->setData("%D");
    fileNameMenu->addAction(tr("File Name"))->setData("%f");
    fileNameMenu->addAction(tr("File Path"))->setData("%F");
    fileNameMenu->addAction(tr("Year"))->setData("%y");
    fileNameMenu->addAction(tr("Condition"))->setData("%if(%p&%t,%p - %t,%f)");
    m_ui.fileNameButton->setMenu(fileNameMenu);
    m_ui.fileNameButton->setPopupMode(QToolButton::InstantPopup);
    connect(fileNameMenu, SIGNAL(triggered(QAction *)), SLOT(addTitleString(QAction *)));

    QMenu *presetMenu = new QMenu(this);
    presetMenu->addAction(tr("Create"), this, SLOT(createPreset()));
    presetMenu->addAction(tr("Edit"), this, SLOT(editPreset()));
    presetMenu->addAction(tr("Create a Copy"), this, SLOT(copyPreset()));
    presetMenu->addAction(tr("Delete"), this, SLOT(deletePreset()));
    m_ui.presetButton->setMenu(presetMenu);
    m_ui.presetButton->setPopupMode(QToolButton::InstantPopup);
}

void ConverterDialog::addTitleString(QAction *a)
{
    if (m_ui.outFileEdit->cursorPosition () < 1)
        m_ui.outFileEdit->insert(a->data().toString());
    else
        m_ui.outFileEdit->insert(" - "+a->data().toString());
}

void ConverterDialog::createPreset()
{
    PresetEditor *editor = new PresetEditor(QVariantMap(), this);
    if(editor->exec() == QDialog::Accepted)
    {
        QVariantMap data = editor->data();
        data["name"] = uniqueName(data["name"].toString());
        if(data["name"].isValid() && data["ext"].isValid() && data["command"].isValid())
            m_ui.presetComboBox->addItem (data["name"].toString(), data);
    }
    editor->deleteLater();
}

void ConverterDialog::editPreset()
{
    if(m_ui.presetComboBox->currentIndex() == -1)
        return;
    int index = m_ui.presetComboBox->currentIndex();

    PresetEditor *editor = new PresetEditor(m_ui.presetComboBox->itemData(index).toMap(), this);
    if(editor->exec() == QDialog::Accepted)
    {
        QVariantMap data = editor->data();
        if(m_ui.presetComboBox->currentText() != data["name"].toString())
            data["name"] = uniqueName(data["name"].toString());
        if(data["name"].isValid() && data["ext"].isValid() && data["command"].isValid())
        {
            m_ui.presetComboBox->setItemText(index, data["name"].toString());
            m_ui.presetComboBox->setItemData(index, data);
        }
    }
    editor->deleteLater();
}

void ConverterDialog::copyPreset()
{
    if(m_ui.presetComboBox->currentIndex() == -1)
        return;
    int index = m_ui.presetComboBox->currentIndex();
    QVariantMap data = m_ui.presetComboBox->itemData(index).toMap();
    data["name"] = uniqueName(data["name"].toString());
    data["read_only"] = false;
    m_ui.presetComboBox->addItem (data["name"].toString(), data);
}

void ConverterDialog::deletePreset()
{
    if(m_ui.presetComboBox->currentIndex() == -1)
        return;
    if(m_ui.presetComboBox->itemData(m_ui.presetComboBox->currentIndex()).toMap()["read_only"].toBool())
        return;
    m_ui.presetComboBox->removeItem(m_ui.presetComboBox->currentIndex());
}

void ConverterDialog::readPresets(const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
        return;

    QList <QVariantMap> dataList;
    while(!file.atEnd())
    {
        QString line = QString::fromUtf8(file.readLine().trimmed());
        if(!line.contains("="))
            continue;
        QString key = line.split("=").at(0);
        QString value = line.split("=").at(1);
        if(key == "name")
            dataList.append(QVariantMap());
        if(dataList.isEmpty())
            continue;
        if(key == "use_16bit" && key == "tags") //boolean keys
            dataList.last()[key] = (value == "true");
        else
            dataList.last()[key] = value;
    }

    foreach(QVariantMap data, dataList)
    {
        data["read_only"] = path.startsWith(":/");
        QString title = data["name"].toString();
        if(data["read_only"].toBool())
            title += " *" ;
        m_ui.presetComboBox->addItem (title, data);
    }
}

void ConverterDialog::savePresets()
{
    QDir dir(Qmmp::configDir());
    dir.mkdir("converter");

    QFile file(Qmmp::configDir() + "converter/presets.conf");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qWarning("ConverterDialog: unable to save presets; error %s", qPrintable(file.errorString()));
        return;
    }

    for(int i = 0; i < m_ui.presetComboBox->count(); ++i)
    {
        QVariantMap data = m_ui.presetComboBox->itemData(i).toMap();
        if(data["read_only"].toBool())
            continue;
        file.write(QString("%1=%2\n").arg("name").arg(data["name"].toString()).toUtf8());
        file.write(QString("%1=%2\n").arg("ext").arg(data["ext"].toString()).toUtf8());
        file.write(QString("%1=%2\n").arg("command").arg(data["command"].toString()).toUtf8());
        file.write(QString("%1=%2\n").arg("use_16bit")
                   .arg(data["use_16bit"].toBool() ? "true" : "false" ).toUtf8());
        file.write(QString("%1=%2\n").arg("tags")
                   .arg(data["tags"].toBool() ? "true" : "false" ).toUtf8());
        file.write("\n");
    }
}

QString ConverterDialog::uniqueName(const QString &name)
{
    QString unique_name = name;
    int i = 0;
    forever
    {
        if(m_ui.presetComboBox->findText(unique_name) == -1)
            break;
        unique_name = name + QString("_%1").arg(++i);
    }
    return unique_name;
}

bool ConverterDialog::checkPreset(const QVariantMap &preset)
{
    QStringList programAndArgs = preset["command"].toString().split(" ", QString::SkipEmptyParts);
    if(programAndArgs.isEmpty())
        return false;

    QString program = programAndArgs.first();

    int result = QProcess::execute(program);

    if(result == -2)
    {
        QMessageBox::warning(this, tr("Error"), tr("Unable to execute \"%1\". Program not found.")
                             .arg(program));
        return false;
    }
    else if(result < 0)
    {
        QMessageBox::warning(this, tr("Error"), tr("Process \"%1\" finished with error.")
                             .arg(program));
        return false;
    }
    return true;
}
