/***************************************************************************
 *   Copyright (C) 2011-2013 by Ilya Kotov                                 *
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

#ifndef EQUALIZER_H
#define EQUALIZER_H

#include <QDialog>
#include <QList>
#include "eqpreset.h"

class QVBoxLayout;
class QCheckBox;
class QSlider;
class QLabel;
class QComboBox;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class Equalizer : public QDialog
{
    Q_OBJECT
public:
    explicit Equalizer(QWidget *parent = 0);
    ~Equalizer();


private slots:
    void applySettings();
    void resetSettings();
    void updateLabel();
    void loadPreset(int index);
    void savePreset();
    void savePresets();
    void deletePreset();

private:
    void readSettigs();
    void loadPresets();

    QVBoxLayout *m_layout;
    QCheckBox *m_enabled;
    QComboBox *m_presetComboBox;
    QList<QSlider *> m_sliders;
    QList<QLabel *> m_labels;
    QList<EQPreset *> m_presets;

};

#endif // EQUALIZER_H
