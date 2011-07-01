/**
 *  @file       MD5Shader.h
 *  @brief      Defines data structures to store MD5 mesh shader.
 *  @author     Sébastien Lussier.
 *  @date       27/08/04.
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
#ifndef     _MD5_SHADER_H_
#define     _MD5_SHADER_H_


#include "Graphic/Color4.h"
#include "Graphic/Renderer.h"

namespace MD5
{


class Shader
{
public:
    enum TextureType
    {
        TexDiffuse,
        TexSpecular,
        TexBump,
        NUM_Tex
    };

    Shader() 
    {
        mNonSolid           = false;
        mBlend[TexDiffuse]  = false;
        mBlend[TexSpecular] = false;
        mBlend[TexBump]     = false;
    }

    String  mName;

    Bool                          mBlend[NUM_Tex];
    Renderer::PixelBlendingFactor mBlendSrc[NUM_Tex];
    Renderer::PixelBlendingFactor mBlendDst[NUM_Tex];
    String                        mTextures[NUM_Tex];

    Bool                          mNonSolid;
};


}


#endif  //  _MD5_SHADER_H_
