/**
 *  @file       MD5Importer.cpp
 *  @brief      Doom 3 MD5 mesh/anim importer.
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
#include "MD5ImporterDll.h"
#include "MD5Importer.h"

#include "MD5Mesh.h"
#include "MD5Reader.h"

#include "FileManager/FileManager.h"
#include "Graphic/Mesh/SkeletalMesh.h"
#include "Graphic/Mesh/SkeletalAnim.h"
#include "Graphic/Shader/Shader.h"
#include "Config/ConfigFile.h"

#include "Debug/PerformanceMonitor.h"


IMPLEMENT_CLASS(MD5Importer);


class ShaderDirectoryProxy
{
public:
    ShaderDirectoryProxy()
        : mShaderDirectory(NULL)
    {
    }

    ~ShaderDirectoryProxy()
    {
        if( mShaderDirectory )
            GD_DELETE(mShaderDirectory);
    }

    MD5::ShaderDirectory* operator -> ()
    {
        if( !mShaderDirectory )
            mShaderDirectory = GD_NEW(MD5::ShaderDirectory, this, "ShaderDirectory");
        
        return mShaderDirectory;
    }

private:
    MD5::ShaderDirectory*   mShaderDirectory;
} GShaderDirectory;


MD5Importer::MD5Importer()
{
    ConfigFile config( "MD5Importer.cfg" );
    config.Load();

    mDoomFolder = config.Get( "Importer", "DoomFolder", String("D:\\Jeux\\Doom 3\\demo\\demo00.pk4") );

    GShaderDirectory->FillDirectory( mDoomFolder );

    AddSupportedType( "md5mesh" );
    AddSupportedType( "md5anim" );
}

Class* MD5Importer::GetResourceClass()
{
    return Mesh::StaticClass();
}

Resource* MD5Importer::Import( const String& pFilename, const String& /*pParams*/ )
{
    MD5::MeshFile   meshFile;
    MD5::AnimFile   animFile;
    MD5::MeshReader meshReader( meshFile );
    MD5::AnimReader animReader( animFile );

    meshReader.Read( pFilename );

    Int32 pos = pFilename.rfind( "." );
    if( pos != -1 )
        animReader.Read( pFilename.substr(0,pos) + String(".md5anim") );

    SkeletalMesh* pMesh = CreateSkeletalMesh( meshFile );

    pMesh->AddAnim( CreateAnim( animFile ) );

    return pMesh;
}


SkeletalMesh* MD5Importer::CreateSkeletalMesh( MD5::MeshFile& meshFile )
{
    SkeletalMesh* pMesh = GD_NEW(SkeletalMesh, this, "SkeletalMesh");

    // Counts
    UInt32 meshCount   = meshFile.mMeshes.size();
    UInt32 jointsCount = meshFile.mJoints.size();
    UInt32 weightCount = 0;
    UInt32 indicesCount = 0;
    UInt32 verticesCount = 0;

     // Joints
    pMesh->mBones.resize( jointsCount );
    for( UInt32 iJoint = 0; iJoint < jointsCount; ++iJoint )
    {   
        SkeletalMesh::Bone& bone = pMesh->mBones[iJoint];

        bone.mName         = meshFile.mJoints[iJoint].mName;
        bone.mParentIndex  = meshFile.mJoints[iJoint].mParentIdx;
        bone.mWorldPosition= meshFile.mJoints[iJoint].mPosition * 0.05f;
        bone.mOrientation  = meshFile.mJoints[iJoint].mOrientation;

        if( bone.mParentIndex != -1 )
            bone.mPosition = bone.mWorldPosition - pMesh->mBones[bone.mParentIndex].mWorldPosition;
    }

    // Fill the child array for each bone
    for( UInt32 iJoint = 0; iJoint < jointsCount; ++iJoint )
    {
        if( pMesh->mBones[iJoint].mParentIndex != -1 )
            pMesh->mBones[pMesh->mBones[iJoint].mParentIndex].mChildsIndex.push_back( iJoint );
    }

    // Sections
    pMesh->mSections.resize( meshCount );
    for( UInt32 iMesh = 0; iMesh < meshCount; ++iMesh )
    {
        SkeletalMesh::Section& section = pMesh->mSections[iMesh];

        String shaderName =meshFile.mMeshes[iMesh].mShader;
        MD5::Shader* doomShader = GShaderDirectory->GetShader( shaderName );
        GD_ASSERT( doomShader != NULL );
        
        if( !doomShader->mNonSolid )
        {
            HTexture2D hdl;
    
            String textureName = doomShader->mTextures[MD5::Shader::TexDiffuse];
            Int32 posPoint = textureName.find( '.' );
            if( posPoint != -1 )
                textureName = textureName.substr( 0, posPoint );

            String textureCompressed = mDoomFolder + String("\\dds\\") + textureName + String(".dds");
            String textureNormal     = mDoomFolder + String("\\") + textureName + String(".tga");
            String filename;

            if( FileManager::FileExist( textureCompressed ) )
                hdl.GetTexture( textureCompressed );
            else if( FileManager::FileExist( textureNormal ) )
                hdl.GetTexture( textureNormal );
                   
            if( hdl )
            {
                TextureShader* shader = GD_NEW(TextureShader, this, "TextureShader");
                shader->mTexture = hdl;  
                section.mShader = shader;
            }
        }
       
        section.mIndexStart = indicesCount;
        section.mIndexCount = meshFile.mMeshes[iMesh].mTriangles.size() * 3;

        section.mWeightStart = weightCount;
        section.mWeightCount = meshFile.mMeshes[iMesh].mWeights.size();

        section.mVertexStart = verticesCount;
        section.mVertexCount = meshFile.mMeshes[iMesh].mVertices.size();

        weightCount    += section.mWeightCount;
        indicesCount   += section.mIndexCount;
        verticesCount  += section.mVertexCount;
    }
    
    // Weights
    UInt32 iTWeight = 0;
    pMesh->mWeights.resize( weightCount );
    for( UInt32 iMesh = 0; iMesh < meshCount; ++iMesh )
    {
        // For each weight
        for( UInt32 iWeight = 0; iWeight < meshFile.mMeshes[iMesh].mWeights.size(); ++iWeight )
        {   
            pMesh->mWeights[iTWeight].mBoneIndex = meshFile.mMeshes[iMesh].mWeights[iWeight].mJointIndex;
            pMesh->mWeights[iTWeight].mWeight    = meshFile.mMeshes[iMesh].mWeights[iWeight].mWeight;
            pMesh->mWeights[iTWeight].mPosition  = meshFile.mMeshes[iMesh].mWeights[iWeight].mPosition;

            iTWeight++;
        }
    }

    // Triangle indices
    pMesh->GetTriangles().Allocate( TriangleBatch::TriangleList, indicesCount );
    UInt16* ptrIndices = pMesh->GetTriangles().GetIndices();
    UInt32  index = 0;
    UInt32  vertexOffset = 0;
    for( UInt32 iMesh = 0; iMesh < meshCount; ++iMesh )
    {
        for( UInt32 iTriangle = 0; iTriangle < meshFile.mMeshes[iMesh].mTriangles.size(); ++iTriangle )
        {   
            ptrIndices[index++] = meshFile.mMeshes[iMesh].mTriangles[iTriangle].mVertices[0] + vertexOffset;
            ptrIndices[index++] = meshFile.mMeshes[iMesh].mTriangles[iTriangle].mVertices[1] + vertexOffset;
            ptrIndices[index++] = meshFile.mMeshes[iMesh].mTriangles[iTriangle].mVertices[2] + vertexOffset;
        }

        vertexOffset += meshFile.mMeshes[iMesh].mVertices.size();
    }

    // Vertices texture coordinates & influences
    pMesh->GetVertexList().Allocate( verticesCount, (VertexFormat::Component) (VertexFormat::TexCoord2) );
    Vector2f* ptrTexCoord = pMesh->GetVertexList().GetTextureCoords();
    pMesh->mInfluences.resize( verticesCount );

    UInt32 i = 0;
    UInt32 meshWeightOffset = 0;
    for( UInt32 iMesh = 0; iMesh < meshCount; ++iMesh )
    {               
        for( UInt32 iVertex = 0; iVertex < meshFile.mMeshes[iMesh].mVertices.size(); ++iVertex )
        {
            ptrTexCoord[i]                     = meshFile.mMeshes[iMesh].mVertices[iVertex].mUV;
            pMesh->mInfluences[i].mWeightIndex = meshFile.mMeshes[iMesh].mVertices[iVertex].mWeightStart + meshWeightOffset;
            pMesh->mInfluences[i].mCount       = meshFile.mMeshes[iMesh].mVertices[iVertex].mWeightCount;

            i++;
        }

        meshWeightOffset += meshFile.mMeshes[iMesh].mWeights.size();
    }

    return pMesh;
}




SkeletalAnim* MD5Importer::CreateAnim( MD5::AnimFile& animFile )
{
    SkeletalAnim* anim = GD_NEW(SkeletalAnim, this, "SkeletalAnim")( animFile.mName );

    anim->mBoneAnims.resize( animFile.mJointInfos.size() );
    anim->mAnimLength = (Float)animFile.mAnimFrames.size() / (Float)animFile.mFrameRate;

    UInt32 numFrameAnimated = animFile.mAnimFrames.size();

    Vector<MD5::AnimFrame>::iterator     itFrame;
    Vector<MD5::JointAnim>::iterator     itJoint = animFile.mJointInfos.begin();
    Vector<BoneAnimation>::iterator      itBone  = anim->mBoneAnims.begin();
    
    for( ; itJoint != animFile.mJointInfos.end(); ++itJoint, ++itBone )
    {
        itBone->mName = itJoint->mName;
        
        UInt32 offset = 0;
        UInt32 iChannel = 0;
        for( UInt32 animFileComponent = 1; animFileComponent <= MD5::AnimFile::RotationZ; animFileComponent *= 2 )
        {
            switch( animFileComponent )
            {
            case MD5::AnimFile::TranslationX:  iChannel = 0;    break;
            case MD5::AnimFile::TranslationY:  iChannel = 2;    break;
            case MD5::AnimFile::TranslationZ:  iChannel = 1;    break;
            case MD5::AnimFile::RotationX:     iChannel = 3;    break;
            case MD5::AnimFile::RotationY:     iChannel = 5;    break;
            case MD5::AnimFile::RotationZ:     iChannel = 4;    break;
            }
    
            itBone->mChannels[iChannel].mCurrentKey = 0;

            itBone->mChannels[iChannel].mKeys.resize( 
                (itJoint->mAnimatedComponents & animFileComponent) ? numFrameAnimated : 1 );

            if( itJoint->mAnimatedComponents & animFileComponent )
            {
                UInt32 iFrame = 0;
                for( itFrame = animFile.mAnimFrames.begin(); itFrame != animFile.mAnimFrames.end(); ++itFrame, iFrame++ )
                {
                    itBone->mChannels[iChannel].mKeys[iFrame].mTime  = iFrame / (Float)animFile.mFrameRate;
                    itBone->mChannels[iChannel].mKeys[iFrame].mValue = itFrame->mData[itJoint->mFrameDataOffset+offset];
                    
                    if( iChannel < 3 )
                        itBone->mChannels[iChannel].mKeys[iFrame].mValue *= 0.05f;
                }

                offset++;
            }
        }
        
        
        // Set initial frame
        itBone->mChannels[BoneAnimation::ChannelTransX].mKeys[0].mTime  = 0;
        itBone->mChannels[BoneAnimation::ChannelTransX].mKeys[0].mValue = itJoint->mInitialPos.x;

        itBone->mChannels[BoneAnimation::ChannelTransY].mKeys[0].mTime  = 0;
        itBone->mChannels[BoneAnimation::ChannelTransY].mKeys[0].mValue = itJoint->mInitialPos.y;

        itBone->mChannels[BoneAnimation::ChannelTransZ].mKeys[0].mTime  = 0;
        itBone->mChannels[BoneAnimation::ChannelTransZ].mKeys[0].mValue = itJoint->mInitialPos.z;

        itBone->mChannels[BoneAnimation::ChannelRotX].mKeys[0].mTime  = 0;
        itBone->mChannels[BoneAnimation::ChannelRotX].mKeys[0].mValue = itJoint->mInitialRot.x;

        itBone->mChannels[BoneAnimation::ChannelRotY].mKeys[0].mTime  = 0;
        itBone->mChannels[BoneAnimation::ChannelRotY].mKeys[0].mValue = itJoint->mInitialRot.y;

        itBone->mChannels[BoneAnimation::ChannelRotZ].mKeys[0].mTime  = 0;
        itBone->mChannels[BoneAnimation::ChannelRotZ].mKeys[0].mValue = itJoint->mInitialRot.z;
    }

    return anim;
}





///////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS( ProcImporter );

ProcImporter::ProcImporter()
{
    ConfigFile config( "MD5Importer.cfg" );
    config.Load();

    mDoomFolder = config.Get( "Importer", "DoomFolder", String("D:\\Gugusse\\Jeux\\Doom 3\\base\\base") );
    GShaderDirectory->FillDirectory( mDoomFolder );

    AddSupportedType( "proc" );
}

Class* ProcImporter::GetResourceClass()
{
    return Mesh::StaticClass();
}

Resource* ProcImporter::Import( const String& pFilename, const String& /*pParams*/ )
{
    MD5::ProcFile   procFile;
    MD5::ProcReader procReader( procFile );

    {Profile("ProcImporter");
    procReader.Read( pFilename );
    }

    Core::DebugOut( "Num Areas : %d\n", procFile.mAreas.size() );
    Core::DebugOut( "Num Nodes : %d\n", procFile.mNodes.size() );
    Core::DebugOut( "Num Portals : %d\n", procFile.mPortals.size() );
    Core::DebugOut( "Num Shadow Models : %d\n", procFile.mShadowModels.size() );

    
    // Shit Begin !

    // Count Vertices
    UInt32 verticesCount = 0;
    UInt32 indicesCount = 0;
    UInt32 numSurface = 0;
    Bool   stopCount = false;
    for( Vector<MD5::Area>::iterator it = procFile.mAreas.begin(); it != procFile.mAreas.end(); ++it )
    {
        for( Vector<MD5::Area::Surface>::iterator surfIt = it->mSurfaces.begin(); surfIt != it->mSurfaces.end(); ++surfIt )
        {
            if( verticesCount + surfIt->mVertices.size() > 65535 )
            {
                stopCount = true;
                break;
            }

            if( indicesCount + surfIt->mIndices.size() > 65535 )
            {
                stopCount = true;
                break;
            }

            verticesCount += surfIt->mVertices.size();
            indicesCount += surfIt->mIndices.size();

            numSurface++;
        }

        if( stopCount )
            break;
    }


    Mesh* newMesh = GD_NEW(Mesh, this, "Mesh");
    newMesh->GetVertexList().Allocate( verticesCount, (VertexFormat::Component) (VertexFormat::Position3 | VertexFormat::Normal3 | VertexFormat::TexCoord2) );
    newMesh->GetTriangles().Allocate( TriangleBatch::TriangleList, indicesCount );

    // Vertices
    Vector3f* ptrPosition = newMesh->GetVertexList().GetPositions();
    Vector3f* ptrNormal   = newMesh->GetVertexList().GetNormals();
    Vector2f* ptrTexCoord = newMesh->GetVertexList().GetTextureCoords();
    UInt16*   ptrIndices  = newMesh->GetTriangles().GetIndices();
    
    UInt32 iVertex = 0;
    UInt32 iIndex = 0;
    UInt32 iSurface = 0;
    Bool   stop = false;
    for( Vector<MD5::Area>::iterator it = procFile.mAreas.begin(); it != procFile.mAreas.end(); ++it )
    {
        for( Vector<MD5::Area::Surface>::iterator surfIt = it->mSurfaces.begin(); surfIt != it->mSurfaces.end(); ++surfIt )
        {
            if( iSurface == numSurface )
            {
                stop = true;
                break;
            }

            for( Vector<UInt32>::iterator indexIt = surfIt->mIndices.begin(); indexIt != surfIt->mIndices.end(); ++indexIt )
            {
                ptrIndices[iIndex] = (*indexIt) + iVertex;
                iIndex++;
            }

            for( Vector<MD5::Area::Surface::Vertex>::iterator vertexIt = surfIt->mVertices.begin(); vertexIt != surfIt->mVertices.end(); ++vertexIt )
            {
                ptrPosition[iVertex] = Vector3f(vertexIt->mPosition.x, vertexIt->mPosition.z, vertexIt->mPosition.y) * 0.05f;
                ptrNormal[iVertex]   = Vector3f(vertexIt->mNormal.x, vertexIt->mNormal.z, vertexIt->mNormal.y).GetNormalized();
                ptrTexCoord[iVertex] = vertexIt->mTexCoord;
                iVertex++;
            }
                 
            iSurface++;
        }

        if( stop )
            break;
    }
    // Shit End !
   
    return newMesh;
}