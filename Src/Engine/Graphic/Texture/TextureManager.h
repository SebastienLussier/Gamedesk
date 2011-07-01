/**
 *  @file       TextureManager.h
 *  @brief	    Manage access to textures.
 *  @author     Sébastien Lussier.
 *  @date       15/01/03.
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
#ifndef     _TEXTURE_MANAGER_H_
#define     _TEXTURE_MANAGER_H_


#include "Texture.h"


namespace Gamedesk {


class ENGINE_API TextureManager
{
    CLASS_DISABLE_COPY(TextureManager);

public:
    static TextureManager& Instance();

    Texture1D* CreateTexture1D( UInt32 pWidth, Image::Format pFormat );
    Texture1D* CreateTexture1D( const String& pTextureFile );

    Texture2D* CreateTexture2D( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat );
    Texture2D* CreateTexture2D( const String& pTextureFile );

    Texture3D* CreateTexture3D( UInt32 pWidth, UInt32 pHeight, UInt32 pDepth, Image::Format pFormat );
    Texture3D* CreateTexture3D( const String& pTextureFile );

    Cubemap*   CreateCubemap( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat );
    Cubemap*   CreateCubemap( const String& pTextureFile );
    
    void Release( const String& pTextureFile );
    
protected:
    // Disable creation from outside.
    TextureManager();

private:
    std::map<String,Texture*>    mLoadedTextures;
    
    static TextureManager        mInstance;
};


} // namespace Gamedesk


#endif  //  _TEXTURE_MANAGER_H_
