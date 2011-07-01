/**
 *  @file       OGLTexture.h
 *  @brief	    OpenGL Texture.
 *  @author     Sébastien Lussier.
 *  @date       04/12/03.
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
#ifndef     _OGL_TEXTURE_H_
#define     _OGL_TEXTURE_H_


#include "Graphic/Texture/Texture.h"


namespace Gamedesk {


class OGLGRAPHIC_API OGLTexture1D : public Texture1D
{
    friend class OGLGraphicSubsystem;
    DECLARE_CLASS(OGLTexture1D,Texture1D);

public:
    GLuint GetTextureID() const
    {
        return mTextureID;
    }

    void SetWrapMode( WrapAxis pWrapAxis, WrapMode pWrapMode );
    void SetMagFilter( MagFilter pFilter );
    void SetMinFilter( MinFilter pFilter );
    void SetAnisotropy( Float pAnisotropy );

private:
    OGLTexture1D();   
    virtual void Init();
    virtual void Kill();

private:
    GLuint  mTextureID;
};


class OGLGRAPHIC_API OGLTexture2D : public Texture2D
{
    friend class OGLGraphicSubsystem;
    DECLARE_CLASS(OGLTexture2D,Texture2D);

public:
    GLuint GetTextureID() const
    {
        return mTextureID;
    }

    void SetWrapMode( WrapAxis pWrapAxis, WrapMode pWrapMode );
    void SetMagFilter( MagFilter pFilter );
    void SetMinFilter( MinFilter pFilter );
    void SetAnisotropy( Float pAnisotropy );

private:
    OGLTexture2D();    
    virtual void Init();
    virtual void Kill();

private:
    GLuint  mTextureID;
};


class OGLGRAPHIC_API OGLTexture3D : public Texture3D
{
    friend class OGLGraphicSubsystem;
    DECLARE_CLASS(OGLTexture3D,Texture3D);

public:
    GLuint GetTextureID() const
    {
        return mTextureID;
    }

    void SetWrapMode( WrapAxis pWrapAxis, WrapMode pWrapMode );
    void SetMagFilter( MagFilter pFilter );
    void SetMinFilter( MinFilter pFilter );
    void SetAnisotropy( Float pAnisotropy );

private:
    OGLTexture3D();
    virtual void Init();
    virtual void Kill();

private:
    GLuint  mTextureID;
};


class OGLGRAPHIC_API OGLCubemap : public Cubemap
{
    friend class OGLGraphicSubsystem;
    DECLARE_CLASS(OGLCubemap,Cubemap);

public:
    GLuint GetTextureID() const
    {
        return mTextureID;
    }
    
    void SetWrapMode( WrapAxis pWrapAxis, WrapMode pWrapMode );
    void SetMagFilter( MagFilter pFilter )  ;
    void SetMinFilter( MinFilter pFilter );
    void SetAnisotropy( Float pAnisotropy );

private:
    OGLCubemap();    
    virtual void Init();
    virtual void Kill();

private:
    GLuint  mTextureID;
};


} // namespace Gamedesk


#endif  //  _OGL_TEXTURE_H_
