/**
 *  @file       Shader.h
 *  @brief
 *  @author     Sébastien Lussier.
 *  @date       02/02/04.
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
#ifndef     _SHADER_H_
#define     _SHADER_H_


#include "Graphic/Material/Material.h"
#include "Graphic/Texture/TextureHdl.h"


namespace Gamedesk {


/**
 *	Base Shader class.
 */
class ENGINE_API Shader
{
public:
    virtual ~Shader();

    virtual UInt32  GetPassCount() = 0;

    virtual void    Prepare();
    virtual void    Apply( UInt32 iPass = 0 ) = 0;
    virtual void    Done();

    virtual Bool    NeedBlending();
};


/**
 *  Texture shader (with material)
 */
class ENGINE_API TextureShader : public Shader
{
public:
    virtual ~TextureShader();

    virtual UInt32  GetPassCount();

    virtual void    Apply( UInt32 iPass = 0 );
    virtual void    Done();

public:
    Material        mMaterial;
    HTexture2D      mTexture;
};


/**
 *  Transparent texture shader (with material).
 */
class ENGINE_API TransparencyTextureShader : public TextureShader
{
public:
    virtual ~TransparencyTextureShader();

    virtual void    Prepare();
    virtual void    Done();

    virtual Bool    NeedBlending();
};

class ENGINE_API EnvMapShader : public Shader
{
public:
    virtual ~EnvMapShader();

    virtual UInt32  GetPassCount();

    virtual void    Apply( UInt32 iPass = 0 );
    virtual void    Done();

public:
    HCubemap        mEnvironment;
};


} // namespace Gamedesk


#endif  //  _SHADER_H_
