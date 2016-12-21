/***************************************************************************
 *   Copyright (C) 2007-2008 by Ilya Kotov                                 *
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

#include "skin.h"
#include "togglebutton.h"
#include <QMouseEvent>

ToggleButton::ToggleButton ( QWidget *parent,uint on_n,uint on_p,uint off_n,uint off_p )
      : PixmapWidget ( parent )
{
   m_on_n = on_n;
   m_on_p = on_p;
   m_off_n = off_n;
   m_off_p = off_p;
   m_on = false;
   skin = Skin::instance();
   setChecked ( false );
   connect ( skin, SIGNAL ( skinChanged() ), this, SLOT ( updateSkin() ) );
}


ToggleButton::~ToggleButton()
{}

bool ToggleButton::isChecked()
{
   return m_on;
}

void ToggleButton::updateSkin()
{
   //setPixmap ( skin->getButton ( name_normal ) );
   setChecked ( m_on );
}

void ToggleButton::click()
{
    m_on = !m_on;
    setChecked (m_on);
    emit clicked(m_on);
}

void ToggleButton::setChecked ( bool on )
{
   m_on = on;
   if ( on )
      setPixmap ( skin->getButton ( m_on_n ) );
   else
      setPixmap ( skin->getButton ( m_off_n ) );
}
void ToggleButton::mousePressEvent ( QMouseEvent* )
{
   m_cursorin = true;
   m_old_on = m_on;
   if ( m_on )
      setPixmap ( skin->getButton ( m_off_p ) );
   else
      setPixmap ( skin->getButton ( m_on_p ) );
}

void ToggleButton::mouseReleaseEvent ( QMouseEvent* )
{
   if ( m_cursorin ) {
       m_on = !m_old_on;
       setChecked ( m_on );
       emit clicked( m_on );
   } else {
       m_on = m_old_on;
       setChecked ( m_on );
   }
}

void ToggleButton::mouseMoveEvent (QMouseEvent *e)
{
    if ( !m_cursorin && rect().contains(e->pos()) ) {
        m_cursorin = true;
        if ( m_old_on )
            setPixmap ( skin->getButton ( m_off_p ) );
        else
            setPixmap ( skin->getButton ( m_on_p ) );            
    } else if ( m_cursorin && !rect().contains(e->pos()) ) {
        m_cursorin = false;
        if ( m_old_on )
            setPixmap ( skin->getButton ( m_on_n ) );
        else
            setPixmap ( skin->getButton ( m_off_n ) );
    }
}
