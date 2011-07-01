/**
 *  @file       SkyDome.cpp
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       12/02/04.
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
#include "SkyDome.h"

#include "Graphic/Buffer/IndexBuffer.h"
#include "Graphic/Buffer/VertexBuffer.h"
#include "Graphic/Buffer/VertexFormat.h"

#include "Graphic/Mesh/Mesh.h"
#include "Graphic/Texture/Texture.h"
#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
SkyDome::SkyDome() : 
    mTextureName("Data/Sky/sky017.tif")
{
    VertexList      sphereVertice;
    TriangleBatch   sphereTriangles;

    Float radius       = 10000.0f;

    UInt32 nbParallels = 24;
    UInt32 nbMeridians = 24;

    Float dParallel = Maths::PI_2 / nbParallels;
    Float dMeridian = Maths::HALF_PI / nbMeridians;

    UInt32 numVertices = (nbParallels+1) * (nbMeridians+1);
    sphereVertice.Allocate( numVertices, VertexFormat::Component(VertexFormat::Position3 | VertexFormat::TexCoord2) );
    sphereTriangles.Allocate( TriangleBatch::TriangleList, (nbParallels * (nbMeridians-1) * 6) + nbParallels*3 );

    Vector3f*   points      = sphereVertice.GetPositions();
    Vector2f*   texCoords   = sphereVertice.GetTextureCoords();
    UInt16*     indices     = sphereTriangles.GetIndices();
    
    UInt32 n = 0;
    UInt32 i = 0;
    
    Float vy;
    Float len;
    Float theta, phi;
    Float sinPhi, cosPhi;
    
    for( UInt32 iMeridian = 0; iMeridian <= nbMeridians; iMeridian++ )
    {
        phi    = dMeridian*iMeridian;
        sinPhi = Maths::Sin( phi );
        cosPhi = Maths::Cos( phi );

        for( UInt32 iParallel = 0; iParallel <= nbParallels; iParallel++ )
        {
            // Build positions.
            theta = dParallel*iParallel;
            points[n].x = radius * sinPhi * Maths::Cos( theta );
            points[n].y = radius * cosPhi;
            points[n].z = radius * sinPhi * Maths::Sin( theta );

            // Build texture coords.
            vy = points[n].y;
            len = points[n].GetLength();
            vy /= len;

            texCoords[n].x = (float)iParallel / (float)nbParallels;
            texCoords[n].y = Maths::ASin( vy ) / Maths::HALF_PI;

            // Don't generate polygons for the first meridian (top most points)
            // and the last parallels (points duplicated for UV correctness).
            if( iParallel == nbParallels || iMeridian == 0 )
            {
                n += 1;
                continue;
            }

            // Build indices.
            if( iMeridian == 1 )
            {
                // Sphere top most cap.
                indices[i+0] = n+1;                     //  iParallel    / \		|
                indices[i+1] = iParallel;               //              /   \		|
                indices[i+2] = n;                       //           n ------ n+1
                i += 3;
            }
            else
            {
                indices[i+0] = n+1;                     //   n-nbParallels-1 | \	|
                indices[i+1] = n - nbParallels - 1;     //                   |  \	|
                indices[i+2] = n;                       //                 n ---- n+1

                indices[i+3] = n - nbParallels;         //   n-nbParallels-1 ---- n-nbParallels
                indices[i+4] = n - nbParallels - 1;     //                    \ |
                indices[i+5] = n+1;                     //                     \| n+1
                
                i += 6;
            }
            
            n += 1;
        }
    }

    //TriangleBatch triangleStrip;
    //sphereTriangles.Stripify( triangleStrip );
    //sphereVertice.Reorder( triangleStrip );
    
    mBufPositions = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
    mBufPositions->Create( sphereVertice.GetVertexCount(), sizeof(Vector3f), VertexBuffer::Usage_Static );

    mBufTexCoords = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
    mBufTexCoords->Create( sphereVertice.GetVertexCount(), sizeof(Vector2f), VertexBuffer::Usage_Static );

    mBufIndices = Cast<IndexBuffer>( GraphicSubsystem::Instance()->Create( IndexBuffer::StaticClass() ) );
    mBufIndices->Create( sphereTriangles.GetIndicesCount(), sizeof(UInt16), IndexBuffer::Usage_Static );

    Vector3f* pointsBuf = reinterpret_cast<Vector3f*>(mBufPositions->Lock( VertexBuffer::Lock_Write ));
    Vector2f* texCoordsBuf = reinterpret_cast<Vector2f*>(mBufTexCoords->Lock( VertexBuffer::Lock_Write ));
    UInt16*   indicesBuf = reinterpret_cast<UInt16*>(mBufIndices->Lock( IndexBuffer::Lock_Write ));

    memcpy( pointsBuf, sphereVertice.GetPositions(), mBufPositions->GetItemCount() * mBufPositions->GetItemSize() );
    memcpy( texCoordsBuf, sphereVertice.GetTextureCoords(), mBufTexCoords->GetItemCount() * mBufTexCoords->GetItemSize() );
    memcpy( indicesBuf, sphereTriangles.GetIndices(), mBufIndices->GetItemCount() * mBufIndices->GetItemSize() );

    mBufPositions->Unlock();
    mBufTexCoords->Unlock();
    mBufIndices->Unlock();

    SetTexture(mTextureName);
}

SkyDome::~SkyDome()
{
}

void SkyDome::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetRenderState( Renderer::Lighting, false );  
    renderer->SetRenderState( Renderer::DepthMask, false );
    renderer->SetRenderState( Renderer::DepthTest, false );

    renderer->SetVertexFormat( VertexFormat::Component(VertexFormat::Position3 | VertexFormat::TexCoord2) );
    renderer->SetStreamSource( VertexFormat::Position3, mBufPositions );
    renderer->SetStreamSource( VertexFormat::TexCoord2, mBufTexCoords );
    renderer->SetIndices( mBufIndices );
    
    renderer->GetTextureStage(0)->SetTexture( *mTexture );
    renderer->DrawIndexedPrimitive( Renderer::TriangleList, 0, mBufIndices->GetItemCount() );
    renderer->GetTextureStage(0)->ResetTexture();

    renderer->SetRenderState( Renderer::DepthMask, true );
    renderer->SetRenderState( Renderer::DepthTest, true );
    renderer->SetRenderState( Renderer::Lighting, true );
}

void SkyDome::SetTexture(const String& pTextureName)
{
    mTextureName = pTextureName;
    mTexture.GetTexture( mTextureName );
    mTexture->SetWrapMode( Texture::Wrap_S, Texture::Wrap_Repeat );
    mTexture->SetWrapMode( Texture::Wrap_T, Texture::Wrap_Clamp );
}

const String& SkyDome::GetTextureName() const
{
    return mTextureName;
}

IMPLEMENT_CLASS(SkyDome);


} // namespace Gamedesk
