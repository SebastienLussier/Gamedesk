/**
 *  @file       BSPImporter.cpp
 *  @brief      Doom 3 BSP mesh/anim importer.
 *  @author     Sébastien Lussier.
 *  @date       12/08/04.
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
#include "BSPImporterDll.h"
#include "BSPImporter.h"

#include "BSPFile.h"
#include "BSPReader.h"

#include "Containers/Bitset.h"

#include "World/BSP.h"
#include "FileManager/FileManager.h"
#include "Debug/PerformanceMonitor.h"


IMPLEMENT_CLASS(BSPImporter);


#define SIZE_RATIO 1


BSPImporter::BSPImporter()
{
    AddSupportedType( "bsp" );
}

Class* BSPImporter::GetResourceClass()
{
    return Bsp::StaticClass();
}

Resource* BSPImporter::Import( const String& pFilename, const String& /*pParams*/ )
{
    BSP::BSPFile    bspFile;
    BSP::BSPReader  bspReader( bspFile );

    bspReader.Read( pFilename );

    Bsp* pBSP = CreateBSP( bspFile );

    return pBSP;
}

Bsp* BSPImporter::CreateBSP( BSP::BSPFile& bspFile )
{
    Bsp* bsp = GD_NEW(Bsp, this, "BSPImporter::Bsp");
	BSP::BSPLump* lump;
	
	// Add vertices
	lump = &bspFile.mHeader->mLump[BSP::LUMP_Vertices];
	bsp->mVertices.resize( lump->mLength / sizeof(Vector3f) );
	for( UInt32 i = 0; i < bsp->mVertices.size(); i++ )
	{
		bsp->mVertices[i].x = ((Float*)(&bspFile.mData[lump->mOffset]))[3*i+0] / SIZE_RATIO;
		bsp->mVertices[i].y = ((Float*)(&bspFile.mData[lump->mOffset]))[3*i+2] / SIZE_RATIO;
		bsp->mVertices[i].z =-((Float*)(&bspFile.mData[lump->mOffset]))[3*i+1] / SIZE_RATIO;
	}

	// Add edges
	lump = &bspFile.mHeader->mLump[BSP::LUMP_Edges];
	bsp->mEdges.resize( lump->mLength / (2*sizeof(UInt16)) );
	for( UInt32 i = 0; i < bsp->mEdges.size(); i++ )
	{
		bsp->mEdges[i].mPoints[0] = ((UInt16*)&bspFile.mData[lump->mOffset])[2*i+0];
		bsp->mEdges[i].mPoints[1] = ((UInt16*)&bspFile.mData[lump->mOffset])[2*i+1];
	}

	// Add faces edge
	lump = &bspFile.mHeader->mLump[BSP::LUMP_FaceEdgeTable];
	bsp->mFacesEdge.resize( lump->mLength / sizeof(Int32) );
	for( UInt32 i = 0; i < bsp->mFacesEdge.size(); i++ )
	{
		bsp->mFacesEdge[i] = ((Int32*)&bspFile.mData[lump->mOffset])[i];
	}

	// Add nodes
	lump = &bspFile.mHeader->mLump[BSP::LUMP_Nodes];
	bsp->mNodes.resize( lump->mLength / sizeof(BSP::BSPNode) );
	for( UInt32 i = 0; i < bsp->mNodes.size(); i++ )
	{
		BSP::BSPNode* node = &((BSP::BSPNode*)&bspFile.mData[lump->mOffset])[i];

		bsp->mNodes[i].mPlaneIndex		= node->mPlane;
		bsp->mNodes[i].mFrontChild		= node->mFrontChild;
		bsp->mNodes[i].mBackChild		= node->mBackChild;
		bsp->mNodes[i].mBBox			= BoundingBox( Vector3f(node->mBBoxMin.x/SIZE_RATIO, node->mBBoxMin.z/SIZE_RATIO, -node->mBBoxMin.y/SIZE_RATIO), 
                                                       Vector3f(node->mBBoxMax.x/SIZE_RATIO, node->mBBoxMax.z/SIZE_RATIO, -node->mBBoxMax.y/SIZE_RATIO) );
		bsp->mNodes[i].mFirstFaceIndex  = node->mFirstFace;
		bsp->mNodes[i].mNumFaces		= node->mNumFaces; 
	}
    
	// Add leaves
	lump = &bspFile.mHeader->mLump[BSP::LUMP_Leaves];
	bsp->mLeaves.resize( lump->mLength / sizeof(BSP::BSPLeaf) );
	for( UInt32 i = 0; i < bsp->mLeaves.size(); i++ )
	{
		BSP::BSPLeaf* leaf = &((BSP::BSPLeaf*)&bspFile.mData[lump->mOffset])[i];

		bsp->mLeaves[i].mBBox			= BoundingBox( Vector3f(leaf->mBBoxMin.x/SIZE_RATIO, leaf->mBBoxMin.z/SIZE_RATIO, -leaf->mBBoxMin.y/SIZE_RATIO), 
                                                       Vector3f(leaf->mBBoxMax.x/SIZE_RATIO, leaf->mBBoxMax.z/SIZE_RATIO, -leaf->mBBoxMax.y/SIZE_RATIO) );
		bsp->mLeaves[i].mFirstFaceIndex = leaf->mFirstLeafFace;
		bsp->mLeaves[i].mNumFaces	    = leaf->mNumLeafFaces;
        bsp->mLeaves[i].mCluster        = leaf->mCluster;
	}

	// Add faces
	lump = &bspFile.mHeader->mLump[BSP::LUMP_Faces];
	bsp->mFaces.resize( lump->mLength / sizeof(BSP::BSPFace) );
	for( UInt32 i = 0; i < bsp->mFaces.size(); i++ )
	{
		BSP::BSPFace* face = &((BSP::BSPFace*)&bspFile.mData[lump->mOffset])[i];

   		bsp->mFaces[i].mPlaneIndex		    = face->mPlane;
        bsp->mFaces[i].mPlaneSide           = face->mPlaneSide == 0;
		bsp->mFaces[i].mFirstEdgeIndex      = face->mFirstEdge;
		bsp->mFaces[i].mNumEdges		    = face->mNumEdges;
        bsp->mFaces[i].mTextureInfo         = face->mTextureInfo;
        bsp->mFaces[i].mLightmapStyles[0]   = face->mLightmapStyles[0];
        bsp->mFaces[i].mLightmapStyles[1]   = face->mLightmapStyles[1];
        bsp->mFaces[i].mLightmapStyles[2]   = face->mLightmapStyles[2];
        bsp->mFaces[i].mLightmapStyles[3]   = face->mLightmapStyles[3];
        bsp->mFaces[i].mLightmapOffset      = face->mLightmapOffset;
	}

	// Add leaves faces
	lump = &bspFile.mHeader->mLump[BSP::LUMP_LeafFaceTable];
	bsp->mLeafFaces.resize( lump->mLength / sizeof(UInt16) );
	for( UInt32 i = 0; i < bsp->mLeafFaces.size(); i++ )
		bsp->mLeafFaces[i] = ((UInt16*)&bspFile.mData[lump->mOffset])[i];

	// Add planes
	lump = &bspFile.mHeader->mLump[BSP::LUMP_Planes];
	bsp->mPlanes.resize( lump->mLength / sizeof(BSP::BSPPlane) );
	for( UInt32 i = 0; i < bsp->mPlanes.size(); i++ )
	{
		BSP::BSPPlane* plane = &((BSP::BSPPlane*)&bspFile.mData[lump->mOffset])[i];
	
		bsp->mPlanes[i].SetNormal( Vector3f(plane->mNormal.x, plane->mNormal.z, -plane->mNormal.y) );
		bsp->mPlanes[i].SetConstant( plane->mDistance / SIZE_RATIO );
	}

    // Add clusters
    lump = &bspFile.mHeader->mLump[BSP::LUMP_Visibility];
    bsp->mClusters.resize( ((UInt32*)&bspFile.mData[lump->mOffset])[0] );
	for( UInt32 i = 0; i < bsp->mClusters.size(); i++ )
	{
		UInt32 pvsOffset = ((UInt32*)&bspFile.mData[lump->mOffset])[2*i+1];
        //UInt32 phsOffset = ((UInt32*)&bspFile.mData[lump->mOffset])[2*i+2];
	
        bsp->mClusters[i].mPVS.SetSize( bsp->mClusters.size() );
        DecompressVisibility( bsp->mClusters[i].mPVS, &bspFile.mData[lump->mOffset + pvsOffset] );
	}

    // Add texture info
    lump = &bspFile.mHeader->mLump[BSP::LUMP_TextureInformation];
    bsp->mTextureInfo.resize( lump->mLength / sizeof(BSP::BSPTexInfo) );
    for( UInt32 i = 0; i < bsp->mTextureInfo.size(); i++ )
	{
        BSP::BSPTexInfo* texInfo = &((BSP::BSPTexInfo*)&bspFile.mData[lump->mOffset])[i];
	
        bsp->mTextureInfo[i].mAxisU         = Vector3f(texInfo->mAxisU.x, texInfo->mAxisU.z, -texInfo->mAxisU.y);
        bsp->mTextureInfo[i].mOffsetU       = texInfo->mOffsetU;
        bsp->mTextureInfo[i].mAxisV         = Vector3f(texInfo->mAxisV.x, texInfo->mAxisV.z, -texInfo->mAxisV.y);
        bsp->mTextureInfo[i].mOffsetV       = texInfo->mOffsetV;
        bsp->mTextureInfo[i].mFlags         = texInfo->mFlags;
        bsp->mTextureInfo[i].mValue         = texInfo->mValue;
        bsp->mTextureInfo[i].mNextTexInfo   = texInfo->mNextTexInfo;

        bsp->mTextureInfo[i].mTexture.GetTexture( String("Data/Quake2/textures/") + texInfo->mTextureName + String( ".wal") );
    }	

    // Lightmap data
    lump = &bspFile.mHeader->mLump[BSP::LUMP_Lightmaps];
    bsp->mLightmapData.resize( lump->mLength );
    memcpy( &bsp->mLightmapData[0], &bspFile.mData[lump->mOffset], lump->mLength );

    return bsp;
}

void BSPImporter::DecompressVisibility( Bitset& pClusterPVS, Byte* pIn )
{
	Int32	numZeros;
	Byte*   decompressedBuf = *pClusterPVS;   // Get internal array 
    Byte*   out = decompressedBuf;
	Int32	numBytes = (pClusterPVS.GetSize()+7) >> 3;

    if( !pIn )
	{	
        // No visibility info, so make all visible
		pClusterPVS.SetAllBits();
        return;
	}

    // Decompress the RLE encoded PVS info.
	do
	{
		if( *pIn )
		{
			*out++ = *pIn++;
			continue;
		}
    
		numZeros = pIn[1];
		pIn += 2;

		while( numZeros > 0 )
		{
			*out++ = 0;
			numZeros--;
		}
	} while (out - decompressedBuf < numBytes);
}