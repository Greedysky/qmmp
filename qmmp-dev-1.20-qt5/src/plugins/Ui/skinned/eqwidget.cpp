/***************************************************************************
 *   Copyright (C) 2006-2016 by Ilya Kotov                                 *
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
#include <QEvent>
#include <QMenu>
#include <QInputDialog>
#include <QCloseEvent>
#include <QFile>
#include <qmmpui/filedialog.h>
#include <qmmpui/playlistmanager.h>
#include <qmmp/soundcore.h>
#include "skin.h"
#include "eqslider.h"
#include "eqtitlebar.h"
#include "togglebutton.h"
#include "eqgraph.h"
#include "button.h"
#include "eqpreset.h"
#include "preseteditor.h"
#include "mainwindow.h"
#include "playlist.h"
#include "windowsystem.h"
#include "eqwidget.h"

EqWidget::EqWidget (QWidget *parent)
        : PixmapWidget (parent)
{
    setWindowTitle(tr("Equalizer"));
    m_shaded = false;
    m_skin = Skin::instance();
    setPixmap (m_skin->getEqPart (Skin::EQ_MAIN), true);
    setCursor (m_skin->getCursor (Skin::CUR_EQNORMAL));
    m_titleBar = new EqTitleBar (this);
    m_titleBar->move (0,0);
    connect (m_skin, SIGNAL (skinChanged()), this, SLOT (updateSkin()));

    m_preamp = new EqSlider (this);
    connect (m_preamp,SIGNAL (sliderMoved (double)),SLOT (writeEq()));

    m_on = new ToggleButton (this,Skin::EQ_BT_ON_N,Skin::EQ_BT_ON_P, Skin::EQ_BT_OFF_N,Skin::EQ_BT_OFF_P);
    connect (m_on, SIGNAL (clicked(bool)), SLOT(writeEq()));

    m_autoButton = new ToggleButton(this, Skin::EQ_BT_AUTO_1_N, Skin::EQ_BT_AUTO_1_P,
                                    Skin::EQ_BT_AUTO_0_N, Skin::EQ_BT_AUTO_0_P);
    m_eqg = new EQGraph(this);
    m_presetsMenu = new QMenu(this);
    m_presetButton = new Button (this, Skin::EQ_BT_PRESETS_N, Skin::EQ_BT_PRESETS_P, Skin::CUR_EQNORMAL);
    connect(m_presetButton, SIGNAL(clicked()), SLOT(showPresetsMenu()));
    connect(SoundCore::instance(), SIGNAL(eqSettingsChanged()), SLOT(readEq()));

    for (int i = 0; i<10; ++i)
    {
        m_sliders << new EqSlider (this);
        connect (m_sliders.at (i), SIGNAL (sliderMoved (double)),SLOT (writeEq()));
    }
    readSettings();
    createActions();
    updatePositions();
    updateMask();
#ifdef QMMP_WS_X11
    QString wm_name = WindowSystem::netWindowManagerName();
    if(wm_name.contains("metacity", Qt::CaseInsensitive) ||
       wm_name.contains("openbox", Qt::CaseInsensitive))
        setWindowFlags (Qt::Tool | Qt::FramelessWindowHint);
    else
#endif
        setWindowFlags (Qt::Dialog | Qt::FramelessWindowHint);
}

EqWidget::~EqWidget()
{
    while (!m_presets.isEmpty())
        delete m_presets.takeFirst();
    while (!m_autoPresets.isEmpty())
        delete m_autoPresets.takeFirst();
}

void EqWidget::updatePositions()
{
    int r = m_skin->ratio();
    m_preamp->move (21*r,38*r);
    m_on->move (14*r,18*r);
    m_autoButton->move(39*r,18*r);
    m_eqg->move(87*r,17*r);
    m_presetButton->move(217*r,18*r);
     for (int i = 0; i < 10; ++i)
         m_sliders.at (i)->move ((78+i*18)*r,38*r);
}

void EqWidget::changeEvent (QEvent * event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        m_titleBar->setActive(isActiveWindow());
    }
}

void EqWidget::closeEvent (QCloseEvent* e)
{
    if (e->spontaneous ())
        parentWidget()->close();
}

void EqWidget::updateSkin()
{
    m_titleBar->setActive (false);
    setPixmap (m_skin->getEqPart (Skin::EQ_MAIN), true);
    setCursor (m_skin->getCursor (Skin::CUR_EQNORMAL));
    setMimimalMode(m_shaded);
    updatePositions();
}

void EqWidget::setMimimalMode(bool b)
{
    m_shaded = b;
    int r = m_skin->ratio();

    if(m_shaded)
        setFixedSize(r*275,r*14);
    else
        setFixedSize(r*275,r*116);

    updateMask();
}

void EqWidget::readSettings()
{
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    move (settings.value ("Skinned/eq_pos", QPoint (100, 216)).toPoint()); //geometry
    readEq();
    //equalizer presets
    QString preset_path = Qmmp::configDir() + "eq.preset";
    if(!QFile::exists(preset_path))
        preset_path = ":/skinned/eq.preset";
    QSettings eq_preset (preset_path, QSettings::IniFormat);
    int i = 0;
    while(eq_preset.contains("Presets/Preset"+QString("%1").arg(++i)))
    {
        QString name = eq_preset.value(QString("Presets/Preset%1").arg(i), tr("preset")).toString();
        EQPreset *preset = new EQPreset();
        preset->setText(name);
        eq_preset.beginGroup(name);
        for (int j = 0; j < 10; ++j)
        {
            preset->setGain(j,eq_preset.value(QString("Band%1").arg(j), 0).toDouble());
        }
        preset->setPreamp(eq_preset.value("Preamp",0).toDouble());
        m_presets.append(preset);
        eq_preset.endGroup();
    }
    //equalizer auto-load presets
    QSettings eq_auto (Qmmp::configDir() + "eq.auto_preset", QSettings::IniFormat);
    i = 0;
    while(eq_auto.contains(QString("Presets/Preset%1").arg(++i)))
    {
        QString name = eq_auto.value(QString("Presets/Preset%1").arg(i), tr("preset")).toString();
        EQPreset *preset = new EQPreset();
        preset->setText(name);
        eq_auto.beginGroup(name);
        for (int j = 0; j < 10; ++j)
        {
            preset->setGain(j,eq_auto.value(QString("Band%1").arg(j), 0).toDouble());
        }
        preset->setPreamp(eq_auto.value("Preamp",0).toDouble());
        m_autoPresets.append(preset);
        eq_auto.endGroup();
    }
}

void EqWidget::writeSettings()
{
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue ("Skinned/eq_pos", this->pos()); //geometry
    //equalizer presets
    QSettings eq_preset (Qmmp::configDir() + "eq.preset", QSettings::IniFormat);
    eq_preset.clear ();
    for (int i = 0; i < m_presets.size(); ++i)
    {
        eq_preset.setValue(QString("Presets/Preset%1").arg(i+1), m_presets.at(i)->text());
        eq_preset.beginGroup(m_presets.at(i)->text());
        for (int j = 0; j < 10; ++j)
        {
            eq_preset.setValue(QString("Band%1").arg(j),m_presets.at(i)->gain(j));
        }
        eq_preset.setValue("Preamp",m_presets.at(i)->preamp());
        eq_preset.endGroup();
    }
    //equalizer auto-load presets
    QSettings eq_auto (Qmmp::configDir() + "eq.auto_preset", QSettings::IniFormat);
    eq_auto.clear();
    for (int i = 0; i < m_autoPresets.size(); ++i)
    {
        eq_auto.setValue("Presets/Preset"+QString("%1").arg(i+1),
                         m_autoPresets.at(i)->text());
        eq_auto.beginGroup(m_autoPresets.at(i)->text());
        for (int j = 0; j < 10; ++j)
        {
            eq_auto.setValue(QString("Band%1").arg(j),m_autoPresets.at(i)->gain(j));
        }
        eq_auto.setValue("Preamp",m_autoPresets.at(i)->preamp());
        eq_auto.endGroup();
    }
}

void EqWidget::readEq()
{
    m_eqg->clear();
    EqSettings eqSettings = SoundCore::instance()->eqSettings();
    if(eqSettings.bands() != 10)
    {
        m_on->setChecked(false);
        return;
    }
    m_preamp->setValue(eqSettings.preamp());
    for (int i=0; i<10; ++i)
    {
        m_sliders.at(i)->setValue(eqSettings.gain(i));
        m_eqg->addValue(m_sliders.at(i)->value());
    }
    m_on->setChecked(eqSettings.isEnabled());
}

void EqWidget::writeEq()
{
    m_eqg->clear();
    EqSettings eqSettings;
    eqSettings.setPreamp(m_preamp->value());
    for (int i=0; i<10; ++i)
    {
        eqSettings.setGain(i,m_sliders.at(i)->value());
        m_eqg->addValue(m_sliders.at(i)->value());
    }
    eqSettings.setEnabled(m_on->isChecked());
    SoundCore::instance()->setEqSettings(eqSettings);
}

void EqWidget::createActions()
{
    m_presetsMenu->addAction(tr("&Load/Delete"),this, SLOT(showEditor()));
    m_presetsMenu->addSeparator();
    m_presetsMenu->addAction(QIcon::fromTheme("document-save"), tr("&Save Preset"),
                             this, SLOT(savePreset()));
    m_presetsMenu->addAction(QIcon::fromTheme("document-save"), tr("&Save Auto-load Preset"),
                             this, SLOT(saveAutoPreset()));
    m_presetsMenu->addAction(QIcon::fromTheme("document-open"), tr("&Import"),
                             this, SLOT(importWinampEQF()));
    m_presetsMenu->addSeparator();
    m_presetsMenu->addAction(QIcon::fromTheme("edit-clear"), tr("&Clear"), this, SLOT(reset()));
}

void EqWidget::showPresetsMenu()
{
    m_presetsMenu->exec(m_presetButton->mapToGlobal(QPoint(0, 0)));
}

void EqWidget::reset()
{
    for (int i = 0; i < m_sliders.size(); ++i)
        m_sliders.at(i)->setValue(0);
    m_preamp->setValue(0);
    writeEq();
}

void EqWidget::showEditor()
{
    PresetEditor *editor = new PresetEditor(this);
    editor->addPresets(m_presets);
    editor->addAutoPresets(m_autoPresets);
    connect (editor, SIGNAL(presetLoaded(EQPreset*)), SLOT(setPreset(EQPreset*)));
    connect (editor, SIGNAL(presetDeleted(EQPreset*)), SLOT(deletePreset(EQPreset*)));
    editor->show();
}

void EqWidget::savePreset()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Saving Preset"),
                                         tr("Preset name:"), QLineEdit::Normal,
                                         tr("preset #")+QString("%1").arg(m_presets.size()+1), &ok);
    if (ok)
    {
        EQPreset* preset = new EQPreset;
        preset->setText(text);
        preset->setPreamp(m_preamp->value());
        for (int i = 0; i<10; ++i)
            preset->setGain(i, m_sliders.at (i)->value());
        foreach(EQPreset *p, m_presets) //remove preset with same name
        {
            if (p->text() == text)
            {
                m_presets.removeAll(p);
                delete p;
            }
        }
        m_presets.append(preset);
    }
}

void EqWidget::saveAutoPreset()
{
    PlayListTrack *track = PlayListManager::instance()->currentPlayList()->currentTrack();
    if (!track)
        return;
    //delete preset if it already exists
    EQPreset* preset = findPreset(track->url().section("/",-1));
    if (preset)
        deletePreset(preset);
    //create new preset
    preset = new EQPreset();
    preset->setText(track->url().section("/",-1));
    preset->setPreamp(m_preamp->value());
    for (int i = 0; i<10; ++i)
    {
        preset->setGain(i, m_sliders.at (i)->value());
    }
    m_autoPresets.append(preset);
}

void EqWidget::setPreset(EQPreset* preset)
{
    for (int i = 0; i<10; ++i)
        m_sliders.at(i)->setValue(preset->gain(i));
    m_preamp->setValue(preset->preamp());
    writeEq();
}

void EqWidget::deletePreset(EQPreset* preset)
{
    int p = m_presets.indexOf(preset);
    if (p != -1)
    {
        delete m_presets.takeAt(p);
        return;
    }
    p = m_autoPresets.indexOf(preset);
    if (p != -1)
    {
        delete m_autoPresets.takeAt(p);
        return;
    }
}

void EqWidget::loadPreset(const QString &name)
{
    if (m_autoButton->isChecked())
    {
        EQPreset *preset = findPreset(name);
        if (preset)
            setPreset(preset);
        else
            reset();
    }
}

EQPreset *EqWidget::findPreset(const QString &name)
{
    foreach(EQPreset *preset, m_autoPresets)
    {
        if (preset->text() == name)
            return preset;
    }
    return 0;
}

void EqWidget::importWinampEQF()
{
    char header[31];
    char name[257];
    char bands[11];
    QString path = FileDialog::getOpenFileName(this, tr("Import Preset"),
                   QDir::homePath(),
                   QString("Winamp EQF (*.q1)"));

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    file.read (header, 31);
    if (QString::fromLatin1(header).contains("Winamp EQ library file v1.1"))
    {
        while (file.read (name, 257))
        {
            EQPreset* preset = new EQPreset;
            preset->setText(QString::fromLatin1(name));

            file.read(bands,11);

            for (int i = 0; i<10; ++i)
            {
                preset->setGain(i, 20 - bands[i]*40/64);
            }
            preset->setPreamp(20 - bands[10]*40/64);
            m_presets.append(preset);
        }
    }
    file.close();

}

void EqWidget::keyPressEvent (QKeyEvent *ke)
{
    QKeyEvent event = QKeyEvent(ke->type(), ke->key(),
                                ke->modifiers(), ke->text(),ke->isAutoRepeat(), ke->count());
    QApplication::sendEvent(qobject_cast<MainWindow*>(parent())->playlist(), &event);
}

#ifdef QMMP_WS_X11
bool EqWidget::event (QEvent *event)
{
    if(event->type() == QEvent::WinIdChange ||
            event->type() == QEvent::WindowActivate ||
            event->type() == QEvent::Show ||
            event->type() == QEvent::ShowToParent)
    {
        WindowSystem::ghostWindow(winId());
        WindowSystem::setWinHint(winId(), "equalizer", "Qmmp");
    }
    return QWidget::event(event);
}
#endif

void EqWidget::updateMask()
{
    clearMask();
    setMask(QRegion(0,0,width(),height()));
    QRegion region = m_skin->getRegion(m_shaded? Skin::EQUALIZER_WS : Skin::EQUALIZER);
    if (!region.isEmpty())
        setMask(region);
}
