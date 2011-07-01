/**
 *  @file       Image.h
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
#ifndef     _IMAGE_H_
#define     _IMAGE_H_


namespace Gamedesk {


class ENGINE_API Image
{
public:
    enum Format
    {
        Format_L8,
        Format_A8,
        Format_L8A8, 
        Format_R8G8B8,
        Format_R8G8B8A8,
        Format_B8G8R8,
        Format_B8G8R8A8,
        Format_DXT1,
        Format_DXT3,
        Format_DXT5,
        Format_Unknown,
        Format_Num,
        Format_MAX = 0xFF
    };

public:
    Image();
    Image( const Image& pOther );
    Image( const char* const* pXPMImage );
    virtual ~Image();

    const Image& operator = ( const Image& pOther );

    void Create( UInt32 pWidth, UInt32 pHeight, Format pFormat, UInt32 pNumMipmaps = 1, UInt32 pDepth = 1 );

    UInt32 GetWidth() const         { return mWidth;      }
    UInt32 GetHeight() const        { return mHeight;     }
    UInt32 GetDepth() const         { return mDepth;      }
    UInt32 GetNumMipmaps() const    { return mNumMipmaps; }
    Format GetFormat() const        { return mFormat;     }
    UInt32 GetDataSize() const      { return mDataSize;   }
    
    UInt32 GetNumChannels() const   { return Image::GetNumChannels( mFormat ); }  
    Bool   IsCompressed() const     { return Image::IsCompressed( mFormat ); }
    Bool   IsVolume() const         { return mDepth > 1;  }

    //! Access to internal data.
    const Byte* GetData() const;
    Byte* GetData();

    void FlipY();

    //! Copy an image into this image's buffer.
    //! Works only for normal textures (not compressed and no volume)
    Bool Copy( const Image& pImage, UInt32 pX, UInt32 pY );

    void ChangeGamma( Float pGamma );
    
    void ToGrayscale();
    void ToNormalMap();

    //! Serialize
    friend Stream& operator << ( Stream& pStream, Image& pImage );

    void DumpRaw( const String& pFilename ) const;

public:
    static UInt32 GetSize( Format pFormat, UInt32 pWidth, UInt32 pHeight, UInt32 pDepth = 1 );
    static UInt32 GetSizeWithMipmaps( Format pFormat, UInt32 pWidth, UInt32 pHeight, UInt32 pDepth = 1, Int32 pMipmapCount = -1 );
    static UInt32 GetNumChannels( Format pFormat );
    static Bool   IsCompressed( Format pFormat );

private:
    void FlipY( Byte* pData, UInt32 pWidth, UInt32 pHeight, UInt32 pDepth );
    
private:
    UInt32 mWidth;          //!< Image width.
    UInt32 mHeight;         //!< Image height.
    UInt32 mDepth;          //!< Image depth. Depth == 1 for normal images, > 1 for depth images.

    UInt32 mNumMipmaps;     //!< Number of mipmaps.
    Format mFormat;         //!< Image format.

    Byte*  mData;           //!< Pointer to image data buffer.
    UInt32 mDataSize;       //!< Size of image buffer.
};


} // namespace Gamedesk


#endif  //  _IMAGE_H_
