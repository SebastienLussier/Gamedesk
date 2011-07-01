/**
 *  @file       TextureManager.cpp
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
#include "Engine.h"
#include "Graphic/Texture/TextureManager.h"


#include "Graphic/Texture/Texture.h"
#include "Resource/ResourceManager.h"


#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
TextureManager TextureManager::mInstance;
TextureManager& TextureManager::Instance()
{
    return mInstance;
}


TextureManager::TextureManager()
{
}


Texture1D* TextureManager::CreateTexture1D( const String& pImageFile )
{
    std::map<String,Texture*>::iterator  itFind = mLoadedTextures.find( pImageFile );
    if( itFind != mLoadedTextures.end() )
    {
        itFind->second->AddRef();
        return Cast<Texture1D>(itFind->second);
    }
    else
    {
        ResourceImporter* importer = ResourceManager::Instance()->GetImporterForFile( pImageFile, Texture::StaticClass() );

        Texture1D* texture = Cast<Texture1D>(importer->Import(pImageFile) );
        texture->Init();
        texture->AddRef();
        mLoadedTextures[pImageFile] = texture;
        return texture;
    }        
}


Texture2D* TextureManager::CreateTexture2D( const String& pImageFile )
{
    std::map<String,Texture*>::iterator  itFind = mLoadedTextures.find( pImageFile );
    if( itFind != mLoadedTextures.end() )
    {
        itFind->second->AddRef();
        return Cast<Texture2D>(itFind->second);
    }
    else
    {
        ResourceImporter* importer = ResourceManager::Instance()->GetImporterForFile( pImageFile, Texture::StaticClass() );

        Texture2D* texture = Cast<Texture2D>(importer->Import(pImageFile) );
        texture->Init();
        texture->AddRef();
        mLoadedTextures[pImageFile] = texture;
        return texture;
    }        
}

Texture3D* TextureManager::CreateTexture3D( const String& pImageFile )
{
    std::map<String,Texture*>::iterator  itFind = mLoadedTextures.find( pImageFile );
    if( itFind != mLoadedTextures.end() )
    {
        itFind->second->AddRef();
        return Cast<Texture3D>(itFind->second);
    }
    else
    {
        ResourceImporter* importer = ResourceManager::Instance()->GetImporterForFile( pImageFile, Texture::StaticClass() );

        Texture3D* texture = Cast<Texture3D>(importer->Import(pImageFile) );
        texture->Init();
        texture->AddRef();
        mLoadedTextures[pImageFile] = texture;
        return texture;
    }        
}

Cubemap* TextureManager::CreateCubemap( const String& pImageFile )
{
    std::map<String,Texture*>::iterator  itFind = mLoadedTextures.find( pImageFile );
    if( itFind != mLoadedTextures.end() )
    {
        itFind->second->AddRef();
        return Cast<Cubemap>(itFind->second);
    }
    else
    {
        ResourceImporter* importer = ResourceManager::Instance()->GetImporterForFile( pImageFile, Texture::StaticClass() );

        Cubemap* texture = Cast<Cubemap>(importer->Import(pImageFile) );
        texture->Init();
        texture->AddRef();
        mLoadedTextures[pImageFile] = texture;
        return texture;
    }        
}

void TextureManager::Release( const String& pTextureFile )
{
    std::map<String,Texture*>::iterator  itFind = mLoadedTextures.find( pTextureFile );
    GD_ASSERT( itFind != mLoadedTextures.end() );
            
    if( itFind->second->RemoveRef() )
    {
        itFind->second->Kill();
        GD_DELETE(itFind->second);

        mLoadedTextures.erase( itFind );
    }
}


} // namespace Gamedesk
