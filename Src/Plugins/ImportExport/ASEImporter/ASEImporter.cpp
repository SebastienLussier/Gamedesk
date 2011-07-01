/**
 *  @file       ASEImporter.cpp
 *  @brief      ASE importer.
 *  @author     Sébastien Lussier.
 *  @date       03/12/10.
 */
/*
 *  Copyright (C) 2010 Gamedesk
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
#include "ASEImporterDll.h"
#include "ASEImporter.h"

#include "ASEFile.h"
#include "ASEReader.h"

#include "FileManager/FileManager.h"
#include "Graphic/Mesh/SkeletalMesh.h"
#include "Graphic/Mesh/SkeletalAnim.h"
#include "Graphic/Shader/Shader.h"
#include "Config/ConfigFile.h"

#include "Debug/PerformanceMonitor.h"


IMPLEMENT_CLASS(ASEImporter);



ASEImporter::ASEImporter()
{
    AddSupportedType( "ase" );
}

Class* ASEImporter::GetResourceClass()
{
    return Mesh::StaticClass();
}

Resource* ASEImporter::Import( const String& pFilename, const String& /*pParams*/ )
{
    ASEFile aseFile;

    try
    {
        ASE::MeshReader   reader(aseFile);
        reader.Read( pFilename );
    }
    catch( Exception& /*e*/ )
    {
        return NULL;
    }    

    if(aseFile.mGeomObjects.size() == 0)
        return NULL;
	
    Map<const ASEFile::Material*, ASESubMesh> mSubMeshes; 
    GenerateSubMeshes(aseFile, mSubMeshes);

    // Create mesh instance
    Mesh* newMesh = Cast<Mesh>(GraphicSubsystem::Instance()->Create( Mesh::StaticClass() ));
    
    // Create all submeshes
    Map<const ASEFile::Material*, ASESubMesh>::const_iterator itSubMesh;
    Map<const ASEFile::Material*, ASESubMesh>::const_iterator itSubMeshBegin = mSubMeshes.begin();
    Map<const ASEFile::Material*, ASESubMesh>::const_iterator itSubMeshEnd = mSubMeshes.end();
    for(itSubMesh = itSubMeshBegin; itSubMesh != itSubMeshEnd; ++itSubMesh)
    {
        const ASEFile::Material& mat = *(*itSubMesh).first;
        const ASESubMesh& aseSubMesh = (*itSubMesh).second;

        // Create the sub mesh
        Mesh* subMesh = Cast<Mesh>(GraphicSubsystem::Instance()->Create( Mesh::StaticClass() ));
        newMesh->AddChild(subMesh);

        // Create VB
	    subMesh->GetVertexList().Allocate( aseSubMesh.mVertices.size(), (VertexFormat::Component) (VertexFormat::Position3 | VertexFormat::Normal3 | VertexFormat::TexCoord2 ));
        Vector3f* ptrPosition = subMesh->GetVertexList().GetPositions();
        Vector3f* ptrNormal   = subMesh->GetVertexList().GetNormals();
        Vector2f* ptrTexCoord = subMesh->GetVertexList().GetTextureCoords();

        UInt32 i = 0;
        for(Vector<ASEVertex>::const_iterator itVertex = aseSubMesh.mVertices.begin(); itVertex != aseSubMesh.mVertices.end(); ++itVertex, ++i)
        {
            ptrPosition[i].x = (*itVertex).mPosition.x;
            ptrPosition[i].y = (*itVertex).mPosition.z;
            ptrPosition[i].z = (*itVertex).mPosition.y;
            
            ptrNormal[i].x = (*itVertex).mNormal.x; 
            ptrNormal[i].y = (*itVertex).mNormal.z; 
            ptrNormal[i].z = (*itVertex).mNormal.y; 

            ptrTexCoord[i] = (*itVertex).mUV; 
        }

        // Create IB
        {
            subMesh->GetTriangles().Allocate( TriangleBatch::TriangleList, aseSubMesh.mIndices.size());
            UInt16* ptrIndices = subMesh->GetTriangles().GetIndices();
        
            UInt32 i = 0;
            for(Vector<UInt32>::const_iterator itIndex = aseSubMesh.mIndices.begin(); itIndex != aseSubMesh.mIndices.end(); ++itIndex, ++i)
                ptrIndices[i] = *itIndex;
        }

        // Shader
        {
            TextureShader* shader = GD_NEW(TextureShader, this, "TextureShader");

            if(mat.mMapDiffuse.mBitmap.size() != 0)
            {
                HTexture2D hdl( mat.mMapDiffuse.mBitmap );
                hdl->GetImage().FlipY();
                hdl->Update();
                shader->mTexture = hdl;
            }
            else if(mat.mMapGeneric.mBitmap.size() != 0)
            {
                HTexture2D hdl( mat.mMapGeneric.mBitmap );
                hdl->GetImage().FlipY();
                hdl->Update();
                shader->mTexture = hdl;
            }

            shader->mMaterial.mAmbient   = Color4f(mat.mAmbient.R, mat.mAmbient.G, mat.mAmbient.B);
            shader->mMaterial.mDiffuse   = Color4f(mat.mDiffuse.R, mat.mDiffuse.G, mat.mDiffuse.B);
            //shader->mMaterial.mSpecular  = Color4f(mat.mSpecular.R, mat.mSpecular.G, mat.mSpecular.B);
            shader->mMaterial.mShininess = mat.mShine;
            subMesh->mShader = shader;
        }
    }    

	return newMesh;
}

void ASEImporter::GenerateSubMeshes(const ASEFile& pASEFile, Map<const ASEFile::Material*, ASESubMesh>& pSubMeshes)
{    
    ASEFile::GeomObjectList::const_iterator itGeomObject;
    ASEFile::GeomObjectList::const_iterator itGeomObjectBegin = pASEFile.mGeomObjects.begin();
    ASEFile::GeomObjectList::const_iterator itGeomObjectEnd = pASEFile.mGeomObjects.end();

    for(itGeomObject = itGeomObjectBegin; itGeomObject != itGeomObjectEnd; ++itGeomObject)
    {
        const ASEFile::GeomObject& geomObject = *(*itGeomObject);

        Vector<ASEFile::GeomObject::Mesh::Face>::const_iterator itFace;
        Vector<ASEFile::GeomObject::Mesh::Face>::const_iterator itFaceBegin = geomObject.mMesh.mFaceList.begin();
        Vector<ASEFile::GeomObject::Mesh::Face>::const_iterator itFaceEnd = geomObject.mMesh.mFaceList.end();
    
        // For each face
        UInt32 faceIdx = 0;
        for(itFace = itFaceBegin; itFace != itFaceEnd; ++itFace, ++faceIdx)
        {
            const ASEFile::GeomObject::Mesh::Face& face = *itFace;

            // Get the submesh associated with the material used by the face
            const ASEFile::Material* mat = &pASEFile.mMaterialList[geomObject.mMaterialRef];
            if(face.mMaterialID < mat->mSubMaterials.size())
                mat = &mat->mSubMaterials[face.mMaterialID];
            ASESubMesh& subMesh = pSubMeshes[mat];

            // For each point
            for(Int32 pointIdx = 2; pointIdx >= 0; pointIdx--)
            {        
                // Build ASEVertex
                ASEVertex newVertex;
                newVertex.mPosition = geomObject.mMesh.mVertexList[face.mIndices[pointIdx]];
                newVertex.mNormal = geomObject.mMesh.mNormals[faceIdx].mVertexNormals[pointIdx];

                const Vector3f& UV = geomObject.mMesh.mTVertList[geomObject.mMesh.mTVFaceList[faceIdx].mIndices[pointIdx]];
                newVertex.mUV = Vector2f(UV.x, UV.y);

                // Look for point in VB, if not found, insert it
                Vector<ASEVertex>::const_iterator itPos = std::find(subMesh.mVertices.begin(), subMesh.mVertices.end(), newVertex);
                UInt32 idx;
                if(itPos == subMesh.mVertices.end())
                {
                    subMesh.mVertices.push_back(newVertex);
                    idx = subMesh.mVertices.size() - 1;
                }
                else
                {
                    idx = itPos - subMesh.mVertices.begin();
                }

                // Push index of point in index buffer
                subMesh.mIndices.push_back(idx);
            }
        }
    }
}
