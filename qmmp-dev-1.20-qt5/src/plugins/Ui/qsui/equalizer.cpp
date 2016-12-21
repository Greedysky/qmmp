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

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QPainter>
#include <QDialogButtonBox>
#include <QSettings>
#include <QMessageBox>
#include <QDir>
#include <qmmp/qmmpsettings.h>
#include "equalizer.h"

Equalizer::Equalizer(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Equalizer"));

    m_layout = new QVBoxLayout(this);
    setLayout(m_layout);
    m_layout->setSpacing(5);
    m_layout->setMargin(5);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(5);
    m_enabled = new QCheckBox(tr("Enable equalizer"), this);
    buttonsLayout->addWidget(m_enabled);

    buttonsLayout->addSpacerItem(new QSpacerItem(30, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));

    QLabel *label = new QLabel(this);
    label->setText(tr("Preset:"));
    buttonsLayout->addWidget(label);

    m_presetComboBox = new QComboBox(this);
    m_presetComboBox->setEditable(true);
    connect(m_presetComboBox, SIGNAL(activated(int)), SLOT(loadPreset(int)));
    buttonsLayout->addWidget(m_presetComboBox);

    QPushButton *saveButton = new QPushButton(tr("Save"), this);
    connect(saveButton, SIGNAL(pressed()), SLOT(savePreset()));
    buttonsLayout->addWidget(saveButton);

    QPushButton *deleteButton = new QPushButton(tr("Delete"), this);
    connect(deleteButton, SIGNAL(pressed()), SLOT(deletePreset()));
    buttonsLayout->addWidget(deleteButton);

    QPushButton *reset = new QPushButton(tr("Reset"), this);
    connect(reset, SIGNAL(clicked()), SLOT(resetSettings()));
    buttonsLayout->addWidget(reset);

    QDialogButtonBox *dialogButtons = new QDialogButtonBox(QDialogButtonBox::Close, Qt::Horizontal, this);
    dialogButtons->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    connect(dialogButtons, SIGNAL(rejected()), SLOT(reject()));
    buttonsLayout->addWidget(dialogButtons);

    QGridLayout *slidersLayout = new QGridLayout;
    slidersLayout->setSpacing(5);

    QStringList names;
    names << tr("Preamp") << "25" << "40" << "63" << "100" << "160" << "250"
           << "400" << "630" << "1k" << "1,6k" << "2,5k"
           << "4k" << "6,3k" << "10k" << "16k";

    for(int i = 0; i < 16; ++i)
    {
        QSlider *slider = new QSlider(this);
        slider->setRange(-20, 20);
        slider->setTickPosition(QSlider::TicksLeft);
        slider->setTickInterval(10);
        QLabel *label = new QLabel(this);
        label->setFrameShape(QFrame::Box);
        label->setText(names.at(i));
        slidersLayout->addWidget(slider, 1, i, Qt::AlignHCenter);
        slidersLayout->addWidget(label, 2, i, Qt::AlignHCenter);

        QLabel *label2 = new QLabel(this);
        label2->setText(tr("%1dB").arg(0));
        label2->setFrameShape(QFrame::Box);
        label2->setMinimumWidth(fontMetrics().width(tr("+%1dB").arg(20) + 5));
        label2->setAlignment(Qt::AlignCenter);
        slidersLayout->addWidget(label2, 0, i, Qt::AlignHCenter);
        connect(slider, SIGNAL(valueChanged(int)), SLOT(updateLabel()));
        m_sliders << slider;
        m_labels << label2;
    }
    m_layout->addItem(slidersLayout);
    m_layout->addItem(buttonsLayout);
    setMinimumHeight(300);
    foreach(QSlider *slider, m_sliders)
    {
        connect(slider, SIGNAL(valueChanged(int)), SLOT(applySettings()));
    }
    connect(m_enabled, SIGNAL(clicked()), SLOT(applySettings()));
    readSettigs();
    loadPresets();
}

Equalizer::~Equalizer()
{
    savePresets();
    qDeleteAll(m_presets);
    m_presets.clear();
}

void Equalizer::readSettigs()
{
    EqSettings settings = QmmpSettings::instance()->eqSettings();
    m_enabled->setChecked(settings.isEnabled());
    m_sliders.at(0)->setValue(settings.preamp());
    for(int i = 0; i < EqSettings::EQ_BANDS_15; ++i)
    {
        m_sliders.at(i+1)->setValue(settings.gain(i));
    }
}

void Equalizer::loadPresets()
{
    m_presetComboBox->clear();
    //equalizer presets
    QString preset_path = Qmmp::configDir() + "eq15.preset";
    if(!QFile::exists(preset_path))
        preset_path = ":/qsui/eq15.preset";
    QSettings eq_preset (preset_path, QSettings::IniFormat);
    int i = 0;
    while(eq_preset.contains("Presets/Preset"+QString("%1").arg(++i)))
    {
        QString name = eq_preset.value(QString("Presets/Preset%1").arg(i), tr("preset")).toString();
        EQPreset *preset = new EQPreset();
        //preset->setText(name);
        eq_preset.beginGroup(name);
        for (int j = 0; j < EqSettings::EQ_BANDS_15; ++j)
        {
            preset->setGain(j,eq_preset.value(QString("Band%1").arg(j), 0).toDouble());
        }
        preset->setPreamp(eq_preset.value("Preamp",0).toDouble());
        m_presets.append(preset);
        m_presetComboBox->addItem(name);
        eq_preset.endGroup();
    }
    m_presetComboBox->clearEditText();
}

void Equalizer::applySettings()
{
    EqSettings settings(EqSettings::EQ_BANDS_15);
    settings.setPreamp(m_sliders.at(0)->value());
    settings.setEnabled(m_enabled->isChecked());
    for(int i = 0; i < EqSettings::EQ_BANDS_15; ++i)
    {
        settings.setGain(i, m_sliders.at(i+1)->value());
    }
    QmmpSettings::instance()->setEqSettings(settings);
}

void Equalizer::resetSettings()
{
    foreach(QSlider *slider, m_sliders)
    {
        slider->setValue(0);
    }
    applySettings();
    m_presetComboBox->setCurrentIndex(-1);
}

void Equalizer::updateLabel()
{
    QSlider *slider = qobject_cast<QSlider *>(sender());
    int index = m_sliders.indexOf(slider);
    if(index < 0)
        return;
    if(slider->value() > 0)
        m_labels[index]->setText(tr("+%1dB").arg(slider->value()));
    else
        m_labels[index]->setText(tr("%1dB").arg(slider->value()));
}

void Equalizer::loadPreset(int index)
{
    EQPreset *preset = m_presets.at(index);
    m_sliders.at(0)->setValue(preset->preamp());
    for(int i = 0; i < EqSettings::EQ_BANDS_15; ++i)
    {
        m_sliders.at(i+1)->setValue(preset->gain(i));
    }
    applySettings();
}

void Equalizer::savePreset()
{
    QString name = m_presetComboBox->currentText();
    if(name.isEmpty())
        return;

    int index = m_presetComboBox->findText(name);
    if(index != -1)
    {
        if(QMessageBox::question(this, tr("Overwrite Request"),
                                 tr("Preset '%1' already exists. Overwrite?").arg(name),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
            return;
        m_presets[index]->setPreamp(m_sliders.at(0)->value());
        for(int i = 0; i < EqSettings::EQ_BANDS_15; ++i)
        {
            m_presets[index]->setGain(i, m_sliders.at(i+1)->value());
        }
    }
    else
    {
        m_presetComboBox->addItem(name);
        EQPreset *preset = new EQPreset();
        preset->setPreamp(m_sliders.at(0)->value());
        for(int i = 0; i < EqSettings::EQ_BANDS_15; ++i)
        {
            preset->setGain(i, m_sliders.at(i+1)->value());
        }
        m_presets.append(preset);
    }
    m_presetComboBox->clearEditText();
}

void Equalizer::savePresets()
{
    QSettings eq_preset (Qmmp::configDir() + "eq15.preset", QSettings::IniFormat);
    eq_preset.clear ();
    for (int i = 0; i < m_presets.size(); ++i)
    {
        eq_preset.setValue(QString("Presets/Preset%1").arg(i+1), m_presetComboBox->itemText(i));
        eq_preset.beginGroup(m_presetComboBox->itemText(i));
        for (int j = 0; j < EqSettings::EQ_BANDS_15; ++j)
        {
            eq_preset.setValue(QString("Band%1").arg(j),m_presets.at(i)->gain(j));
        }
        eq_preset.setValue("Preamp",m_presets.at(i)->preamp());
        eq_preset.endGroup();
    }
}

void Equalizer::deletePreset()
{
    QString name = m_presetComboBox->currentText();
    if(name.isEmpty())
        return;

    int index = m_presetComboBox->findText(name);
    if(index != -1)
    {
        m_presetComboBox->removeItem(index);
        delete m_presets.takeAt(index);
    }
    m_presetComboBox->clearEditText();
}
