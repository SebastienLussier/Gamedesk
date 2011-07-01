/**
*  @file       VertexList.cpp
*  @brief      Array of vertex.
*  @author     Sébastien Lussier.
*  @date       04/05/05.
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
#include "Graphic/Buffer/VertexList.h"
#include "Graphic/Buffer/TriangleBatch.h"

#if GD_PLATFORM == GD_PLATFORM_WIN32
	#include "Graphic/Tools/Win32/NvTriStrip/NvTriStrip.h"
#endif


namespace Gamedesk {
	
	
VertexList::VertexList() 
    : mVertexCount(0)
    , mVertexFormat(VertexFormat::None)
    , mPositions(NULL)
    , mNormals(NULL)
    , mColors(NULL)
    , mTexCoords(NULL)
    , mTexCoords_2(NULL)
{
}

VertexList::~VertexList()
{
    Free();   
}

void VertexList::Allocate( UInt32 pVertexCount, VertexFormat pVertexFormat )
{
    Free();

    mVertexCount = pVertexCount;
    mVertexFormat  = pVertexFormat;

    if( mVertexFormat.HasComponent( VertexFormat::Position3 ) )
        mPositions = GD_NEW_ARRAY(Vector3f, mVertexCount, this, "Engine::Graphic::VertexList::Positions");

    if( mVertexFormat.HasComponent( VertexFormat::Normal3 ) )
        mNormals = GD_NEW_ARRAY(Vector3f, mVertexCount, this, "Engine::Graphic::VertexList::Normals");

   if( mVertexFormat.HasComponent( VertexFormat::Color4 ) )
        mColors = GD_NEW_ARRAY(Color4f, mVertexCount, this, "Engine::Graphic::VertexList::Colors");

    if( mVertexFormat.HasComponent( VertexFormat::TexCoord2 ) )
        mTexCoords = GD_NEW_ARRAY(Vector2f, mVertexCount, this, "Engine::Graphic::VertexList::TexCoords");

    if( mVertexFormat.HasComponent( VertexFormat::TexCoord2_2 ) )
    {
        GD_ASSERT_M( mVertexFormat.HasComponent(VertexFormat::TexCoord2), "Using second UV set without using the first ??" );
        mTexCoords_2 = GD_NEW_ARRAY(Vector2f, mVertexCount, this, "Engine::Graphic::VertexList::TexCoords");
    }
}

void VertexList::Free()
{
    if( mPositions )
    {
        GD_DELETE_ARRAY(mPositions);
        mPositions = NULL;
    }

    if( mNormals )
    {
        GD_DELETE_ARRAY(mNormals);
        mNormals = NULL;
    }

    if( mColors )
    {
        GD_DELETE_ARRAY(mColors);
        mColors = NULL;
    }

    if( mTexCoords )
    {
        GD_DELETE_ARRAY(mTexCoords);
        mTexCoords = NULL;
    }

    if( mTexCoords_2 )
    {
        GD_DELETE_ARRAY(mTexCoords_2);
        mTexCoords_2 = NULL;
    }

    mVertexCount    = 0;
    mVertexFormat   = VertexFormat::None;
}

UInt32 VertexList::GetVertexCount() const
{ 
    return mVertexCount;
}

VertexFormat VertexList::GetVertexFormat() const
{
    return mVertexFormat;
}

Vector3f* VertexList::GetPositions()
{
    GD_ASSERT(mPositions); 
    return mPositions;
}

Vector3f* VertexList::GetNormals()
{
    GD_ASSERT(mNormals); 
    return mNormals;
}

Color4f* VertexList::GetColors()
{
    GD_ASSERT(mColors); 
    return mColors;
}

Vector2f* VertexList::GetTextureCoords()
{
    GD_ASSERT(mTexCoords); 
    return mTexCoords;
}

Vector2f* VertexList::GetTextureCoords_2()
{
    GD_ASSERT(mTexCoords_2); 
    return mTexCoords_2;
}

void VertexList::Reorder( TriangleBatch& pUnorderedStrip )
{
#if GD_PLATFORM == GD_PLATFORM_WIN32
    Vector3f*   newPositions    = NULL;
    Vector3f*   newNormals      = NULL;
    Color4f*    newColors       = NULL;
    Vector2f*   newTexCoords    = NULL;
    Vector2f*   newTexCoords_2  = NULL;

    PrimitiveGroup  unorderedPrimitiveGroup;
    PrimitiveGroup* orderedPrimitiveGroup;

    unorderedPrimitiveGroup.type        = pUnorderedStrip.GetBatchType() == TriangleBatch::TriangleStrip? PT_STRIP :
    pUnorderedStrip.GetBatchType() == TriangleBatch::TriangleList ? PT_LIST : PT_FAN;

    unorderedPrimitiveGroup.numIndices  = pUnorderedStrip.GetIndicesCount();
    unorderedPrimitiveGroup.indices     = GD_NEW_ARRAY(UInt16, unorderedPrimitiveGroup.numIndices, this, "Engine::Graphic::VertexList");
    memcpy( unorderedPrimitiveGroup.indices, pUnorderedStrip.GetIndices(), sizeof(UInt16)*unorderedPrimitiveGroup.numIndices );

    // Remap!
    RemapIndices( &unorderedPrimitiveGroup, 1, mVertexCount, &orderedPrimitiveGroup );

    UInt16*     oldIndices = unorderedPrimitiveGroup.indices;
    UInt16*     newIndices = orderedPrimitiveGroup->indices;

    if( mPositions )
        newPositions = GD_NEW_ARRAY(Vector3f, mVertexCount, this, "Engine::Graphic::VertexList::Positions");

    if( mNormals )
        newNormals = GD_NEW_ARRAY(Vector3f, mVertexCount, this, "Engine::Graphic::VertexList::Normals");

    if( mColors )
        newColors = GD_NEW_ARRAY(Color4f, mVertexCount, this, "Engine::Graphic::VertexList::Colors");

    if( mTexCoords )
        newTexCoords = GD_NEW_ARRAY(Vector2f, mVertexCount, this, "Engine::Graphic::VertexList::TexCoords");

    if( mTexCoords_2 )
        newTexCoords_2 = GD_NEW_ARRAY(Vector2f, mVertexCount, this, "Engine::Graphic::VertexList::TexCoords");

    for( UInt32 iIndex = 0; iIndex < orderedPrimitiveGroup->numIndices; iIndex++ )
    {
        // Grab old index.
        UInt16 oldIndex = oldIndices[iIndex];

        // Grab new index.
        UInt16 newIndex = newIndices[iIndex];

        if( mPositions )
            newPositions[newIndex] = mPositions[oldIndex];

        if( mNormals )
            newNormals[newIndex] = mNormals[oldIndex];

        if( mColors )
            newColors[newIndex] = mColors[oldIndex];

        if( mTexCoords )
            newTexCoords[newIndex] = mTexCoords[oldIndex];

        if( mTexCoords_2 )
            newTexCoords_2[newIndex] = mTexCoords_2[oldIndex];
    }

    if( mPositions )
    {
        GD_DELETE_ARRAY(mPositions);
        mPositions = newPositions;
    }

    if( mNormals )
    {
        GD_DELETE_ARRAY(mNormals);
        mNormals = newNormals;
    }

    if( mColors )
    {
        GD_DELETE_ARRAY(mColors);
        mColors = newColors;
    }

    if( mTexCoords )
    {
        GD_DELETE_ARRAY(mTexCoords);
        mTexCoords = newTexCoords;
    }

    if( mTexCoords_2 )
    {
        GD_DELETE_ARRAY(mTexCoords_2);
        mTexCoords_2 = newTexCoords_2;
    }

    // Return the ordered triangle strip.
    memcpy( pUnorderedStrip.GetIndices(), orderedPrimitiveGroup->indices, orderedPrimitiveGroup->numIndices*sizeof(UInt16) );

    //    GD_DELETE(orderedPrimitiveGroup);
#else
	debugBreak();
#endif
}


} // namespace Gamedesk
