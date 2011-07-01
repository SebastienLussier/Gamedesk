/**
 *  @file       OGLRenderTexture_ARB.h
 *  @brief	    Render to texture using WGL_ARB_render_texture extension.
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
#ifndef     _OGL_RENDER_TEXTURE_ARB_H_
#define     _OGL_RENDER_TEXTURE_ARB_H_


#include "Graphic/RenderTarget/RenderTexture.h"


namespace Gamedesk {
	
	
typedef struct
{
    HPBUFFERARB mPBuffer;
    HDC         mDC;
    HGLRC       mRC;
} OGLPixelBuffer;


class OGLGRAPHIC_API OGLRenderTexture1D_ARB : public RenderTexture1D
{
    friend class OGLGraphicSubsystem;
    DECLARE_CLASS(OGLRenderTexture1D_ARB,RenderTexture1D);

public:
    virtual ~OGLRenderTexture1D_ARB();
    void Create( UInt32 pWidth, Image::Format pFormat );

    void MakeCurrent();
    void Bind();
    void UnBind();

private:
    OGLRenderTexture1D_ARB();

protected:
    OGLPixelBuffer  mPixelBuffer;
};


class OGLGRAPHIC_API OGLRenderTexture2D_ARB : public RenderTexture2D
{
    friend class OGLGraphicSubsystem;
    DECLARE_CLASS(OGLRenderTexture2D_ARB,RenderTexture2D);

public:
    virtual ~OGLRenderTexture2D_ARB();
    void Create( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat );    
       
    void MakeCurrent();
    void Bind();
    void UnBind();

private:
    OGLRenderTexture2D_ARB();
    
protected:
    OGLPixelBuffer  mPixelBuffer;
};


class OGLGRAPHIC_API OGLRenderCubemap_ARB : public RenderCubemap
{
    friend class OGLGraphicSubsystem;
    DECLARE_CLASS(OGLRenderCubemap_ARB,RenderCubemap);

public:
    virtual ~OGLRenderCubemap_ARB();
    void Create( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat );

    void MakeCurrent();
    void Bind();
    void UnBind();

private:
    OGLRenderCubemap_ARB();

protected:
    OGLPixelBuffer  mPixelBuffer;
};


} // namespace Gamedesk


#endif  //  _OGL_RENDER_TEXTURE_ARB_H_
