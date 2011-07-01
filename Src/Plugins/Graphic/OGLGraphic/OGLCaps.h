/**
 *  @file       OGLCaps.h
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
#ifndef     _OGL_CAPS_H_
#define     _OGL_CAPS_H_


namespace Gamedesk {


/**
 *  Easy access to the card OpenGL capabilities.
 */
class OGLGRAPHIC_API OGLCaps
{
public:
    static OGLCaps& Instance();

    void Init();
    
    UInt32 GetMaxClipPlanes() const;
    UInt32 GetMaxLights() const;
    UInt32 GetMaxTextureSize() const;

private:
    OGLCaps();

    UInt32  mMaxClipPlanes;
    UInt32  mMaxLights;
    UInt32  mMaxTextureSize;

    static OGLCaps mInstance;
};


} // namespace Gamedesk


#endif  //  _OGL_CAPS_H_
