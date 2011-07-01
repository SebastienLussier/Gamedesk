/**
 *  @file       FontManager.h
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
#ifndef     _FONT_MANAGER_H_
#define     _FONT_MANAGER_H_


namespace Gamedesk {


class Font;


class ENGINE_API FontManager
{
    CLASS_DISABLE_COPY(FontManager);

public:
    static FontManager& Instance();

    Font* Create( const String& pFontFile, const UInt32 pFontSize );
    
    void Release( const String& pFontFile, const UInt32 pFontSize );
    
protected:
    // Disable creation from outside.
    FontManager();

private:
    struct FontInfo
    {
        FontInfo( const String& pFontFile, const UInt32 pFontSize ) : mFontName(pFontFile), mFontSize(pFontSize) {}

        Bool operator == ( const FontInfo& pOther ) const { return mFontName == pOther.mFontName && mFontSize == pOther.mFontSize; }
        Bool operator < ( const FontInfo& pOther ) const  { return mFontName < pOther.mFontName; }
        
        String      mFontName;
        UInt32      mFontSize;
    };

    std::map<FontInfo,Font*>    mLoadedFonts;
    
    static FontManager          mInstance;
};


} // namespace Gamedesk


#endif  //  _FONT_MANAGER_H_
