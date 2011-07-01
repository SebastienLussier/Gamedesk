/**
 *  @file       TextureStage.h
 *  @brief	    Manage access to a texture unit.
 *  @author     Sébastien Lussier.
 *  @date       03/02/04.
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
#ifndef     _OGL_TEXTURE_STAGE_H_
#define     _OGL_TEXTURE_STAGE_H_


#include "Graphic/Texture/TextureStage.h"


namespace Gamedesk {
	
	
class OGLGRAPHIC_API OGLTextureStage : public TextureStage
{
    CLASS_DISABLE_COPY(OGLTextureStage);

public:
    OGLTextureStage( UInt32 pStage );
    virtual ~OGLTextureStage();

    void MakeCurrent();

    void ResetTexture();

    void SetTexture( Texture& pTexture );
    void SetTexture( RenderTexture& pRenderTexture );

    void SetTextureMode( TextureMode pMode );
    void SetTextureCoordGenMode( TextureCoordGenMode pGenMode );
    void SetPointSpriteCoordGen( Bool pEnable );

private:
    Int32           mTextureType;       //!< Type of the texture currently binded to this stage (-1 if no texture binded).
    Texture*        mTexture;           //!< Texture currently binded to this stage (can be NULL).
    RenderTexture*  mRenderTexture;     //!< Render texture currently binded to this stage (can be NULL).
};


} // namespace Gamedesk


#endif  //  _OGL_TEXTURE_STAGE_H_
