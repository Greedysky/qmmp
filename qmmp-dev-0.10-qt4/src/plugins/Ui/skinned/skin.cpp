/***************************************************************************
 *   Copyright (C) 2007-2016 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   Based on Promoe, an XMMS2 Client                                      *
 *   Copyright (C) 2005-2006 by XMMS2 Team                                 *
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

#include <QDir>
#include <QSettings>
#include <QPainter>
#include <QPolygon>
#include <QImage>
#include <QAction>
#include <QTextStream>
#include <QStringList>
#include <qmmp/qmmp.h>
#include "actionmanager.h"
#include "skin.h"
#include "cursorimage.h"

Skin *Skin::m_instance = 0;

Skin *Skin::instance()
{
    if (!m_instance)
        m_instance = new Skin();
    return m_instance;
}

QPixmap Skin::getPixmap (const QString &name, QDir dir)
{
    dir.setFilter (QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << name + ".*");
    QFileInfoList f = dir.entryInfoList();
    if(!f.isEmpty())
        return QPixmap(f.first().filePath());
    return QPixmap();
}

Skin::Skin (QObject *parent) : QObject (parent)
{
    m_instance = this;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    QString path = settings.value("Skinned/skin_path").toString();
    if (path.isEmpty() || !QDir(path).exists ())
        path = ":/glare";
    m_double_size = settings.value("Skinned/double_size", false).toBool();
    m_antialiasing = settings.value("Skinned/antialiasing", false).toBool();
    ACTION(ActionManager::WM_DOUBLE_SIZE)->setChecked(m_double_size);
    ACTION(ActionManager::WM_ANTIALIASING)->setChecked(m_antialiasing);
    setSkin (QDir::cleanPath(path));
    /* skin directory */
    QDir skinDir(Qmmp::configDir());
    skinDir.mkdir ("skins");
}

Skin::~Skin()
{}

void Skin::setSkin (const QString& path)
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_use_cursors = settings.value("Skinned/skin_cursors", false).toBool();
    m_double_size = ACTION(ActionManager::WM_DOUBLE_SIZE)->isChecked();
    m_antialiasing = ACTION(ActionManager::WM_ANTIALIASING)->isChecked();
    settings.setValue("Skinned/skin_path",path);
    qDebug ("Skin: using %s",qPrintable(path));
    m_skin_dir = QDir (path);
    //clear old values
    m_pledit_txt.clear();
    m_buttons.clear();
    m_titlebar.clear();
    m_numbers.clear();
    m_pl_parts.clear();
    m_eq_parts.clear();
    m_eq_bar.clear();
    m_eq_spline.clear();
    m_vis_colors.clear();
    m_cursors.clear();
    //load skin parts
    loadPLEdit();
    loadMain();
    loadButtons();
    loadShufRep();
    loadTitleBar();
    loadPosBar();
    loadNumbers();
    loadPlayList();
    loadEq_ex();
    loadEqMain();
    loadVisColor();
    loadLetters();
    loadMonoSter();
    loadVolume();
    loadBalance();
    loadRegion();
    loadCursors();
    loadColors();
    if(m_double_size)
    {
        uint key;
        foreach(key, m_buttons.keys())
            m_buttons[key] = scalePixmap(m_buttons[key]);
        foreach(key, m_titlebar.keys())
            m_titlebar[key] = scalePixmap(m_titlebar[key]);
        foreach(key, m_pl_parts.keys())
            m_pl_parts[key] = scalePixmap(m_pl_parts[key]);
        foreach(key, m_eq_parts.keys())
            m_eq_parts[key] = scalePixmap(m_eq_parts[key]);
        foreach(key, m_ms_parts.keys())
            m_ms_parts[key] = scalePixmap(m_ms_parts[key]);
        foreach(key, m_parts.keys())
            m_parts[key] = scalePixmap(m_parts[key]);
        foreach(QChar c, m_letters.keys())
            m_letters[c] = scalePixmap(m_letters[c]);
        m_main = scalePixmap(m_main);
        posbar = scalePixmap(posbar);
        int i;
        for(i = 0; i < m_numbers.size(); ++i)
            m_numbers[i] = scalePixmap(m_numbers[i]);

        for(i = 0; i < m_eq_bar.size(); ++i)
            m_eq_bar[i] = scalePixmap(m_eq_bar[i]);

        for(i = 0; i < m_eq_spline.size(); ++i)
            m_eq_spline[i] = scalePixmap(m_eq_spline[i]);

        for(i = 0; i < m_volume.size(); ++i)
            m_volume[i] = scalePixmap(m_volume[i]);
        for(i = 0; i < m_balance.size(); ++i)
            m_balance[i] = scalePixmap(m_balance[i]);
    }
    emit skinChanged();
}

void Skin::reloadSkin()
{
    setSkin (m_skin_dir.absolutePath ());
}

void Skin::loadMain()
{
    QPixmap *pixmap = getPixmap ("main");
    m_main = pixmap->copy (0,0,275,116);
    delete pixmap;
}

void Skin::loadCursors()
{
    if(!m_use_cursors)
    {
        for(int i = CUR_NORMAL; i <= CUR_WSWINBUT; ++i)
            m_cursors[i] = QCursor(Qt::ArrowCursor);
        m_cursors[CUR_PSIZE] = QCursor(Qt::SizeFDiagCursor);
        return;
    }
    m_cursors[CUR_NORMAL] = createCursor(getPath("normal"));
    m_cursors[CUR_CLOSE] = createCursor(getPath("close"));
    m_cursors[CUR_MAINMENU] = createCursor(getPath("mainmenu"));
    m_cursors[CUR_MIN] = createCursor(getPath("min"));
    m_cursors[CUR_POSBAR] = createCursor(getPath("posbar.cur"));
    m_cursors[CUR_SONGNAME] = createCursor(getPath("songname"));
    m_cursors[CUR_TITLEBAR] = createCursor(getPath("titlebar.cur"));
    m_cursors[CUR_VOLBAL] = createCursor(getPath("volbal"));
    m_cursors[CUR_WINBUT] = createCursor(getPath("winbut"));

    m_cursors[CUR_WSNORMAL] = createCursor(getPath("wsnormal"));
    m_cursors[CUR_WSPOSBAR] = createCursor(getPath("wsposbar"));

    m_cursors[CUR_EQCLOSE] = createCursor(getPath("eqclose"));
    m_cursors[CUR_EQNORMAL] = createCursor(getPath("eqnormal"));
    m_cursors[CUR_EQSLID] = createCursor(getPath("eqslid"));
    m_cursors[CUR_EQTITLE] = createCursor(getPath("eqtitle"));

    m_cursors[CUR_PCLOSE] = createCursor(getPath("pclose"));
    m_cursors[CUR_PNORMAL] = createCursor(getPath("pnormal"));
    m_cursors[CUR_PSIZE] = createCursor(getPath("psize"));
    if(m_cursors[CUR_PSIZE].shape() == Qt::ArrowCursor)
        m_cursors[CUR_PSIZE] = QCursor(Qt::SizeFDiagCursor);
    m_cursors[CUR_PTBAR] = createCursor(getPath("ptbar"));
    m_cursors[CUR_PVSCROLL] = createCursor(getPath("pvscroll"));
    m_cursors[CUR_PWINBUT] = createCursor(getPath("pwinbut"));

    m_cursors[CUR_PWSNORM] = createCursor(getPath("pwsnorm"));
    m_cursors[CUR_PWSSIZE] = createCursor(getPath("pwssize"));

    m_cursors[CUR_VOLBAR] = createCursor(getPath("volbar"));
    m_cursors[CUR_WSCLOSE] = createCursor(getPath("wsclose"));
    m_cursors[CUR_WSMIN] = createCursor(getPath("wsmin"));
    m_cursors[CUR_WSWINBUT] = createCursor(getPath("wswinbut"));
}

void Skin::loadColors()
{
    QPixmap *pixmap = getPixmap ("text");
    QImage img = pixmap->toImage();
    m_main_colors[MW_BACKGROUND] = QColor::fromRgb(img.pixel(144, 3));
    QRgb mwfg = 0, mwbg = img.pixel(144, 3);
    uint diff = 0;

    for(int x = 0; x < pixmap->width(); ++x)
    {
        for(int y = 0; y < pixmap->height(); ++y)
        {
            QRgb c = img.pixel(x, y);
            uint d = abs(qRed(mwbg) - qRed(c)) +
                    abs(qBlue(mwbg) - qBlue(c)) +
                    abs(qGreen(mwbg) - qGreen(c));
            if(d > diff)
            {
                diff = d;
                mwfg = c;
            }
        }
    }
    m_main_colors[MW_FOREGROUND] = QColor::fromRgb(mwfg);
    delete pixmap;
}

void Skin::loadButtons()
{
    QPixmap *pixmap = getPixmap ("cbuttons");
    pixmap = correctSize(pixmap, 136, pixmap->height());

    m_buttons[BT_PREVIOUS_N] = pixmap->copy (0, 0,23,18);
    m_buttons[BT_PREVIOUS_P] = pixmap->copy (0,18,23,18);

    m_buttons[BT_PLAY_N] = pixmap->copy (23, 0,23,18);
    m_buttons[BT_PLAY_P] = pixmap->copy (23,18,23,18);

    m_buttons[BT_PAUSE_N] = pixmap->copy (46, 0,23,18);
    m_buttons[BT_PAUSE_P] = pixmap->copy (46,18,23,18);

    m_buttons[BT_STOP_N] = pixmap->copy (69, 0,23,18);
    m_buttons[BT_STOP_P] = pixmap->copy (69,18,23,18);

    m_buttons[BT_NEXT_N] = pixmap->copy (92, 0,22,18);
    m_buttons[BT_NEXT_P] = pixmap->copy (92,18,22,18);

    m_buttons[BT_EJECT_N] = pixmap->copy (114, 0,22,16);
    m_buttons[BT_EJECT_P] = pixmap->copy (114,16,22,16);
    delete pixmap;
}

void Skin::loadTitleBar()
{
    QPixmap *pixmap = getPixmap ("titlebar");
    m_buttons[BT_MENU_N] = pixmap->copy (0,0,9,9);
    m_buttons[BT_MENU_P] = pixmap->copy (0,9,9,9);
    m_buttons[BT_MINIMIZE_N] = pixmap->copy (9,0,9,9);
    m_buttons[BT_MINIMIZE_P] = pixmap->copy (9,9,9,9);
    m_buttons[BT_CLOSE_N] = pixmap->copy (18,0,9,9);
    m_buttons[BT_CLOSE_P] = pixmap->copy (18,9,9,9);
    m_buttons[BT_SHADE1_N] = pixmap->copy (0,18,9,9);
    m_buttons[BT_SHADE1_P] = pixmap->copy (9,18,9,9);
    m_buttons[BT_SHADE2_N] = pixmap->copy (0,27,9,9);
    m_buttons[BT_SHADE2_P] = pixmap->copy (9,27,9,9);
    m_titlebar[TITLEBAR_A] = pixmap->copy (27, 0,275,14);
    m_titlebar[TITLEBAR_I] = pixmap->copy (27,15,275,14);
    m_titlebar[TITLEBAR_SHADED_A] = pixmap->copy (27,29,275,14);
    m_titlebar[TITLEBAR_SHADED_I] = pixmap->copy (27,42,275,14);
    delete pixmap;
}

void Skin::loadPosBar()
{
    QPixmap *pixmap = getPixmap ("posbar");

    if (pixmap->width() > 249)
    {
        m_buttons[BT_POSBAR_N] = pixmap->copy (248,0,29, pixmap->height());
        m_buttons[BT_POSBAR_P] = pixmap->copy (278,0,29, pixmap->height());
    }
    else
    {
        QPixmap dummy(29, pixmap->height());
        dummy.fill(Qt::transparent);
        m_buttons[BT_POSBAR_N] = dummy;
        m_buttons[BT_POSBAR_P] = dummy;
    }
    posbar = pixmap->copy (0,0,248, qMin(pixmap->height(), 10));
    delete pixmap;
}

void Skin::loadNumbers()
{
    QPixmap *pixmap = getPixmap ("nums_ex","numbers");

    for (uint i = 0; i < 10; i++)
        m_numbers << pixmap->copy (i*9, 0, 9, pixmap->height());

    if (pixmap->width() > 107)
        m_numbers << pixmap->copy(99, 0, 9, pixmap->height());
    else
    {
        // We didn't find "-" symbol. So we have to extract it from "2".
        // Winamp uses this method too.
        QPixmap pix;
        if(pixmap->width() > 98)
            pix = pixmap->copy(90,0,9,pixmap->height());
        else
        {
            pix = QPixmap(9, pixmap->height());
            pix.fill(Qt::transparent);
        }
        QPixmap minus = pixmap->copy(18,pixmap->height()/2,9,1);
        QPainter paint(&pix);
        paint.drawPixmap(0,pixmap->height()/2, minus);
        m_numbers << pix;
    }
    delete pixmap;
}

void Skin::loadPlayList()
{
    QPixmap *pixmap = getPixmap ("pledit");

    m_pl_parts[PL_CORNER_UL_A] = pixmap->copy (0,0,25,20);
    m_pl_parts[PL_CORNER_UL_I] = pixmap->copy (0,21,25,20);

    m_pl_parts[PL_CORNER_UR_A] = pixmap->copy (153,0,25,20);
    m_pl_parts[PL_CORNER_UR_I] = pixmap->copy (153,21,25,20);

    m_pl_parts[PL_TITLEBAR_A] = pixmap->copy (26,0,100,20);
    m_pl_parts[PL_TITLEBAR_I] = pixmap->copy (26,21,100,20);

    m_pl_parts[PL_TFILL1_A] = pixmap->copy (127,0,25,20);
    m_pl_parts[PL_TFILL1_I] = pixmap->copy (127,21,25,20);

    //m_pl_parts[PL_TFILL2_A] = pixmap->copy();//FIXME: Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½
    //m_pl_parts[PL_TFILL2_I] = pixmap->copy();

    m_pl_parts[PL_LFILL] = pixmap->copy (0,42,12,29);
    m_pl_parts[PL_RFILL] = pixmap->copy (31,42,20,29); //???

    m_pl_parts[PL_LSBAR] = pixmap->copy (0,72,125,38);
    m_pl_parts[PL_RSBAR] = pixmap->copy (126,72,150,38);
    m_pl_parts[PL_SFILL1] = pixmap->copy (179,0,25,38);
    m_pl_parts[PL_SFILL2] = pixmap->copy (250,21,75,38);
    m_pl_parts[PL_TITLEBAR_SHADED1_A] = pixmap->copy (99,42,50,14);
    m_pl_parts[PL_TITLEBAR_SHADED1_I] = pixmap->copy (99,57,50,14);
    m_pl_parts[PL_TITLEBAR_SHADED2] = pixmap->copy (72,42,25,14);
    m_pl_parts[PL_TFILL_SHADED] = pixmap->copy (72,57,25,14);

    m_pl_parts[PL_CONTROL] = pixmap->copy(129,94,60,8);

    m_buttons[PL_BT_ADD] = pixmap->copy (11,80,25,18);
    m_buttons[PL_BT_SUB] = pixmap->copy (40,80,25,18);
    m_buttons[PL_BT_SEL] = pixmap->copy (70,80,25,18);
    m_buttons[PL_BT_SORT] = pixmap->copy (99,80,25,18);
    m_buttons[PL_BT_LST] = pixmap->copy(229, 80, 25, 18);
    m_buttons[PL_BT_SCROLL_N] = pixmap->copy (52,53,8,18);
    m_buttons[PL_BT_SCROLL_P] = pixmap->copy (61,53,8,18);

    m_buttons[PL_BT_CLOSE_N] = pixmap->copy (167,3,9,9);
    m_buttons[PL_BT_CLOSE_P] = pixmap->copy (52,42,9,9);
    m_buttons[PL_BT_SHADE1_N] = pixmap->copy (158,3,9,9);
    m_buttons[PL_BT_SHADE1_P] = pixmap->copy (62,42,9,9);
    m_buttons[PL_BT_SHADE2_N] = pixmap->copy (129,45,9,9);
    m_buttons[PL_BT_SHADE2_P] = pixmap->copy (150,42,9,9);

    delete pixmap;

}

QPixmap *Skin::getPixmap (const QString& name, const QString &fallback)
{
    m_skin_dir.setFilter (QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    m_skin_dir.setNameFilters(QStringList() << name + ".*");

    foreach(QFileInfo info, m_skin_dir.entryInfoList())
    {
        if(info.suffix().toLower() != "cur")
            return new QPixmap (info.filePath());
    }

    if(!fallback.isEmpty())
    {
        m_skin_dir.setNameFilters(QStringList() << fallback + ".*");
        foreach(QFileInfo info, m_skin_dir.entryInfoList())
        {
            if(info.suffix().toLower() != "cur")
                return new QPixmap (info.filePath());
        }
    }
    return getDummyPixmap(name, fallback);
}

QString Skin::getPath (const QString& name)
{
    m_skin_dir.setFilter (QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList f = m_skin_dir.entryInfoList();
    bool nameHasExt = name.contains('.');
    for (int j = 0; j < f.size(); ++j)
    {
        QFileInfo fileInfo = f.at (j);
        QString fn = fileInfo.fileName().toLower();
        if (!nameHasExt && fn.section (".",0,0) == name)
        {
            return fileInfo.filePath();
        } else if (nameHasExt && fn == name)
        {
            return fileInfo.filePath();
        }
    }
    return QString();
}


void Skin::loadPLEdit()
{
    QByteArray key, value;
    QString path = findFile("pledit.txt");

    if (path.isEmpty())
        qFatal("Skin: invalid default skin");

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qFatal("Skin: unable to open %s", qPrintable(path));
        return;
    }

    QTextStream stream(&file);

    while (!stream.atEnd ())
    {
        QString line = stream.readLine ();

        line = line.trimmed ();
        line.replace("\"","");
        if(line.contains("//"))
            line.truncate (line.indexOf ("//"));

        QStringList l = line.split ('=');
        if (l.count () == 2)
        {
            key = l[0].toLower().toLatin1();
            value = l[1].trimmed().toLatin1();
            if(!value.startsWith("#") && key != "font")
                value.prepend("#");  //add # for color if needed

            m_pledit_txt[key] = value.trimmed();

            //remove alpha channel from argb
            if(key != "font" && m_pledit_txt[key].size() > 7)
                m_pledit_txt[key].remove(1, m_pledit_txt[key].size() - 7);

        }
    }
}

void Skin::loadEqMain()
{
    QPixmap *pixmap = getPixmap ("eqmain");
    pixmap = correctSize(pixmap, pixmap->width(), 292);

    m_eq_parts[ EQ_MAIN ] = pixmap->copy (0,0,275,116);
    m_eq_parts[ EQ_TITLEBAR_A ] = pixmap->copy (0,134,275,14);
    m_eq_parts[ EQ_TITLEBAR_I ] = pixmap->copy (0,149,275,14);

    if (pixmap->height() > 295)
        m_eq_parts[ EQ_GRAPH ] = pixmap->copy (0,294,113,19);
    else
        m_eq_parts[ EQ_GRAPH ] = QPixmap();

    for (int i = 0; i < 14; ++i)
    {
        m_eq_bar << pixmap->copy (13 + i*15,164,14,63);
    }
    for (int i = 0; i < 14; ++i)
    {
        m_eq_bar << pixmap->copy (13 + i*15,229,14,63);
    }
    m_buttons[ EQ_BT_BAR_N ] = pixmap->copy (0,164,11,11);
    m_buttons[ EQ_BT_BAR_P ] = pixmap->copy (0,164+12,11,11);

    m_buttons[ EQ_BT_ON_N ] = pixmap->copy (69,119,28,12);
    m_buttons[ EQ_BT_ON_P ] = pixmap->copy (128,119,28,12);
    m_buttons[ EQ_BT_OFF_N ] = pixmap->copy (10, 119,28,12);
    m_buttons[ EQ_BT_OFF_P ] = pixmap->copy (187,119,28,12);

    m_buttons[ EQ_BT_PRESETS_N ] = pixmap->copy (224,164,44,12);
    m_buttons[ EQ_BT_PRESETS_P ] = pixmap->copy (224,176,44,12);

    m_buttons[ EQ_BT_AUTO_1_N ] = pixmap->copy (94,119,33,12);
    m_buttons[ EQ_BT_AUTO_1_P ] = pixmap->copy (153,119,33,12);
    m_buttons[ EQ_BT_AUTO_0_N ] = pixmap->copy (35, 119,33,12);
    m_buttons[ EQ_BT_AUTO_0_P ] = pixmap->copy (212,119,33,12);

    m_buttons[ EQ_BT_CLOSE_N ] = pixmap->copy (0,116,9,9);
    m_buttons[ EQ_BT_CLOSE_P ] = pixmap->copy (0,125,9,9);
    m_buttons[ EQ_BT_SHADE1_N ] = pixmap->copy (254,137,9,9);

    for (int i = 0; i < 19; ++i)
    {
        m_eq_spline << pixmap->copy (115, 294+i, 1, 1);
    }
    delete pixmap;
}

void Skin::loadEq_ex()
{
    QPixmap *pixmap = getPixmap ("eq_ex");

    m_buttons[ EQ_BT_SHADE1_P ] = pixmap->copy (1,38,9,9);
    m_buttons[ EQ_BT_SHADE2_N ] = pixmap->copy (254,3,9,9);
    m_buttons[ EQ_BT_SHADE2_P ] = pixmap->copy (1,47,9,9);
    m_eq_parts[ EQ_TITLEBAR_SHADED_A ] = pixmap->copy(0,0,275,14);
    m_eq_parts[ EQ_TITLEBAR_SHADED_I ] = pixmap->copy(0,15,275,14);
    m_eq_parts[ EQ_VOLUME1 ] = pixmap->copy(1,30,3,8);
    m_eq_parts[ EQ_VOLUME2 ] = pixmap->copy(4,30,3,8);
    m_eq_parts[ EQ_VOLUME3 ] = pixmap->copy(7,30,3,8);
    m_eq_parts[ EQ_BALANCE1 ] = pixmap->copy(11,30,3,8);
    m_eq_parts[ EQ_BALANCE2 ] = pixmap->copy(14,30,3,8);
    m_eq_parts[ EQ_BALANCE3 ] = pixmap->copy(17,30,3,8);

    delete pixmap;
}

void Skin::loadVisColor()
{
    QString path = findFile("viscolor.txt");

    if (path.isEmpty())
        qFatal("Skin: invalid default skin");

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qFatal("Skin: unable to open %s", qPrintable(path));

    int j = 0;
    while (!file.atEnd () && j<24)
    {
        j++;
        QByteArray line = file.readLine ();
        QString tmp = QString::fromAscii (line);
        tmp = tmp.trimmed ();
        tmp.remove("\"");
        int i = tmp.indexOf ("//");
        if (i>0)
            tmp.truncate (tmp.indexOf ("//"));
        QStringList list = tmp.split (",");
        if (list.count () >= 3)
        {
            //colors
            int r = list.at (0).toInt();
            int g = list.at (1).toInt();
            int b = list.at (2).toInt();
            m_vis_colors << QColor (r,g,b);
        }
        else if (line.length() == 0)
        {
            break;
        }
    }
    if (m_vis_colors.size() < 24)
    {
        qWarning ("Skin: cannot parse viscolor.txt");
        while (m_vis_colors.size() < 24)
            m_vis_colors << QColor (0,0,0);
    }
}

void Skin::loadShufRep()
{
    QPixmap *pixmap = getPixmap ("shufrep");

    m_buttons[ BT_EQ_ON_N ] = pixmap->copy (0,73,23,12);
    m_buttons[ BT_EQ_ON_P ] = pixmap->copy (46,73,23,12);
    m_buttons[ BT_EQ_OFF_N ] = pixmap->copy (0,61,23,12);
    m_buttons[ BT_EQ_OFF_P ] = pixmap->copy (46,61,23,12);

    m_buttons[ BT_PL_ON_N ] = pixmap->copy (23,73,23,12);
    m_buttons[ BT_PL_ON_P ] = pixmap->copy (69,73,23,12);
    m_buttons[ BT_PL_OFF_N ] = pixmap->copy (23,61,23,12);
    m_buttons[ BT_PL_OFF_P ] = pixmap->copy (69,61,23,12);

    //buttons[ BT_PL_CLOSE_N ] = pixmap->copy ();
    //buttons[ BT_PL_CLOSE_P ] = pixmap->copy ();

    m_buttons[REPEAT_ON_N] = pixmap->copy (0,30, 28, 15);
    m_buttons[REPEAT_ON_P] = pixmap->copy (0,45, 28, 15);

    m_buttons[REPEAT_OFF_N] = pixmap->copy (0, 0,28,15);
    m_buttons[REPEAT_OFF_P] = pixmap->copy (0,15,28,15);

    m_buttons[SHUFFLE_ON_N] = pixmap->copy (28,30,46,15);
    m_buttons[SHUFFLE_ON_P] = pixmap->copy (28,45,46,15);

    m_buttons[SHUFFLE_OFF_N] = pixmap->copy (28, 0,46,15);
    m_buttons[SHUFFLE_OFF_P] = pixmap->copy (28,15,46,15);

    delete pixmap;
}

void Skin::loadLetters(void)
{
    QPixmap *img = getPixmap("text");

    QList<QList<QPixmap> > (letters);
    for (int i = 0; i < 3; i++)
    {
        QList<QPixmap> (l);
        for (int j = 0; j < 31; j++)
        {
            l.append (img->copy (j*5, i*6, 5, 6));
        }
        letters.append (l);
    }

    delete img;


    /* alphabet */
    for (uint i = 97; i < 123; i++)
    {
        m_letters.insert(i, letters[0][i-97]);
    }

    /* digits */
    for (uint i = 0; i <= 9; i++)
    {
        m_letters.insert (i+48, letters[1][i]);
    }

    /* special characters */
    m_letters.insert('"',  letters[0][27]);
    m_letters.insert('@',  letters[0][28]);
    m_letters.insert(':',  letters[1][12]);
    m_letters.insert('(',  letters[1][13]);
    m_letters.insert(')',  letters[1][14]);
    m_letters.insert('-',  letters[1][15]);
    m_letters.insert('\'', letters[1][16]);
    m_letters.insert('`',  letters[1][16]);
    m_letters.insert('!',  letters[1][17]);
    m_letters.insert('_',  letters[1][18]);
    m_letters.insert('+',  letters[1][19]);
    m_letters.insert('\\', letters[1][20]);
    m_letters.insert('/',  letters[1][21]);
    m_letters.insert('[',  letters[1][22]);
    m_letters.insert(']',  letters[1][23]);
    m_letters.insert('^',  letters[1][24]);
    m_letters.insert('&',  letters[1][25]);
    m_letters.insert('%',  letters[1][26]);
    m_letters.insert('.',  letters[1][27]);
    m_letters.insert(',',  letters[1][27]);
    m_letters.insert('=',  letters[1][28]);
    m_letters.insert('$',  letters[1][29]);
    m_letters.insert('#',  letters[1][30]);

    m_letters.insert(229, letters[2][0]);
    m_letters.insert(246, letters[2][1]);
    m_letters.insert(228, letters[2][2]);
    m_letters.insert('?', letters[2][3]);
    m_letters.insert('*', letters[2][4]);
    m_letters.insert(' ', letters[2][5]);

    /* text background */
    //m_items->insert (TEXTBG, letters[2][6]);
}

void Skin::loadMonoSter()
{
    QPixmap *pixmap = getPixmap("monoster");

    m_ms_parts.clear();
    m_ms_parts[ MONO_A ] = pixmap->copy (29,0,27,12);
    m_ms_parts[ MONO_I ] = pixmap->copy (29,12,27,12);
    m_ms_parts[ STEREO_A ] = pixmap->copy (0,0,27,12);
    m_ms_parts[ STEREO_I ] = pixmap->copy (0,12,27,12);

    delete pixmap;

    m_parts.clear();
    QPainter paint;
    pixmap = getPixmap("playpaus");

    QPixmap part(11, 9);
    paint.begin(&part);
    paint.drawPixmap (0, 0, 3, 9, *pixmap, 36, 0, 3, 9);
    paint.drawPixmap (3, 0, 8, 9, *pixmap,  1, 0, 8, 9);
    paint.end();
    m_parts [PLAY] = part.copy();

    part = QPixmap(11, 9);
    paint.begin(&part);
    paint.drawPixmap (0, 0, 2, 9, *pixmap, 27, 0, 2, 9);
    paint.drawPixmap (2, 0, 9, 9, *pixmap,  9, 0, 9, 9);
    paint.end();
    m_parts [PAUSE] = part.copy();

    part = QPixmap(11, 9);
    paint.begin(&part);
    paint.drawPixmap (0, 0, 2, 9, *pixmap, 27, 0, 2, 9);
    paint.drawPixmap (2, 0, 9, 9, *pixmap, 18, 0, 9, 9);
    paint.end();
    m_parts [STOP]  = part.copy();

    delete pixmap;
}

void Skin::loadVolume()
{
    QPixmap *pixmap = getPixmap("volume");

    m_volume.clear();
    for (int i = 0; i < 28; ++i)
        m_volume.append(pixmap->copy (0,i*15, qMin(pixmap->width(), 68), 13));
    if (pixmap->height() > 425)
    {
        m_buttons [BT_VOL_N] = pixmap->copy (15,422,14, pixmap->height() - 422);
        m_buttons [BT_VOL_P] = pixmap->copy (0, 422,14, pixmap->height() - 422);
    }
    else
    {
        m_buttons [BT_VOL_N] = QPixmap();
        m_buttons [BT_VOL_P] = QPixmap();
    }
    delete pixmap;
}

void Skin::loadBalance()
{
    QPixmap *pixmap = getPixmap ("balance", "volume");

    m_balance.clear();
    for (int i = 0; i < 28; ++i)
        m_balance.append(pixmap->copy (9,i*15,38,13));
    if (pixmap->height() > 427)
    {
        m_buttons [BT_BAL_N] = pixmap->copy (15, 422,14,pixmap->height()-422);
        m_buttons [BT_BAL_P] = pixmap->copy (0,422,14,pixmap->height()-422);
    }
    else
    {
        m_buttons [BT_BAL_N] = QPixmap();
        m_buttons [BT_BAL_P] = QPixmap();
    }
    delete pixmap;
}

void Skin::loadRegion()
{
    m_regions.clear();
    QString path = findFile("region.txt");

    if (path.isEmpty())
    {
        qDebug ("Skin: cannot find region.txt. Transparency disabled");
        return;
    }
    m_regions[NORMAL] = createRegion(path, "Normal");
    m_regions[EQUALIZER] = createRegion(path, "Equalizer");
    m_regions[WINDOW_SHADE] = createRegion(path, "WindowShade");
    m_regions[EQUALIZER_WS] = createRegion(path, "EqualizerWS");
}

QRegion Skin::createRegion(const QString &path, const QString &key)
{
    QRegion region;
    QSettings settings(path, QSettings::IniFormat);
    QStringList numPoints = settings.value(key+"/NumPoints").toStringList();
    QStringList value = settings.value(key+"/PointList").toStringList();
    QStringList numbers;
    foreach(QString str, value)
    numbers << str.split(" ", QString::SkipEmptyParts);

    QList <QRegion> regions;

    QList<QString>::iterator n;
    n = numbers.begin();
    int r = m_double_size ? 2 : 1;
    for (int i = 0; i < numPoints.size(); ++i)
    {
        QList <int> lp;
        for (int j = 0; j < numPoints.at(i).toInt()*2; j++)
        {
            lp << n->toInt();
            n ++;
        }
        QVector<QPoint> points;

        for (int l = 0; l < lp.size(); l+=2)
        {
            points << QPoint(lp.at(l)*r, lp.at(l+1)*r);
        }
        region = region.united(QRegion(QPolygon(points)));
    }
    return region;
}

QPixmap *Skin::correctSize(QPixmap *pixmap, int w, int h)
{
    if(pixmap->width() < w || pixmap->height() < h)
    {
        QPixmap *fullSizePixmap = new QPixmap(w, h);
        fullSizePixmap->fill(Qt::transparent);
        QPainter p(fullSizePixmap);
        p.drawPixmap(0,0,*pixmap);
        delete pixmap;
        return fullSizePixmap;
    }
    return pixmap;
}

QPixmap * Skin::getDummyPixmap(const QString &name, const QString &fallback)
{
    QDir dir (":/glare");
    dir.setFilter (QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << name + ".*");
    QFileInfoList f = dir.entryInfoList();
    if(!f.isEmpty())
    {
        return new QPixmap (f.first().filePath());
    }

    if(!fallback.isEmpty())
    {
        dir.setNameFilters(QStringList() << fallback + ".*");
        f = dir.entryInfoList();
        if(!f.isEmpty())
        {
            return new QPixmap (f.first().filePath());
        }
    }

    qFatal("Skin: default skin is corrupted");
    return 0;
}

QPixmap Skin::scalePixmap(const QPixmap &pix, int ratio)
{
    return pix.scaled(pix.width() * ratio, pix.height() * ratio,
                      Qt::KeepAspectRatio,
                      m_antialiasing ? Qt::SmoothTransformation : Qt::FastTransformation);
}

const QString Skin::findFile(const QString &name)
{
    m_skin_dir.setFilter (QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    m_skin_dir.setNameFilters(QStringList() << name);
    QFileInfoList f = m_skin_dir.entryInfoList();
    if(!f.isEmpty())
    {
        return f.first().filePath();
    }

    QDir dir(":/glare");
    dir.setFilter (QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << name);
    f = dir.entryInfoList();
    if(!f.isEmpty())
    {
        return f.first().filePath();
    }

    return QString();
}
