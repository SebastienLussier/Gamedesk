/**
 *  @file       FTFontImporter.cpp
 *  @brief      FreeType font importer for Gamedesk..
 *  @author     Sébastien Lussier.
 *  @date       24/01/04.
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
#include "FTImporter.h"
#include "FTFontImporter.h"

#include "Graphic/Font/Font.h"
#include "Graphic/Texture/Texture.h"
#include "Graphic/GraphicSubsystem.h"


IMPLEMENT_CLASS(FTFontImporter);


FTFontImporter::FTFontImporter()
{
    AddSupportedType( "ttf" );
    AddSupportedType( "fon" );

    FT_Init_FreeType( &mFTLibrary );
}

FTFontImporter::~FTFontImporter()
{
    FT_Done_FreeType( mFTLibrary );
}

Class* FTFontImporter::GetResourceClass()
{
    return Font::StaticClass();
}

Resource* FTFontImporter::Import( const String& pFilename, const String& pParams )
{
    FT_Face     face;
    FT_Error    error;

    Font*       newFont = NULL;

    error = FT_New_Face( mFTLibrary, pFilename.c_str(), 0, &face );
    if( error == FT_Err_Unknown_File_Format )
    {
        // ... the font file could be opened and read, but it appears
        // ... that its font format is unsupported
        return NULL;
    }
    else if( error )
    {
        // ... another error code means that the font file could not
        // ... be opened or read, or simply that it is broken...
        return NULL;
    }
    
    const UInt32 TEXTURE_WIDTH  = 512;
    const UInt32 TEXTURE_HEIGHT = 512;
    const UInt32 PADDING        = 0;

    newFont = GD_NEW(Font, this, "FTImporter::Font");

    UInt32 fontSize = 18;
    if( !pParams.empty() )
    {
        if( pParams.substr( 0, 5 ) == "Size=" )
        {
            fontSize = StringTo<UInt32>( pParams.substr( 5, pParams.length() - 5) );
            fontSize = Maths::Clamp<UInt32>( fontSize, 4, 150 );
        }
    }
    
    error = FT_Set_Pixel_Sizes( face, 0, fontSize );

    UInt32       textureCount   = 1;
    FT_UInt      glyphIndex     = 0;
    
    UInt32       glyphStartX    = PADDING;
    UInt32       glyphStartY    = PADDING;
    Int32        maxHeight      = 0;

    // Compute each glyph rect in the texture, and find how many texture we will need to contain all glyphs
    for( UInt32 car = 32; car < 256; ++car )
    {
        glyphIndex = FT_Get_Char_Index( face, car );

        error = FT_Load_Glyph( face, glyphIndex, FT_LOAD_DEFAULT );
        if( error ) continue;

        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        if( error ) continue;
        
        // If we're going to write past the texture right border, start on a new line.
        if( glyphStartX + face->glyph->bitmap.width >= TEXTURE_WIDTH )
        {
            glyphStartX  = PADDING;
            glyphStartY += maxHeight + PADDING;
            maxHeight    = 0;
        }

        // If we're going to write past the texture bottom border, start with a new texture page.
        if( glyphStartY + face->glyph->bitmap.rows >= TEXTURE_HEIGHT )
        {
            glyphStartX  = PADDING;
            glyphStartY  = PADDING;
            textureCount++;
            maxHeight    = 0;
        }            
        
        // Fill in info for this glyph.
        newFont->GetGlyph(car).pos.x          = glyphStartX;
        newFont->GetGlyph(car).pos.y          = glyphStartY;
        newFont->GetGlyph(car).size.x         = face->glyph->bitmap.width;
        newFont->GetGlyph(car).size.y         = face->glyph->bitmap.rows;
        newFont->GetGlyph(car).advance.x      = face->glyph->advance.x / 64.0f;
        newFont->GetGlyph(car).advance.y      = face->glyph->advance.y / 64.0f;            
        newFont->GetGlyph(car).texture        = textureCount - 1;
        newFont->GetGlyph(car).draw_offset.x  = face->glyph->bitmap_left;
        newFont->GetGlyph(car).draw_offset.y  = face->glyph->bitmap_top-face->glyph->bitmap.rows;

        // Store the maximum glyph height encontered on the current row.
        if( maxHeight < newFont->GetGlyph(car).size.y )
            maxHeight = newFont->GetGlyph(car).size.y;

        // Next glyph start position.
        glyphStartX += newFont->GetGlyph(car).size.x + PADDING;
    }

    
    Vector<Image*>  listImages;
    
    // Allocate each image
    for( UInt32 i = 0; i < textureCount; i++ )
    {
        Image* image = GD_NEW(Image, this, "FTImporter::Image");
        image->Create( TEXTURE_WIDTH, TEXTURE_HEIGHT, Image::Format_A8 );
        listImages.push_back(image);

        memset( image->GetData(), 0, TEXTURE_WIDTH*TEXTURE_HEIGHT );
    }

    // Now copy each glyph symbols in it's corresponding texture
    for( UInt32 car = 32; car < 256; ++car )
    {
        glyphIndex = FT_Get_Char_Index( face, car );

        error = FT_Load_Glyph( face, glyphIndex, FT_LOAD_DEFAULT );
        if( error ) continue;

        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        if( error ) continue;

        FT_Bitmap*  source      = &face->glyph->bitmap;

        Font::FontGlyph&  glyph = newFont->GetGlyph( car );
        Byte*       texData = listImages[glyph.texture]->GetData();
        
        UInt32      yTexture;
        UInt32      xTexture;
        UInt32      index;

        // Copy pixel one by one, invert x & y.
        for( Int32 row = 0; row < glyph.size.y; ++row )
        {
            // Y coords need to be flipped.
            yTexture = (TEXTURE_HEIGHT-1) - (glyph.pos.y + row);

            for( Int32 col = 0; col < glyph.size.x; ++col )
            {
                // X coords need to be flipped.
                xTexture = (TEXTURE_WIDTH-1) - (glyph.pos.x + col);
                
                // Calculate the offset to the pixel in the texture memory buffer.
                index = yTexture*TEXTURE_WIDTH + xTexture;

                // Write the actual pixel value.
                texData[index] = source->buffer[row*source->pitch + col];
            }
        }

        // Flip coordinates.
        glyph.pos.x = TEXTURE_WIDTH - glyph.pos.x - glyph.size.x;
        glyph.pos.y = TEXTURE_HEIGHT - glyph.pos.y - glyph.size.y;

        // Compute UV mapping for each glyph.
        glyph.uv_start = Vector2f( glyph.pos.x/(Float)TEXTURE_WIDTH, glyph.pos.y/(Float)TEXTURE_HEIGHT );
        glyph.uv_end   = Vector2f( (glyph.pos.x+glyph.size.x)/(Float)TEXTURE_WIDTH, (glyph.pos.y+glyph.size.y)/(Float)TEXTURE_HEIGHT );
    }        

    // Allocate each texture    
    for( Vector<Image*>::const_iterator itImage = listImages.begin(); itImage != listImages.end(); ++itImage )
    {
        Texture2D* texture = Cast<Texture2D>( GraphicSubsystem::Instance()->Create( Texture2D::StaticClass() ) );
        texture->Create( **itImage, false );
        texture->Init();

        // Add each texture page to the font.
        newFont->AddFontPage( texture );
    }

    // Free image memory
    for( Vector<Image*>::iterator itImage = listImages.begin(); itImage != listImages.end(); ++itImage )
        GD_DELETE(*itImage);

    // Release FreeType face object.
    FT_Done_Face( face );

    return newFont;
}

