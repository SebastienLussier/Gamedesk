/**
 *  @file       PSPTexture.cpp
 *  @brief	    PSP Texture.
 *  @author     Sébastien Lussier.
 *  @date       12/09/2008.
 */
/*
 *  Copyright (C) 2008 Gamedesk
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
#include "PSPGraphic.h"
#include "PSPTexture.h"
#include "PSPGraphicSubsystem.h"
#include "PSPRenderer.h"


namespace Gamedesk {


IMPLEMENT_CLASS(PSPTexture2D);

///////////////////////////////////////////////////////////////////////////////
// PSPTexture2D
///////////////////////////////////////////////////////////////////////////////
PSPTexture2D::PSPTexture2D()
{
}

void PSPTexture2D::Init()
{
    Super::Init();

    if( mImage.IsCompressed() )
        mImage.FlipY();

    if( mImage.GetData() != NULL )
    {
        // If asked to have mipmaps but texture has none, automatically generate them.
        if( mImage.GetNumMipmaps() == 1 && mHasMipmaps )
        {
            // #TODO-PSP: Generate mipmaps, copy to vram ?
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
                
                // #TODO-PSP: Copy every mipmap to vram ?
                
                offset += dataSize;
                width  >>= 1;
                height >>= 1;
            }
        }
    }
}

void PSPTexture2D::Kill()
{
    Super::Kill();
}

void PSPTexture2D::SetWrapMode( Texture::WrapAxis pWrapAxis, Texture::WrapMode pWrapMode )
{
    Super::SetWrapMode( pWrapAxis, pWrapMode );
}

void PSPTexture2D::SetMinFilter( Texture::MinFilter pFilter )
{
    Super::SetMinFilter( pFilter );
}

void PSPTexture2D::SetMagFilter( Texture::MagFilter pFilter )
{
    Super::SetMagFilter( pFilter );
}

void PSPTexture2D::SetAnisotropy( Float pAnisotropy )
{
    Super::SetAnisotropy( pAnisotropy );
}


} // namespace Gamedesk
