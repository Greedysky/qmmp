/***************************************************************************
 * Based on smplayer - GUI front-end for mplayer                           *
 *                                                                         *
 * Copyright (C) 2006-2014 Ricardo Villalba <rvm@users.sourceforge.net>    *
 * Copyright (C) 2014 Ilya Kotov forkotov02@hotmail.ru                     *
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
#include <QApplication>
#include <QMessageBox>
#include <qmmp/metadatamanager.h>
#include "ui_winfileassocpage.h"
#include "winfileassocpage_p.h"
#include "winfileassoc.h"

WinFileAssocPage::WinFileAssocPage(QWidget *parent): QWidget(parent)
{
    m_ui = new Ui::WinFileAssocPage;
    m_ui->setupUi(this);

    if (QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA)
	{
		//Hide Select None - One cannot restore an association in Vista. Go figure.
        m_ui->selectNone->hide();
    }

    foreach (QString ext, MetaDataManager::instance()->nameFilters())
    {
        ext.remove("*.");
        m_ui->listWidget->addItem(ext);
        m_extensions.append(ext);
    }
    loadAssociations();
    createHelp();
}

WinFileAssocPage::~WinFileAssocPage()
{
    saveAssociations();
}

void WinFileAssocPage::on_selectAll_clicked()
{
    for (int k = 0; k < m_ui->listWidget->count(); k++)
        m_ui->listWidget->item(k)->setCheckState(Qt::Checked);
    m_ui->listWidget->setFocus();
}

void WinFileAssocPage::on_selectNone_clicked()
{
    for (int k = 0; k < m_ui->listWidget->count(); k++)
        m_ui->listWidget->item(k)->setCheckState(Qt::Unchecked);
    m_ui->listWidget->setFocus();
}

void WinFileAssocPage::loadAssociations()
{
	m_regExtensions.clear(); 
    WinFileAssoc ().GetRegisteredExtensions(m_extensions, m_regExtensions);

    for (int k = 0; k < m_ui->listWidget->count(); k++)
	{
        QListWidgetItem* pItem = m_ui->listWidget->item(k);
		if (pItem)
		{
			pItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

            if (m_regExtensions.contains(pItem->text()))
			{
				pItem->setCheckState(Qt::Checked);
				//Don't allow de-selection in windows VISTA if extension is registered.
				//VISTA doesn't seem to support extension 'restoration' in the API.
                if (QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA)
                {
					pItem->setFlags(0);
				}
			}
			else
			{
				pItem->setCheckState(Qt::Unchecked);
			}
		}
	}
}

int WinFileAssocPage::ProcessAssociations(QStringList& current, QStringList& old)
{
	WinFileAssoc RegAssoc; 

	QStringList toRestore; 

	//Restore unselected associations
	foreach(const QString& ext, old)
	{
		if (!current.contains(ext))
			toRestore.append(ext); 
	}

	RegAssoc.RestoreFileAssociations(toRestore); 
	return RegAssoc.CreateFileAssociations(current); 
}

void WinFileAssocPage::saveAssociations()
{
	QStringList extensions; 

    for (int k = 0; k < m_ui->listWidget->count(); k++)
	{
        QListWidgetItem* pItem = m_ui->listWidget->item(k);
		if (pItem && pItem->checkState() == Qt::Checked)
			extensions.append(pItem->text()); 
	}

    if(extensions == m_regExtensions)
        return;

	int processed = ProcessAssociations(extensions, m_regExtensions); 

	if (processed != extensions.count())
	{
		QMessageBox::warning(this, tr("Warning"), 
            tr("Not all files could be associated. Please check your "
               "security permissions and retry."), QMessageBox::Ok);
	}
}

void WinFileAssocPage::createHelp()
{
    m_ui->selectAll->setToolTip(tr("Check all file types in the list"));
    m_ui->selectNone->setToolTip(tr("Uncheck all file types in the list"));
    m_ui->listWidget->setToolTip(tr("Check the media file extensions you would like Qmmp to handle. "
                                    "When you click Apply, the checked files will be associated with "
                                    "Qmmp. If you uncheck a media type, the file association will "
                                    "be restored.") + "<br>" +
                                 tr("<b>Note:</b> Restoration doesn't work on Windows Vista/7."));
}
