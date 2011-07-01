/**
 *  @file       PackedTexture.h
 *  @brief	    Pack some images into a bigger one to minimize lost space and texture changes.
 *  @author     Sébastien Lussier.
 *  @date       07/03/05.
 */
/*
 *  Copyright (C) 2005 Gamedesk
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
#ifndef     _PACKED_TEXTURE_H_
#define     _PACKED_TEXTURE_H_


namespace Gamedesk
{
	class Texture2D;
}


#include "Graphic/Image/Image.h"


namespace Gamedesk {


class ENGINE_API PackedTexture
{
public:
    class Info
    {
    public:
        Info()
            : mValid(false)
        {
        }

        Info( UInt32 pTextureIndex, UInt32 pOffsetU, UInt32 pOffsetV )
            : mTextureIndex(pTextureIndex)
            , mOffsetU(pOffsetU)
            , mOffsetV(pOffsetV)
            , mValid(true)
        {
        }

        UInt32 mTextureIndex;
        UInt32 mOffsetU;
        UInt32 mOffsetV;
        Byte   mValid;
    };

public:
    PackedTexture( UInt32 pSize, Image::Format pFormat );
    ~PackedTexture();

    void ClearAll();

    void BeginPacking();
    void EndPacking();

    Bool InsertImage( const Image& pImg, PackedTexture::Info& info );

    Texture2D& GetTexture( UInt32 pTexture );

protected:
    class TextureTree
    {   
    public:
        class Rect
        {
        public:
            Rect()
            {
            }
            Rect( UInt32 pLeft, UInt32 pTop, UInt32 pRight, UInt32 pBottom )
                : mLeft(pLeft)
                , mRight(pRight)
                , mTop(pTop)
                , mBottom(pBottom)
            {
            }

            UInt32  mLeft;
            UInt32  mRight;
            UInt32  mTop;
            UInt32  mBottom;
        };
        
        class Node
        {
        public:
            Node();
            ~Node();

            Node* InsertImage( const Image& pImg );

        public:
            Node*       mChilds[2];
            Rect        mRect;
            Byte        mInUse;
        };

    public:
        TextureTree( UInt32 pSize );
        Node* InsertImage( const Image& pImg );

    private:
        Node    mRoot;
    };

private:
    Vector<Image*>          mImages;
    Vector<Texture2D*>      mTextures;
    Vector<TextureTree*>    mTexTrees;

    UInt32                  mTexSize;
    Image::Format           mTexFormat;
};


} // namespace Gamedesk


#endif  //  _PACKED_TEXTURE_H_
