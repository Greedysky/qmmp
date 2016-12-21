/***************************************************************************
 *   Copyright (C) 2013 by Ilya Kotov                                      *
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
#include "eqpreset.h"

EQPreset::EQPreset()
{
    m_preamp = 0;
    for(int i = 0; i < 10; ++i)
    m_bands[i] = 0;
}


EQPreset::~EQPreset()
{}

void EQPreset::setGain(int n, double value)
{
    if(n > 14 || n < 0)
        return;
    m_bands[n] = value;
}

void EQPreset::setPreamp(double preamp)
{
    m_preamp = preamp;
}

double EQPreset::gain(int n)
{
    if(n > 14 || n < 0)
        return 0;
    return m_bands[n];
}

double EQPreset::preamp()
{
    return m_preamp;
}
