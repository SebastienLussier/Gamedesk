/**
*  @file       Image.cpp
*  @brief
*  @author     Sébastien Lussier.
*  @date       19/09/04.
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
#include "Image.h"

#include "Maths/Maths.h"


namespace Gamedesk {
	
	
const UInt32 SIZE_TABLE[Image::Format_Num] =
{
        1,          // Format_L8,
        1,          // Format_A8,
        2,          // Format_L8A8,
        3,          // Format_R8G8B8,
        4,          // Format_R8G8B8A8,
        3,          // Format_B8G8R8,
        4,          // Format_B8G8R8A8,
        8,          // Format_DXT1,
        16,         // Format_DXT3,
        16,         // Format_DXT5,
        0,          // Format_Unknown
};


Image::Image()
    : mWidth(0)
    , mHeight(0)
    , mDepth(0)
    , mNumMipmaps(0)
    , mFormat(Format_Unknown)
    , mData(NULL)
    , mDataSize(0)
{
}

Image::Image( const Image& pOther )
    : mData(NULL)
{
    *this = pOther;
}

Image::Image( const char* const* pXPMImage )
    : mData(NULL)
    , mDataSize(0)
{
    UInt32 w = 0;
    UInt32 h = 0;
    UInt32 numColors = 0;
    UInt32 bpp = 0;

    const char* buf = pXPMImage[0];
    if( sscanf_s( buf, "%d %d %d %d", &w, &h, &numColors, &bpp ) < 4 )
        return;

    Map<char,UInt32> colors;
    for( UInt32 i = 0; i < numColors; i++ )
    {
        String str = pXPMImage[i+1];

        if( str.empty() )
            return;

        char index = str[0];
        UInt32 colorVal = 0;

        if( str.length() < 11 )
        {
            colors[index] = 0xFFFFFF00; // Fully transparent
        }
        else
        {
            String color = str.substr( 5, str.length() - 4 );
            for( UInt32 j = 0; j < 6; j++ )
            {
                if( color[j] >= '0' && color[j] <= '9' )
                    colorVal = (colorVal << 4) | ((color[j] - '0') & 0x000000FF);
                else if( color[j] >= 'a' && color[j] <= 'f' )
                    colorVal = (colorVal << 4) | ((color[j] - 'a' + 10) & 0x000000FF);
                else
                    colorVal = (colorVal << 4) | ((color[j] - 'A' + 10) & 0x000000FF);
            }

            colors[index] = (colorVal << 8) | 0x000000FF; // Fully opaque
        }
    }

    Create( w, h, Format_R8G8B8A8 );
    UInt32* data = (UInt32*)mData;

    for( UInt32 y = 0; y < h; y++ )
    {
        buf = pXPMImage[y+1+numColors];

        for( UInt32 x = 0; x < w; x++ )
        {
            UInt32 val = colors[buf[x]];
            *data = ((val >> 24) & 0x000000ff) |
                    ((val >>  8) & 0x0000ff00) |
                    ((val <<  8) & 0x00ff0000) |
                    ((val << 24) & 0xff000000);
            data++;
        }
    }
}

Image::~Image()
{
    if( mData )
        GD_DELETE_ARRAY(mData);
}

const Image& Image::operator = ( const Image& pOther )
{
    // Ignore self copy!
    if( &pOther == this )
        return *this;

    if( mData )
        GD_DELETE_ARRAY(mData);

    mWidth          = pOther.mWidth;
    mHeight         = pOther.mHeight;
    mDepth          = pOther.mDepth;
    mNumMipmaps     = pOther.mNumMipmaps;
    mFormat         = pOther.mFormat;
    mDataSize       = pOther.mDataSize;
    mData           = GD_NEW_ARRAY(Byte, mDataSize, this, "Engine::Graphic::Image");

    memcpy( mData, pOther.mData, mDataSize );

    return *this;
}

UInt32 Image::GetSize( Image::Format pFormat, UInt32 pWidth, UInt32 pHeight, UInt32 pDepth )
{
    if (pWidth == 0)
        pWidth = 1;

    if (pHeight == 0)
        pHeight = 1;

    if( pFormat >= Format_DXT1 && pFormat <= Format_DXT5 )
        return ((pWidth+3) >> 2) * ((pHeight+3) >> 2) * SIZE_TABLE[pFormat];
    else
        return pWidth * pHeight * SIZE_TABLE[pFormat] * pDepth;
}

UInt32 Image::GetSizeWithMipmaps( Image::Format pFormat, UInt32 pWidth, UInt32 pHeight, UInt32 pDepth, Int32 pMipmapCount )
{
    UInt32 dataSize = 0;

    UInt32 width  = pWidth;
    UInt32 height = pHeight;
    UInt32 depth  = pDepth;
    UInt32 mip    = pMipmapCount == -1 ? 0x7FFFFFFF : pMipmapCount;

    while( (width || height) && mip != 0 )
    {
        dataSize += Image::GetSize( pFormat, width, height, depth );

        width  >>= 1;
        height >>= 1;

        if( depth != 1 )
            depth  >>= 1;

        mip--;
    }

    return dataSize;
}

UInt32 Image::GetNumChannels( Image::Format pFormat )
{
    switch( pFormat )
    {
        case Format_L8:         return 1;
        case Format_A8:         return 1;
        case Format_L8A8:       return 2;
        case Format_R8G8B8:     return 3;
        case Format_R8G8B8A8:   return 4;
        case Format_B8G8R8:     return 3;
        case Format_B8G8R8A8:   return 4;
        case Format_DXT1:       return 3;
        case Format_DXT3:       return 4;
        case Format_DXT5:       return 4;
        default:                return 0;
    }
}

Bool Image::IsCompressed( Image::Format pFormat )
{
    return pFormat >= Format_DXT1 && pFormat <= Format_DXT5;
}

void Image::Create( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat, UInt32 pNumMipmaps, UInt32 pDepth )
{
    GD_ASSERT_M( pWidth > 0, "[Image::Create] Width is 0!" );
    GD_ASSERT_M( pHeight > 0, "[Image::Create] Width is 0!" );
    GD_ASSERT_M( pFormat < Format_Unknown, "[Image::Create] Unknown texture format!" );
    GD_ASSERT_M( pNumMipmaps > 0, "[Image::Create] Mipmap count must at least be 1!" );
    GD_ASSERT_M( pDepth > 0, "[Image::Create] Image depth must at least be 1!" );

    if( mData )
        GD_DELETE_ARRAY(mData);

    mWidth  = pWidth;
    mHeight = pHeight;
    mDepth  = pDepth;
    mFormat = pFormat;
    mNumMipmaps = pNumMipmaps;

    mDataSize = Image::GetSizeWithMipmaps( mFormat, mWidth, mHeight, mDepth, mNumMipmaps );
    mData = GD_NEW_ARRAY(Byte, mDataSize, this, "Engine::Graphic::Image");
}

const Byte* Image::GetData() const
{
    return mData;
}

Byte* Image::GetData()
{
    return mData;
}

Bool Image::Copy( const Image& pImage, UInt32 pX, UInt32 pY )
{
    GD_ASSERT_M( !IsCompressed(), "[Image::Copy] Copy of compressed images not supported yet!" );
    GD_ASSERT_M( !IsVolume(), "[Image::Copy] Copy of volume images not supported yet!" );

    UInt32 bpp = SIZE_TABLE[mFormat];

    // Format must match.
    if( pImage.GetFormat() != mFormat )
        return false;

    // Must fit inside boundaries
    if( pX + pImage.GetWidth() > mWidth || pY + pImage.GetHeight() > mHeight )
        return false;

    // Copy scanline by scanline
    Byte* dst = &mData[pY * mWidth * bpp];
    const Byte* src = pImage.GetData();
    for( UInt32 i = 0; i < pImage.GetHeight(); i++ )
    {
        memcpy( &dst[pX * bpp], src, pImage.GetWidth() * bpp );

        dst += mWidth * bpp;
        src += pImage.GetWidth() * bpp;
    }

    return true;
}

void Image::ChangeGamma( Float pGamma )
{
    GD_ASSERT_M( !IsCompressed(), "[Image::ChangeGamma] Gamma change of compressed images not supported yet!" );
    GD_ASSERT_M( !IsVolume(), "[Image::ChangeGamma] Gamma change of volume images not supported yet!" );

    // This function was taken from a couple engines that I saw,
    // which most likely originated from the Aftershock engine.
    // Kudos to them!  What it does is increase/decrease the intensity
    // of the lightmap so that it isn't so dark.  Quake uses hardware to
    // do this, but we will do it in code.
    UInt32 numComponents = SIZE_TABLE[mFormat];
    UInt32 size = mWidth * mHeight;
    Byte*  data = mData;

    const UInt32 MAX_COMPONENTS      = 4;
    const Float  MAX_COMPONENT_VALUE = 255.0f;

    Float components[MAX_COMPONENTS];

    // Nothing to do in this case!
    if( pGamma == 1.0f )
        return;

    // Go through every pixel in the image
    for( UInt32 i = 0; i < size; i++, data += numComponents )
    {
        Float scale = 1.0f;
        Float temp  = 0.0f;

        for( UInt32 j = 0; j < numComponents; j++ )
        {
            // Extract the current component value
            components[j] = (Float)data[j];

            // Multiply the factor by the component value, while keeping it to a 255 ratio
            components[j] *= pGamma;

            // Check if the the value went past the highest value
            if( components[j] > MAX_COMPONENT_VALUE && (temp = (MAX_COMPONENT_VALUE/components[j])) < scale )
                scale = temp;
        }

        for( UInt32 j = 0; j < numComponents; j++ )
        {
            // Get the scale for this pixel and multiply it by the component value
            components[j] *= scale;

            // Assign the new gamma'nized value to the image
            data[j] = (Byte)components[j];
        }
    }
}

void Image::ToGrayscale()
{
    Int32 numChannels = GetNumChannels();

    if( !IsCompressed() || numChannels < 3 )
        return;

    UInt32 size, len;
    Byte* src = mData;

    size = len = GetSizeWithMipmaps(Format_L8, mWidth, mHeight, mDepth, mNumMipmaps);
    Byte* dest = GD_NEW_ARRAY(Byte, size, this, "Engine::Graphic::Image");

    do
    {
        *dest++ = (77 * src[0] + 151 * src[1] + 28 * src[2] + 128) >> 8;
        src += numChannels;
    } while (--len);

    GD_DELETE_ARRAY(mData);
    mData = dest;

    mFormat = Format_L8;
}

void Image::ToNormalMap()
{
    if( mFormat != Format_L8 )
        return;

    Byte *row0, *row1, *row2;
    UInt32 x,y,w,h,mipmap;
    UInt32 sx, sy, len, predx, succx;

    mFormat = Format_R8G8B8;
    mDataSize = GetSizeWithMipmaps(mFormat, mWidth, mHeight, mDepth, mNumMipmaps);

    Byte* newPixels = GD_NEW_ARRAY(Byte, mDataSize, this, "Engine::Graphic::Image");
    Byte* dest = newPixels;
    Byte* src = mData;

    mipmap = 0;
    w = mWidth;
    h = mHeight;

    do
    {
        row1 = src + (h - 1) * w;
        row2 = src;

        y = h;

        do
        {
            row0 = row1;
            row1 = row2;
            row2 += w;

            if( y == 1 )
                row2 = src;

            x = w - 1;
            succx = 0;

            do
            {
                predx = x;
                x = succx++;

                if(succx == w)
                    succx = 0;

                sx = (row0[predx] + 2 * row1[predx] + row2[predx]) - (row0[succx] + 2 * row1[succx] + row2[succx]);
                sy = (row0[predx] + 2 * row0[x]     + row0[succx]) - (row2[predx] + 2 * row2[x]     + row2[succx]);

                len = UInt32(0x000FF000 * Maths::FastRSqrt(Float(sx * sx + sy * sy + 256*256)));
                sx *= len;
                sy *= len;

                dest[0] = ((sx  + 0x000FF000) >> 13);
                dest[1] = ((sy  + 0x000FF000) >> 13);
                dest[2] = ((len + 0x00000FF0) >> 5);
                dest += 3;
            } while (succx);
        } while (--y);

        src += w * h;

        if( w > 1 )
            w >>= 1;

        if( h > 1 )
            h >>= 1;

        mipmap++;
    } while (mipmap < mNumMipmaps);

    GD_DELETE_ARRAY(mData);
    mData = newPixels;
}

void Image::FlipY()
{
    UInt32 width  = mWidth;
    UInt32 height = mHeight;
    UInt32 depth  = mDepth;

    UInt32 offset = 0;

    for( UInt32 i = 0; i < mNumMipmaps && (width || height); i++ )
    {
        if( width == 0 )
            width = 1;

        if( height == 0 )
            height = 1;

        if( depth == 0 )
            depth = 1;

        FlipY( mData + offset, width, height, depth );
        offset += Image::GetSize( mFormat, width, height, depth );

        width  >>= 1;
        height >>= 1;
        depth  >>= 1;
    }
}

struct DXTColBlock
{
    UInt16  mCol0;
    UInt16  mCol1;
    Byte    mRow[4];
};

struct DXT3AlphaBlock
{
    UInt16  mRow[4];
};

struct DXT5AlphaBlock
{
    Byte    mAlpha0;
    Byte    mAlpha1;
    Byte    mRow[6];
};

void FlipDXT1(Byte* data, UInt32 count);
void FlipDXT3(Byte* data, UInt32 count);
void FlipDXT5(Byte* data, UInt32 count);

void Image::FlipY( Byte* pData, UInt32 pWidth, UInt32 pHeight, UInt32 pDepth )
{
    UInt32 offset;

    if( !IsCompressed() )
    {
        UInt32 imageSize = GetSize(mFormat, pWidth, pHeight);
        UInt32 lineSize  = imageSize / pHeight;

        Byte* swapTmp = GD_ALLOC(Byte, lineSize, this, "TempBuffer");
		GD_ASSERT(swapTmp);

        for( UInt32 n = 0; n < pDepth; n++ )
        {
            offset = imageSize * n;

            Byte* top    = pData + offset;
            Byte* bottom = top + (imageSize-lineSize);

            for( UInt32 i = 0; i < (pHeight >> 1); i++ )
            {
                memcpy( swapTmp, bottom, lineSize );
                memcpy( bottom, top, lineSize );
                memcpy( top, swapTmp, lineSize );

                top += lineSize;
                bottom -= lineSize;
            }
        }

		GD_FREE(swapTmp);
    }
    else
    {
        void (*flipDXTn)(Byte*, UInt32) = NULL;

        UInt32 xBlocks   = (pWidth+3) / 4;
        UInt32 yBlocks   = (pHeight+3) / 4;
        UInt32 blockSize = SIZE_TABLE[mFormat];
        UInt32 lineSize  = xBlocks * blockSize;

		Byte* swapTmp = GD_ALLOC(Byte, lineSize, this, "TempBuffer");
		GD_ASSERT(swapTmp);

        DXTColBlock* top    = NULL;
        DXTColBlock* bottom = NULL;

		switch(mFormat)
		{
		case Format_DXT1:
			flipDXTn = &FlipDXT1;
			break;

		case Format_DXT3:
			flipDXTn = &FlipDXT3;
			break;

		case Format_DXT5:
			flipDXTn = &FlipDXT5;
			break;

		default:
			debugBreak();
		}

		GD_ASSERT(flipDXTn);

        for( UInt32 j = 0; j < (yBlocks >> 1); j++ )
        {
            top = (DXTColBlock*)(pData + j * lineSize);
            bottom = (DXTColBlock*)(pData + (((yBlocks-j)-1) * lineSize));

            (*flipDXTn)( (Byte*)top, xBlocks );
            (*flipDXTn)( (Byte*)bottom, xBlocks );

            memcpy( swapTmp, bottom, lineSize );
            memcpy( bottom, top, lineSize );
            memcpy( top, swapTmp, lineSize );
        }

		GD_FREE(swapTmp);
    }
}


void FlipColorBlock(Byte *data)
{
    Byte tmp;

    tmp = data[4];
    data[4] = data[7];
    data[7] = tmp;

    tmp = data[5];
    data[5] = data[6];
    data[6] = tmp;
}

void FlipSimpleAlphaBlock(UInt16 *data)
{
	UInt16 tmp;

	tmp = data[0];
	data[0] = data[3];
	data[3] = tmp;

	tmp = data[1];
	data[1] = data[2];
	data[2] = tmp;
}

void ComplexAlphaHelper(Byte* Data)
{
	UInt16 tmp[2];

	// One 4 pixel line is 12 bit, copy each line into
	// a ushort, swap them and copy back
	tmp[0] = (Data[0] | (Data[1] << 8)) & 0xfff;
	tmp[1] = ((Data[1] >> 4) | (Data[2] << 4)) & 0xfff;

	Data[0] = tmp[1];
	Data[1] = (tmp[1] >> 8) | (tmp[0] << 4);
	Data[2] = tmp[0] >> 4;
}

void FlipComplexAlphaBlock(Byte *Data)
{
	Byte tmp[3];
	Data += 2; // Skip 'palette'

	// Swap upper two rows with lower two rows
	memcpy(tmp, Data, 3);
	memcpy(Data, Data + 3, 3);
	memcpy(Data + 3, tmp, 3);

	// Swap 1st with 2nd row, 3rd with 4th
	ComplexAlphaHelper(Data);
	ComplexAlphaHelper(Data + 3);
}

void FlipDXT1(Byte* data, UInt32 count)
{
	for(UInt32 i = 0; i < count; ++i)
	{
		FlipColorBlock(data);
		data += 8; // Advance to next block
	}
}

void FlipDXT3(Byte* data, UInt32 count)
{
	for(UInt32 i = 0; i < count; ++i)
	{
		FlipSimpleAlphaBlock((UInt16*)data);
		FlipColorBlock(data + 8);
		data += 16; // Advance to next block
	}
}

void FlipDXT5(Byte* data, UInt32 count)
{
	for(UInt32 i = 0; i < count; ++i)
	{
		FlipComplexAlphaBlock(data);
		FlipColorBlock(data + 8);
		data += 16; // Advance to next block
	}
}

void Flip3dc(Byte* data, UInt32 count)
{
	for(UInt32 i = 0; i < count; ++i)
	{
		FlipComplexAlphaBlock(data);
		FlipComplexAlphaBlock(data + 8);
		data += 16; // Advance to next block
	}
}


Stream& operator << ( Stream& pStream, Image& pImage )
{
    pStream << pImage.mWidth;
    pStream << pImage.mHeight;
    pStream << pImage.mDepth;
    pStream << pImage.mNumMipmaps;

	pStream.Serialize( &pImage.mFormat, sizeof(pImage.mFormat) );

    pStream << pImage.mDataSize;

    if( pStream.In() )
    {
        if( pImage.mData )
            GD_DELETE_ARRAY(pImage.mData);

        pImage.mData = GD_NEW_ARRAY(Byte, pImage.mDataSize, &pImage, "Engine::Graphic::Image");
    }
    pStream.Serialize( pImage.mData, pImage.mDataSize );

    return pStream;
}

void Image::DumpRaw( const String& pFilename ) const
{
    FILE* file;
    
    fopen_s( &file, pFilename.c_str(), "wb" );
    GD_ASSERT(file);

    fwrite( mData, mDataSize, 1, file );
    fclose( file );
}


} // namespace Gamedesk
