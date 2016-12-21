/***************************************************************************
 *   Copyright (C) 2010-2013 by Ilya Kotov                                 *
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
#include <QDir>
#include <qmmp/inputsourcefactory.h>
#include <qmmp/inputsource.h>
#include <qmmp/decoderfactory.h>
#include <qmmp/outputfactory.h>
#include <qmmp/visualfactory.h>
#include <qmmp/effectfactory.h>
#include <qmmp/effect.h>
#include <qmmp/soundcore.h>
#include <qmmp/enginefactory.h>
#include <qmmp/abstractengine.h>
#include <qmmpui/generalfactory.h>
#include <qmmpui/general.h>
#include <qmmpui/uihelper.h>
#include <qmmpui/filedialogfactory.h>
#include <qmmpui/filedialog.h>
#include <qmmpui/uiloader.h>
#include "radioitemdelegate_p.h"
#include "pluginitem_p.h"

PluginItem::PluginItem(QTreeWidgetItem *parent, InputSourceFactory *factory, const QString &path)
    : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/',-1), TRANSPORT)
{
    setCheckState(0, InputSource::isEnabled(factory) ? Qt::Checked : Qt::Unchecked);
    m_has_about = factory->properties().hasAbout;
    m_has_config = factory->properties().hasSettings;
    m_factory = factory;
}


PluginItem::PluginItem(QTreeWidgetItem *parent, DecoderFactory *factory, const QString &path)
    : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/',-1), DECODER)
{
    setCheckState(0, Decoder::isEnabled(factory) ? Qt::Checked : Qt::Unchecked);
    m_has_about = factory->properties().hasAbout;
    m_has_config = factory->properties().hasSettings;
    m_factory = factory;
}

PluginItem::PluginItem(QTreeWidgetItem *parent, EngineFactory *factory, const QString &path)
    : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/',-1), ENGINE)
{
    setCheckState(0, AbstractEngine::isEnabled(factory) ? Qt::Checked : Qt::Unchecked);
    m_has_about = factory->properties().hasAbout;
    m_has_config = factory->properties().hasSettings;
    m_factory = factory;
}

PluginItem::PluginItem(QTreeWidgetItem *parent, EffectFactory *factory, const QString &path)
    : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/',-1), EFFECT)
{
    setCheckState(0, Effect::isEnabled(factory) ? Qt::Checked : Qt::Unchecked);
    m_has_about = factory->properties().hasAbout;
    m_has_config = factory->properties().hasSettings;
    m_factory = factory;
}

PluginItem::PluginItem(QTreeWidgetItem *parent, VisualFactory *factory, const QString &path)
    : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/',-1), VISUAL)
{
    setCheckState(0, Visual::isEnabled(factory) ? Qt::Checked : Qt::Unchecked);
    m_has_about = factory->properties().hasAbout;
    m_has_config = factory->properties().hasSettings;
    m_factory = factory;
}

PluginItem::PluginItem(QTreeWidgetItem *parent, GeneralFactory *factory, const QString &path)
    : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/',-1), GENERAL)
{
    setCheckState(0, General::isEnabled(factory) ? Qt::Checked : Qt::Unchecked);
    m_has_about = factory->properties().hasAbout;
    m_has_config = factory->properties().hasSettings;
    m_factory = factory;
}

PluginItem::PluginItem(QTreeWidgetItem *parent, OutputFactory *factory, const QString &path)
    : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/',-1), OUTPUT)
{
    setCheckState(0, (Output::currentFactory() == factory) ? Qt::Checked : Qt::Unchecked);
    m_has_about = factory->properties().hasAbout;
    m_has_config = factory->properties().hasSettings;
    m_factory = factory;
    setData(0, RadioButtonRole, true);
}

PluginItem::PluginItem(QTreeWidgetItem *parent, FileDialogFactory *factory, const QString &path)
    : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/',-1), FILE_DIALOG)
{
    setCheckState(0, FileDialog::isEnabled(factory) ? Qt::Checked : Qt::Unchecked);
    m_has_about = factory->properties().hasAbout;
    m_has_config = false;
    m_factory = factory;
    setData(0, RadioButtonRole, true);
}

PluginItem::PluginItem(QTreeWidgetItem *parent, UiFactory *factory, const QString &path)
    : QTreeWidgetItem(parent, QStringList() << factory->properties().name << path.section('/',-1), USER_INTERFACE)
{
    setCheckState(0, (UiLoader::selected() == factory) ? Qt::Checked : Qt::Unchecked);
    m_has_about = factory->properties().hasAbout;
    m_has_config = false;
    m_factory = factory;
    setData(0, RadioButtonRole, true);
}

PluginItem::~PluginItem()
{}

bool PluginItem::hasAbout() const
{
    return m_has_about;
}
bool PluginItem::hasSettings() const
{
    return m_has_config;
}

void PluginItem::showAbout(QWidget *parent)
{
    switch(type())
    {
    case PluginItem::TRANSPORT:
        static_cast<InputSourceFactory *>(m_factory)->showAbout(parent);
        break;
    case PluginItem::DECODER:
        static_cast<DecoderFactory *>(m_factory)->showAbout(parent);
        break;
    case PluginItem::ENGINE:
        static_cast<EngineFactory *>(m_factory)->showAbout(parent);
        break;
    case PluginItem::EFFECT:
        static_cast<EffectFactory *>(m_factory)->showAbout(parent);
        break;
    case PluginItem::VISUAL:
        static_cast<VisualFactory *>(m_factory)->showAbout(parent);
        break;
    case PluginItem::GENERAL:
        static_cast<GeneralFactory *>(m_factory)->showAbout(parent);
        break;
    case PluginItem::OUTPUT:
        static_cast<OutputFactory *>(m_factory)->showAbout(parent);
        break;
    case PluginItem::FILE_DIALOG:
        static_cast<FileDialogFactory *>(m_factory)->showAbout(parent);
        break;
    case PluginItem::USER_INTERFACE:
        static_cast<UiFactory *>(m_factory)->showAbout(parent);
        break;
    default:
        ;
    }
}

void PluginItem::showSettings(QWidget *parent)
{
    switch(type())
    {
    case PluginItem::TRANSPORT:
        static_cast<InputSourceFactory *>(m_factory)->showSettings(parent);
        break;
    case PluginItem::DECODER:
        static_cast<DecoderFactory *>(m_factory)->showSettings(parent);
        break;
    case PluginItem::ENGINE:
        static_cast<EngineFactory *>(m_factory)->showSettings(parent);
        break;
    case PluginItem::EFFECT:
        static_cast<EffectFactory *>(m_factory)->showSettings(parent);
        break;
    case PluginItem::VISUAL:
        Visual::showSettings(static_cast<VisualFactory *>(m_factory), parent);
        break;
    case PluginItem::GENERAL:
        General::showSettings(static_cast<GeneralFactory *>(m_factory), parent);
        break;
    case PluginItem::OUTPUT:
        static_cast<OutputFactory *>(m_factory)->showSettings(parent);
        break;
    default:
        ;
    }
}

void PluginItem::setEnabled(bool enabled)
{
    switch(type())
    {
    case PluginItem::TRANSPORT:
        InputSource::setEnabled(static_cast<InputSourceFactory *>(m_factory), enabled);
        break;
    case PluginItem::DECODER:
        Decoder::setEnabled(static_cast<DecoderFactory *>(m_factory), enabled);
        break;
    case PluginItem::ENGINE:
        AbstractEngine::setEnabled(static_cast<EngineFactory *>(m_factory), enabled);
        break;
    case PluginItem::EFFECT:
        Effect::setEnabled(static_cast<EffectFactory *>(m_factory), enabled);
        break;
    case PluginItem::VISUAL:
        Visual::setEnabled(static_cast<VisualFactory *>(m_factory), enabled);
        break;
    case PluginItem::GENERAL:
        General::setEnabled(static_cast<GeneralFactory *>(m_factory), enabled);
        break;
    case PluginItem::OUTPUT:
        if(enabled)
        {
            Output::setCurrentFactory(static_cast<OutputFactory *>(m_factory));
        }
        break;
    case PluginItem::FILE_DIALOG:
        if(enabled)
        {
            FileDialog::setEnabled(static_cast<FileDialogFactory *>(m_factory));
        }
        break;
    case PluginItem::USER_INTERFACE:
        if(enabled)
        {
             UiLoader::select(static_cast<UiFactory *>(m_factory));
        }
        break;
    default:
        ;
    }
}
