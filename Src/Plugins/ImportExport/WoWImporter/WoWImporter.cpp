/**
 *  @file       WoWImporter.cpp
 *  @brief      WoW mesh/anim importer.
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
#include "WoWImporter.h"

#include "WoWFiles.h"
#include "WoWReader.h"

#include "FileManager/FileManager.h"
#include "World/WorldTile.h"


IMPLEMENT_CLASS(WoWImporter);


const Float TILE_SIZE    = 533.33333f;
const Float CHUNK_SIZE   = TILE_SIZE / 16.0f;
const Float UNIT_SIZE    = CHUNK_SIZE / 8.0f;


WoWImporter::WoWImporter()
{
    AddSupportedType( "adt" );
}

Class* WoWImporter::GetResourceClass()
{
    return WorldTile::StaticClass();
}

Resource* WoWImporter::Import( const String& pFilename, const String& /*pParams*/ )
{
    WoW::ADTFile        adtFile;
    WoW::ADTFileReader  adtFileReader( adtFile );

    adtFileReader.Read( pFilename );


    WorldTile* worldTile = GD_NEW(WorldTile, this, "WoW::WorldTile");
    worldTile->GetTerrainChunks().resize( 256 );
    worldTile->GetVertexList().Allocate( 16*16 * (9*9 + 8*8), (VertexFormat::Component) (VertexFormat::Position3 | VertexFormat::TexCoord2 | VertexFormat::TexCoord2_2 | VertexFormat::Normal3) );

    Vector3f* ptrPosition = worldTile->GetVertexList().GetPositions();
    Vector3f* ptrNormal   = worldTile->GetVertexList().GetNormals();
	Vector2f* ptrTexCoord = worldTile->GetVertexList().GetTextureCoords();
    Vector2f* ptrTexCoordAlpha = worldTile->GetVertexList().GetTextureCoords_2();
    UInt32    verticesOffset = 0;

    Vector3f firstChunkPos(0,0,0);//-1.0f*adtFile.mMapChunks[0].mHeader.mPosition.y, adtFile.mMapChunks[0].mHeader.mPosition.z, -1.0f*adtFile.mMapChunks[0].mHeader.mPosition.x );

    for( UInt32 iChunk = 0; iChunk < 256; iChunk++ )
    {
        WoW::ADTFile::MapChunk* mapChunk = &adtFile.mMapChunks[iChunk];

        // Append vertex data to vertex buffer
        Float* ptrDataHeight  = mapChunk->mHeightMap;
        Char*  ptrDataNormals = mapChunk->mHeightMapNormals[0];

        Vector3f chunkPos( -1.0f*mapChunk->mHeader.mPosition.y, mapChunk->mHeader.mPosition.z, -1.0f*mapChunk->mHeader.mPosition.x );
        Float  posX = chunkPos.x - firstChunkPos.x;
        Float  posZ = chunkPos.z - firstChunkPos.z;
        for( UInt32 z = 0; z < 9; z++ )
        {
            // Normal row
            posX = chunkPos.x - firstChunkPos.x;
            for( UInt32 x = 0; x < 9; x++ )
            {
                ptrPosition->x = posX;
                ptrPosition->y = chunkPos.y + (*ptrDataHeight) - firstChunkPos.y;
                ptrPosition->z = posZ;
                (*ptrPosition) *= 0.1f;

                ptrNormal->x = ((Float)ptrDataNormals[0]) / 127.0f;
                ptrNormal->y = ((Float)ptrDataNormals[1]) / 127.0f;
                ptrNormal->z = ((Float)ptrDataNormals[2]) / 127.0f;

                ptrTexCoord->x = x / 8.0f;
                ptrTexCoord->y = z / 8.0f;

				ptrTexCoordAlpha->x = (x+0.125f) / 8.25f;
				ptrTexCoordAlpha->y = (z+0.125f) / 8.25f;

                ptrDataHeight++;
                ptrDataNormals += 3;

                ptrPosition++;
                ptrNormal++;
				ptrTexCoord++;
                ptrTexCoordAlpha++;

                posX += UNIT_SIZE;
            }
            posZ += 0.5f * UNIT_SIZE;

            if( z == 8 )
                break;

            // Detailed row
            posX = chunkPos.x + 0.5f * UNIT_SIZE - firstChunkPos.x;
            for( UInt32 x = 0; x < 8; x++ )
            {
                ptrPosition->x = posX;
                ptrPosition->y = chunkPos.y + (*ptrDataHeight) - firstChunkPos.y;
                ptrPosition->z = posZ;
                (*ptrPosition) *= 0.1f;

                ptrNormal->x = ((Float)ptrDataNormals[0]) / 127.0f;
                ptrNormal->y = ((Float)ptrDataNormals[1]) / 127.0f;
                ptrNormal->z = ((Float)ptrDataNormals[2]) / 127.0f;

                ptrTexCoord->x = (x+0.5f) / 8.0f;
                ptrTexCoord->y = (z+0.5f) / 8.0f;

				ptrTexCoordAlpha->x = (x+0.5f+0.125f) / 8.25f;
				ptrTexCoordAlpha->y = (z+0.5f+0.125f) / 8.25f;

                ptrDataHeight++;
                ptrDataNormals += 3;
                
				ptrPosition++;
                ptrNormal++;
				ptrTexCoord++;
                ptrTexCoordAlpha++;

                posX += UNIT_SIZE;
            }
            posZ += 0.5f * UNIT_SIZE;
        }


        WorldTile::TerrainChunk* newChunk = GD_NEW(WorldTile::TerrainChunk, this, "WoW::WorldTile::TerrainChunk");

		for( UInt32 iLayer = 0; iLayer < mapChunk->mTextureLayers.size(); iLayer++ )
        {
			String textureName("Data/");
			textureName += adtFile.mTextureNames[mapChunk->mTextureLayers[iLayer].mTextureID];
			HTexture2D texture(textureName);

            Texture2D* alpha = NULL;

            if( iLayer != 0 )
            {
                alpha = Cast<Texture2D>(Texture2D::StaticClass()->AllocateNew( "AlphaMap" ));
                alpha->Create( mapChunk->mAlphaMaps[iLayer-1], true );
                alpha->Init();
                alpha->SetWrapMode( Texture::Wrap_S, Texture::Wrap_Clamp );
                alpha->SetWrapMode( Texture::Wrap_T, Texture::Wrap_Clamp );
                alpha->SetMinFilter( Texture::MinFilter_Linear );
                alpha->SetMagFilter( Texture::MagFilter_Linear );
            }

			newChunk->AddTextureLayer( texture, alpha );
		}

        UInt32 detailedStripSize = (16*18 + 7*2 + 8*2);
        UInt32 normalStripSize   = 158;

        // Build index buffer
        newChunk->GetHiResTriangles().Allocate( TriangleBatch::TriangleStrip, detailedStripSize );
        newChunk->GetLowResTriangles().Allocate( TriangleBatch::TriangleStrip, normalStripSize );
        UInt16* ptrHiIndices = newChunk->GetHiResTriangles().GetIndices();
        UInt16* ptrLoIndices = newChunk->GetLowResTriangles().GetIndices();
        
        for( int row = 0; row < 8; row++ )
        { 
            UInt32 topRow       = verticesOffset + row*(9+8);
            UInt32 detailrow    = verticesOffset + row*(9+8) + 9;
            UInt32 nextRow      = verticesOffset + (row+1)*(9+8);
            
            if( row > 0 )
            {
                *ptrHiIndices++ = topRow + 0;     // jump end
                *ptrLoIndices++ = topRow + 0;
            }
            
            for( int col = 0; col < 8; col++ )
            {
                *ptrHiIndices++ = topRow + col;
                *ptrHiIndices++ = detailrow + col;
                *ptrLoIndices++ = topRow + col;
                *ptrLoIndices++ = nextRow + col;
            }
            
            *ptrHiIndices++ = topRow  + 8;
            *ptrHiIndices++ = nextRow + 8;
            *ptrHiIndices++ = nextRow + 8;        // jump start
            *ptrHiIndices++ = topRow  + 0;        // jump end
            *ptrHiIndices++ = topRow  + 0;   
            
            *ptrLoIndices++ = topRow  + 8;
            *ptrLoIndices++ = nextRow + 8;
            
            for( int col = 0; col < 8; col++ )
            {
                *ptrHiIndices++ = nextRow + col;
                *ptrHiIndices++ = detailrow + col;
            }

            if( row < 8 )
                *ptrHiIndices++ = nextRow + 8;

            if( row < 7 ) 
            {
                *ptrHiIndices++ = nextRow + 8;    // jump start
                *ptrLoIndices++ = nextRow + 8;
            }
        }

        verticesOffset += 9*9 + 8*8;

        worldTile->GetTerrainChunks()[iChunk] = newChunk;
    }
    
    return (Resource*)worldTile;
}


// 1 10 2 11 3 12 4 13 5 14 5 15 7 16 8 17 9 26
// 26 17

// 1 18 2 19 3 20 4 21 5 22 6 23 7 24 8 25 9 26 26
// 18 
    
/*

1       2       3       4       5       6       7       8       9  
    10      11      12      13      14      15      16      17   
18      19      20      21      22      23      24      25      26  
    27      28      29      30      31      32      33      34
35      36      37      38      39      40      41      42      43
    44      45      46      47      48      49      50      51  
52      53      54      55      56      57      58      59      60
    61      62      63      64      65      66      67      68
69      70      71      72      73      74      75      76      77
    78      79      80      81      82      83      84      85
86      87      88      89      90      91      92      93      94
    95      96      97      98      99      100     101     102
103     104     105     106     107     108     109     110     111
    112     113     114     115     116     117     118     119
120     121     122     123     124     125     126     127     128
    129     130     131     132     133     134     135     136
137     138     139     140     141     142     143     144     145

*/




IMPLEMENT_CLASS(BLPImporter);

BLPImporter::BLPImporter()
{
	AddSupportedType( "blp" );
}

Class* BLPImporter::GetResourceClass()
{
	return Texture::StaticClass();
}

Resource* BLPImporter::Import( const String& pFilename, const String& /*pParams*/ )
{
	WoW::BLPFile        blpFile;
	WoW::BLPFileReader  blpFileReader( blpFile );

	blpFileReader.Read( pFilename );

	Image::Format format;
	if( blpFile.mHeader.mCompression == 2 )
	{
		if( blpFile.mHeader.mAlphaBits == 0 )
			format = Image::Format_DXT1;
		else if( blpFile.mHeader.mAlphaBits == 8 )
			format = Image::Format_DXT3;
		else
			format = Image::Format_DXT5;
	}
	else
	{
		format = Image::Format_R8G8B8A8;
	}

	Image img;
	img.Create( blpFile.mHeader.mSizeX, blpFile.mHeader.mSizeY, format, blpFile.mMipmapCount );

	memcpy( img.GetData(), &blpFile.mData[0], blpFile.mData.size() );

	Texture2D* tex2D = Cast<Texture2D>(Texture2D::StaticClass()->AllocateNew( pFilename ));
	tex2D->Create( img );
	
	return tex2D;	
}