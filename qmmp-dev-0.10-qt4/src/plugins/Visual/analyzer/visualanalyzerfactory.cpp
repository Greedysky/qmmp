/***************************************************************************
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

#include <QMessageBox>
#include <QTranslator>
#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "settingsdialog.h"
#include "visualanalyzerfactory.h"
#include "analyzer.h"

const VisualProperties VisualAnalyzerFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Analyzer Plugin");
    properties.shortName = "analyzer";
    properties.hasSettings = true;
    properties.hasAbout = true;
    return properties;
}

Visual *VisualAnalyzerFactory::create(QWidget *parent)
{
    return new Analyzer(parent);
}

QDialog *VisualAnalyzerFactory::createConfigDialog(QWidget *parent)
{
    return new SettingsDialog(parent);
}

void VisualAnalyzerFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About Analyzer Visual Plugin"),
                        tr("Qmmp Analyzer Visual Plugin")+"\n"+
                        tr("Written by: Ilya Kotov <forkotov02@hotmail.ru>"));
}

QTranslator *VisualAnalyzerFactory::createTranslator(QObject *parent)
{
    QTranslator *translator = new QTranslator(parent);
    QString locale = Qmmp::systemLanguageID();
    translator->load(QString(":/analyzer_plugin_") + locale);
    return translator;
}

Q_EXPORT_PLUGIN2(analyzer,VisualAnalyzerFactory)
