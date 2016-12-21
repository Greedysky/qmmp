/***************************************************************************
 *   Copyright (C) 2011-2015 by Ilya Kotov                                 *
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

#include <QFile>
#include <QTextStream>
#include <qmmp/qmmp.h>
#include "aboutqsuidialog.h"

AboutQSUIDialog::AboutQSUIDialog(QWidget *parent) :
    QDialog(parent)
{
    m_ui.setupUi(this);
    m_ui.aboutTextEdit->setHtml(loadAbout());
}

AboutQSUIDialog::~AboutQSUIDialog(){}

QString AboutQSUIDialog::loadAbout()
{
    QString text;
    text.append("<head>");
    text.append("<META content=\"text/html; charset=UTF-8\">");
    text.append("</head>");
    text.append("<h3>"+tr("Qmmp Simple User Interface (QSUI)")+"</h3>");
    text.append(tr("Qmmp version: <b>%1</b>").arg(Qmmp::strVersion()));
    text.append("<p>");
    text.append(tr("Simple user interface based on standard widgets set."));
    text.append("</p>");
    text.append("<b>" + tr("Developers:") + "</b>");
    text.append("<p>" + tr("Ilya Kotov <forkotov02@hotmail.ru>") + "</p>");

    text.append("<b>" + tr("Translators:") + "</b>");
    text.append("<p>");
    text.append(getStringFromResource(":translators").replace("<", "&lt;")
                .replace(">", "&gt;").replace("\n", "<br>"));
    text.append("</p>");
    return text;
}

QString AboutQSUIDialog::getStringFromResource(const QString& res_file)
{
    QString ret_string;
    QStringList paths;
    paths << res_file + "_" + Qmmp::systemLanguageID() + ".txt";
    if(Qmmp::systemLanguageID().contains("."))
        paths << res_file + "_" + Qmmp::systemLanguageID().split(".").at(0) + ".txt";
    if(Qmmp::systemLanguageID().contains("_"))
        paths << res_file + "_" + Qmmp::systemLanguageID().split("_").at(0) + ".txt";
    paths << res_file + ".txt";
    paths << res_file;

    foreach(QString path, paths)
    {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream ts(&file);
            ts.setCodec("UTF-8");
            ret_string = ts.readAll();
            file.close();
            return ret_string;
        }
    }
    return ret_string;
}
