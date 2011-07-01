/**
 *  @file       TextureHdl.h
 *  @brief	    Managed handle to a texture.
 *  @author     Sébastien Lussier.
 *  @date       29/01/04.
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
#ifndef     _TEXTURE_HDL_H_
#define     _TEXTURE_HDL_H_


#include "TextureManager.h"


namespace Gamedesk {


class ENGINE_API HTexture1D
{
public:
    HTexture1D() : mTextureFile(), mTexture(NULL)
    {
    }

    HTexture1D( const String& pTextureFile ) : mTexture(NULL)
    {
        GetTexture( pTextureFile );
    }    

    HTexture1D( const HTexture1D& pOther ) : mTexture(NULL)
    {
        if( pOther )
            GetTexture( pOther.mTextureFile );
    }

    ~HTexture1D()
    {
        if( mTexture )
            TextureManager::Instance().Release( mTextureFile );
    }

    HTexture1D& operator = ( const HTexture1D& pOther )
    {
        if( *this == pOther || mTexture == pOther.mTexture )
            return *this;

        GetTexture( pOther.mTextureFile );
        return *this;
    }

    Bool operator == ( const HTexture1D& pOther ) const
    {
        return mTexture == pOther.mTexture;
    }

    Bool operator != ( const HTexture1D& pOther ) const
    {
        return mTexture != pOther.mTexture;
    }

    Texture1D* operator -> () const
    {
        GD_ASSERT( mTexture != NULL );
        return mTexture;
    }

    Texture1D& operator * () const
    {
        GD_ASSERT( mTexture != NULL );
        return *mTexture;
    }

    operator Bool () const
    {
        return mTexture != NULL;
    }

    friend Stream& operator << ( Stream& pStream, HTexture1D& pTexture1D )
    {
        pStream << pTexture1D.mTextureFile;

        if( pStream.In() )
            pTexture1D.GetTexture(pTexture1D.mTextureFile);

        return pStream;
    }

    void GetTexture( const String& pTextureFile )
    {
        try
        {
            if( mTexture )
            {
                TextureManager::Instance().Release( mTextureFile );
                mTexture = NULL;
            }

            mTextureFile = pTextureFile;
            mTexture = TextureManager::Instance().CreateTexture1D( mTextureFile );
        }
        catch( Exception& /*e*/ )
        {
            mTexture = NULL;
        } 
    }

private:
    String      mTextureFile;
    Texture1D*  mTexture;
};


class ENGINE_API HTexture2D
{
public:
    HTexture2D() : mTextureFile(), mTexture(NULL)
    {
    }

    HTexture2D( const String& pTextureFile ) : mTexture(NULL)
    {
        GetTexture( pTextureFile );
    }    

    HTexture2D( const HTexture2D& pOther ) : mTexture(NULL)
    {
        if( pOther )
            GetTexture( pOther.mTextureFile );
    }

    ~HTexture2D()
    {
        if( mTexture )
            TextureManager::Instance().Release( mTextureFile );
    }

    HTexture2D& operator = ( const HTexture2D& pOther )
    {
        if( *this == pOther || mTexture == pOther.mTexture )
            return *this;

        GetTexture( pOther.mTextureFile );
        return *this;
    }

    Bool operator == ( const HTexture2D& pOther ) const
    {
        return mTexture == pOther.mTexture;
    }

    Bool operator != ( const HTexture2D& pOther ) const
    {
        return mTexture != pOther.mTexture;
    }

    Texture2D* operator -> () const
    {
        GD_ASSERT( mTexture != NULL );
        return mTexture;
    }

    Texture2D& operator * () const
    {
        GD_ASSERT( mTexture != NULL );
        return *mTexture;
    }

    operator Bool () const
    {
        return mTexture != NULL;
    }

    friend Stream& operator << ( Stream& pStream, HTexture2D& pTexture2D )
    {
        pStream << pTexture2D.mTextureFile;

        if( pStream.In() )
            pTexture2D.GetTexture(pTexture2D.mTextureFile);

        return pStream;
    }

    void GetTexture( const String& pTextureFile )
    {
        try
        {
            if( mTexture )
            {
                TextureManager::Instance().Release( mTextureFile );
                mTexture = NULL;
            }

            mTextureFile = pTextureFile;
            mTexture = TextureManager::Instance().CreateTexture2D( mTextureFile );
        }
        catch( Exception& /*e*/ )
        {
            mTexture = NULL;
        } 
    }

private:
    String      mTextureFile;
    Texture2D*  mTexture;
};


class ENGINE_API HTexture3D
{
public:
    HTexture3D() : mTextureFile(), mTexture(NULL)
    {
    }

    HTexture3D( const String& pTextureFile ) : mTexture(NULL)
    {
        GetTexture( pTextureFile );
    }    

    HTexture3D( const HTexture3D& pOther ) : mTexture(NULL)
    {
        if( pOther )
            GetTexture( pOther.mTextureFile );
    }

    ~HTexture3D()
    {
        if( mTexture )
            TextureManager::Instance().Release( mTextureFile );
    }

    HTexture3D& operator = ( const HTexture3D& pOther )
    {
        if( *this == pOther || mTexture == pOther.mTexture )
            return *this;

        GetTexture( pOther.mTextureFile );
        return *this;
    }

    Bool operator == ( const HTexture3D& pOther ) const
    {
        return mTexture == pOther.mTexture;
    }

    Bool operator != ( const HTexture3D& pOther ) const
    {
        return mTexture != pOther.mTexture;
    }

    Texture3D* operator -> () const
    {
        GD_ASSERT( mTexture != NULL );
        return mTexture;
    }

    Texture3D& operator * () const
    {
        GD_ASSERT( mTexture != NULL );
        return *mTexture;
    }

    operator Bool () const
    {
        return mTexture != NULL;
    }

    friend Stream& operator << ( Stream& pStream, HTexture3D& pTexture3D )
    {
        pStream << pTexture3D.mTextureFile;

        if( pStream.In() )
            pTexture3D.GetTexture(pTexture3D.mTextureFile);

        return pStream;
    }

    void GetTexture( const String& pTextureFile )
    {
        try
        {
            if( mTexture )
            {
                TextureManager::Instance().Release( mTextureFile );
                mTexture = NULL;
            }

            mTextureFile = pTextureFile;
            mTexture = TextureManager::Instance().CreateTexture3D( mTextureFile );
        }
        catch( Exception& /*e*/ )
        {
            mTexture = NULL;
        } 
    }

private:
    String      mTextureFile;
    Texture3D*  mTexture;
};


class ENGINE_API HCubemap
{
public:
    HCubemap() : mTextureFile(), mTexture(NULL)
    {
    }

    HCubemap( const String& pTextureFile ) : mTexture(NULL)
    {
        GetTexture( pTextureFile );
    }    

    HCubemap( const HCubemap& pOther ) : mTexture(NULL)
    {
        if( pOther )
            GetTexture( pOther.mTextureFile );
    }

    ~HCubemap()
    {
        if( mTexture )
            TextureManager::Instance().Release( mTextureFile );
    }

    HCubemap& operator = ( const HCubemap& pOther )
    {
        if( *this == pOther || mTexture == pOther.mTexture )
            return *this;

        GetTexture( pOther.mTextureFile );
        return *this;
    }

    Bool operator == ( const HCubemap& pOther ) const
    {
        return mTexture == pOther.mTexture;
    }

    Bool operator != ( const HCubemap& pOther ) const
    {
        return mTexture != pOther.mTexture;
    }

    Cubemap* operator -> () const
    {
        GD_ASSERT( mTexture != NULL );
        return mTexture;
    }

    Cubemap& operator * () const
    {
        GD_ASSERT( mTexture != NULL );
        return *mTexture;
    }

    operator Bool () const
    {
        return mTexture != NULL;
    }

    friend Stream& operator << ( Stream& pStream, HCubemap& pCubemap )
    {
        pStream << pCubemap.mTextureFile;

        if( pStream.In() )
            pCubemap.GetTexture(pCubemap.mTextureFile);

        return pStream;
    }

    void GetTexture( const String& pTextureFile )
    {
        try
        {
            if( mTexture )
            {
                TextureManager::Instance().Release( mTextureFile );
                mTexture = NULL;
            }

            mTextureFile = pTextureFile;
            mTexture = TextureManager::Instance().CreateCubemap( mTextureFile );
        }
        catch( Exception& /*e*/ )
        {
            mTexture = NULL;
        } 
    }

private:
    String      mTextureFile;
    Cubemap*    mTexture;
};


} // namespace Gamedesk


#endif  //  _TEXTURE_HDL_H_
