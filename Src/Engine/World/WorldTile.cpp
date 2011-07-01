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
#include "WorldTile.h"


#include "Graphic/Buffer/IndexBuffer.h"
#include "Graphic/Buffer/VertexBuffer.h"
#include "Graphic/Shader/ShaderProgram.h"
#include "Graphic/Shader/ShaderObject.h"
#include "Graphic/Texture/Texture.h"
#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(WorldTile);


//! Default constructor.
WorldTile::WorldTile()
{
}

//! Destructor.
WorldTile::~WorldTile()
{
    Vector<TerrainChunk*>::iterator itChunk;
    for( itChunk = mTerrainChunks.begin(); itChunk != mTerrainChunks.end(); ++itChunk )
        GD_DELETE(*itChunk);
    
    mTerrainChunks.clear();
}

void WorldTile::Init()
{
    mPixelShader   = Cast<ShaderObject>( GraphicSubsystem::Instance()->Create( ShaderObject::StaticClass() ) );
    mVertexShader  = Cast<ShaderObject>( GraphicSubsystem::Instance()->Create( ShaderObject::StaticClass() ) );
    mShaderProgram = Cast<ShaderProgram>( GraphicSubsystem::Instance()->Create( ShaderProgram::StaticClass() ) );

    mPixelShader->Create( ShaderObject::Shader_Pixel );
    mPixelShader->SetShaderTextFile( "Data/Shaders/Terrain.ps" );
    mPixelShader->Compile();

    mVertexShader->Create( ShaderObject::Shader_Vertex );
    mVertexShader->SetShaderTextFile( "Data/Shaders/Terrain.vs" );
    mVertexShader->Compile();

    mShaderProgram->AddShader( mVertexShader );
    mShaderProgram->AddShader( mPixelShader );
    mShaderProgram->Link();


    // VertexFormat::Position3
    if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::Position3 ) )
    {
        mTerrainPositions = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
        mTerrainPositions->Create( mVertexList.GetVertexCount(), sizeof(Vector3f), VertexBuffer::Usage_Static );
        Vector3f* points = reinterpret_cast<Vector3f*>(mTerrainPositions->Lock( VertexBuffer::Lock_Write ));
        if( points )
            memcpy( points, mVertexList.GetPositions(), mVertexList.GetVertexCount()*sizeof(Vector3f) );
        mTerrainPositions->Unlock();
    }

    // VertexFormat::Normal3
    if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::Normal3 ) )
    {
        mTerrainNormals = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
        mTerrainNormals->Create( mVertexList.GetVertexCount(), sizeof(Vector3f), VertexBuffer::Usage_Static );
        Vector3f* normals = reinterpret_cast<Vector3f*>(mTerrainNormals->Lock( VertexBuffer::Lock_Write ));
        if( normals )
            memcpy( normals, mVertexList.GetNormals(), mVertexList.GetVertexCount()*sizeof(Vector3f) );
        mTerrainNormals->Unlock();
    }

    // VertexFormat::TexCoord2
    if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::TexCoord2 ) )
    {
        mTerrainTexCoords = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
        mTerrainTexCoords->Create( mVertexList.GetVertexCount(), sizeof(Vector2f), VertexBuffer::Usage_Static );
        Vector2f* texCoords = reinterpret_cast<Vector2f*>(mTerrainTexCoords->Lock( VertexBuffer::Lock_Write ));
        if( texCoords )
            memcpy( texCoords, mVertexList.GetTextureCoords(), mVertexList.GetVertexCount()*sizeof(Vector2f) );
        mTerrainTexCoords->Unlock();
    }

    // VertexFormat::TexCoord2_2
    if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::TexCoord2_2 ) )
    {
        mTerrainTexCoords_2 = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
        mTerrainTexCoords_2->Create( mVertexList.GetVertexCount(), sizeof(Vector2f), VertexBuffer::Usage_Static );
        Vector2f* texCoords = reinterpret_cast<Vector2f*>(mTerrainTexCoords_2->Lock( VertexBuffer::Lock_Write ));
        if( texCoords )
            memcpy( texCoords, mVertexList.GetTextureCoords_2(), mVertexList.GetVertexCount()*sizeof(Vector2f) );
        mTerrainTexCoords_2->Unlock();
    }

    // Indices buffer
    mTerrainIndices = Cast<IndexBuffer>( GraphicSubsystem::Instance()->Create( IndexBuffer::StaticClass() ) );
    mTerrainIndices->Create( 40448, sizeof(UInt16), IndexBuffer::Usage_Static );

    UInt16* indices = reinterpret_cast<UInt16*>(mTerrainIndices->Lock( IndexBuffer::Lock_Write ));
    UInt32  numIndices = 0;
    if( indices )
    {
        Vector<TerrainChunk*>::iterator itChunk;
        UInt32 iChunk = 0;

        for( itChunk = ((WorldTile*)this)->mTerrainChunks.begin(); itChunk != ((WorldTile*)this)->mTerrainChunks.end(); ++itChunk, iChunk++ )
        {
            UInt16* chunkIndices = (*itChunk)->GetLowResTriangles().GetIndices();
            UInt32  indicesCount = (*itChunk)->GetLowResTriangles().GetIndicesCount();

            memcpy( indices, chunkIndices, indicesCount*sizeof(UInt16) );
            numIndices += indicesCount;
            indices	   += indicesCount;
            iChunk++;
        }
    }        
    mTerrainIndices->Unlock();
}

//! Render the model.
void WorldTile::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetVertexFormat( VertexFormat::Component(VertexFormat::Position3 | VertexFormat::TexCoord2 | VertexFormat::TexCoord2_2 | VertexFormat::Normal3) );
    renderer->SetStreamSource( VertexFormat::Position3,   mTerrainPositions );
    renderer->SetStreamSource( VertexFormat::TexCoord2,   mTerrainTexCoords );
    renderer->SetStreamSource( VertexFormat::TexCoord2_2, mTerrainTexCoords_2 );
    renderer->SetStreamSource( VertexFormat::Normal3,     mTerrainNormals );
    renderer->SetIndices( mTerrainIndices );

    mShaderProgram->Apply();

    mShaderProgram->SetSampler( "BaseTexture", 0 );
    mShaderProgram->SetSampler( "Layer1", 1 );
    mShaderProgram->SetSampler( "Layer2", 2 );
    mShaderProgram->SetSampler( "Layer3", 3 );
    mShaderProgram->SetSampler( "Alpha1", 4 );
    mShaderProgram->SetSampler( "Alpha2", 5 );
    mShaderProgram->SetSampler( "Alpha3", 6 );
    

    //renderer->SetBlendFunc( Renderer::BlendSrcAlpha, Renderer::BlendInvSrcAlpha );

    UInt32 startIndex = 0;
	Vector<TerrainChunk*>::iterator itChunk;
	for( itChunk = ((WorldTile*)this)->mTerrainChunks.begin(); itChunk != ((WorldTile*)this)->mTerrainChunks.end(); ++itChunk )
    {
		UInt32 indicesCount = (*itChunk)->GetLowResTriangles().GetIndicesCount();
        Int32 layerCount   = (*itChunk)->GetLayerCount();
        
        mShaderProgram->SetUniform( "LayerCount", layerCount );

        switch( layerCount )
        {
        case 4:
            renderer->GetTextureStage(3)->SetTexture( (*itChunk)->GetTexture(3) );
            renderer->GetTextureStage(6)->SetTexture( (*itChunk)->GetAlphaMap(3) );

        case 3:
            renderer->GetTextureStage(2)->SetTexture( (*itChunk)->GetTexture(2) );
            renderer->GetTextureStage(5)->SetTexture( (*itChunk)->GetAlphaMap(2) );

        case 2:
            renderer->GetTextureStage(1)->SetTexture( (*itChunk)->GetTexture(1) );
            renderer->GetTextureStage(4)->SetTexture( (*itChunk)->GetAlphaMap(1) );

        case 1:
            renderer->GetTextureStage(0)->SetTexture( (*itChunk)->GetTexture(0) );
        }
        
        renderer->DrawIndexedPrimitive( Renderer::TriangleStrip, startIndex, indicesCount );

        startIndex += indicesCount;

        /*if( layerCount < 1 )
        {
            startIndex += indicesCount;
            continue;
        }
        
        renderer->SetRenderState( Renderer::Blend, false );           
        renderer->SetDepthFunc( Renderer::CmpLessEqual );
        renderer->SetRenderState( Renderer::DepthMask, true );

        renderer->GetTextureStage(0)->SetTexture( (*itChunk)->GetTexture(0) );
        renderer->GetTextureStage(1)->ResetTexture();
        renderer->DrawIndexedPrimitive( Renderer::TriangleStrip, startIndex, indicesCount );

        if( layerCount < 2 )
        {
            startIndex += indicesCount;
            continue;
        }
        
        renderer->SetRenderState( Renderer::Blend, true );
        renderer->SetDepthFunc( Renderer::CmpEqual );
        renderer->SetRenderState( Renderer::DepthMask, false );
        
        renderer->GetTextureStage(0)->SetTexture( (*itChunk)->GetTexture(1) );
        renderer->GetTextureStage(1)->SetTexture( (*itChunk)->GetAlphaMap(1) );
        renderer->DrawIndexedPrimitive( Renderer::TriangleStrip, startIndex, indicesCount );
    
        if( layerCount < 3 )
        {
            startIndex += indicesCount;
            continue;
        }

        renderer->GetTextureStage(0)->SetTexture( (*itChunk)->GetTexture(2) );
        renderer->GetTextureStage(1)->SetTexture( (*itChunk)->GetAlphaMap(2) );
        renderer->DrawIndexedPrimitive( Renderer::TriangleStrip, startIndex, indicesCount );
        
        
        if( layerCount < 4 )
        {
            startIndex += indicesCount;
            continue;
        }

        renderer->GetTextureStage(0)->SetTexture( (*itChunk)->GetTexture(3) );
        renderer->GetTextureStage(1)->SetTexture( (*itChunk)->GetAlphaMap(3) );
        renderer->DrawIndexedPrimitive( Renderer::TriangleStrip, startIndex, indicesCount );
        */
		//startIndex += indicesCount;
	}

    mShaderProgram->Done();

    renderer->GetTextureStage(0)->ResetTexture();
    renderer->GetTextureStage(1)->ResetTexture();
    renderer->GetTextureStage(2)->ResetTexture();
    renderer->GetTextureStage(3)->ResetTexture();
    renderer->GetTextureStage(4)->ResetTexture();
    renderer->GetTextureStage(5)->ResetTexture();
    renderer->GetTextureStage(6)->ResetTexture();
    renderer->GetTextureStage(7)->ResetTexture();
	    
    //renderer->SetRenderState( Renderer::Blend, false );
    //renderer->SetRenderState( Renderer::DepthMask, true );

    //renderer->SetDepthFunc( Renderer::CmpLess );
}

VertexList& WorldTile::GetVertexList()
{
    return mVertexList;
}

void WorldTile::TerrainChunk::AddTextureLayer( const HTexture2D& pTexture, Texture2D* pAlphaMap )
{
    TextureLayer newLayer;

    newLayer.mTexture  = pTexture;
    newLayer.mAlphaMap = pAlphaMap;

    mTextureLayers.push_back( newLayer );
}

UInt32 WorldTile::TerrainChunk::GetLayerCount() const
{
    return mTextureLayers.size();
}

Texture2D& WorldTile::TerrainChunk::GetTexture( UInt32 pLayer )
{
    GD_ASSERT( pLayer < mTextureLayers.size() );
    return *mTextureLayers[pLayer].mTexture;
}

Texture2D& WorldTile::TerrainChunk::GetAlphaMap( UInt32 pLayer )
{
    GD_ASSERT( pLayer < mTextureLayers.size() );
    return *mTextureLayers[pLayer].mAlphaMap;
}

Vector<WorldTile::TerrainChunk*>& WorldTile::GetTerrainChunks()
{
    return mTerrainChunks;
}

TriangleBatch& WorldTile::TerrainChunk::GetHiResTriangles()
{
    return mHiResTriangles;
}

TriangleBatch& WorldTile::TerrainChunk::GetLowResTriangles()
{
    return mLowResTriangles;
}


} // namespace Gamedesk
