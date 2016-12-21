/***************************************************************************
 *   Copyright (C) 2007-2012 by Ilya Kotov                                 *
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

#include <QAction>

#include <qmmp/visual.h>
#include <qmmp/visualfactory.h>
#include "visualmenu.h"

VisualMenu::VisualMenu(QWidget *parent) : QMenu(tr("Visualization"), parent)
{
    VisualFactory *factory = 0;
    foreach(factory, Visual::factories())
    {
        VisualAction *act = new VisualAction(factory, this);
        addAction(act);
    }
}

VisualMenu::~VisualMenu()
{
}

void VisualMenu::updateActions()
{
    for(int i = 0; i < Visual::factories().size(); ++i)
    {
        actions()[i]->setChecked(Visual::isEnabled(Visual::factories().at(i)));
    }
}

VisualAction::VisualAction(VisualFactory *factory, QWidget *parent) :
        QAction(factory->properties().name, parent)
{
    setCheckable (true);
    setChecked (Visual::isEnabled(factory));
    m_factory = factory;
    connect(this, SIGNAL(triggered(bool)), SLOT(select(bool)));
}

void VisualAction::select(bool select)
{
    Visual::setEnabled(m_factory, select);
}
