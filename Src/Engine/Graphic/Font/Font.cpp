/**
 *  @file       Font.cpp
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
#include "Engine.h"
#include "Font.h"

#include "Graphic/Texture/Texture.h"
#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Font);


Font::Font()
{
}

Font::~Font()
{
    for( Vector<Texture*>::iterator itTexture = mFontPages.begin(); itTexture != mFontPages.end(); ++itTexture )
        GD_DELETE(*itTexture);
}

void Font::Init()
{
    for( Vector<Texture*>::iterator itTexture = mFontPages.begin(); itTexture != mFontPages.end(); ++itTexture )
        (*itTexture)->Init();
}

void Font::Kill()
{
    for( Vector<Texture*>::iterator itTexture = mFontPages.begin(); itTexture != mFontPages.end(); ++itTexture )
        (*itTexture)->Kill();
}

void Font::FontGlyph::Serialize( Stream& pStream )
{
	pStream << pos;
	pStream << size;
	pStream << draw_offset;
	pStream << texture;
	pStream << advance;
	pStream << uv_start;
	pStream << uv_end;
}

void Font::Serialize( Stream& pStream )
{
	Super::Serialize( pStream );

	for( int i = 0; i < GLYPH_COUNT; ++ i)
		mGlyphs[i].Serialize(pStream);


}

void Font::AddFontPage( Texture* pPage )
{
    mFontPages.push_back( pPage );
}

Font::FontGlyph& Font::GetGlyph( UInt32 pCaracter )
{
    return mGlyphs[pCaracter-32];
}

void Font::DrawString( UInt32 pX, UInt32 pY, const Char* pString, ... ) const
{
    va_list	    ptrArguments;
	static char	string[2048];

	va_start( ptrArguments, pString );
		vsprintf_s( string, pString, ptrArguments );
	va_end( ptrArguments );

    String      finalString = string;

    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetRenderState( Renderer::Lighting, false );
    renderer->SetRenderState( Renderer::Blend, true );
    renderer->SetBlendFunc( Renderer::BlendSrcAlpha, Renderer::BlendInvSrcAlpha );

    Int32 viewport[4];
    renderer->GetViewport( viewport );
    renderer->Begin2DProjection( viewport[0], viewport[2], viewport[1], viewport[3], -1, 1 );

    UInt32      penX = pX;
    UInt32      penY = pY;
    const FontGlyph* glyph;

    for( UInt32 car = 0; car < finalString.size(); ++car )
    {
        glyph = &mGlyphs[ finalString[car]-32 ];

        renderer->GetTextureStage(0)->SetTexture( *mFontPages[glyph->texture] );
        renderer->Draw2DTile( penX + glyph->draw_offset.x, penY + glyph->draw_offset.y, glyph->size.x, glyph->size.y, glyph->uv_start.x, glyph->uv_start.y, glyph->uv_end.x, glyph->uv_end.y );

        penX += glyph->advance.x;
        penY += glyph->advance.y;
    }

    renderer->GetTextureStage(0)->ResetTexture();

    renderer->End2DProjection();
    renderer->SetRenderState( Renderer::Blend, false );
}

Vector2i Font::GetStringSize( const Char* pString, ... ) const
{
    va_list	    ptrArguments;
    static char	string[2048];

    va_start( ptrArguments, pString );
        vsprintf_s( string, pString, ptrArguments );
    va_end( ptrArguments );

    String finalString = string;

    UInt32      penX = 0;
    UInt32      penY = 0;
    Vector2i    size;

    const FontGlyph* glyph;

    UInt32 numCar = finalString.size();
    for( UInt32 car = 0; car < numCar; ++car )
    {
        glyph = &mGlyphs[ finalString[car]-32 ];

        if( car == 0 )
        {
            size.x = penX + glyph->draw_offset.x;
            size.y = penY + glyph->draw_offset.y;
        }

        if( car == numCar - 1 )
        {
            size.x = (penX + glyph->draw_offset.x + glyph->size.x) - size.x;
            size.y = (penY + glyph->draw_offset.y + glyph->size.y) - size.y;
        }

        penX += glyph->advance.x;
        penY += glyph->advance.y;
    }

    return size;
}


} // namespace Gamedesk
