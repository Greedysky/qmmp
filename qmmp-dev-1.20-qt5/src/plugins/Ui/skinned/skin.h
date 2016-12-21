/***************************************************************************
 *   Copyright (C) 2007-2015 by Ilya Kotov                                 *
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

#ifndef SKIN_H
#define SKIN_H

#include <QObject>
#include <QMap>
#include <QPixmap>
#include <QDir>
#include <QRegion>
#include <QCursor>

class Skin : public QObject
{
    Q_OBJECT
public:
    Skin(QObject *parent = 0);

    ~Skin();

    static Skin *instance();
    static QPixmap getPixmap(const QString &name, QDir dir);
    int ratio()
    {
        return m_double_size ? 2 : 1;
    }
    const QPixmap getMain() const
    {
        return m_main;
    }
    const QPixmap getButton(uint bt) const
    {
        return m_buttons[bt];
    }
    const QCursor getCursor(uint cu) const
    {
        return m_cursors[cu];
    }
    const QPixmap getTitleBar(uint tb) const
    {
        return m_titlebar[tb];
    }
    const QPixmap getPosBar() const
    {
        return posbar;
    }
    const QPixmap getNumber(uint n) const
    {
        return m_numbers[n];
    }
    /*!
     * Returns count of numbers in number list.
     * We need this to check if we have "-" in pixmaps.
     * if no we should draw it manually.
     */
    uint getNumCount(void) const
    {
        return m_numbers.count();
    }
    const QPixmap getPlPart(uint p) const
    {
        return m_pl_parts[p];
    }
    const QPixmap getEqPart(uint p) const
    {
        return m_eq_parts[p];
    }
    const QPixmap getEqSlider(uint n) const
    {
        return m_eq_bar[n];
    }
    const QPixmap getEqSpline(uint n) const
    {
        return m_eq_spline[n];
    }
    const QPixmap getMSPart(uint n) const
    {
        return m_ms_parts[n];
    }
    const QPixmap getLetter(const QChar& ch) const
    {
        return m_letters[ch];
    }
    const QPixmap getItem(uint n) const
    {
        return m_parts[n];
    }
    const QPixmap getVolumeBar(int n) const
    {
        return m_volume[n];
    }
    const QPixmap getBalanceBar(int n) const
    {
        return m_balance[n];
    }
    const QByteArray getPLValue (QByteArray c) const
    {
        return m_pledit_txt[c];
    }
    const QColor getMainColor(int n) const
    {
        return m_main_colors[n];
    }
    const QColor getVisColor(int n) const
    {
        return m_vis_colors[n];
    }
    const QRegion getRegion(uint r) const
    {
        return m_regions[r];
    }

    enum Buttons
    {
        BT_PREVIOUS_N = 0,
        BT_PREVIOUS_P,
        BT_PLAY_N,
        BT_PLAY_P,
        BT_PAUSE_N,
        BT_PAUSE_P,
        BT_STOP_N,
        BT_STOP_P,
        BT_NEXT_N,
        BT_NEXT_P,
        BT_EJECT_N,
        BT_EJECT_P,
        /*titlebar.* */
        BT_MENU_N,
        BT_MENU_P,
        BT_MINIMIZE_N,
        BT_MINIMIZE_P,
        BT_CLOSE_N,
        BT_CLOSE_P,
        BT_SHADE1_N,
        BT_SHADE1_P,
        BT_SHADE2_N,
        BT_SHADE2_P,
        BT_CLOSE_SHADED_N,
        BT_CLOSE_SHADED_P,

        /* posbar.* */
        BT_POSBAR_N,
        BT_POSBAR_P,
        /* pledit.* */
        PL_BT_ADD,
        PL_BT_SUB,
        PL_BT_SEL,
        PL_BT_SORT,
        PL_BT_LST,
        PL_BT_SCROLL_N,
        PL_BT_SCROLL_P,
        PL_BT_CLOSE_N,
        PL_BT_CLOSE_P,
        PL_BT_SHADE1_N,
        PL_BT_SHADE1_P,
        PL_BT_SHADE2_N,
        PL_BT_SHADE2_P,

        /* eqmain.* */
        EQ_BT_BAR_N,
        EQ_BT_BAR_P,
        EQ_BT_ON_N,
        EQ_BT_ON_P,
        EQ_BT_OFF_N,
        EQ_BT_OFF_P,
        EQ_BT_PRESETS_N,
        EQ_BT_PRESETS_P,
        EQ_BT_AUTO_1_N,
        EQ_BT_AUTO_1_P,
        EQ_BT_AUTO_0_N,
        EQ_BT_AUTO_0_P,
        EQ_BT_CLOSE_N,
        EQ_BT_CLOSE_P,
        EQ_BT_SHADE1_N,

        /* eq_ex.* */
        EQ_BT_SHADE1_P,
        EQ_BT_SHADE2_N,
        EQ_BT_SHADE2_P,

        /* shufrep.* */
        BT_EQ_ON_N,
        BT_EQ_ON_P,
        BT_EQ_OFF_N,
        BT_EQ_OFF_P,
        BT_PL_ON_N,
        BT_PL_ON_P,
        BT_PL_OFF_N,
        BT_PL_OFF_P,
        BT_PL_CLOSE_N,
        BT_PL_CLOSE_P,
        REPEAT_ON_N,
        REPEAT_ON_P,
        REPEAT_OFF_N,
        REPEAT_OFF_P,
        SHUFFLE_ON_N,
        SHUFFLE_ON_P,
        SHUFFLE_OFF_N,
        SHUFFLE_OFF_P,
        /* volume.* */
        BT_VOL_N,
        BT_VOL_P,
        /* balance.* */
        BT_BAL_N,
        BT_BAL_P,
    };
    enum TitleBar
    {
        TITLEBAR_A = 0,
        TITLEBAR_I,
        TITLEBAR_SHADED_A,
        TITLEBAR_SHADED_I
    };
    enum PlayList
    {
        PL_CORNER_UL_A = 0,
        PL_CORNER_UL_I,
        PL_CORNER_UR_A,
        PL_CORNER_UR_I,
        PL_TITLEBAR_A,
        PL_TITLEBAR_I,
        PL_TFILL1_A,
        PL_TFILL1_I,
        PL_TFILL2_A,
        PL_TFILL2_I,
        PL_LFILL,
        PL_RFILL,
        PL_LSBAR,
        PL_RSBAR,
        PL_SFILL1,
        PL_SFILL2,
        PL_CONTROL,
        PL_TITLEBAR_SHADED1_A,
        PL_TITLEBAR_SHADED1_I,
        PL_TITLEBAR_SHADED2,
        PL_TFILL_SHADED
    };
    enum Equalizer
    {
        EQ_MAIN = 0,
        EQ_TITLEBAR_A,
        EQ_TITLEBAR_I,
        EQ_GRAPH,
        EQ_TITLEBAR_SHADED_A,
        EQ_TITLEBAR_SHADED_I,
        EQ_VOLUME1,
        EQ_VOLUME2,
        EQ_VOLUME3,
        EQ_BALANCE1,
        EQ_BALANCE2,
        EQ_BALANCE3
    };
    enum MonoSter
    {
        MONO_A = 0,
        MONO_I,
        STEREO_A,
        STEREO_I
    };
    enum OtherParts
    {
        PLAY = 0,
        PAUSE,
        STOP
    };
    enum Regions
    {
        NORMAL = 0,
        EQUALIZER,
        WINDOW_SHADE,
        EQUALIZER_WS
    };
    enum Cursors
    {
        CUR_NORMAL = 0,
        CUR_CLOSE,
        CUR_MAINMENU,
        CUR_MIN,
        CUR_POSBAR,
        CUR_SONGNAME,
        CUR_TITLEBAR,
        CUR_VOLBAL,
        CUR_WINBUT,

        CUR_WSNORMAL,
        CUR_WSPOSBAR,

        CUR_EQCLOSE,
        CUR_EQNORMAL,
        CUR_EQSLID,
        CUR_EQTITLE,

        CUR_PCLOSE,
        CUR_PNORMAL,
        CUR_PSIZE,
        CUR_PTBAR,
        CUR_PVSCROLL,
        CUR_PWINBUT,

        CUR_PWSNORM,
        CUR_PWSSIZE,

        CUR_VOLBAR,
        CUR_WSCLOSE,
        CUR_WSMIN,
        CUR_WSWINBUT
    };
    enum Colors
    {
        MW_BACKGROUND,
        MW_FOREGROUND
    };

public slots:
    void setSkin(const QString& path);
    void reloadSkin();

signals:
    void skinChanged();

private:
    QPixmap *getPixmap(const QString &name, const QString &fallback = QString());
    QString getPath(const QString&);
    const QString findFile(const QString &name);
    /*!
     * As far as there is no standard in skin making we cannot be sure
     * that all needful images we can find in skin :( This will cause
     * segfaults and asserts. So to prevent this we need such method
     * to load pixmap from default skin.
     */
    QPixmap *getDummyPixmap(const QString &name, const QString &fallback);
    QPixmap scalePixmap(const QPixmap &pix, int ratio = 2);
    static Skin *m_instance;
    QDir m_skin_dir;
    QMap<uint, QPixmap> m_buttons;
    QMap<uint, QCursor> m_cursors;
    QMap<uint, QPixmap> m_titlebar;
    QMap<uint, QPixmap> m_pl_parts;
    QMap<uint, QPixmap> m_eq_parts;
    QMap<uint, QPixmap> m_ms_parts;
    QMap<uint, QPixmap> m_parts;
    QMap<QChar, QPixmap> m_letters;
    QMap<QByteArray, QByteArray> m_pledit_txt;
    QMap<uint, QRegion> m_regions;
    QPixmap m_main;
    QPixmap posbar;
    QList<QPixmap> m_numbers;
    QList<QPixmap> m_eq_bar;
    QList<QPixmap> m_eq_spline;
    QList<QPixmap> m_volume;
    QList<QPixmap> m_balance;
    QList<QColor> m_vis_colors;
    QMap<uint, QColor> m_main_colors;
    bool m_use_cursors;
    bool m_double_size;
    bool m_antialiasing;

    void loadMain();
    void loadButtons();
    void loadCursors();
    void loadColors();
    void loadTitleBar();
    void loadPosBar();
    void loadNumbers();
    void loadPlayList();
    void loadPLEdit();
    void loadEqMain();
    void loadEq_ex();
    void loadVisColor();
    void loadShufRep();
    void loadLetters();
    void loadMonoSter();
    void loadVolume();
    void loadBalance();
    void loadRegion();
    QRegion createRegion(const QString &path, const QString &key);
    QPixmap *correctSize(QPixmap *pixmap, int w, int h);
};

#endif
