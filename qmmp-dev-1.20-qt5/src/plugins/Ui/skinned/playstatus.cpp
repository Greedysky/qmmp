/***************************************************************************
 *   Copyright (C) 2006 by Ilya Kotov                                      *
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
#include "skin.h"
#include "playstatus.h"

PlayStatus::PlayStatus ( QWidget *parent )
      : PixmapWidget ( parent )
{
   m_skin = Skin::instance();
   setStatus ( STOP );
   connect(m_skin, SIGNAL(skinChanged()), this, SLOT(updateSkin()));
}


PlayStatus::~PlayStatus()
{}

void PlayStatus::setStatus ( Type st )
{
   m_status = st;
   switch ( ( uint ) st )
   {
      case PLAY:
      {
         setPixmap ( m_skin->getItem ( Skin::PLAY ));
         break;
      }
      case STOP:
      {
         setPixmap ( m_skin->getItem ( Skin::STOP ));
         break;
      }
      case PAUSE:
      {
         setPixmap ( m_skin->getItem ( Skin::PAUSE ));
         break;
      }
   }
}

void PlayStatus::updateSkin()
{
   setStatus ( m_status );
}
