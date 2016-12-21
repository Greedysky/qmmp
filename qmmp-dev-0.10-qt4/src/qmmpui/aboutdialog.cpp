/**************************************************************************
*   Copyright (C) 2006-2015 by Ilya Kotov                                 *
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
#include <qmmp/decoder.h>
#include <qmmp/decoderfactory.h>
#include <qmmp/output.h>
#include <qmmp/outputfactory.h>
#include <qmmp/visual.h>
#include <qmmp/visualfactory.h>
#include <qmmp/effect.h>
#include <qmmp/effectfactory.h>
#include <qmmp/qmmp.h>
#include <qmmpui/general.h>
#include <qmmpui/generalfactory.h>
#include "aboutdialog_p.h"


AboutDialog::AboutDialog(QWidget* parent, Qt::WFlags fl)
        : QDialog( parent, fl )
{
    setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    licenseTextBrowser->setPlainText(getStringFromResource(":COPYING"));
    aboutTextBrowser->setHtml(loadAbout());
    authorsTextBrowser->setPlainText(getStringFromResource(":authors"));
    thanksToTextBrowser->setPlainText(getStringFromResource(":thanks"));
    translatorsTextBrowser->setPlainText(getStringFromResource(":translators"));
}

AboutDialog::~AboutDialog()
{}

/*$SPECIALIZATION$*/
QString AboutDialog::loadAbout()
{
    QString text;
    text.append("<head>");
    text.append("<META content=\"text/html; charset=UTF-8\">");
    text.append("</head>");
    if(layoutDirection() == Qt::RightToLeft)
        text.append("<div dir='rtl'>");
    else
        text.append("<div>");
    text.append("<h3>"+tr("Qt-based Multimedia Player (Qmmp)")+"</h3>");

    text.append("<p>"+getStringFromResource(":description")+"</p>");

    text.append("<p><b>"+tr("Version: %1").arg(Qmmp::strVersion()) + "</b><br>");
    text.append(tr("Using Qt %1 (compiled with Qt %2)" ).arg(qVersion()).arg(QT_VERSION_STR) + "</p>");

    text.append("<p>");
    text.append(tr("(c) %1-%2 Qmmp Development Team").arg(2006).arg(2016)+"<br>");
    text.append("<a href=\"http://qmmp.ylsoftware.com/\">http://qmmp.ylsoftware.com/</a><br>");
    text.append("<a href=\"https://sourceforge.net/projects/qmmp-dev/\">https://sourceforge.net/projects/qmmp-dev/</a>");
    text.append("</p>");

    text.append("<h5>"+tr("Input plugins:")+"</h5>");
    text.append("<ul type=\"square\">");
    foreach(DecoderFactory *fact, Decoder::factories())
    {
        text.append("<li>");
        text.append(fact->properties().name);
        text.append("</li>");
    }
    text.append("</ul>");
    text.append("<h5>"+tr("Output plugins:")+"</h5>");
    text.append("<ul type=\"square\">");
    foreach(OutputFactory *fact, Output::factories())
    {
        text.append("<li>");
        text.append(fact->properties().name);
        text.append("</li>");
    }
    text.append("</ul>");
    text.append("<h5>"+tr("Visual plugins:")+"</h5>");
    text.append("<ul type=\"square\">");
    foreach(VisualFactory *fact, Visual::factories())
    {
        text.append("<li>");
        text.append(fact->properties().name);
        text.append("</li>");
    }
    text.append("</ul>");
    text.append("<h5>"+tr("Effect plugins:")+"</h5>");
    text.append("<ul type=\"square\">");
    foreach(EffectFactory *fact, Effect::factories())
    {
        text.append("<li>");
        text.append(fact->properties().name);
        text.append("</li>");
    }
    text.append("</ul>");
    text.append("<h5>"+tr("General plugins:")+"</h5>");
    text.append("<ul type=\"square\">");
    foreach(GeneralFactory *fact, General::factories())
    {
        text.append("<li>");
        text.append(fact->properties().name);
        text.append("</li>");
    }
    text.append("</ul>");
    text.append("<div>");

    return text;
}

QString AboutDialog::getStringFromResource(const QString& res_file)
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
