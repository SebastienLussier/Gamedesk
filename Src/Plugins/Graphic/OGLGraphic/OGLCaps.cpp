/**
 *  @file       OGLCaps.cpp
 *  @brief	    Access to the card OpenGL capabilities.
 *  @author     Sébastien Lussier.
 *  @date       20/01/04.
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
#include "OGLGraphic.h"
#include "OGLCaps.h"


namespace Gamedesk {
	
	
OGLCaps OGLCaps::mInstance;

OGLCaps::OGLCaps() :
       mMaxClipPlanes(0),
       mMaxLights(0),
       mMaxTextureSize(0)
{
}

OGLCaps& OGLCaps::Instance()
{
    return mInstance;
}

void OGLCaps::Init()
{
    GLint val;
    
    glGetIntegerv( GL_MAX_CLIP_PLANES, &val );
    mMaxClipPlanes = val;
    
    glGetIntegerv( GL_MAX_LIGHTS, &val );
    mMaxLights = val;

    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &val );
    mMaxTextureSize = val;
}

UInt32 OGLCaps::GetMaxClipPlanes() const
{
    return mMaxClipPlanes;
}

UInt32 OGLCaps::GetMaxLights() const
{
    return mMaxLights;
}

UInt32 OGLCaps::GetMaxTextureSize() const
{
    return mMaxTextureSize;
}


} // namespace Gamedesk
