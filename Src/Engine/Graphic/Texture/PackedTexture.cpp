/**
 *  @file       PackedTexture.cpp
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
#include "Engine.h"
#include "Graphic/Texture/PackedTexture.h"

#include "Graphic/Texture/Texture.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
PackedTexture::PackedTexture( UInt32 pSize, Image::Format pFormat )
    : mTexSize(pSize)
    , mTexFormat(pFormat)
{
}

PackedTexture::~PackedTexture()
{
    ClearAll();
}

void PackedTexture::ClearAll()
{
    for( UInt32 i = 0; i < mTexTrees.size(); i++ )
    {
        GD_DELETE(mTexTrees[i]);
    }
    mTexTrees.clear();

    for( UInt32 i = 0; i < mImages.size(); i++ )
    {
        GD_DELETE(mImages[i]);
    }
    mImages.clear();

    for( UInt32 i = 0; i < mTextures.size(); i++ )
    {
        GD_DELETE(mTextures[i]);
    }
    mTextures.clear();
}

void PackedTexture::BeginPacking()
{
    ClearAll();
}

void PackedTexture::EndPacking()
{
    // Trees are now useless.
    for( UInt32 i = 0; i < mTexTrees.size(); i++ )
    {
        GD_DELETE(mTexTrees[i]);
    }
    mTexTrees.clear();

    mTextures.resize( mImages.size() );

    // Create a texture with each images.
    for( UInt32 i = 0; i < mTextures.size(); i++ )
    {
        mTextures[i] = Cast<Texture2D>( GraphicSubsystem::Instance()->Create( Texture2D::StaticClass() ) );
        mTextures[i]->Create( *mImages[i], false );
        mTextures[i]->Init();        
                
        mTextures[i]->SetWrapMode( Texture::Wrap_S, Texture::Wrap_Clamp );
        mTextures[i]->SetWrapMode( Texture::Wrap_T, Texture::Wrap_Clamp );
        mTextures[i]->SetMinFilter( Texture::MinFilter_Linear );
        mTextures[i]->SetMagFilter( Texture::MagFilter_Linear );
        
        // Images are not needed anymore.
        GD_DELETE(mImages[i]);
    }

    mImages.clear();
}

Bool PackedTexture::InsertImage( const Image& pImg, PackedTexture::Info& info )
{
    // Validate image size
    if( pImg.GetWidth() > mTexSize || pImg.GetHeight() > mTexSize )
        return false;

    // Copy to one of the existing image
    TextureTree::Node* node     = NULL;
    UInt32             nodeTree = 0;

    for( UInt32 i = 0; i < mTexTrees.size(); i++ )
    {
        node = mTexTrees[i]->InsertImage( pImg );
        nodeTree = i;
    }
    
    // No space found, create a new tree
    if( !node )
    {
        mTexTrees.push_back( GD_NEW(TextureTree, this, "Engine::Graphic::Texture::PackedTexture")(mTexSize) );
        mImages.push_back( GD_NEW(Image, this, "Engine::Graphic::Texture::PackedTexture") );

        node = mTexTrees[mTexTrees.size()-1]->InsertImage( pImg );
        nodeTree = mTexTrees.size()-1;

        mImages[mImages.size()-1]->Create( mTexSize, mTexSize, mTexFormat );
    }

    // A node must have been found.
    GD_ASSERT( node );
    
    // Copy texture there
    if( node )
    {
        mImages[nodeTree]->Copy( pImg, node->mRect.mLeft, node->mRect.mTop );

        // Copy values back into info structure.
        info = PackedTexture::Info( nodeTree, node->mRect.mLeft, node->mRect.mTop );
    }

    return node != NULL;
}

Texture2D& PackedTexture::GetTexture( UInt32 pTexture )
{
    GD_ASSERT( pTexture < mTextures.size() );
    GD_ASSERT( mTextures[pTexture] );
    return *mTextures[pTexture];
}

PackedTexture::TextureTree::Node::Node()
{
    mChilds[0] = NULL;
    mChilds[1] = NULL;
    mInUse     = 0;
}

PackedTexture::TextureTree::Node::~Node()
{
    if( mChilds[0] )
        GD_DELETE(mChilds[0]);

    if( mChilds[1] )
        GD_DELETE(mChilds[1]);
}

PackedTexture::TextureTree::Node* PackedTexture::TextureTree::Node::InsertImage( const Image& pImg )
{
    Node* result = NULL;

    // We're in a full node/leaf, return immediately.
    if( mInUse )
        return NULL;

    // If we're not a leaf, try inserting in childs
    if( mChilds[0] )
    {
        result = mChilds[0]->InsertImage( pImg );
        
        if( !result )
            result = mChilds[1]->InsertImage( pImg );

        mInUse = mChilds[0]->mInUse && mChilds[1]->mInUse;
        return result;
    }

    Int32 diffW = (mRect.mRight-mRect.mLeft+1) - pImg.GetWidth();
    Int32 diffH = (mRect.mBottom-mRect.mTop+1) - pImg.GetHeight();
    
    // If we're too small, return.
    if( diffW < 0 || diffH < 0 )
        return NULL;

    // Perfect match !
    if( diffW == 0 && diffH == 0 )
    {
        mInUse = true;
        return this;
    }

    // Otherwise, gotta split this node and create some kids.
    mChilds[0] = GD_NEW(Node, this, "Engine::Graphic::Texture::PackedTexture");
    mChilds[1] = GD_NEW(Node, this, "Engine::Graphic::Texture::PackedTexture");
    
    if( diffW > diffH )
    {
        mChilds[0]->mRect = Rect( mRect.mLeft, mRect.mTop, mRect.mLeft + pImg.GetWidth() - 1, mRect.mBottom );
        mChilds[1]->mRect = Rect( mRect.mLeft + pImg.GetWidth(), mRect.mTop, mRect.mRight, mRect.mBottom );
    }
    else
    {
        mChilds[0]->mRect = Rect( mRect.mLeft, mRect.mTop, mRect.mRight, mRect.mTop + pImg.GetHeight() - 1 );
        mChilds[1]->mRect = Rect( mRect.mLeft, mRect.mTop + pImg.GetHeight(), mRect.mRight, mRect.mBottom );
    }
    
    // Insert into first child we created
    return mChilds[0]->InsertImage( pImg );
}

PackedTexture::TextureTree::TextureTree( UInt32 pSize )
{
    mRoot.mRect.mLeft = 0;
    mRoot.mRect.mRight = pSize-1;
    mRoot.mRect.mTop = 0;
    mRoot.mRect.mBottom = pSize-1;
}

PackedTexture::TextureTree::Node* PackedTexture::TextureTree::InsertImage( const Image& pImg )
{
    return mRoot.InsertImage( pImg );
}


} // namespace Gamedesk
