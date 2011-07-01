/**
 *  @file       Texture.cpp
 *  @brief	    
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
#include "Texture.h"

#include "Graphic/Renderer.h"


namespace Gamedesk {
	
	
IMPLEMENT_ABSTRACT_CLASS(Texture);
IMPLEMENT_ABSTRACT_CLASS(Texture1D);
IMPLEMENT_ABSTRACT_CLASS(Texture2D);
IMPLEMENT_ABSTRACT_CLASS(Texture3D);
IMPLEMENT_ABSTRACT_CLASS(Cubemap);

void Texture::Serialize( Stream& pStream )
{
    Super::Serialize( pStream );

	pStream << mHasMipmaps;

	pStream.Serialize(&mFormat, sizeof(mFormat));
	pStream.Serialize(&mMinFilter, sizeof(mMinFilter));
	pStream.Serialize(&mMagFilter, sizeof(mMagFilter));
	pStream.Serialize(&mMagFilter, sizeof(mMagFilter));
	pStream.Serialize(&mWrapMode, sizeof(mWrapMode));
    
    pStream << mWidth;
    pStream << mHeight;
    pStream << mDepth;
    pStream << mAnisotropy;
}

void Texture1D::Serialize( Stream& pStream )
{
    Super::Serialize( pStream );
    pStream << mImage;
}

void Texture2D::Serialize( Stream& pStream )
{
    Super::Serialize( pStream );
    pStream << mImage;
}

void Texture3D::Serialize( Stream& pStream )
{
    Super::Serialize( pStream );
    pStream << mImage;
}

void Cubemap::Serialize( Stream& pStream )
{
    Super::Serialize( pStream );

    pStream << mImages[PositiveX];
    pStream << mImages[NegativeX];
    pStream << mImages[PositiveY];
    pStream << mImages[NegativeY];
    pStream << mImages[PositiveZ];
    pStream << mImages[NegativeZ];
}


Bool Texture1D::Create( const Image& pImage, Bool pCreateMipmaps )
{
    GD_ASSERT_M( pImage.GetHeight() == 1, "[Texture1D::Create] 1D texture must be created using images of height == 1" );
    if( pImage.GetHeight() != 1 )
        return false;

    mImage  = pImage;
    mFormat = pImage.GetFormat();
    mWidth  = pImage.GetWidth();
    mHasMipmaps = pCreateMipmaps;

    // Filtering
    if( mHasMipmaps )
    {
        mMagFilter = MagFilter_Linear; 
        mMinFilter = MinFilter_LinearMipmapLinear;
    }
    else
    {
        mMagFilter = MagFilter_Linear; 
        mMinFilter = MinFilter_Linear;
    }

    mAnisotropy = GraphicSubsystem::Instance()->GetRenderer()->GetMaxAnisotropy();

    return true;
}

Bool Texture1D::Create( UInt32 pWidth, Image::Format pFormat)
{
    mImage  = Image();
    mWidth  = pWidth;
    mFormat = pFormat;
    mHasMipmaps = false;

    // Filtering
    mMagFilter = MagFilter_Linear; 
    mMinFilter = MinFilter_Linear;
    mAnisotropy = GraphicSubsystem::Instance()->GetRenderer()->GetMaxAnisotropy();

    return true;
}

Image& Texture1D::GetImage()
{
    return mImage;
}

void Texture1D::Update()
{
    Create( mImage, mHasMipmaps );
    Init();
}

Bool Texture2D::Create( const Image& pImage, Bool pCreateMipmaps )
{
    mImage  = pImage;
    mFormat = pImage.GetFormat();
    mWidth  = pImage.GetWidth();
    mHeight = pImage.GetHeight();
    mHasMipmaps = pCreateMipmaps;

    // Filtering
    if( mHasMipmaps )
    {
        mMagFilter = MagFilter_Linear; 
        mMinFilter = MinFilter_LinearMipmapLinear;
    }
    else
    {
        mMagFilter = MagFilter_Linear; 
        mMinFilter = MinFilter_Linear;
    }

    mAnisotropy = GraphicSubsystem::Instance()->GetRenderer()->GetMaxAnisotropy();

    return true;
}

Bool Texture2D::Create( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat )
{
    mImage  = Image();
    mWidth  = pWidth;
    mHeight = pHeight;
    mFormat = pFormat;
    mHasMipmaps = false;

    // Filtering
    mMagFilter = MagFilter_Linear; 
    mMinFilter = MinFilter_Linear;
    mAnisotropy = GraphicSubsystem::Instance()->GetRenderer()->GetMaxAnisotropy();

    return true;
}

Image& Texture2D::GetImage()
{
    return mImage;
}

void Texture2D::Update()
{
    Create( mImage, mHasMipmaps );
    Init();
}

Bool Texture3D::Create( const Image& pImage, Bool pCreateMipmaps )
{
    mImage  = pImage;
    mFormat = pImage.GetFormat();
    mWidth  = pImage.GetWidth();
    mHeight = pImage.GetHeight();
    mDepth  = pImage.GetDepth();
    mHasMipmaps = pCreateMipmaps;

    // Filtering
    if( mHasMipmaps )
    {
        mMagFilter = MagFilter_Linear; 
        mMinFilter = MinFilter_LinearMipmapLinear;
    }
    else
    {
        mMagFilter = MagFilter_Linear; 
        mMinFilter = MinFilter_Linear;
    }

    mAnisotropy = GraphicSubsystem::Instance()->GetRenderer()->GetMaxAnisotropy();

    return true;
}

Bool Texture3D::Create( UInt32 pWidth, UInt32 pHeight, UInt32 pDepth, Image::Format pFormat )
{
    mImage  = Image();
    mWidth  = pWidth;
    mHeight = pHeight;
    mDepth  = pDepth;
    mFormat = pFormat;
    mHasMipmaps = false;

    // Filtering
    mMagFilter = MagFilter_Linear; 
    mMinFilter = MinFilter_Linear;
    mAnisotropy = GraphicSubsystem::Instance()->GetRenderer()->GetMaxAnisotropy();

    return true;
}

Image& Texture3D::GetImage()
{
    return mImage;
}

void Texture3D::Update()
{
    Create( mImage, mHasMipmaps );
    Init();
}

Bool Cubemap::Create( const Vector<Image*>& pImages, Bool pCreateMipmaps )
{
    // User must provide 6 images.
    GD_ASSERT_M( pImages.size() == Cubemap::NumFaces, "[Cubemap::Create] Six images must be given to create a cubemap." );
    if( pImages.size() != Cubemap::NumFaces )
        return false;

    Image::Format format = pImages[PositiveX]->GetFormat();
    UInt32        width  = pImages[PositiveX]->GetWidth();
    UInt32        height = pImages[PositiveX]->GetHeight();

    GD_ASSERT_M( width == height, "[Cubemap::Create] Square images must be used for the cubemap faces." )

    // Validate that all the face are of the same dimension/format.
    for( UInt32 i = 0; i < Cubemap::NumFaces; i++ )
    {
        GD_ASSERT_M( pImages[i]->GetFormat() == format && pImages[i]->GetWidth() == width && pImages[i]->GetHeight() == height, 
            "[Cubemap::Create] The Six cubemap images must be of the same dimension and format." );

        if( pImages[i]->GetFormat() != format || pImages[i]->GetWidth() != width || pImages[i]->GetHeight() != height )
            return false;
    }

    mImages[PositiveX] = *pImages[PositiveX];
    mImages[NegativeX] = *pImages[NegativeX];
    mImages[PositiveY] = *pImages[PositiveY];
    mImages[NegativeY] = *pImages[NegativeY];
    mImages[PositiveZ] = *pImages[PositiveZ];
    mImages[NegativeZ] = *pImages[NegativeZ];

    mFormat = format;
    mWidth  = width;
    mHeight = height;
    mHasMipmaps = pCreateMipmaps;

    // Filtering
    if( mHasMipmaps && mImages[PositiveX].GetNumMipmaps() > 1 )
    {
        mMagFilter = MagFilter_Linear; 
        mMinFilter = MinFilter_LinearMipmapLinear;
    }
    else
    {
        mMagFilter = MagFilter_Linear; 
        mMinFilter = MinFilter_Linear;
    }

    mAnisotropy = GraphicSubsystem::Instance()->GetRenderer()->GetMaxAnisotropy();

    return true;
}

Bool Cubemap::Create( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat )
{
    mImages[PositiveX] = Image();
    mImages[NegativeX] = Image();
    mImages[PositiveY] = Image();
    mImages[NegativeY] = Image();
    mImages[PositiveZ] = Image();
    mImages[NegativeZ] = Image();

    mWidth  = pWidth;
    mHeight = pHeight;
    mFormat = pFormat;
    mHasMipmaps = false;

    // Filtering
    mMagFilter = MagFilter_Linear; 
    mMinFilter = MinFilter_Linear;
    mAnisotropy = GraphicSubsystem::Instance()->GetRenderer()->GetMaxAnisotropy();

    return true;
}

Image& Cubemap::GetImage( CubemapFace pFace )
{
    GD_ASSERT_M( pFace > 0 && pFace < 6, "[Cubemap::GetImage] Invalid face given." );
    return mImages[pFace];
}

void Cubemap::Update()
{
    Vector<Image*> vectImages;
    vectImages.push_back( &mImages[PositiveX] );
    vectImages.push_back( &mImages[NegativeX] );
    vectImages.push_back( &mImages[PositiveY] );
    vectImages.push_back( &mImages[NegativeY] );
    vectImages.push_back( &mImages[PositiveZ] );
    vectImages.push_back( &mImages[NegativeZ] );

    Create( vectImages, mHasMipmaps );
    Init();
}


} // namespace Gamedesk
