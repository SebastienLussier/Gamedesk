/**
 *  @file       BSPFile.h
 *  @brief      Defines data structures to store BSP file info.
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
#ifndef     _BSP_FILE_H_
#define     _BSP_FILE_H_


#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Maths/Quaternion.h"
#include "Maths/BoundingBox.h"
#include "Maths/Plane3.h"
#include "Graphic/Color4.h"


namespace BSP
{

enum LumpType
{
	LUMP_Entities,				// MAP entity text buffer	
	LUMP_Planes,                // Plane array	
	LUMP_Vertices,              // Vertex array	
	LUMP_Visibility,            // Compressed PVS data and directory for all clusters	
	LUMP_Nodes,                 // Internal node array for the BSP tree	
	LUMP_TextureInformation,    // Face texture application array	
	LUMP_Faces,                 // Face array	
	LUMP_Lightmaps,             // Lightmaps	
	LUMP_Leaves,                // Internal leaf array of the BSP tree	
	LUMP_LeafFaceTable,         // Index lookup table for referencing the face array from a leaf	
	LUMP_LeafBrushTable,        // ?	
	LUMP_Edges,                 // Edge array	
	LUMP_FaceEdgeTable,         // Index lookup table for referencing the edge array from a face	
	LUMP_Models,                // ?	
	LUMP_Brushes,               // ?	
	LUMP_BrushSides,            // ?	
	LUMP_Pop,                   // ?	
	LUMP_Areas,                 // ?       	
	LUMP_AreaPortals,           // ?
	LUMP_NumTypes
};

struct BSPLump
{   
	UInt32		mOffset;			// offset (in bytes) of the data from the beginning of the file
    UInt32		mLength;			// length (in bytes) of the data
};

struct BSPHeader
{
	UInt32		mMagic;			// magic number ("IBSP")
    UInt32		mVersion;			// version of the BSP format (38)

    BSPLump		mLump[19];			// directory of the lumps
};

struct BSPFace
{
	UInt16		mPlane;             // index of the plane the face is parallel to
    UInt16		mPlaneSide;        // set if the normal is parallel to the plane normal

    UInt32		mFirstEdge;        // index of the first edge (in the face edge array)
    UInt16		mNumEdges;         // number of consecutive edges (in the face edge array)
	
    UInt16		mTextureInfo;      // index of the texture info structure	
   
    Byte		mLightmapStyles[4]; // styles (bit flags) for the lightmaps
    UInt32		mLightmapOffset;   // offset of the lightmap (in bytes) in the lightmap lump
};

struct BSPPlane
{    
	Vector3f	mNormal;			// A, B, C components of the plane equation
    Float		mDistance;			// D component of the plane equation
    UInt32		mType;				// ?
};

struct BSPNode
{   
	UInt32		mPlane;             // index of the splitting plane (in the plane array)
    
    Int32		mFrontChild;       // index of the front child node or leaf
    Int32		mBackChild;        // index of the back child node or leaf
   
    Vector3<Int16>	mBBoxMin;         // minimum x, y and z of the bounding box
    Vector3<Int16>	mBBoxMax;         // maximum x, y and z of the bounding box
	
    UInt16		mFirstFace;        // index of the first face (in the face array)
    UInt16		mNumFaces;         // number of consecutive edges (in the face array)
};

struct BSPLeaf
{   
    UInt32		mBrushOr;          // ?
	
    UInt16		mCluster;           // -1 for cluster indicates no visibility information
    UInt16		mArea;              // ?

    Vector3<Int16>	mBBoxMin;         // bounding box minimums
    Vector3<Int16>	mBBoxMax;         // bounding box maximums

    UInt16		mFirstLeafFace;   // index of the first face (in the face leaf array)
    UInt16		mNumLeafFaces;    // number of consecutive edges (in the face leaf array)

    UInt16		mFirstLeafBrush;  // ?
    UInt16		mNumLeafBrushes;  // ?
};

struct BSPTexInfo
{    
	Vector3f	mAxisU;
    Float		mOffsetU;
   
    Vector3f	mAxisV;
    Float		mOffsetV;    

	UInt32		mFlags;
    UInt32		mValue;    
	char		mTextureName[32];    
	UInt32		mNextTexInfo;
};

struct BSPVisOffset
{	
	UInt32		mPVS;					// offset (in bytes) from the beginning of the visibility lump
	UInt32		mPHS;					// ?
};

struct WALHeader
{    
	char		mName[32];			// name of the texture
 
    UInt32		mWidth;				// width (in pixels) of the largest mipmap level
    UInt32		mHeight;				// height (in pixels) of the largest mipmap level
 
    Int32		mOffset[4];			// Byte offset of the start of each of the 4 mipmap levels

    char		mNextName[32];		// name of the next texture in the animation

    UInt32		mFlags;				// ?
    UInt32		mContents;			// ?
    UInt32		mValue;				// ?
};

class BSPFile
{
	friend class BSPReader;

public:
	BSPFile()
		: mHeader(NULL)
		, mData(NULL)
		, mFileSize(0)
	{
	}

	~BSPFile()
	{
		if( mData )
			GD_DELETE_ARRAY(mData);
	}

public:
	BSPHeader*	mHeader;
	Byte*		mData;
	UInt32		mFileSize;
};


};

#endif  //  _BSP_FILE_H_
