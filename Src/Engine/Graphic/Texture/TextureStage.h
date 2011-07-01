/**
 *  @file       TextureStage.h
 *  @brief	    Abstract representation of a texture unit.
 *  @author     Sébastien Lussier.
 *  @date       05/02/04.
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
#ifndef     _TEXTURE_STAGE_H_
#define     _TEXTURE_STAGE_H_


namespace Gamedesk {


class Texture;
class RenderTexture;

class ENGINE_API TextureStage
{
    CLASS_DISABLE_COPY(TextureStage);

public:
    //! Defines how the source (to be applied) and destination (current in the framebuffer) colors are blended.
    enum TextureMode
    {
        Tex_Modulate,           //!< Modulates the dest color with the src color.
        Tex_Decal,              //!< Applies the src color to the dest color as a decal.
        Tex_Blend,              //!< Blends the src color with the dest color.
        Tex_Replace,            //!< Replaces the dest color with the src color.
        Tex_Add                 //!< Adds the dest color with the src color.
    };

    //! Control the automatic texture coordinate generation.
    enum TextureCoordGenMode
    {
        Gen_Disabled,           //!< No automatic texture coordinates generation.
        Gen_SphereMap,          //!< Automatic sphere mapping coordinates.
        Gen_ReflectionMap,      //!< Environment map reflection
        Gen_NormalMap           //!< Environment map normal
    };

public:
    TextureStage( UInt32 pStage );
    virtual ~TextureStage();

    virtual void ResetTexture() = 0;

    virtual void SetTexture( Texture& pTexture ) = 0;
    virtual void SetTexture( RenderTexture& pRenderTexture ) = 0;
    
    virtual void SetTextureMode( TextureMode pMode ) = 0;
    virtual void SetTextureCoordGenMode( TextureCoordGenMode pGenMode ) = 0;
    virtual void SetPointSpriteCoordGen( Bool pEnable ) = 0;

protected:
    virtual void MakeCurrent() = 0;

    UInt32          mStage;
    static UInt32   mActiveStage;
};


} // namespace Gamedesk


#endif  //  _TEXTURE_STAGE_H_
