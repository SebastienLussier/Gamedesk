/**
 *  @file       LWOImporter.cpp
 *  @brief      Lightwave model importer for Gamedesk.
 *  @author     Sébastien Lussier.
 *  @date       07/01/04.
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
#include "LWOImpExp.h"
#include "LWOImporter.h"

#include "LWOReader.h"
#include "LWOTags.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Mesh/Mesh.h"
#include "Graphic/Material/Material.h"
#include "Graphic/Shader/Shader.h"

#include "SystemInfo/SystemInfo.h"


IMPLEMENT_CLASS(LWOImporter);


LWOImporter::LWOImporter()
{
    AddSupportedType( "lwo" );
}

Class* LWOImporter::GetResourceClass()
{
    return Mesh::StaticClass();
}

Resource* LWOImporter::Import( const String& pFilename, const String& /*pParams*/ )
{
    LWObject lwObject;

    try
    {
        LWOReader   reader;
        reader.Read( pFilename, lwObject );
    }
    catch( Exception& /*e*/ )
    {
        return NULL;
    }    

    ResolveVertexUV( lwObject );

    
    Mesh*  newMesh = Cast<Mesh>(GraphicSubsystem::Instance()->Create( Mesh::StaticClass() ));
   
    // Build the vertex list.
    Vector<LWVertex>            newVertices;
    Vector<LWVertex>::iterator  itVertex;
    DuplicatePoints( lwObject, newVertices );
/*    
    newMesh->GetVertexList().Allocate( newVertices.size(), (VertexFormat::Component) ( VertexFormat::Position3 | 
                                                                                       VertexFormat::Normal3 |
                                                                                       VertexFormat::Color4 |
                                                                                       VertexFormat::TexCoord2) );
    
    Vector3f*       ptrPosition = newMesh->GetVertexList().GetPositions();
    Vector3f*       ptrNormal   = newMesh->GetVertexList().GetNormals();
    Color4f*        ptrColor4   = newMesh->GetVertexList().GetColors();
    Vector2f*       ptrTexCoord = newMesh->GetVertexList().GetTextureCoords();

    UInt32 i;
    for( i = 0, itVertex = newVertices.begin(); itVertex != newVertices.end(); ++itVertex, ++i )
    {
        ptrPosition[i]  = (*itVertex).mPos;
        ptrNormal[i]    = (*itVertex).mNormal;
        ptrColor4[i]    = (*itVertex).mColor;
        ptrTexCoord[i]  = (*itVertex).mUV;
    }
*/
    // Build the triangle list.
    Vector<LWTriangle>              newTriangles;
    Vector<LWTriangle>::iterator    itTriangle;
    UInt32                          i;
    Triangulate( lwObject, newTriangles );
        
    Vector<LWSurface*>::iterator    itSurfs;
    for( itSurfs = lwObject.mSurfaces.begin(); itSurfs != lwObject.mSurfaces.end(); ++itSurfs )
    {
        // fill a list of triangle
        Vector<LWTriangle*> tris;
        for( i = 0, itTriangle = newTriangles.begin(); itTriangle != newTriangles.end(); ++itTriangle, i+=3 )
        {
            if( (*itTriangle).mSurface == (*itSurfs) )
                tris.push_back( &(*itTriangle) );
        }

        if( tris.size() > 0 )
        {
            Mesh*  child = Cast<Mesh>(GraphicSubsystem::Instance()->Create( Mesh::StaticClass() ));
            child->mShader = GD_NEW(TextureShader, this, "Texture Shader");

            Color4f baseColor( (*itSurfs)->mBaseColor.R, (*itSurfs)->mBaseColor.G, (*itSurfs)->mBaseColor.B, 1.0f );
            
            ((TextureShader*)child->mShader)->mMaterial.mAmbient   = baseColor * (*itSurfs)->mDiffuse;
            ((TextureShader*)child->mShader)->mMaterial.mDiffuse   = baseColor * (*itSurfs)->mDiffuse;
            ((TextureShader*)child->mShader)->mMaterial.mSpecular  = baseColor * (*itSurfs)->mSpecular;
            ((TextureShader*)child->mShader)->mMaterial.mShininess = 128 * (*itSurfs)->mSpecularGlossiness;
            ((TextureShader*)child->mShader)->mMaterial.mEmissive  = baseColor * (*itSurfs)->mLuminosity;
            
            child->GetVertexList().Allocate( newVertices.size(), (VertexFormat::Component) ( VertexFormat::Position3 | VertexFormat::Normal3) );
    
            Vector3f*       ptrPosition = child->GetVertexList().GetPositions();
            Vector3f*       ptrNormal   = child->GetVertexList().GetNormals();

            UInt32 i;
            for( i = 0, itVertex = newVertices.begin(); itVertex != newVertices.end(); ++itVertex, ++i )
            {
                ptrPosition[i]  = (*itVertex).mPos;
                ptrNormal[i]    = (*itVertex).mNormal;
            }

            // copier les triangles de la liste
            child->GetTriangles().Allocate( TriangleBatch::TriangleList, tris.size() * 3 );
            UInt16* ptrIndices = child->GetTriangles().GetIndices();

            Vector<LWTriangle*>::iterator it;
            for( i = 0, it = tris.begin(); it != tris.end(); ++it, i+=3 )
            {
                ptrIndices[i+0] = (*it)->mPt1;
                ptrIndices[i+1] = (*it)->mPt2;
                ptrIndices[i+2] = (*it)->mPt3;
            }    

            newMesh->SetName( (*itSurfs)->mSurfaceName );
            newMesh->AddChild( child );
        }
    }        
        
   
    
    return newMesh;
}

void LWOImporter::ResolveVertexUV( LWObject& pObject )
{
    Vector<LWLayer*>::iterator      itLayer;
    Vector<LWPolygon*>::iterator    itPolygon;

    LWLayer*                        layer;
    LWPolygon*                      polygon;
    LWPolygonVertex*                vertex;
    LWVMapPt*                       vMapPoint;
    LWPoint*                        point;
    Bool                            found;
       
    // For each layer.
    for( itLayer = pObject.mLayers.begin(); itLayer != pObject.mLayers.end(); ++itLayer )
    {
        layer = (*itLayer);
    
        // For each polygon.
        for( itPolygon = layer->mPolygonList.mPolygons.begin(); itPolygon != layer->mPolygonList.mPolygons.end(); ++itPolygon )
        {
            polygon = (*itPolygon);

            // For each vertex.
            for( UInt32 iVertex = 0; iVertex < polygon->mVertexCount; ++iVertex )
            {
                vertex = &polygon->mVertex[iVertex];

                // Look for per poly UV first (VMAD)
                found = false;

                // For each vmap on this vertex.
                for( UInt32 iVMap = 0; iVMap < vertex->mVertexMapsCount; iVMap++ )
                {
                    vMapPoint = &vertex->mVertexMaps[iVMap];

                    // If we've got an UV vertex mapping
                    if( vMapPoint->mVMap->mType == ID_TXUV )
                    {
                        vertex->mUV = vMapPoint->mVMap->mValues[vMapPoint->mIndex].mUV;
                        found = true;
                        break;
                    }
                }

                // No UV per poly, look for per point UV then
                if( !found )
                {
                    point = &layer->mPointList.mPoints[vertex->mIndex];

                    for( UInt32 iVMap = 0; iVMap < point->mVertexMapsCount; iVMap++ )
                    {
                        vMapPoint = &point->mVertexMaps[iVMap];

                        // If we've got an UV vertex mapping
                        if( vMapPoint->mVMap->mType == ID_TXUV )
                        {
                            vertex->mUV = vMapPoint->mVMap->mValues[vMapPoint->mIndex].mUV;
                            found = true;
                            break;
                        }
                    }
                }

                if( !found )
                {
                    vertex->mUV.U = 0;
                    vertex->mUV.V = 0;
                }
            }
        }
    }        
}

void LWOImporter::DuplicatePoints( LWObject& pObject, Vector<LWVertex>& pVertices )
{
    // Create the final list of points, with duplicate (for normals, color and UV diff)
    LWVertex                    vertex;
    
    LWLayer*                    layer;
    Vector<LWLayer*>::iterator  itLayer;

    LWPoint*                    point;
    UInt32                      indexOffset;

    LWPolygon*                  poly;

    Vector<LWVertex>            newVertices;
    Bool                        bContain = false;
    UInt32                      i;

    UInt32 pointsCount = 0;
    for( itLayer = pObject.mLayers.begin(); itLayer != pObject.mLayers.end(); ++itLayer )
        pointsCount += (*itLayer)->mPointList.mPointsCount;

    pVertices.clear();
    pVertices.reserve( pointsCount * 3 );
    
    // For each layer
    for( itLayer = pObject.mLayers.begin(); itLayer != pObject.mLayers.end(); ++itLayer )
    {        
        layer = (*itLayer);

        // For each point
        for( UInt32 iPtIndex = 0; iPtIndex < layer->mPointList.mPointsCount; ++iPtIndex )
        {
            point       = &layer->mPointList.mPoints[iPtIndex];
            indexOffset = pVertices.size();

            newVertices.clear();
            newVertices.reserve( point->mPolygonsCount * 3 );
            
            // For each polygon using this point.
            for( UInt32 iPoly = 0; iPoly < point->mPolygonsCount; ++iPoly )
            {
                poly = layer->mPolygonList.mPolygons[point->mPolygons[iPoly]];

                // Find the polygon vertex using the point.
                for( UInt32 iVertex = 0; iVertex < poly->mVertexCount; ++iVertex )
                {
                    // If the index of the vertex point match the point index... we've found it.
                    if( poly->mVertex[iVertex].mIndex == iPtIndex )
                    {
                        vertex.mPos      = point->mPos;
                        vertex.mNormal   = poly->mVertex[iVertex].mNormal;
                        vertex.mColor.R  = poly->mSurface->mBaseColor.R;
                        vertex.mColor.G  = poly->mSurface->mBaseColor.G;
                        vertex.mColor.B  = poly->mSurface->mBaseColor.B;
                        vertex.mColor.A  = poly->mSurface->mAlphaValue;
                        vertex.mUV.x     = poly->mVertex[iVertex].mUV.U;
                        vertex.mUV.y     = poly->mVertex[iVertex].mUV.V;

                        // See if the vertex is in the new vertex list
                        bContain = false;
                        for( i = 0; i < newVertices.size(); ++i )
                        {
                            if( newVertices[i] == vertex )
                            {
                                bContain = true;
                                poly->mVertex[iVertex].mNewIndex = indexOffset + i; 
                                break;
                            }
                        }

                        if( !bContain )
                        {
                            poly->mVertex[iVertex].mNewIndex = pVertices.size();
                            newVertices.push_back(vertex);
                            pVertices.push_back(vertex);
                        }
                        break;
                    }
                }
            }
        }
    }
}


void LWOImporter::Triangulate( LWObject& pObject, Vector<LWTriangle>& pTriangles )
{
    LWLayer*                    layer;
    Vector<LWLayer*>::iterator  itLayer;

    LWPolygon*                      polygon;
    Vector<LWPolygon*>::iterator    itPolygon;

    LWTriangle triangle;

    // For each layer
    for( itLayer = pObject.mLayers.begin(); itLayer != pObject.mLayers.end(); ++itLayer )
    {
        layer = (*itLayer);

        // For each polygon
        for( itPolygon = layer->mPolygonList.mPolygons.begin(); itPolygon != layer->mPolygonList.mPolygons.end(); ++itPolygon )
        {
            polygon = (*itPolygon);
            
            if( polygon->mType == ID_FACE )
            {
                if( polygon->mVertexCount == 3 )
                {
                    triangle.mPt1       = polygon->mVertex[0].mNewIndex;
                    triangle.mPt2       = polygon->mVertex[1].mNewIndex;
                    triangle.mPt3       = polygon->mVertex[2].mNewIndex;
                    triangle.mSurface   = polygon->mSurface;
                    pTriangles.push_back(triangle);
                }
            }
        }
    }
}