/**
 *  @file       SkeletalMesh.cpp
 *  @brief	    An animated mesh with bones.
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
#include "Graphic/Mesh/SkeletalMesh.h"
#include "Graphic/Mesh/SkeletalAnim.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/Buffer/VertexBuffer.h"
#include "Graphic/Buffer/IndexBuffer.h"
#include "Graphic/Shader/Shader.h"

#include "SystemInfo/SystemInfo.h"

#include "Maths/Number.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(SkeletalMesh);


SkeletalMesh::Section::Section() 
    : mShader(NULL)
    , mVertexStart(0)
    , mVertexCount(0)
    , mVertexNormals(NULL)
    , mIndexStart(0)
    , mIndexCount(0)
    , mFaceNormals(NULL)
    , mWeightStart(0)
    , mWeightCount(0)
{
}

SkeletalMesh::Section::~Section()
{
    if( mShader )
        GD_DELETE(mShader);

    if( mVertexNormals )
        GD_DELETE_ARRAY(mVertexNormals);

    if( mFaceNormals )
        GD_DELETE_ARRAY(mFaceNormals);
}

SkeletalMesh::SkeletalMesh()
{
}

SkeletalMesh::~SkeletalMesh()
{
}

void SkeletalMesh::Init()
{
    mVertexPositions.resize( mInfluences.size() );
    mVertexNormals.resize( mInfluences.size() );
    mVertexTriangles.resize( mInfluences.size() );
    mTriangleNormals.resize( mTriangles.GetIndicesCount()/3 );

    // Compute each vertex triangles
    UInt32  numTriangles = mTriangleNormals.size();
    UInt16* triIndices = mTriangles.GetIndices();
    for( UInt32 iTri = 0; iTri < numTriangles; iTri++ )
    {
        mVertexTriangles[triIndices[iTri*3 + 0]].push_back(iTri);
        mVertexTriangles[triIndices[iTri*3 + 1]].push_back(iTri);
        mVertexTriangles[triIndices[iTri*3 + 2]].push_back(iTri);
    }    

    // VertexFormat::Position3
    mBufPositions = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
    mBufPositions->Create( mInfluences.size(), sizeof(Vector3f), VertexBuffer::Usage_Dynamic );   
    
    // VertexFormat::Normal3
    mBufNormals = Cast<VertexBuffer>( GraphicSubsystem::Instance()->Create( VertexBuffer::StaticClass() ) );
    mBufNormals->Create( mInfluences.size(), sizeof(Vector3f), VertexBuffer::Usage_Dynamic );   
        
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

    mTimeStart = SystemInfo::Instance()->GetSeconds();
    Update( 0 );
}

void SkeletalMesh::AddAnim( SkeletalAnim* pAnim )
{
    mAnims.push_back( pAnim );
}

void SkeletalMesh::Update( Double /*pElapsed*/ )
{
    Double time = SystemInfo::Instance()->GetSeconds() - mTimeStart;
    if( time >= mAnims[0]->mAnimLength )
        mTimeStart = SystemInfo::Instance()->GetSeconds();
 
    UpdateBone( 0, time );
    ApplyWeight();
}

void SkeletalMesh::UpdateBone( UInt32 pIndex, Float pTime )
{
    Bone& pBone = mBones[pIndex];

    Quaternionf quat;

    GD_ASSERT( mAnims[0]->mBoneAnims[pIndex].mName == pBone.mName );
    
    quat.x = mAnims[0]->mBoneAnims[pIndex].mChannels[BoneAnimation::ChannelRotX].GetValue( pTime );
    quat.y = mAnims[0]->mBoneAnims[pIndex].mChannels[BoneAnimation::ChannelRotY].GetValue( pTime );
    quat.z = mAnims[0]->mBoneAnims[pIndex].mChannels[BoneAnimation::ChannelRotZ].GetValue( pTime ); 
    Float term = 1.0000f - ((quat.x*quat.x) + (quat.y*quat.y) + (quat.z*quat.z));
    quat.w = term < 0.0f ? 0 : -Maths::Sqrt( term );
    
    pBone.mTransfo = quat.ToMatrix( pBone.mTransfo );
    pBone.mTransfo(3,0) = mAnims[0]->mBoneAnims[pIndex].mChannels[BoneAnimation::ChannelTransX].GetValue( pTime ); 
    pBone.mTransfo(3,1) = mAnims[0]->mBoneAnims[pIndex].mChannels[BoneAnimation::ChannelTransY].GetValue( pTime ); 
    pBone.mTransfo(3,2) = mAnims[0]->mBoneAnims[pIndex].mChannels[BoneAnimation::ChannelTransZ].GetValue( pTime ); 
    
    if( pBone.mParentIndex != -1 )
        pBone.mTransfo *= mBones[pBone.mParentIndex].mTransfo;
 
    pBone.mWorldPosition = Vector3f( pBone.mTransfo(3,0), pBone.mTransfo(3,1), pBone.mTransfo(3,2) );
    
    for( UInt32 i = 0; i < pBone.mChildsIndex.size(); i++ )
        UpdateBone( pBone.mChildsIndex[i], pTime );
}

void SkeletalMesh::ApplyWeight()
{      
    mBoundingBox = BoundingBox();
    
    Vector3f pos;

    UInt32 count = mInfluences.size();
    for( UInt32 i = 0; i < count; i++ )
    {
        VertexInfluence& influence = mInfluences[i];
        Vector3f sum(0,0,0);

        for( UInt32 w = 0; w < influence.mCount; w++ )
        {
            Weight& weight = mWeights[influence.mWeightIndex + w];
            Bone&   bone   = mBones[weight.mBoneIndex];
            
            pos = weight.mPosition * bone.mTransfo;
            sum += pos * weight.mWeight;
        }

        mVertexPositions[i] = sum;
        mBoundingBox.Grow( sum );
    }

    // Fill position buffer
    Vector3f* points = reinterpret_cast<Vector3f*>(mBufPositions->Lock( VertexBuffer::Lock_Write ));
    if( points )
        memcpy( points, &(mVertexPositions[0]), mVertexPositions.size() * sizeof(Vector3f) );
    mBufPositions->Unlock();

    ComputeTriangleNormals();
    ComputeVertexNormals();
}

void SkeletalMesh::ComputeTriangleNormals()
{
    Vector3f pos1;
    Vector3f pos2;
    Vector3f pos3;
    UInt16*  indices = mTriangles.GetIndices();
    UInt32   triangleCount = mTriangles.GetIndicesCount()/3;

    for( UInt32 iTri = 0; iTri < triangleCount; iTri++ )
    {
        pos1 = mVertexPositions[ indices[iTri*3 + 0] ];
        pos2 = mVertexPositions[ indices[iTri*3 + 1] ];
        pos3 = mVertexPositions[ indices[iTri*3 + 2] ];

        mTriangleNormals[iTri] = (pos2-pos1) cross (pos3-pos1);
        mTriangleNormals[iTri].Normalize();
    }
}

void SkeletalMesh::ComputeVertexNormals()
{
    UInt32 numVertex = mVertexNormals.size();

    for( UInt32 iVertex = 0; iVertex < numVertex; iVertex++ )
    {
        mVertexNormals[iVertex] = Vector3f(0,0,0);
        
        for( UInt32 iTriangle = 0; iTriangle < mVertexTriangles[iVertex].size(); iTriangle++ )
            mVertexNormals[iVertex] += mTriangleNormals[ mVertexTriangles[iVertex][iTriangle] ];

        mVertexNormals[iVertex].Normalize();
    }

    // Fill position buffer
    Vector3f* normals = reinterpret_cast<Vector3f*>(mBufNormals->Lock( VertexBuffer::Lock_Write ));
    if( normals )
        memcpy( normals, &(mVertexNormals[0]), mVertexNormals.size() * sizeof(Vector3f) );
    mBufNormals->Unlock();
}

void SkeletalMesh::Render(Bool /*pRenderChild*/) const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    
    if( mBufPositions )
    {
        renderer->SetVertexFormat( (VertexFormat::Component)(VertexFormat::Position3 | VertexFormat::TexCoord2 | VertexFormat::Normal3) );

        // VertexFormat::Position3
        renderer->SetStreamSource( VertexFormat::Position3, mBufPositions );

        // VertexFormat::Normal3
        renderer->SetStreamSource( VertexFormat::Normal3, mBufNormals );

        // VertexFormat::TexCoord2
        if( mVertexList.GetVertexFormat().HasComponent( VertexFormat::TexCoord2 ) )
            renderer->SetStreamSource( VertexFormat::TexCoord2, mBufTexCoords );

        // Set active index buffer
        renderer->SetIndices( mBufIndices );

        renderer->SetRenderState( Renderer::CullFace, true );
        renderer->SetCulling( Renderer::CullBackFace );
                
        for( UInt32 iSection = 0; iSection < mSections.size(); iSection++ )
        {
            const Section& section = mSections[iSection];

            if( section.mShader != NULL )
            {
                section.mShader->Prepare();
                for( UInt32 iPass = 0; iPass < section.mShader->GetPassCount(); iPass++ )
                {
                    section.mShader->Apply( iPass );
                    renderer->DrawIndexedPrimitive( Renderer::TriangleList, section.mIndexStart, section.mIndexCount );
                }
                section.mShader->Done();
            }
        }
    }
 
    renderer->SetPolygonMode( Renderer::FrontFace, Renderer::FillSolid );
}

void SkeletalMesh::RenderBones() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    renderer->SetRenderState( Renderer::Lighting, false );
    
    // Render all bones
    renderer->SetColor( Color4f(1.0f, 0.0f, 0.0f, 1.0f) );
    renderer->BeginScene( Renderer::LineList );
    for( UInt32 i = 0; i < mBones.size(); i++ )
    {
        if( mBones[i].mParentIndex != -1 )
        {
            renderer->SetVertex( mBones[i].mWorldPosition );
            renderer->SetVertex( mBones[mBones[i].mParentIndex].mWorldPosition );    
        }
    }    
    renderer->EndScene();
    renderer->SetColor( Color4f(1.0f, 1.0f, 1.0f, 1.0f) );

    renderer->SetRenderState( Renderer::Lighting, true );
}

void SkeletalMesh::RenderNormals() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    renderer->SetRenderState( Renderer::Lighting, false );
    
    // Render all bones
    renderer->SetColor( Color4f(0.0f, 1.0f, 0.0f, 1.0f) );
    renderer->BeginScene( Renderer::LineList );
    for( UInt32 i = 0; i < mVertexNormals.size(); i++ )
    {
        renderer->SetVertex( mVertexPositions[i] );
        renderer->SetVertex( mVertexPositions[i] + (mVertexNormals[i]*0.2f) );    
    }
    renderer->EndScene();
    renderer->SetColor( Color4f(1.0f, 1.0f, 1.0f, 1.0f) );

    renderer->SetRenderState( Renderer::Lighting, true );
}

BoundingBox	SkeletalMesh::GetBoundingBox(const Matrix4f& /*pTransformation*/)
{
    return mBoundingBox;
}


} // namespace Gamedesk
