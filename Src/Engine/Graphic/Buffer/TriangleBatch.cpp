/**
*  @file       TriangleBatch.h
*  @brief      Group of triangle (list, fan or strip).
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
#include "Graphic/Buffer/TriangleBatch.h"

#if GD_PLATFORM == GD_PLATFORM_WIN32
	#include "Graphic/Tools/Win32/NvTriStrip/NvTriStrip.h"
#endif


namespace Gamedesk {
	
	
TriangleBatch::TriangleBatch() 
    : mIndicesCount(0)
	, mIndices(NULL)
    , mType(TriangleList)
{
}

TriangleBatch::~TriangleBatch()
{
    Free();
}

TriangleBatch::TriangleBatch( const TriangleBatch& pOther )
{
    *this = pOther;
}

const TriangleBatch& TriangleBatch::operator = ( const TriangleBatch& pOther )
{
    Allocate( pOther.mType, pOther.mIndicesCount );
    memcpy( mIndices, pOther.mIndices, mIndicesCount*sizeof(UInt16) );
    return *this;
}

void TriangleBatch::Allocate( BatchType pType, UInt32 pIndexCount )
{
    Free();

    mType = pType;
    mIndicesCount = pIndexCount;
    mIndices = GD_NEW_ARRAY(UInt16, mIndicesCount, this, "Engine::Graphic::TriangleBatch");
}

void TriangleBatch::Free()
{
    if( mIndices )
    {
        GD_DELETE_ARRAY(mIndices);
        mIndices = NULL;
    }

    mIndicesCount = 0;
}

UInt32 TriangleBatch::GetIndicesCount() const
{
    return mIndicesCount;
}

UInt16* TriangleBatch::GetIndices()
{
    return mIndices;
}

TriangleBatch::BatchType TriangleBatch::GetBatchType() const
{
    return mType;
}

void TriangleBatch::Stripify( TriangleBatch& pNewBatch )
{
#if GD_PLATFORM == GD_PLATFORM_WIN32
	GD_ASSERT( mIndices );

    PrimitiveGroup* strip;
    UInt16          numGroups;

    SetCacheSize(CACHESIZE_GEFORCE1_2);
    SetStitchStrips(true);
    SetMinStripSize(0);
    SetListsOnly(false);

    // Stripify!
    GenerateStrips( mIndices, mIndicesCount, &strip, &numGroups );

    GD_ASSERT( numGroups == 1 );

    // Copy the result in our triangle batch.
    pNewBatch.Allocate( TriangleStrip, strip->numIndices );
    memcpy( pNewBatch.GetIndices(), strip->indices, strip->numIndices*sizeof(UInt16) ); 

    //GD_DELETE_ARRAY(strip);
#else
	debugBreak();
#endif
}


} // namespace Gamedesk
