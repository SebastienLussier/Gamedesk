/**
 *  @file       WoWFiles.h
 *  @brief      Defines data structures for WoW files.
 *  @author     Sébastien Lussier.
 *  @date       30/04/05.
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
#ifndef     _WoW_MESH_H_
#define     _WoW_MESH_H_


#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Maths/Quaternion.h"
#include "Maths/BoundingBox.h"
#include "Maths/Plane3.h"
#include "Graphic/Color4.h"

#include "Graphic/Image.h"

namespace WoW
{


class File
{
public:
    UInt32  mVersion;
};


const NUM_MAP_CHUNKS = 256;                  // Map chunks count pet ADT file.
const UInt32 HEIGHT_MAP_SIZE = 9;
const UInt32 DETAIL_HEIGHT_MAP_SIZE = 8;


class ADTFile : public File
{
public:
    struct Header // 03-29-2005 By ObscuR
    {
        UInt32 mPad;
        UInt32 mOffsInfo;		
        UInt32 mOffsTex;		
        UInt32 mOffsModels;		
        UInt32 mOffsModelsIds;		
        UInt32 mOffsMapObejcts;		
        UInt32 mOffsMapObejctsIds;		
        UInt32 mOffsDoodsDef;		
        UInt32 mOffsObjectsDef;	
        UInt32 mPad1;	
        UInt32 mPad2;		
        UInt32 mPad3;	
        UInt32 mPad4;		
        UInt32 mPad5;		
        UInt32 mPad6;		
        UInt32 mPad7;	
    };

    struct MapChunkIndex
    {
        UInt32 mOffset;
        UInt32 mSize;		
        UInt32 mFlags;		
        UInt32 mAsyncID;;
    };

    struct MapChunk
    {
        enum 
        {
            FLAG_SHADOW,
            FLAG_IMPASS,
            FLAG_LQ_RIVER,
            FLAG_LQ_OCEAN,
            FLAG_LQ_MAGMA,
        };

        struct Header
        {
            UInt32 mFlags;
            UInt32 mIndexX;
            UInt32 mIndexY;
            UInt32 mNumLayers;
            UInt32 mNumDoodadRefs;
            UInt32 mOffsHeight;
            UInt32 mOffsNormal;
            UInt32 mOffsLayer;
            UInt32 mOffsRefs;
            UInt32 mOffsAlpha;
            UInt32 mSizeAlpha;
            UInt32 mOffsShadow;
            UInt32 mSizeShadow;
            UInt32 mAreaID;
            UInt32 mNumMapObjRefs;
            UInt32 mHoles;
            UInt16 mUnk1;
            UInt16 mUnk2;
            UInt32 mUnk3;
            UInt32 mUnk4;
            UInt32 mUnk5;
            UInt32 mPredTex;
            UInt32 mNumEffectDoodad;
            UInt32 mOffsSndEmitters;
            UInt32 mNumSndEmitters;
            UInt32 mOffsLiquid;
            UInt32 mSizeLiquid;
            Vector3f mPosition;
            UInt32 mTextureID;
            UInt32 mProps;
            UInt32 mEffectID;
        };

        struct TextureLayer
        {
            UInt32 mTextureID;          // Index into MTEX list.
            UInt32 mFlags;
            UInt32 mAlphaOffset;        // Offset to alpha map.
            UInt32 mDetailTextureID;    // Name says it all.
        };

        Header  mHeader;

        // Height values.
        Float   mHeightMap[HEIGHT_MAP_SIZE*HEIGHT_MAP_SIZE + DETAIL_HEIGHT_MAP_SIZE*DETAIL_HEIGHT_MAP_SIZE];
        
        // Packed normals.
        Char    mHeightMapNormals[HEIGHT_MAP_SIZE*HEIGHT_MAP_SIZE + DETAIL_HEIGHT_MAP_SIZE*DETAIL_HEIGHT_MAP_SIZE][3];

        Vector<TextureLayer>    mTextureLayers;
        Vector<Image>           mAlphaMaps;
    };


    struct ModelInstance
    {
        UInt32      mNameID;
        UInt32      mUniqueID;		
        Vector3f    mPosition;		
        Float       mRotation[3];		
        UInt16      mFlags;	
        UInt16      mScale;
    };


    struct MapObjInstance
    {
        UInt32      mNameID;		
        UInt32      mUniqueID;		
        Vector3f    mPosition;
        Float       mRotation[3];
        BoundingBox mExtents;
        UInt32      mFlags;		
        UInt16      mModelInstanceSet;
        UInt16      mNameSet;
    };


    Header          mHeader;
    MapChunkIndex   mMapChunksIndex[NUM_MAP_CHUNKS];
    
    Vector<String>  mTextureNames;

    Vector<String>  mModelNames;
    Vector<UInt32>  mModelNamesOff;

    Vector<String>  mMapObjNames;
    Vector<UInt32>  mMapObjNamesOff;

    Vector<ModelInstance>   mModelInstances;
    Vector<MapObjInstance>  mMapObjInstances;

    MapChunk        mMapChunks[NUM_MAP_CHUNKS];
};


class BLPFile
{
public:
	struct Header
	{
		Char	mFileTag[4];	// Always BLP2
		UInt32  mVersion;		// Always 1  
		Byte	mCompression;	// Compression: 1 for uncompressed, 2 for DXTC  
		Byte	mAlphaBits;		// Alpha channel bits (?) 0, 1 or 8  
		Byte	mUnknown1;		// Something about alpha? 0, 1, sometimes 8 for uncompressed textures  
		Byte	mUnknown2;		// 0, 1 or 2 (?)  
		UInt32	mSizeX;			// X resolution  
		UInt32	mSizeY;			// Y resolution  
		UInt32	mOffsets[16];	// For every mipmap level  
		UInt32  mSizes[16];		// For every mipmap level  
	};

	Header			mHeader;
	Vector<Byte>	mData;
	UInt32			mMipmapCount;
};

}


#endif  //  _WoW_MESH_H_
