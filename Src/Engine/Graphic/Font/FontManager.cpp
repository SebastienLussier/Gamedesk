/**
 *  @file       Font.cpp
 *  @brief      Manage access to Texture Fonts.
 *  @author     Sébastien Lussier.
 *  @date       28/01/04.
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
#include "FontManager.h"

#include "Font.h"
#include "Resource/ResourceManager.h"


namespace Gamedesk {
	
	
FontManager FontManager::mInstance;
FontManager& FontManager::Instance()
{
    return mInstance;
}


FontManager::FontManager()
{
}


Font* FontManager::Create( const String& pFontFile, const UInt32 pFontSize )
{
    FontInfo    fontInfo( pFontFile, pFontSize );
    
    std::map<FontInfo,Font*>::iterator  itFind = mLoadedFonts.find( fontInfo );
    if( itFind != mLoadedFonts.end() )
    {
        itFind->second->AddRef();
        return itFind->second;
    }
    else
    {
        ResourceImporter* importer = ResourceManager::Instance()->GetImporterForFile( pFontFile, Font::StaticClass() );
        GD_ASSERT(importer);
        
        Font* font = Cast<Font>(importer->Import( pFontFile, String("Size=")+ToString(pFontSize) ) );
        font->Init();
        font->AddRef();
        mLoadedFonts[fontInfo] = font;
        return font;
    }        
}

void FontManager::Release( const String& pFontFile, const UInt32 pFontSize )
{
    FontInfo    fontInfo( pFontFile, pFontSize );
    
    std::map<FontInfo,Font*>::iterator  itFind = mLoadedFonts.find( fontInfo );
    GD_ASSERT( itFind != mLoadedFonts.end() );
            
    if( itFind->second->RemoveRef() )
    {
        itFind->second->Kill();
        GD_DELETE(itFind->second);

        mLoadedFonts.erase( itFind );
    }
}


} // namespace Gamedesk
