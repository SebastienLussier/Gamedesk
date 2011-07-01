/**
 *  @file       RenderTexture.h
 *  @brief      .
 *  @author     Sébastien Lussier.
 *  @date       08/03/05.
 */
/*
 *  Copyright (C) 2005 Gamedesk
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
#ifndef     _RENDER_TEXTURE_H_
#define     _RENDER_TEXTURE_H_


#include "Graphic/RenderTarget/RenderTarget.h"
#include "Graphic/Image/Image.h"


namespace Gamedesk {


class Texture;
class Texture1D;
class Texture2D;
class Cubemap;


class ENGINE_API RenderTexture : public RenderTarget
{
    friend class TextureManager;
    DECLARE_ABSTRACT_CLASS(RenderTexture,RenderTarget);

public:
    virtual ~RenderTexture();
    
    //! Make sure the render texture is ready for use as a normal texture.
    virtual void Bind();

    //! Make sure the render texture can be used as a render target.
    virtual void UnBind();

    //! Return the binding state of this render texture.
    Bool IsBinded() const;

    //! Get the internal texture associated with this render texture.
    Texture& GetTexture();

protected:
    RenderTexture();

protected:
    Bool        mBinded;
    Texture*    mTexture;
};


class ENGINE_API RenderTexture1D : public RenderTexture
{
    friend class TextureManager;
    DECLARE_ABSTRACT_CLASS(RenderTexture1D,RenderTexture);

public:
    virtual ~RenderTexture1D() {}
    virtual void Create( UInt32 pWidth, Image::Format pFormat ) = 0;

protected:
    RenderTexture1D() {}

protected:
    Texture1D* mTexture1D;
};


class ENGINE_API RenderTexture2D : public RenderTexture
{
    friend class TextureManager;
    DECLARE_ABSTRACT_CLASS(RenderTexture2D,RenderTexture);

public:
    virtual ~RenderTexture2D() {}
    virtual void Create( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat ) = 0;
 
protected:
    RenderTexture2D() {}

protected:
    Texture2D* mTexture2D;
};


class ENGINE_API RenderCubemap : public RenderTexture
{
    friend class TextureManager;
    DECLARE_ABSTRACT_CLASS(RenderCubemap,RenderTexture);

public:
    virtual ~RenderCubemap() {}
    virtual void Create( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat ) = 0;
 
protected:
    RenderCubemap() {}

protected:
    Cubemap* mCubemap;
};


} // namespace Gamedesk


#endif  //  _RENDER_TEXTURE_H_
