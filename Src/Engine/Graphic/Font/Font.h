/**
 *  @file       Font.h
 *  @brief      Texture Font.
 *  @author     Sébastien Lussier.
 *  @date       25/01/04.
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
#ifndef     _FONT_H_
#define     _FONT_H_


#include "Maths/Vector2.h"
#include "Resource/Resource.h"


namespace Gamedesk {
	
class Texture;


const UInt32 GLYPH_COUNT = 224;

class ENGINE_API Font : public Resource
{
    DECLARE_CLASS(Font,Resource);
    friend class FontManager;

public:
    //! Representation of a glyph.
    struct ENGINE_API FontGlyph
    {
        Vector2i    pos;            //!< Position of glyph in the font page.
        Vector2i    size;           //!< Size of the glyph.
        Vector2i    draw_offset;    //!< Offset to use when drawing.

        UInt32      texture;        //!< Texture page on which the glyph can be found.

        Vector2i    advance;        //!< Pen advance after write this glyph.

        Vector2f    uv_start;       //!< UV coordinates.
        Vector2f    uv_end;         //!< UV coordinates.

        void Serialize( Stream& pStream );
    };

public:
    Font();
    virtual ~Font();

    virtual void Init();
    virtual void Kill();
    virtual void Serialize( Stream& pStream );

    void     AddFontPage( Texture* pPage );
    void     DrawString( UInt32 pX, UInt32 pY, const Char* pString, ... ) const;
    Vector2i GetStringSize( const Char* pString, ... ) const;

    FontGlyph& GetGlyph( UInt32 pCaracter );

private:
    FontGlyph           mGlyphs[GLYPH_COUNT];
    Vector<Texture*>    mFontPages;
};


} // namespace Gamedesk


#endif  //  _FONT_H_
