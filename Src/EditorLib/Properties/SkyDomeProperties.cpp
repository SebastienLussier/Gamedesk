/**
 *  @file       SkyDomeProperties.cpp
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       12/02/04.
 */
/*
 *  Copyright (C) 2004 Gamedesk
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  Gamedesk
 *  http://gamedesk.type-cast.com
 *
 */
#include "EditorLib.h"
#include "SkyDomeProperties.h"
#include "World/SkyDome.h"


ImplementProperty( SkyDomeProperties, mTextureName, String, GetTexture, SetTexture );


SkyDomeProperties::SkyDomeProperties()
{
}

SkyDomeProperties::~SkyDomeProperties()
{
}

void SkyDomeProperties::InitProperties()
{
    RemoveProperty(&mPosition);
    RemoveProperty(&mOrientation);
    RegisterProperty(SkyDomeProperties, "Texture", mTextureName);
}

void SkyDomeProperties::SetTexture(const String& pTextureName)
{
    Cast<SkyDome>(GetObject())->SetTexture(pTextureName);
    FirePropertyChanged(&mTextureName);
}

const String& SkyDomeProperties::GetTexture() const
{
    return Cast<SkyDome>(GetObject())->GetTextureName();
}

IMPLEMENT_CLASS(SkyDomeProperties);
