/**
 *  @file       Mesh.cpp
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       15/01/03.
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
#include "Mesh.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/Buffer/VertexBuffer.h"
#include "Graphic/Buffer/IndexBuffer.h"
#include "Graphic/Shader/Shader.h"

#include "Maths/Number.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Mesh);


Mesh::Mesh() :
	mShader(NULL),
    mBufPositions(NULL),
    mBufNormals(NULL),
    mBufColors(NULL),
    mBufTexCoords(NULL),
    mBufIndices(NULL)
{
}

Mesh::~Mesh()
{
    if( mShader )
        GD_DELETE(mShader);
    
    if( mBufPositions )
        GD_DELETE(mBufPositions);

    if( mBufNormals )
        GD_DELETE(mBufNormals);

    if( mBufColors )
        GD_DELETE(mBufColors);

    if( mBufTexCoords )
        GD_DELETE(mBufTexCoords);
    
    if( mBufIndices )
        GD_DELETE(mBufIndices);

	// Delete the childs.
	Vector<Mesh*>::iterator itChild;
    for(itChild = mChildMeshes.begin(); itChild != mChildMeshes.end(); ++itChild)
        GD_DELETE(*itChild);
}

void Mesh::Init()
{
    if( mVertexList.GetVertexCount() != 0 )
    {
        /*Vector<Vector3f> vertices;
        vertices.resize( mVertexList.GetVertexCount() );
        for( UInt32 i = 0; i < vertices.size(); i++ )
            vertices[i] = mVertexList.GetPositions()[i];

        Vector<Trianglef> triangleSoup;
        triangleSoup.resize( mTriangles.GetIndicesCount() / 3 );
        for( UInt32 i = 0; i < triangleSoup.size(); i++ )
        {
            triangleSoup[i].mIndices[0] = mTriangles.GetIndices()[i*3 + 0];
            triangleSoup[i].mIndices[1] = mTriangles.GetIndices()[i*3 + 1];
            triangleSoup[i].mIndices[2] = mTriangles.GetIndices()[i*3 + 2];
        }

        mkDOPTree.BuildTree( triangleSoup, vertices );
        */

        //TriangleBatch triangleStrip;
        //mTriangles.Stripify( triangleStrip );
        //mVertexList.Reorder( triangleStrip );

        // VertexFormat::Position3
        if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::Position3 ) )
        {
            mBufPositions = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
            mBufPositions->Create( mVertexList.GetVertexCount(), sizeof(Vector3f), VertexBuffer::Usage_Static );
            Vector3f* points = reinterpret_cast<Vector3f*>(mBufPositions->Lock( VertexBuffer::Lock_Write ));
            if( points )
                memcpy( points, mVertexList.GetPositions(), mVertexList.GetVertexCount()*sizeof(Vector3f) );
            mBufPositions->Unlock();
        }

        // VertexFormat::Normal3
        if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::Normal3 ) )
        {
            mBufNormals = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
            mBufNormals->Create( mVertexList.GetVertexCount(), sizeof(Vector3f), VertexBuffer::Usage_Static );
            Vector3f* normals = reinterpret_cast<Vector3f*>(mBufNormals->Lock( VertexBuffer::Lock_Write ));
            if( normals )
                memcpy( normals, mVertexList.GetNormals(), mVertexList.GetVertexCount()*sizeof(Vector3f) );
            mBufNormals->Unlock();
        }

        // VertexFormat::Color3
        if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::Color3 ) )
        {
            mBufColors = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
            mBufColors->Create( mVertexList.GetVertexCount(), sizeof(Color3f), VertexBuffer::Usage_Static );
            Color3f* colors = reinterpret_cast<Color3f*>(mBufColors->Lock( VertexBuffer::Lock_Write ));
            if( colors )
                memcpy( colors, mVertexList.GetColors(), mVertexList.GetVertexCount()*sizeof(Color3f) );
            mBufColors->Unlock();
        }

        // VertexFormat::Color4
        if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::Color4 ) )
        {
            mBufColors = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
            mBufColors->Create( mVertexList.GetVertexCount(), sizeof(Color4f), VertexBuffer::Usage_Static );
            Color4f* colors = reinterpret_cast<Color4f*>(mBufColors->Lock( VertexBuffer::Lock_Write ));
            if( colors )
                memcpy( colors, mVertexList.GetColors(), mVertexList.GetVertexCount()*sizeof(Color4f) );
            mBufColors->Unlock();
        }

        // VertexFormat::TexCoord2
        if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::TexCoord2 ) )
        {
            mBufTexCoords = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
            mBufTexCoords->Create( mVertexList.GetVertexCount(), sizeof(Vector2f), VertexBuffer::Usage_Static );
            Vector2f* texCoords = reinterpret_cast<Vector2f*>(mBufTexCoords->Lock( VertexBuffer::Lock_Write ));
            if( texCoords )
                memcpy( texCoords, mVertexList.GetTextureCoords(), mVertexList.GetVertexCount()*sizeof(Vector2f) );
            mBufTexCoords->Unlock();
        }
    
        // Indices buffer
        mBufIndices = Cast<IndexBuffer>( GraphicSubsystem::Instance()->Create( IndexBuffer::StaticClass() ) );
        mBufIndices->Create( mTriangles.GetIndicesCount(), sizeof(UInt16), IndexBuffer::Usage_Static );
        UInt16* indices = reinterpret_cast<UInt16*>(mBufIndices->Lock( IndexBuffer::Lock_Write ));
        if( indices )
            memcpy( indices, mTriangles.GetIndices(), mTriangles.GetIndicesCount()*sizeof(UInt16) );
        mBufIndices->Unlock();

        Vector3f* vertices = mVertexList.GetPositions();
		for( UInt32 i = 0; i < mVertexList.GetVertexCount(); i++ )
			mBoundingBox.Grow( vertices[i] );
    }

	Vector<Mesh*>::iterator itChild;
    for( itChild = mChildMeshes.begin(); itChild != mChildMeshes.end(); ++itChild )
	{
        (*itChild)->Init();
        mBoundingBox.Grow((*itChild)->GetBoundingBox());
	}
}

void Mesh::Update(Double /*pElapsedTime*/)
{
}

void Mesh::Render(Bool pRenderChild) const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();

    if( mBufPositions )
    {
        renderer->SetVertexFormat( mVertexList.GetVertexFormat() );

        // VertexFormat::Position3
        if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::Position3 ) )
            renderer->SetStreamSource( VertexFormat::Position3, mBufPositions );

        // VertexFormat::Normal3
        if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::Normal3 ) )
            renderer->SetStreamSource( VertexFormat::Normal3, mBufNormals );

        // VertexFormat::Color3
        if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::Color3 ) )
            renderer->SetStreamSource( VertexFormat::Color3, mBufColors );

        // VertexFormat::Color4
        if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::Color4 ) )
            renderer->SetStreamSource( VertexFormat::Color4, mBufColors );

        // VertexFormat::TexCoord2
        if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::TexCoord2 ) )
            renderer->SetStreamSource( VertexFormat::TexCoord2, mBufTexCoords );

        // Set active index buffer
        renderer->SetIndices( mBufIndices );

        renderer->SetRenderState( Renderer::CullFace, true );
        renderer->SetRenderState( Renderer::Lighting, false );
        renderer->SetCulling( Renderer::CullBackFace );

        Renderer::PrimitiveType type;

        switch( mTriangles.GetBatchType() )
        {
        case TriangleBatch::TriangleStrip:
            type = Renderer::TriangleStrip;
            break;

        case TriangleBatch::TriangleFan:
            type = Renderer::TriangleFan;
            break;

        case TriangleBatch::TriangleList:
        default:
            type = Renderer::TriangleList;
            break;
        }

     

        // Now render all primitives using the shader (if there's one).
        if( mShader )
        {
            mShader->Prepare();
            for( UInt32 iPass = 0; iPass < mShader->GetPassCount(); iPass++ )
            {
                mShader->Apply( iPass );
                renderer->DrawIndexedPrimitive( type, 0, mBufIndices->GetItemCount() );
            }
            mShader->Done();
        }
        else
        {
            renderer->DrawIndexedPrimitive( type, 0, mBufIndices->GetItemCount() );
        }

        renderer->SetRenderState( Renderer::Lighting, true );
        
// Draw debug
#if 0
        renderer->SetRenderState( Renderer::Lighting, false );
        renderer->BeginScene( Renderer::LineList );

        renderer->SetColor( 0.0f, 0.0f, 1.0f, 1.0f );
        renderer->SetVertex( -50,0,0 );
        renderer->SetVertex( 50,0,0 );

        renderer->SetColor( 0.0f, 1.0f, 0 , 1.0f );
        renderer->SetVertex( 0,-50,0 );
        renderer->SetVertex( 0,50,0 );

        renderer->SetColor( 1.0f, 0, 0 , 1.0f );
        renderer->SetVertex( 0,0,-50 );
        renderer->SetVertex( 0,0,50 );

        renderer->EndScene();

        renderer->BeginScene( Renderer::PointList );

        for( UInt32 i = 0; i < mVertexList.GetVertexCount(); i++ )
        {
            renderer->SetVertex( *(const_cast<Mesh*>(this)->mVertexList.GetPositions() + i) );
        }

        renderer->EndScene();

        renderer->SetRenderState( Renderer::Lighting, true );
#endif
    }

	if(pRenderChild)
	{
		Vector<Mesh*>::const_iterator itChild;
		for( itChild = mChildMeshes.begin(); itChild != mChildMeshes.end(); ++itChild )
		{
			(*itChild)->Render();
		}
	}
}

VertexList& Mesh::GetVertexList()
{
    return mVertexList;
}

TriangleBatch& Mesh::GetTriangles()
{
    return mTriangles;
}

const BoundingBox& Mesh::GetBoundingBox() const
{
	return mBoundingBox;
}

void Mesh::AddChild( Mesh* pMesh )
{
    mChildMeshes.push_back( pMesh );
}

Bool Mesh::LineCheck( const Ray3f& pRay ) const
{
    // Test intersection on self
    if( mkDOPTree.LineCheck( pRay ) )
        return true;

    // Test intersection on childs (transform ray for them too).
    Vector<Mesh*>::const_iterator itChild;
	for( itChild = mChildMeshes.begin(); itChild != mChildMeshes.end(); ++itChild )
    {
        /*
        // Compute the current transformation.
	    Matrix4f translation = Matrix4f::Translation((*itChild)->mPosition);
		Matrix4f rotation; 

        (*itChild)->mOrientation.ToMatrix(rotation);
		Matrix4f trsMatrix = (rotation * translation).GetAffineInverse();
        
        // Transform global ray to local        
        Ray3f ray( pRay.GetOrigin() * trsMatrix, trsMatrix.Transform( pRay.GetDirection(), 0 ) );     
        */
        if( (*itChild)->LineCheck( pRay ) )
            return true;
    }

    return false;
}


} // namespace Gamedesk
