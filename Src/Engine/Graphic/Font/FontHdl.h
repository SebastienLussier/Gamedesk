/**
 *  @file       FontHdl.h
 *  @brief	    Managed handle to a font.
 *  @author     Sébastien Lussier.
 *  @date       29/01/04.
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
#ifndef     _FONT_HDL_H_
#define     _FONT_HDL_H_

#include "Graphic/Font/FontManager.h"


namespace Gamedesk {
	

class Font;

class ENGINE_API FontHdl
{
public:
    FontHdl();    
    FontHdl( const String& pFontFile, UInt32 pFontSize );    
    FontHdl( const FontHdl& pOther );
    
    ~FontHdl();
    
    INLINE FontHdl& operator = ( const FontHdl& pOther );
    INLINE Bool operator == ( const FontHdl& pOther );
    INLINE Bool operator != ( const FontHdl& pOther );
    INLINE Font* operator -> () const;
    INLINE Font* operator * () const;
    INLINE operator Bool () const;

    INLINE void GetFont( const String& pFontFile, UInt32 pFontSize );

private:
    String      mFontFile;
    UInt32      mFontSize;
    Font*       mFont;
};


INLINE FontHdl::FontHdl() : mFontFile(), mFontSize(0), mFont(NULL)
{
}

INLINE FontHdl::FontHdl( const String& pFontFile, UInt32 pFontSize ) : mFont(NULL)
{
    GetFont( pFontFile, pFontSize );
}    

INLINE FontHdl::FontHdl( const FontHdl& pOther ) : mFont(NULL)
{
    if( pOther )
        GetFont( pOther.mFontFile, pOther.mFontSize );
}

INLINE FontHdl::~FontHdl()
{
    if( mFont )
        FontManager::Instance().Release( mFontFile, mFontSize );
}

INLINE FontHdl& FontHdl::operator = ( const FontHdl& pOther )
{
    if( *this == pOther || mFont == pOther.mFont )
        return *this;

    GetFont( pOther.mFontFile, pOther.mFontSize );
    return *this;
}

INLINE Bool FontHdl::operator == ( const FontHdl& pOther )
{
    return mFont == pOther.mFont;
}

INLINE Bool FontHdl::operator != ( const FontHdl& pOther )
{
    return mFont != pOther.mFont;
}

INLINE Font* FontHdl::operator -> () const
{
    GD_ASSERT( mFont != NULL );
    return mFont;
}

INLINE Font* FontHdl::operator * () const
{
    GD_ASSERT( mFont != NULL );
    return mFont;
}

INLINE FontHdl::operator Bool () const
{
    return mFont != NULL;
}

INLINE void FontHdl::GetFont( const String& pFontFile, UInt32 pFontSize )
{
    try
    {
        if( mFont )
        {
            FontManager::Instance().Release( mFontFile, mFontSize );
            mFont = NULL;
        }

        mFontFile = pFontFile;
        mFontSize = pFontSize;
        mFont = FontManager::Instance().Create( mFontFile, mFontSize );
    }
    catch( Exception& /*e*/ )
    {
        mFont = NULL;
    }        
}


} // namespace Gamedesk


#endif  //  _FONT_HDL_H_
