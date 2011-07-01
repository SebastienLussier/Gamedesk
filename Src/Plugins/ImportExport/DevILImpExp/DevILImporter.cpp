/**
 *  @file       DevILImporter.cpp
 *  @brief      Texture importer that uses DevIL Image Library.
 *  @author     Sébastien Lussier.
 *  @date       01/12/03.
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
#include "DevILImpExp.h"
#include "DevILImporter.h"
#include "Graphic/Image/Image.h"
#include "Graphic/Texture/Texture.h"


IMPLEMENT_CLASS(DevILImporter);


DevILImporter::DevILImporter()
{
    AddSupportedType( "bmp" );
    AddSupportedType( "gif" );
    AddSupportedType( "jpg" );
    AddSupportedType( "jpe" );
    AddSupportedType( "jpeg" );
    AddSupportedType( "pcx" );
    AddSupportedType( "jpe" );
    AddSupportedType( "png" );
    AddSupportedType( "pbm" );
    AddSupportedType( "pgm" );
    AddSupportedType( "ppm" );
    AddSupportedType( "pnm" );
    AddSupportedType( "psd" );
    AddSupportedType( "pnm" );
    AddSupportedType( "tga" );
    AddSupportedType( "tif" );
    AddSupportedType( "pnm" );
    AddSupportedType( "tiff" );
    AddSupportedType( "wal" );
    AddSupportedType( "xpm" );
}


Class* DevILImporter::GetResourceClass()
{
    return Texture::StaticClass();
}


Image::Format GetImageFormat( ILint pImgTextureFormat, ILint pBPP )
{
    // Convert DevIL image format to our internal format.
    switch( pBPP )
    {
    case 1:
        return Image::Format_L8;
        break;
            
    case 2:
        switch( pImgTextureFormat )
        {
        case IL_LUMINANCE_ALPHA:    return Image::Format_L8A8;
        }
        break;
    case 3:
        switch( pImgTextureFormat )
        {
        case IL_RGB:    return Image::Format_R8G8B8;
        case IL_BGR:    return Image::Format_B8G8R8;
        }
        break;
    case 4:
        switch( pImgTextureFormat )
        {
        case IL_RGBA:   return Image::Format_R8G8B8A8;
        case IL_BGRA:   return Image::Format_B8G8R8A8;
        }
        break;
    }

    return Image::Format_Unknown;
}


Resource* DevILImporter::Import( const String& pFilename, const String& /*pParams*/ )
{
	ILuint  imageName;

    // Load the image. DevIL will guess the type of the image file using it's extension and if needed it's header.
    ilGenImages( 1, &imageName );
    ilBindImage( imageName );    

    // Load the image.
    if( !ilLoadImage( const_cast<char*>(pFilename.c_str()) ) ) 
        throw ResourceImportException( ToString(ilGetError()), Here );

    // Get the image params.
    ILint bytesPerPixel   = ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL );
    ILint imgFormat       = ilGetInteger( IL_IMAGE_FORMAT );
    ILint imgWidth        = ilGetInteger( IL_IMAGE_WIDTH );
    ILint imgHeight       = ilGetInteger( IL_IMAGE_HEIGHT );
          
    // We do not support palettized texture currently, so un-palettize them!
    if( imgFormat == IL_COLOR_INDEX )
    {
        switch( ilGetInteger( IL_PALETTE_TYPE ) )
        {
            case IL_PAL_RGB24:
            case IL_PAL_RGB32:  imgFormat = IL_RGB;  break;        
            case IL_PAL_BGR24:
            case IL_PAL_BGR32:  imgFormat = IL_BGR;  break;
            case IL_PAL_RGBA32: imgFormat = IL_RGBA; break;
            case IL_PAL_BGRA32: imgFormat = IL_BGRA; break;
            default:            debugBreak();
        }
     
        ilConvertImage( imgFormat, IL_UNSIGNED_BYTE );
        bytesPerPixel = ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL );
        imgFormat     = ilGetInteger( IL_IMAGE_FORMAT );
    }
    
    // Find what is the gamedesk internal image format that will be used.
    Image::Format gdImgFormat = GetImageFormat( imgFormat, bytesPerPixel );
    
    Image newImage;
    newImage.Create( imgWidth, imgHeight, gdImgFormat );
    memcpy( newImage.GetData(), ilGetData(), imgWidth*imgHeight*bytesPerPixel );

    Texture* newTexture = NULL;

    // Allocate and create using image.
    if( imgWidth == 1 || imgHeight == 1 )
    {
        Texture1D* tex = Cast<Texture1D>(Texture1D::StaticClass()->AllocateNew( pFilename ));
        tex->Create( newImage );
        newTexture = tex;
    }
    else 
    {
        Texture2D* tex = Cast<Texture2D>(Texture2D::StaticClass()->AllocateNew( pFilename ));
        tex->Create( newImage );
        newTexture = tex;
    }

    // The DevIL copy of the image is not needed anymore, so destroy it.
    ilDeleteImages( 1, &imageName );

    return newTexture;
}
