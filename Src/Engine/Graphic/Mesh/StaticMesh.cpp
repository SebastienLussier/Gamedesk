/**
 *  @file       StaticMesh.cpp
 *  @brief	    Mesh with no animated part.
 *  @author     Sébastien Lussier.
 *  @date       18/08/04.
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
#include "Graphic/Mesh/StaticMesh.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/Buffer/VertexBuffer.h"
#include "Graphic/Buffer/IndexBuffer.h"
#include "Graphic/Shader/Shader.h"

#include "Maths/Number.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(StaticMesh);


StaticMesh::StaticMesh()
{
}

StaticMesh::~StaticMesh()
{
}

void StaticMesh::Init()
{
    Super::Init();

    if( mVertexList.GetVertexCount() != 0 )
    {
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
    }
}

void StaticMesh::Render(Bool pRenderChild) const
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
        renderer->SetCulling( Renderer::CullBackFace );
        //renderer->SetPolygonMode( Renderer::FrontFace, Renderer::FillWireframe );

        // Now render all primitives using the shader (if there's one).
        if( mShader )
        {
            mShader->Prepare();
            for( UInt32 iPass = 0; iPass < mShader->GetPassCount(); iPass++ )
            {
                mShader->Apply( iPass );
                renderer->DrawIndexedPrimitive( Renderer::TriangleList, 0, mBufIndices->GetItemCount() );
            }
            mShader->Done();
        }
        else
        {
            renderer->DrawIndexedPrimitive( Renderer::TriangleList, 0, mBufIndices->GetItemCount() );
        }
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


} // namespace Gamedesk
