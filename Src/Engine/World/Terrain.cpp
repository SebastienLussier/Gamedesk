 /**
 *  @file       Terrain.cpp
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       18/02/04.
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
#include "Engine.h"
#include "Terrain.h"

#include "FileManager/FileManager.h"

#include "Graphic/Buffer/IndexBuffer.h"
#include "Graphic/Buffer/VertexBuffer.h"
#include "Graphic/Buffer/VertexFormat.h"

#include "Graphic/Mesh/Mesh.h"
#include "Graphic/Texture/Texture.h"
#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
Terrain::Terrain() :
    mTextureName("Data/Terrain/terrain001.dds"),
    mBufPositions(NULL),
    mBufNormals(NULL),
    mBufTexCoords(NULL),
    mBufIndices(NULL),
    mMapSize(0),
    mMapWidth(0),
    mHeightData(NULL)    
{
    LoadRawFile( "Data/Terrain/Terrain.raw" );
    BuildBuffers();

    SetTexture(mTextureName);
}

Terrain::~Terrain()
{
    if( mHeightData )
        GD_DELETE(mHeightData);
}

void Terrain::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetRenderState( Renderer::Lighting, true );

    renderer->SetVertexFormat( VertexFormat::Component(VertexFormat::Position3 | VertexFormat::TexCoord2 | VertexFormat::Normal3) );
    renderer->SetStreamSource( VertexFormat::Position3, mBufPositions );
    renderer->SetStreamSource( VertexFormat::TexCoord2, mBufTexCoords );
    renderer->SetStreamSource( VertexFormat::Normal3,   mBufNormals );
    renderer->SetIndices( mBufIndices );

    renderer->SetMatrixMode( Renderer::TextureMatrix );
    renderer->PushMatrix();
    renderer->LoadIdentity();
    renderer->Scale( Vector3f( 10.0f, 10.0f, 10.0f ) );
    
    renderer->GetTextureStage(0)->SetTexture( *mTexture );
    renderer->DrawIndexedPrimitive( Renderer::TriangleList, 0, mBufIndices->GetItemCount() );
    renderer->GetTextureStage(0)->ResetTexture();

    renderer->PopMatrix();
    renderer->SetMatrixMode( Renderer::ModelViewMatrix );    
}

void Terrain::LoadRawFile( const String& pHeightMapFile )
{
    std::ifstream file( pHeightMapFile.c_str(), std::ios::in | std::ios_base::binary );
        
    if( file.fail() )
        throw FileNotFoundException( pHeightMapFile, Here );

    file.seekg( 0, std::ios_base::end );
    int fileSize = file.tellg();
    file.seekg( 0, std::ios_base::beg );

    mMapSize = Maths::Sqrt( (Float)fileSize );
    mMapWidth = mMapSize * 5.0f;

    mHeightData = GD_NEW_ARRAY(Byte, fileSize, this, "Engine::World::Terrain");
    file.read( (Char*)mHeightData, fileSize );
}

void Terrain::BuildBuffers()
{
    VertexList      terrainVertice;
    TriangleBatch   terrainTriangles;

    UInt32 nbTrianglesIndex = ((mMapSize)-1) * ((mMapSize)-1) * 6;
    UInt32 nbPoints         = mMapSize * mMapSize;

    terrainVertice.Allocate( nbPoints, VertexFormat::Component(VertexFormat::Position3 | VertexFormat::TexCoord2 | VertexFormat::Normal3) );
    terrainTriangles.Allocate( TriangleBatch::TriangleList, nbTrianglesIndex );

    Vector3f*   points      = terrainVertice.GetPositions();
    Vector3f*   normals     = terrainVertice.GetNormals();
    Vector2f*   texCoords   = terrainVertice.GetTextureCoords();
    UInt16*     indices     = terrainTriangles.GetIndices();

    UInt32      n = 0;
    UInt32      i = 0;

    for( UInt32 z = 0; z < mMapSize; z++ )
    {
        for( UInt32 x = 0; x < mMapSize; x++ )
        {
            points[n].x = x * 5.0f;
            points[n].y = GetHeight( x, z ) / 7.8125f;
            points[n].z = z * 5.0f;

            points[n].x -= mMapWidth * 0.5f;
            points[n].z -= mMapWidth * 0.5f;

			normals[n].x = 0;
            normals[n].y = 0;
            normals[n].z = 0;
            
            // Find the (u, v) coordinate for the current vertex
            texCoords[n].x =  (Float)x / (Float)mMapSize;
            texCoords[n].y =  (Float)z / (Float)mMapSize;

            if( z+1 != mMapSize && x+1 != mMapSize )
            {
                indices[i+0] = n;
                indices[i+1] = n + mMapSize;
                indices[i+2] = n + 1;                

                indices[i+3] = n + mMapSize;
                indices[i+4] = n + mMapSize + 1;
                indices[i+5] = n + 1;
                
                i += 6;
            }

            n++;
        }
    }

    // First pass, compute face normals.
    for( i = 0; i < nbTrianglesIndex; i += 3 )
    {
        Vector3f v1 = points[indices[i+1]] - points[indices[i+0]];
        Vector3f v2 = points[indices[i+2]] - points[indices[i+0]];

        Vector3f vn = (v1 cross v2).Normalize();

        // Add normal to each point in the triangle.
        normals[indices[i+0]] += vn;
        normals[indices[i+1]] += vn;
        normals[indices[i+2]] += vn;
    }

    // Second pass, normalize!
    for( i = 0; i < nbPoints; i++ )
        normals[i].Normalize();

    //TriangleBatch triangleStrip;
    //terrainTriangles.Stripify( triangleStrip );
    //terrainVertice.Reorder( triangleStrip );
    
    mBufPositions = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
    mBufPositions->Create( terrainVertice.GetVertexCount(), sizeof(Vector3f), VertexBuffer::Usage_Static );

    mBufNormals = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
    mBufNormals->Create( terrainVertice.GetVertexCount(), sizeof(Vector3f), VertexBuffer::Usage_Static );

    mBufTexCoords = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
    mBufTexCoords->Create( terrainVertice.GetVertexCount(), sizeof(Vector2f), VertexBuffer::Usage_Static );

    mBufIndices = Cast<IndexBuffer>( GraphicSubsystem::Instance()->Create( IndexBuffer::StaticClass() ) );
    mBufIndices->Create( terrainTriangles.GetIndicesCount(), sizeof(UInt16), IndexBuffer::Usage_Static );

    Vector3f* pointsBuf     = reinterpret_cast<Vector3f*>(mBufPositions->Lock( VertexBuffer::Lock_Write ));
    Vector3f* normalsBuf    = reinterpret_cast<Vector3f*>(mBufNormals->Lock( VertexBuffer::Lock_Write ));
    Vector2f* texCoordsBuf  = reinterpret_cast<Vector2f*>(mBufTexCoords->Lock( VertexBuffer::Lock_Write ));
    UInt16*   indicesBuf    = reinterpret_cast<UInt16*>(mBufIndices->Lock( IndexBuffer::Lock_Write ));

    memcpy( pointsBuf, terrainVertice.GetPositions(), mBufPositions->GetItemCount() * mBufPositions->GetItemSize() );
    memcpy( normalsBuf, terrainVertice.GetNormals(), mBufNormals->GetItemCount() * mBufNormals->GetItemSize() );
    memcpy( texCoordsBuf, terrainVertice.GetTextureCoords(), mBufTexCoords->GetItemCount() * mBufTexCoords->GetItemSize() );
    memcpy( indicesBuf, terrainTriangles.GetIndices(), mBufIndices->GetItemCount() * mBufIndices->GetItemSize() );

    mBufPositions->Unlock();
    mBufNormals->Unlock();
    mBufTexCoords->Unlock();
    mBufIndices->Unlock();
}

Byte Terrain::GetHeight( UInt32 pX, UInt32 pZ )
{
    UInt32 x = pX % mMapSize;
    UInt32 z = pZ % mMapSize;

    if( !mHeightData )
        return 0;

    return mHeightData[x + (z * mMapSize)];
}

void Terrain::SetTexture(const String& pTextureName)
{
    mTextureName = pTextureName;
    mTexture.GetTexture( mTextureName );
}

const String& Terrain::GetTextureName() const
{
    return mTextureName;
}

IMPLEMENT_CLASS(Terrain);


} // namespace Gamedesk
