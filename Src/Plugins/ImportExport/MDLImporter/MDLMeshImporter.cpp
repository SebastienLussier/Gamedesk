/**
 *  @file       MDLMeshImporter.cpp
 *  @brief      Neverwinter Nights model importer (.mdl).
 *  @author     Sébastien Lussier.
 *  @date       29/01/04.
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
#include "MDLImporter.h"
#include "MDLMeshImporter.h"

#include "MDLReader.h"

#include "Graphic/Mesh/Mesh.h"
#include "Graphic/Shader/Shader.h"
#include "Graphic/Texture/Texture.h"


IMPLEMENT_CLASS(MDLMeshImporter);


MDLMeshImporter::MDLMeshImporter()
{
    AddSupportedType( "mdl" );
}

Class* MDLMeshImporter::GetResourceClass()
{
    return Mesh::StaticClass();
}

Resource* MDLMeshImporter::Import( const String& pFilename, const String& /*pParams*/ )
{
    NWN::Model model( pFilename );
    MDLReader  modelReader;

    modelReader.Read( pFilename, model );

    // Recursivly create all meshes.
    Mesh* pMesh = CreateMesh( model.mGeometry, NULL, model.mAnimations.size() > 0 || model.mSuperModelName != "null" );
	pMesh->SetName(pFilename);

    return pMesh;
}


Mesh* MDLMeshImporter::CreateMesh( NWN::GeometryNode* pNode, Mesh* pParent, Bool pMetallic )
{
    Mesh* mesh = NULL;

    switch( pNode->GetType() )
    {
    // Trimesh (& danglymesh) have geometry, so create it.
    case NWN::DANGLY_MESH_ID:
    case NWN::TRIMESH_ID:
    
        mesh = GD_NEW(Mesh, this, "Mesh");
        if( ((NWN::TrimeshGeometryNode*)pNode)->mRender && !((NWN::TrimeshGeometryNode*)pNode)->mTransparencyHint )
        {
            CreateGeometry( (NWN::TrimeshGeometryNode*)pNode, mesh );
            CreateShader( (NWN::TrimeshGeometryNode*)pNode, mesh, pMetallic );
        }
        break;
        
    case NWN::EMITTER_ID:
    case NWN::DUMMY_ID:
    case NWN::AABB_ID:
        mesh = GD_NEW(Mesh, this, "Mesh");
        break;
    
    default:
        return NULL;
    }    

    // Create all childs recursivly.
    for( std::map<String,NWN::Node*>::iterator itMap = pNode->mChilds.begin(); itMap != pNode->mChilds.end(); ++itMap )
        CreateMesh( (NWN::GeometryNode*)(itMap->second), mesh, pMetallic );
    
    // Add the new mesh to it's parent (and store his relative position and orientation).
    if( pParent )
	{
		mesh->SetName(pNode->mName);
		//mesh->SetPosition(((NWN::DummyGeometryNode*)pNode)->mPosition);
		//mesh->SetOrientation(Quaternionf( ((NWN::DummyGeometryNode*)pNode)->mRotationVector, ((NWN::DummyGeometryNode*)pNode)->mRotationAngle));
        pParent->AddChild(mesh);
	}

    return mesh;
}

void MDLMeshImporter::CreateGeometry( NWN::TrimeshGeometryNode* pTrimesh, Mesh* pMesh )
{
    pTrimesh->Indexify();
    
    pMesh->GetVertexList().Allocate( pTrimesh->mVerts.size(), (VertexFormat::Component) (VertexFormat::Position3 | 
                                                                                          VertexFormat::Normal3 |
                                                                                          /*VertexFormat::Color4 |*/
                                                                                          VertexFormat::TexCoord2) );
    // Vertices
    Vector3f*       ptrPosition = pMesh->GetVertexList().GetPositions();
    Vector3f*       ptrNormal   = pMesh->GetVertexList().GetNormals();
    //Color4f*        ptrColor4   = pMesh->GetVertexList().GetColors();
    Vector2f*       ptrTexCoord = pMesh->GetVertexList().GetTextureCoords();

    UInt32 i;
    
    i = 0;
    for( Vector<Vector3f>::iterator itPos = pTrimesh->mVerts.begin(); itPos != pTrimesh->mVerts.end(); ++itPos, ++i )
        ptrPosition[i]  = *itPos;

    i = 0;
    for( Vector<Vector3f>::iterator itNormal = pTrimesh->mNormals.begin(); itNormal != pTrimesh->mNormals.end(); ++itNormal, ++i )
        ptrNormal[i]    = *itNormal;
/*
    i = 0;
    for( Vector<Color4f>::iterator itColor = pTrimesh->mColors.begin(); itColor != pTrimesh->mColors.end(); ++itColor, ++i )
        ptrColor4[i]    = (*itColor);
*/
    i = 0;
    for( Vector<Vector2f>::iterator itTexCoord = pTrimesh->mTexCoords.begin(); itTexCoord != pTrimesh->mTexCoords.end(); ++itTexCoord, ++i )
        ptrTexCoord[i]  = *itTexCoord;

    // Indices
    pMesh->GetTriangles().Allocate( TriangleBatch::TriangleList, pTrimesh->mFaces.size() * 3 );
    UInt16* ptrIndices = pMesh->GetTriangles().GetIndices();
    i = 0;
    for( Vector<NWN::Face>::iterator itFace = pTrimesh->mFaces.begin(); itFace != pTrimesh->mFaces.end(); ++itFace, i+=3 )
    {
        ptrIndices[i+0] = (*itFace).mV1;
        ptrIndices[i+1] = (*itFace).mV2;
        ptrIndices[i+2] = (*itFace).mV3;
    }    
}

void MDLMeshImporter::CreateShader( NWN::TrimeshGeometryNode* pTrimesh, Mesh* pMesh, Bool pMetallic )
{
    TextureShader* shader = NULL;    

    if( ToLower(pTrimesh->mBitmap) != "null" )
    {
        String textureName = "Data/Textures/";
        if( pTrimesh->mBitmap.find( '.' ) != -1 )
        {
            textureName += pTrimesh->mBitmap;
        }
        else
        {
            textureName += pTrimesh->mBitmap;
            textureName += String(".tga");
        }


        HTexture2D hdl( textureName );

        if( hdl && (hdl->GetFormat() == Image::Format_R8G8B8A8 || hdl->GetFormat() == Image::Format_B8G8R8A8) )
        {
            if( pMetallic )
            {
                //shader = GD_NEW(MetallicShader, this, "MetallicShader");
                //((MetallicShader*)shader)->mReflectionTexture.GetTexture( "Data/Textures/chrome1.tga" );

                shader = GD_NEW(TextureShader, this, "TextureShader");
            }
            else
            {
                shader = GD_NEW(TransparencyTextureShader, this, "TransparencyTextureShader");
            }
        }
        else 
        {
            shader = GD_NEW(TextureShader, this, "TextureShader");
        }
                        
        shader->mTexture = hdl;
    }

    if( !shader )
        shader = GD_NEW(TextureShader, this, "TextureShader");

    shader->mMaterial.mAmbient   = pTrimesh->mAmbient;
    shader->mMaterial.mDiffuse   = pTrimesh->mDiffuse;
    shader->mMaterial.mSpecular  = pTrimesh->mSpecular;
    shader->mMaterial.mShininess = pTrimesh->mShininess;
    shader->mMaterial.mEmissive  = pTrimesh->mEmissive;

    pMesh->mShader = shader;
}



