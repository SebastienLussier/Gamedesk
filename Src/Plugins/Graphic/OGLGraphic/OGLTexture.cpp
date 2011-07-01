/**
 *  @file       OGLTexture.cpp
 *  @brief	    OpenGL Texture.
 *  @author     Sébastien Lussier.
 *  @date       04/12/03.
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
#include "OGLGraphic.h"
#include "OGLTexture.h"
#include "OGLGraphicSubsystem.h"
#include "OGLRenderer.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(OGLTexture1D);
IMPLEMENT_CLASS(OGLTexture2D);
IMPLEMENT_CLASS(OGLTexture3D);
IMPLEMENT_CLASS(OGLCubemap);


const GLenum GDToGLIntTexFormat[] = {
                        GL_LUMINANCE8,                      // Format_L8,
                        GL_ALPHA,                           // Format_A8,
                        GL_LUMINANCE8_ALPHA8,               // Format_L8A8, 
                        GL_RGB,                             // Format_R8G8B8,
                        GL_RGBA,                            // Format_R8G8B8A8,
                        GL_RGB,                             // Format_B8G8R8,
                        GL_RGBA,                            // Format_B8G8R8A8,
                        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,   // Format_DXT1
                        GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,   // Format_DXT3
                        GL_COMPRESSED_RGBA_S3TC_DXT5_EXT    // Format_DXT5
                                    };   

const GLenum GDToGLTexFormat[] = {
                        GL_LUMINANCE,           // Format_L8,
                        GL_ALPHA,               // Format_A8,
                        GL_LUMINANCE_ALPHA,     // Format_L8A8, 
                        GL_RGB,                 // Format_R8G8B8,
                        GL_RGBA,                // Format_R8G8B8A8,
                        GL_BGR,                 // Format_B8G8R8,
                        GL_BGRA,                // Format_B8G8R8A8,
                        GL_RGBA,                // Format_DXT1
                        GL_RGBA,                // Format_DXT3
                        GL_RGBA                 // Format_DXT5
                                    };

const GLenum GDToGLWrapAxis[] = {
                        GL_TEXTURE_WRAP_S,      // Wrap_S,
                        GL_TEXTURE_WRAP_T       // Wrap_T
                                    };

const GLenum GDToGLWrapMode[] = {
                        GL_CLAMP_TO_EDGE_EXT,   // Wrap_Clamp,
                        GL_REPEAT               // Wrap_Repeat
                                    };

const GLenum GDToGLMagFilter[] = {
                        GL_NEAREST,             // MagFilter_Nearest,
                        GL_LINEAR               // MagFilter_Linear
                                    };

const GLenum GDToGLMinFilter[] = {
                        GL_NEAREST,                 // MinFilter_Nearest,
                        GL_LINEAR,                  // MinFilter_Linear
                        GL_NEAREST_MIPMAP_NEAREST,  // MinFilter_NearestMipmapNearest,
                        GL_LINEAR_MIPMAP_NEAREST,   // MinFilter_LinearMipmapNearest,
                        GL_NEAREST_MIPMAP_LINEAR,   // MinFilter_NearestMipmapLinear,
                        GL_LINEAR_MIPMAP_LINEAR     // MinFilter_LinearMipmapLinear
                                    };



///////////////////////////////////////////////////////////////////////////////
// OGLTexture1D
///////////////////////////////////////////////////////////////////////////////
OGLTexture1D::OGLTexture1D()
    : mTextureID(0)
{
}

void OGLTexture1D::Init()
{
    Super::Init();

    if( mTextureID == 0 )
        glGenTextures( 1, &mTextureID );

    glBindTexture( GL_TEXTURE_1D, mTextureID );

    if( mImage.GetData() != NULL )
    {
        // If asked to have mipmaps but texture has none, automatically generate them.
        if( mImage.GetNumMipmaps() == 1 && mHasMipmaps )
        {
            gluBuild1DMipmaps( GL_TEXTURE_1D, GDToGLIntTexFormat[mFormat], mWidth, GDToGLTexFormat[mFormat], GL_UNSIGNED_BYTE, mImage.GetData() );
        }
        else
        {
            UInt32 width     = mWidth;
            UInt32 offset    = 0;
            UInt32 dataSize  = 0;

            // Load each mipmap individually
            for( UInt32 i = 0; i < mImage.GetNumMipmaps() && width; ++i )
            {
                dataSize  = Image::GetSize( mFormat, width, 1 );

                if( mImage.IsCompressed() )
                    glCompressedTexImage1DARB( GL_TEXTURE_1D, i, GDToGLIntTexFormat[mFormat], width, 0, dataSize, mImage.GetData() + offset );
                else
                    glTexImage1D( GL_TEXTURE_1D, i, GDToGLIntTexFormat[mFormat], width, 0, GDToGLTexFormat[mFormat], GL_UNSIGNED_BYTE, mImage.GetData() + offset );

                offset += dataSize;
                width >>= 1;
            }
        }
    }

    // Set filtering params
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GDToGLMagFilter[mMagFilter] );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GDToGLMinFilter[mMinFilter] );      

    // Anisotropy
    if( mAnisotropy != 0.0f )
        glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MAX_ANISOTROPY_EXT, mAnisotropy );
}

void OGLTexture1D::Kill()
{
    glDeleteTextures( 1, &mTextureID );
    Super::Kill();
}



void OGLTexture1D::SetWrapMode( Texture::WrapAxis pWrapAxis, Texture::WrapMode pWrapMode )
{
    Super::SetWrapMode( pWrapAxis, pWrapMode );

    glBindTexture( GL_TEXTURE_1D, mTextureID );
    glTexParameteri( GL_TEXTURE_1D, GDToGLWrapAxis[pWrapAxis], GDToGLWrapMode[pWrapMode] );
}

void OGLTexture1D::SetMinFilter( Texture::MinFilter pFilter )
{
    Super::SetMinFilter( pFilter );

    glBindTexture( GL_TEXTURE_1D, mTextureID );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GDToGLMinFilter[pFilter] );
}

void OGLTexture1D::SetMagFilter( Texture::MagFilter pFilter )
{
    Super::SetMagFilter( pFilter );

    glBindTexture( GL_TEXTURE_1D, mTextureID );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GDToGLMagFilter[pFilter] );
}

void OGLTexture1D::SetAnisotropy( Float pAnisotropy )
{
    Super::SetAnisotropy( pAnisotropy );

    glBindTexture( GL_TEXTURE_1D, mTextureID );
    glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MAX_ANISOTROPY_EXT, pAnisotropy );
}


///////////////////////////////////////////////////////////////////////////////
// OGLTexture2D
///////////////////////////////////////////////////////////////////////////////
OGLTexture2D::OGLTexture2D()
    : mTextureID(0)
{
}

void OGLTexture2D::Init()
{
    Super::Init();

    if( mTextureID == 0 )
        glGenTextures( 1, &mTextureID );

    glBindTexture( GL_TEXTURE_2D, mTextureID );

    //if( mImage.IsCompressed() )
        //mImage.FlipY();

    if( mImage.GetData() != NULL )
    {
        // If asked to have mipmaps but texture has none, automatically generate them.
        if( mImage.GetNumMipmaps() == 1 && mHasMipmaps )
        {
            gluBuild2DMipmaps( GL_TEXTURE_2D, GDToGLIntTexFormat[mFormat], mWidth, mHeight, GDToGLTexFormat[mFormat], GL_UNSIGNED_BYTE, mImage.GetData() );
        }
        else
        {
            UInt32 height    = mHeight;
            UInt32 width     = mWidth;
            UInt32 offset    = 0;
            UInt32 dataSize  = 0;

            // Load each mipmap individually
            for( UInt32 i = 0; i < mImage.GetNumMipmaps() && (width || height); ++i )
            {
                if (width == 0)
                    width = 1;

                if (height == 0)
                    height = 1;

                dataSize  = Image::GetSize( mFormat, width, height );
                
                if( mImage.IsCompressed() )
                    glCompressedTexImage2DARB( GL_TEXTURE_2D, i, GDToGLIntTexFormat[mFormat], width, height, 0, dataSize, mImage.GetData() + offset );
                else
                    glTexImage2D( GL_TEXTURE_2D, i, GDToGLIntTexFormat[mFormat], width, height, 0, GDToGLTexFormat[mFormat], GL_UNSIGNED_BYTE, mImage.GetData() + offset );
                
                offset += dataSize;
                width  >>= 1;
                height >>= 1;
            }
        }
    }

    // Set filtering params
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GDToGLMagFilter[mMagFilter] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GDToGLMinFilter[mMinFilter] );      

    // Anisotropy
    if( mAnisotropy != 0.0f )
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, mAnisotropy );
}

void OGLTexture2D::Kill()
{
    glDeleteTextures( 1, &mTextureID );
    Super::Kill();
}

void OGLTexture2D::SetWrapMode( Texture::WrapAxis pWrapAxis, Texture::WrapMode pWrapMode )
{
    Super::SetWrapMode( pWrapAxis, pWrapMode );

    glBindTexture( GL_TEXTURE_2D, mTextureID );
    glTexParameteri( GL_TEXTURE_2D, GDToGLWrapAxis[pWrapAxis], GDToGLWrapMode[pWrapMode] );
}

void OGLTexture2D::SetMinFilter( Texture::MinFilter pFilter )
{
    Super::SetMinFilter( pFilter );

    glBindTexture( GL_TEXTURE_2D, mTextureID );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GDToGLMinFilter[pFilter] );
}

void OGLTexture2D::SetMagFilter( Texture::MagFilter pFilter )
{
    Super::SetMagFilter( pFilter );

    glBindTexture( GL_TEXTURE_2D, mTextureID );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GDToGLMagFilter[pFilter] );
}

void OGLTexture2D::SetAnisotropy( Float pAnisotropy )
{
    Super::SetAnisotropy( pAnisotropy );

    glBindTexture( GL_TEXTURE_2D, mTextureID );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, pAnisotropy );
}


///////////////////////////////////////////////////////////////////////////////
// OGLTexture3D
///////////////////////////////////////////////////////////////////////////////
OGLTexture3D::OGLTexture3D()
    : mTextureID(0)
{
}

void OGLTexture3D::Init()
{
    Super::Init();

    if( mTextureID == 0 )
        glGenTextures( 1, &mTextureID );

    glBindTexture( GL_TEXTURE_3D, mTextureID );

    if( mImage.GetData() != NULL )
    {
        UInt32 height    = mHeight;
        UInt32 width     = mWidth;
        UInt32 depth     = mDepth;
        UInt32 offset    = 0;
        UInt32 dataSize  = 0;

        // Load each mipmap individually
        for( UInt32 i = 0; i < mImage.GetNumMipmaps() && (width || height || depth); ++i )
        {
            if( width == 0 )
                width = 1;

            if( height == 0 )
                height = 1;

            if( depth == 0 )
                depth = 1;

            dataSize  = Image::GetSize( mFormat, width, height, depth );

            if( mImage.IsCompressed() )
                glCompressedTexImage3DARB( GL_TEXTURE_3D, i, GDToGLIntTexFormat[mFormat], width, height, depth, 0, dataSize, mImage.GetData() + offset );
            else
                glTexImage3D( GL_TEXTURE_3D, i, GDToGLIntTexFormat[mFormat], width, height, depth, 0, GDToGLTexFormat[mFormat], GL_UNSIGNED_BYTE, mImage.GetData() + offset );

            offset += dataSize;
            width  >>= 1;
            height >>= 1;
        }
    }

    // Set filtering params
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GDToGLMagFilter[mMagFilter] );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GDToGLMinFilter[mMinFilter] );      

    // Anisotropy
    if( mAnisotropy != 0.0f )
        glTexParameterf( GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, mAnisotropy );
}

void OGLTexture3D::Kill()
{
    glDeleteTextures( 1, &mTextureID );
    Super::Kill();
}

void OGLTexture3D::SetWrapMode( Texture::WrapAxis pWrapAxis, Texture::WrapMode pWrapMode )
{
    Super::SetWrapMode( pWrapAxis, pWrapMode );

    glBindTexture( GL_TEXTURE_3D, mTextureID );
    glTexParameteri( GL_TEXTURE_3D, GDToGLWrapAxis[pWrapAxis], GDToGLWrapMode[pWrapMode] );
}

void OGLTexture3D::SetMinFilter( Texture::MinFilter pFilter )
{
    Super::SetMinFilter( pFilter );

    glBindTexture( GL_TEXTURE_3D, mTextureID );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GDToGLMinFilter[pFilter] );
}

void OGLTexture3D::SetMagFilter( Texture::MagFilter pFilter )
{
    Super::SetMagFilter( pFilter );

    glBindTexture( GL_TEXTURE_3D, mTextureID );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GDToGLMagFilter[pFilter] );
}

void OGLTexture3D::SetAnisotropy( Float pAnisotropy )
{
    Super::SetAnisotropy( pAnisotropy );

    glBindTexture( GL_TEXTURE_3D, mTextureID );
    glTexParameterf( GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, pAnisotropy );
}


///////////////////////////////////////////////////////////////////////////////
// OGLCubemap
///////////////////////////////////////////////////////////////////////////////
OGLCubemap::OGLCubemap()
    : mTextureID(0)
{
}

void OGLCubemap::Init()
{
    Super::Init();

    if( mTextureID == 0 )
        glGenTextures( 1, &mTextureID );

    glBindTexture( GL_TEXTURE_CUBE_MAP, mTextureID );

    /*
    for( UInt32 i = 0; i < NumFaces; i++ )
    {
        if( mImages[i].IsCompressed() )
            mImages[i].FlipY();
    }

    if( mImages[0].IsCompressed() )
    {
        Image tmp( mImages[PositiveY] );
        mImages[PositiveY] = mImages[NegativeY];
        mImages[NegativeY] = tmp;
    }
    */

    if( mImages[PositiveX].GetData() != NULL )
    {
        // If asked to have mipmaps but texture has none, automatically generate them.
        if( mImages[PositiveX].GetNumMipmaps() == 1 && mHasMipmaps )
        {
            // Load every face.
            for( Int32 iFace = 0; iFace < NumFaces; iFace++ )
            {
                if( mImages[iFace].IsCompressed() )
                    glCompressedTexImage2DARB( GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace, 0, GDToGLIntTexFormat[mFormat], mWidth, mHeight, 0, Image::GetSize(mFormat, mWidth, mHeight), mImages[iFace].GetData() );
                else
                    gluBuild2DMipmaps( GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace, GDToGLIntTexFormat[mFormat], mWidth, mHeight, GDToGLTexFormat[mFormat], GL_UNSIGNED_BYTE, mImages[iFace].GetData() );
            }
        }
        else
        {
            // Load every face.
            for( Int32 iFace = 0; iFace < NumFaces; iFace++ )
            {
                UInt32 height    = mHeight;
                UInt32 width     = mWidth;
                UInt32 offset    = 0;
                UInt32 dataSize  = 0;

                // Load each mipmap individually.
                for( UInt32 iMipmap = 0; iMipmap < mImages[iFace].GetNumMipmaps() && (width || height); ++iMipmap )
                {
                    if (width == 0)
                        width = 1;

                    if (height == 0)
                        height = 1;

                    dataSize  = Image::GetSize( mFormat, width, height );

                    if( mImages[iFace].IsCompressed() )
                        glCompressedTexImage2DARB( GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace, iMipmap, GDToGLIntTexFormat[mFormat], width, height, 0, dataSize, mImages[iFace].GetData() + offset );
                    else
                        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace, iMipmap, GDToGLIntTexFormat[mFormat], width, height, 0, GDToGLTexFormat[mFormat], GL_UNSIGNED_BYTE, mImages[iFace].GetData() + offset );

                    offset += dataSize;
                    width  >>= 1;
                    height >>= 1;
                }
            }
        }
    }

    // Set filtering params
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GDToGLMagFilter[mMagFilter] );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GDToGLMinFilter[mMinFilter] );      

    // Anisotropy
    if( mAnisotropy != 0.0f )
        glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, mAnisotropy );    
}

void OGLCubemap::Kill()
{
    glDeleteTextures( 1, &mTextureID );
    Super::Kill();
}

void OGLCubemap::SetWrapMode( Texture::WrapAxis pWrapAxis, Texture::WrapMode pWrapMode )
{
    Super::SetWrapMode( pWrapAxis, pWrapMode );

    glBindTexture( GL_TEXTURE_CUBE_MAP, mTextureID );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GDToGLWrapAxis[pWrapAxis], GDToGLWrapMode[pWrapMode] );
}

void OGLCubemap::SetMinFilter( Texture::MinFilter pFilter )
{
    Super::SetMinFilter( pFilter );

    glBindTexture( GL_TEXTURE_CUBE_MAP, mTextureID );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GDToGLMinFilter[pFilter] );
}

void OGLCubemap::SetMagFilter( Texture::MagFilter pFilter )
{
    Super::SetMagFilter( pFilter );

    glBindTexture( GL_TEXTURE_CUBE_MAP, mTextureID );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GDToGLMagFilter[pFilter] );
}

void OGLCubemap::SetAnisotropy( Float pAnisotropy )
{
    Super::SetAnisotropy( pAnisotropy );

    glBindTexture( GL_TEXTURE_CUBE_MAP, mTextureID );
    glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, pAnisotropy );
}


} // namespace Gamedesk
