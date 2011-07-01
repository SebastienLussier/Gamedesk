/**
 *  @file       WoWReader.cpp
 *  @brief      Read a WoW file into it's corresponding data structures.
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
#include "WoWImporterDll.h"
#include "WoWReader.h"

#include "FileManager/FileManager.h"
#include "Resource/ResourceManager.h"


#define M_ID( a, b, c, d )  (((a) << 24) + ((b) << 16) + ((c) << 8) + ((d) << 0))

    
const UInt32 W_MVER = M_ID('M','V','E','R');
const UInt32 W_MHDR = M_ID('M','H','D','R');
const UInt32 W_MCIN = M_ID('M','C','I','N');
const UInt32 W_MTEX = M_ID('M','T','E','X');
const UInt32 W_MMDX = M_ID('M','M','D','X'); 
const UInt32 W_MMID = M_ID('M','M','I','D');
const UInt32 W_MWMO = M_ID('M','W','M','O');
const UInt32 W_MWID = M_ID('M','W','I','D');
const UInt32 W_MDDF = M_ID('M','D','D','F');
const UInt32 W_MODF = M_ID('M','O','D','F');
const UInt32 W_MCNK = M_ID('M','C','N','K');
const UInt32 W_MCVT = M_ID('M','C','V','T');
const UInt32 W_MCNR = M_ID('M','C','N','R');
const UInt32 W_MCLY = M_ID('M','C','L','Y');
const UInt32 W_MCRF = M_ID('M','C','R','F');
const UInt32 W_MCSH = M_ID('M','C','S','H');
const UInt32 W_MCAL = M_ID('M','C','A','L');
const UInt32 W_MCLQ = M_ID('M','C','L','Q');
const UInt32 W_MCSE = M_ID('M','C','S','E');


namespace WoW
{

ADTFileReader::ADTFileReader( ADTFile& pFile )
    : mADT(pFile)
{
}

void ADTFileReader::Read( const String& pFileName )
{
    WowFileStream stream;
    stream.Open( pFileName );
    
    UInt32  iMapChunk = 0;
    UInt32  chunkId;
    UInt32  chunkSize;
    Char    chunkName[5];

    chunkName[4] = '\0';

    while( iMapChunk != 256 )
    {
        stream << chunkId;
        stream << chunkSize;

        switch( chunkId )
        {

        // Version
        case W_MVER:
            stream << mADT.mVersion;
            break;

        // Header
        case W_MHDR:
            stream << mADT.mHeader.mPad;
            stream << mADT.mHeader.mOffsInfo;		
            stream << mADT.mHeader.mOffsTex;		
            stream << mADT.mHeader.mOffsModels;		
            stream << mADT.mHeader.mOffsModelsIds;		
            stream << mADT.mHeader.mOffsMapObejcts;		
            stream << mADT.mHeader.mOffsMapObejctsIds;		
            stream << mADT.mHeader.mOffsDoodsDef;		
            stream << mADT.mHeader.mOffsObjectsDef;	
            stream << mADT.mHeader.mPad1;	
            stream << mADT.mHeader.mPad2;		
            stream << mADT.mHeader.mPad3;	
            stream << mADT.mHeader.mPad4;		
            stream << mADT.mHeader.mPad5;		
            stream << mADT.mHeader.mPad6;		
            stream << mADT.mHeader.mPad7;
            break;

        // Map chunk index
        case W_MCIN:
            for( UInt32 i = 0; i < NUM_MAP_CHUNKS; i++ )
            {
                stream << mADT.mMapChunksIndex[i].mOffset;
                stream << mADT.mMapChunksIndex[i].mSize;
                stream << mADT.mMapChunksIndex[i].mFlags;
                stream << mADT.mMapChunksIndex[i].mAsyncID;
            }
            break;

        // Map chunks
        case W_MCNK:
            ReadMapChunk( stream, mADT.mMapChunks[iMapChunk], chunkSize );
            iMapChunk++;
            break;

        case W_MTEX:
		{
			UInt32 nameCount = 0;
			while( chunkSize != 0 )
			{
				mADT.mTextureNames.resize( nameCount + 1 );
				
				Char next;
				stream << next;
				while( next != 0 )
				{
					mADT.mTextureNames[nameCount] += next;
					stream << next;
				}
				
				chunkSize -= mADT.mTextureNames[nameCount].length() + 1;
				nameCount++;				 
			}
			break;
		}

		case W_MMDX:
        case W_MMID:
        case W_MWMO:
        case W_MWID:
        case W_MDDF:
        case W_MODF:
        default:
            stream.SeekRel( chunkSize );
        }
    }
}

void ADTFileReader::ReadMapChunk( WowFileStream& pStream, ADTFile::MapChunk& pMapChunk, UInt32 pChunkSize )
{
    UInt32 endPos = pStream.Pos() + pChunkSize;

    // Header
    pStream << pMapChunk.mHeader.mFlags;
    pStream << pMapChunk.mHeader.mIndexX;
    pStream << pMapChunk.mHeader.mIndexY;
    pStream << pMapChunk.mHeader.mNumLayers;
    pStream << pMapChunk.mHeader.mNumDoodadRefs;
    pStream << pMapChunk.mHeader.mOffsHeight;
    pStream << pMapChunk.mHeader.mOffsNormal;
    pStream << pMapChunk.mHeader.mOffsLayer;
    pStream << pMapChunk.mHeader.mOffsRefs;
    pStream << pMapChunk.mHeader.mOffsAlpha;
    pStream << pMapChunk.mHeader.mSizeAlpha;
    pStream << pMapChunk.mHeader.mOffsShadow;
    pStream << pMapChunk.mHeader.mSizeShadow;
    pStream << pMapChunk.mHeader.mAreaID;
    pStream << pMapChunk.mHeader.mNumMapObjRefs;
    pStream << pMapChunk.mHeader.mHoles;
    pStream << pMapChunk.mHeader.mUnk1;
    pStream << pMapChunk.mHeader.mUnk2;
    pStream << pMapChunk.mHeader.mUnk3;
    pStream << pMapChunk.mHeader.mUnk4;
    pStream << pMapChunk.mHeader.mUnk5;
    pStream << pMapChunk.mHeader.mPredTex;
    pStream << pMapChunk.mHeader.mNumEffectDoodad;
    pStream << pMapChunk.mHeader.mOffsSndEmitters;
    pStream << pMapChunk.mHeader.mNumSndEmitters;
    pStream << pMapChunk.mHeader.mOffsLiquid;
    pStream << pMapChunk.mHeader.mSizeLiquid;
    pStream << pMapChunk.mHeader.mPosition;
    pStream << pMapChunk.mHeader.mTextureID;
    pStream << pMapChunk.mHeader.mProps;
    pStream << pMapChunk.mHeader.mEffectID;

    UInt32  subChunkId;
    UInt32  subChunkSize;
    Char    subChunkName[5];

    subChunkName[4] = '\0';

    // Read sub chunks
    while( pStream.Pos() != endPos )
    {
        pStream << subChunkId;
        pStream << subChunkSize;

        switch( subChunkId )
        {
        // Vertices
        case W_MCVT:
            {
                UInt32 numItems = HEIGHT_MAP_SIZE*HEIGHT_MAP_SIZE + DETAIL_HEIGHT_MAP_SIZE*DETAIL_HEIGHT_MAP_SIZE;
                Float* array = pMapChunk.mHeightMap;
                for( UInt32 i = 0; i < numItems; i++, array++ )
                    pStream << (*array);
            }
            break;

        // Normals
        case W_MCNR:
            {
                UInt32 numItems = 3*(HEIGHT_MAP_SIZE*HEIGHT_MAP_SIZE + DETAIL_HEIGHT_MAP_SIZE*DETAIL_HEIGHT_MAP_SIZE);
                pStream.Serialize( pMapChunk.mHeightMapNormals, numItems );
                pStream.SeekRel( 13 ); // Skip alignment bytes...
                break;
            }

        // Texture layers
        case W_MCLY:
            pMapChunk.mTextureLayers.resize( subChunkSize / sizeof(ADTFile::MapChunk::TextureLayer) );
            for( UInt32 i = 0; i < pMapChunk.mTextureLayers.size(); i++ )
            {
                pStream << pMapChunk.mTextureLayers[i].mTextureID;
                pStream << pMapChunk.mTextureLayers[i].mFlags;
                pStream << pMapChunk.mTextureLayers[i].mAlphaOffset;
                pStream << pMapChunk.mTextureLayers[i].mDetailTextureID;
            }
            break;

        // Unknown
        case W_MCRF:
            pStream.SeekRel( subChunkSize );
            break;

        // Shadow map
        case W_MCSH:
            pStream.SeekRel( subChunkSize );
            break;

        // Alpha map
        case W_MCAL:
            pMapChunk.mAlphaMaps.resize( pMapChunk.mTextureLayers.size() - 1 );
            for( UInt32 i = 0; i < pMapChunk.mAlphaMaps.size(); i++ )
            {
                pMapChunk.mAlphaMaps[i].Create( 64, 64, Image::Format_A8 );
                Byte* data = pMapChunk.mAlphaMaps[i].GetData();

                pStream.Serialize( data, 32*64 );

                // Next "uncompress data"
                Byte* ptrCompressed = data + 32*64 - 1;
                Byte* ptrUncompressed = data + 64*64 - 1;
                while( ptrCompressed > data )
                {
                    Byte low = (*ptrCompressed & 0x0F) >> 0;
                    Byte hi  = (*ptrCompressed & 0xF0) >> 4;
                    *ptrUncompressed-- = hi << 4;
                    *ptrUncompressed-- = low << 4;
                    *ptrCompressed--;
                }
            }
            break;
        
        // Water/Magma ?
        case W_MCLQ:
            pStream.SeekRel( pMapChunk.mHeader.mSizeLiquid );
            break;
            
        // Sound emitter
        case W_MCSE:
            pStream.SeekRel( subChunkSize );
            break;

        default:
            pStream.SeekRel( subChunkSize );
            break;        
        }
    }
}


BLPFileReader::BLPFileReader( BLPFile& pFile ) 
	: mBLP(pFile)
{
}

void BLPFileReader::Read( const String& pFilename )
{
	WowFileStream stream;
	stream.Open( pFilename );

	stream << mBLP.mHeader.mFileTag[0];		// Always BLP2
	stream << mBLP.mHeader.mFileTag[1];		// Always BLP2
	stream << mBLP.mHeader.mFileTag[2];		// Always BLP2
	stream << mBLP.mHeader.mFileTag[3];		// Always BLP2
	stream << mBLP.mHeader.mVersion;		// Always 1  
	stream << mBLP.mHeader.mCompression;	// Compression: 1 for uncompressed, 2 for DXTC  
	stream << mBLP.mHeader.mAlphaBits;		// Alpha channel bits (?) 0, 1 or 8  
	stream << mBLP.mHeader.mUnknown1;		// Something about alpha? 0, 1, sometimes 8 for uncompressed textures  
	stream << mBLP.mHeader.mUnknown2;		// 0, 1 or 2 (?)  
	stream << mBLP.mHeader.mSizeX;			// X resolution  
	stream << mBLP.mHeader.mSizeY;			// Y resolution  

	// For every mipmap level  
	for( UInt32 i = 0; i < 16; i++ )
		stream << mBLP.mHeader.mOffsets[i];
	
	// For every mipmap level  
	UInt32 totalSize = 0;
    mBLP.mMipmapCount = 0;
	for( UInt32 i = 0; i < 16; i++ )
	{
		stream << mBLP.mHeader.mSizes[i];
		totalSize += mBLP.mHeader.mSizes[i];

		if( mBLP.mHeader.mSizes[i] > 0 )
			mBLP.mMipmapCount++;
	}

	mBLP.mData.resize(totalSize);

    UInt32 offset = 0;
    for( UInt32 i = 0; i < mBLP.mMipmapCount; i++ )
    {
        stream.Seek( mBLP.mHeader.mOffsets[i] );
    	stream.Serialize( &mBLP.mData[offset], mBLP.mHeader.mSizes[i] );
        offset += mBLP.mHeader.mSizes[i];
    }
}

}