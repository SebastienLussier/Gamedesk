/**
 *  @file       Texture.h
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
#ifndef     _TEXTURE_H_
#define     _TEXTURE_H_


#include "Resource/Resource.h"
#include "Graphic/Image/Image.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {


class ENGINE_API Texture : public Resource
{
    friend class TextureManager;
    DECLARE_ABSTRACT_CLASS(Texture, Resource);
    
public:
    enum WrapAxis
    {
        Wrap_S,
        Wrap_T,
        Wrap_AxisNum,
        Wrap_AxisMAX = 0xFFFFFFFF
    };

    enum WrapMode
    {
        Wrap_Clamp,
        Wrap_Repeat,
        Wrap_ModeNum,
        Wrap_ModeMAX = 0xFFFFFFFF
    };

    enum MagFilter
    {
        MagFilter_Nearest,
        MagFilter_Linear,
        MagFilter_Num,
        MagFilter_MAX = 0xFFFFFFFF
    };

    enum MinFilter
    {
        MinFilter_Nearest,
        MinFilter_Linear,
        MinFilter_NearestMipmapNearest,
        MinFilter_LinearMipmapNearest,
        MinFilter_NearestMipmapLinear,
        MinFilter_LinearMipmapLinear,
        MinFilter_Num,
        MinFilter_MAX = 0xFFFFFFFF
    };

public:
    virtual ~Texture()
    {
    }

    UInt32 GetWidth() const
    {
        return mWidth;
    }

    UInt32 GetHeight() const
    {
        return mHeight; 
    }

    UInt32 GetDepth() const
    {
        return mDepth; 
    }

    Image::Format GetFormat() const
    {
        return mFormat;
    }
        
    virtual void SetWrapMode( WrapAxis pWrapAxis, WrapMode pWrapMode )
    {
        mWrapMode[pWrapAxis] = pWrapMode;        
    }
    
    virtual void SetMagFilter( MagFilter pFilter )
    {
        mMagFilter = pFilter;
    }
    
    virtual void SetMinFilter( MinFilter pFilter )
    {
        mMinFilter = pFilter;
    }

    virtual void SetAnisotropy( Float pAnisotropy )
    {
        mAnisotropy = pAnisotropy;
    }

    Bool HasMipmaps() const
    {
        return mHasMipmaps;
    }

    MinFilter GetMinFilter() const
    {
        return mMinFilter;
    }

    MagFilter GetMagFilter() const
    {
        return mMagFilter;
    }

    WrapMode GetWrapMode( WrapAxis pWrapAxis ) const
    {
        return mWrapMode[pWrapAxis];
    }

    Float GetAnisotropy() const
    {
        return mAnisotropy;
    }

    virtual void Serialize( Stream& pStream );
    
protected:
    Texture()
        : mFormat(Image::Format_Unknown)
        , mHasMipmaps(false)
        , mMinFilter(MinFilter_NearestMipmapLinear)
        , mMagFilter(MagFilter_Linear)
        , mWidth(0)
        , mHeight(1)
        , mDepth(1)
        , mAnisotropy(0.0f)
    {
        mWrapMode[Wrap_S] = Wrap_Repeat;
        mWrapMode[Wrap_T] = Wrap_Repeat;
    }
    
protected:
    Image::Format   mFormat;
    Bool            mHasMipmaps;
    MinFilter       mMinFilter;
    MagFilter       mMagFilter;
    WrapMode        mWrapMode[2];    
    UInt32          mWidth;
    UInt32          mHeight;
    UInt32          mDepth;
    Float           mAnisotropy;
};


class ENGINE_API Texture1D : public Texture
{
    friend class TextureManager;
    DECLARE_ABSTRACT_RESOURCE_CLASS(Texture1D,Texture,GraphicSubsystem);
    
public:
    virtual ~Texture1D() {}
    virtual void Serialize( Stream& pStream );
    
    Bool Create( const Image& pImage, Bool pCreateMipmaps = true );
    Bool Create( UInt32 pWidth, Image::Format pFormat );

    Image&  GetImage();
    void    Update();

protected:
    Texture1D() {}

    Image   mImage;
};


class ENGINE_API Texture2D : public Texture
{
    friend class TextureManager;
    DECLARE_ABSTRACT_RESOURCE_CLASS(Texture2D,Texture,GraphicSubsystem);

public:
    virtual ~Texture2D() {}
    virtual void Serialize( Stream& pStream );

    Bool Create( const Image& pImage, Bool pCreateMipmaps = true );
    Bool Create( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat );

    Image&  GetImage();
    void    Update();

protected:
    Texture2D() {} 

    Image   mImage;
};


class ENGINE_API Texture3D : public Texture
{
    friend class TextureManager;
    DECLARE_ABSTRACT_RESOURCE_CLASS(Texture3D,Texture,GraphicSubsystem);

public:
    virtual ~Texture3D() {}
    virtual void Serialize( Stream& pStream );

    Bool Create( const Image& pImage, Bool pCreateMipmaps = true );
    Bool Create( UInt32 pWidth, UInt32 pHeight, UInt32 pDepth, Image::Format pFormat );
       
    Image&  GetImage();
    void    Update();

protected:
    Texture3D() {}

    Image   mImage;
};


class ENGINE_API Cubemap : public Texture
{
    friend class TextureManager;
    DECLARE_ABSTRACT_RESOURCE_CLASS(Cubemap,Texture,GraphicSubsystem);

public:
    enum CubemapFace
    {
        PositiveX,
        NegativeX,
        PositiveY,
        NegativeY,
        PositiveZ,
        NegativeZ,
        NumFaces
    };

public:
    virtual ~Cubemap() {}
    virtual void Serialize( Stream& pStream );

    Bool Create( const Vector<Image*>& pImages, Bool pCreateMipmaps = true );
    Bool Create( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat );

    Image&  GetImage( CubemapFace pFace );
    void    Update();

protected:
    Cubemap() {}   

    Image   mImages[NumFaces];
};


} // namespace Gamedesk


#endif  //  _TEXTURE_H_
