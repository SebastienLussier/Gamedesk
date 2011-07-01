/**
 *  @file       LWObject.h
 *  @brief      Lightwave objects representation.
 *  @author     Sébastien Lussier.
 *  @date       12/01/04.
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
#include "LWObject.h"


LWObject::~LWObject()
{
    // Delete all layers.
    Vector<LWLayer*>::iterator   itLayers;
    for( itLayers = mLayers.begin(); itLayers != mLayers.end(); ++itLayers )
        GD_DELETE(*itLayers);

     // Delete all clips.
    Vector<LWClip*>::iterator            itClips;
    for( itClips = mClips.begin(); itClips != mClips.end(); ++itClips )
        GD_DELETE(*itClips);

    // Delete all surfaces.
    Vector<LWSurface*>::iterator         itSurfaces;
    for( itSurfaces = mSurfaces.begin(); itSurfaces != mSurfaces.end(); ++itSurfaces )
        GD_DELETE(*itSurfaces);
}


void LWObject::GetBoundingBox( LWPointList& pPointList, LWBoundingBox& pBoundingBox  )
{
    if( pPointList.mPointsCount == 0 )
        return;

    if( pBoundingBox.mMin != Vector3f(0.0f,0.0f,0.0f) || pBoundingBox.mMax != Vector3f(0.0f,0.0f,0.0f) )
        return;

    pBoundingBox.mMin = Vector3f(  1e20f,  1e20f,  1e20f );
    pBoundingBox.mMax = Vector3f( -1e20f, -1e20f, -1e20f );
    
    for( UInt32 i = 0; i < pPointList.mPointsCount; i++ )
    {
        if( pBoundingBox.mMin.x > pPointList.mPoints[i].mPos.x )    pBoundingBox.mMin.x = pPointList.mPoints[i].mPos.x;
        if( pBoundingBox.mMin.y > pPointList.mPoints[i].mPos.y )    pBoundingBox.mMin.y = pPointList.mPoints[i].mPos.y;
        if( pBoundingBox.mMin.z > pPointList.mPoints[i].mPos.z )    pBoundingBox.mMin.z = pPointList.mPoints[i].mPos.z;

        if( pBoundingBox.mMax.x < pPointList.mPoints[i].mPos.x )    pBoundingBox.mMax.x = pPointList.mPoints[i].mPos.x;
        if( pBoundingBox.mMax.y < pPointList.mPoints[i].mPos.y )    pBoundingBox.mMax.y = pPointList.mPoints[i].mPos.y;
        if( pBoundingBox.mMax.z < pPointList.mPoints[i].mPos.z )    pBoundingBox.mMax.z = pPointList.mPoints[i].mPos.z;
    }
}


void LWObject::GetPolyNormals( LWPointList& pPointList, LWPolygonList& pPolygonList )
{
    Vector3f    p1, p2, pn;
    LWPolygon*  polygon;
    
    Vector<LWPolygon*>::iterator itPolygon;
    for( itPolygon = pPolygonList.mPolygons.begin(); itPolygon != pPolygonList.mPolygons.end(); ++itPolygon )
    {
        polygon = (*itPolygon);

        if( polygon->mVertexCount < 3 ) 
            continue;
        
        p1 = pPointList.mPoints[ polygon->mVertex[ 0 ].mIndex ].mPos;
        p2 = pPointList.mPoints[ polygon->mVertex[ 1 ].mIndex ].mPos;
        pn = pPointList.mPoints[ polygon->mVertex[polygon->mVertexCount - 1].mIndex ].mPos;
        
        polygon->mNormal = (p2 - p1) cross (pn - p1);
        polygon->mNormal.Normalize();
    }
}


void LWObject::GetPointPolygons( LWPointList& pPointList, LWPolygonList& pPolygonList )
{
    UInt32      i;
    LWIndex     index;
    LWIndex     polyIndex = 0;
    LWPolygon*  polygon;

    // Count the number of polygons per point.
    Vector<LWPolygon*>::iterator itPolygon;
    for( itPolygon = pPolygonList.mPolygons.begin(); itPolygon != pPolygonList.mPolygons.end(); ++itPolygon )
    {
        polygon = (*itPolygon);

        for( i = 0; i < polygon->mVertexCount; i++ )
            pPointList.mPoints[polygon->mVertex[i].mIndex].mPolygonsCount++;
    }

    // Alloc per-point polygon arrays.
    for( i = 0; i < pPointList.mPointsCount; i++ )
    {
        if( pPointList.mPoints[i].mPolygonsCount == 0 ) 
            continue;

        pPointList.mPoints[i].mPolygons = GD_NEW_ARRAY(LWIndex, pPointList.mPoints[i].mPolygonsCount, this, "Polygons");
        pPointList.mPoints[i].mPolygonsCount = 0;   // Reset polygon count (see next loop)
    }

    // Fill in polygon array for each point.
    for( itPolygon = pPolygonList.mPolygons.begin(); itPolygon != pPolygonList.mPolygons.end(); ++itPolygon, polyIndex++ )
    {
        polygon = (*itPolygon);

        for( i = 0; i < polygon->mVertexCount; i++ )
        {
            index = polygon->mVertex[i].mIndex;

            pPointList.mPoints[index].mPolygons[pPointList.mPoints[index].mPolygonsCount] = polyIndex;
            pPointList.mPoints[index].mPolygonsCount++;
        }
    }
}


void LWObject::ResolvePolySurfaces( LWPolygonList& pPolygonList )
{
    std::map<LWTag,LWSurface*>  mTagSurfaceMap;

    LWPolygon*      polygon;
    UInt32          index;

    if( mTags.size() == 0 )
        return;
    
    Vector<LWTag>::iterator         itTag;
    Vector<LWSurface*>::iterator    itSurface;

    for( itTag = mTags.begin(); itTag != mTags.end(); ++itTag ) 
    {
        for( itSurface = mSurfaces.begin(); itSurface != mSurfaces.end(); ++itSurface )
        {
            // If the name of the surface match with the tag string 
            if( (*itSurface)->mSurfaceName == (*itTag) )
            {
                mTagSurfaceMap[(*itTag)] = (*itSurface);
                break;
            }
        }
    }

    Vector<LWPolygon*>::iterator    itPolygon;
    for( itPolygon = pPolygonList.mPolygons.begin(); itPolygon != pPolygonList.mPolygons.end(); ++itPolygon )
    {
        polygon = (*itPolygon);
        
        index = polygon->mSurfaceIndex;

        if( index >= mTags.size() )
        {
            OutputDebugString( "Invalid tag index!" );
            continue;
        }

        LWSurface* surface = NULL;
        std::map<LWTag,LWSurface*>::iterator itMap = mTagSurfaceMap.find( mTags[index] );
        if( itMap != mTagSurfaceMap.end() )
            surface = itMap->second;
                                
        if( !surface ) 
        {
            surface = GD_NEW(LWSurface, this, "Surface");
            surface->mSurfaceName = mTags[index];
            mSurfaces.push_back(surface);
        }

        polygon->mSurface = surface;
    }
}


void LWObject::GetVertNormals( LWPointList& pPointList, LWPolygonList& pPolygonList )
{
    UInt32          n, g, h, p;
    Float           angle;
    LWPolygon*      polygon;

    Vector<LWPolygon*>::iterator    itPolygon;
    for( itPolygon = pPolygonList.mPolygons.begin(); itPolygon != pPolygonList.mPolygons.end(); ++itPolygon )
    {
        polygon = (*itPolygon);
        
        for( n = 0; n < polygon->mVertexCount; n++ ) 
        {
            polygon->mVertex[n].mNormal = polygon->mNormal;

            if( polygon->mSurface->mMaxSmoothingAngle <= 0 )
                continue;

            p = polygon->mVertex[n].mIndex;

            for( g = 0; g < pPointList.mPoints[p].mPolygonsCount; g++ )
            {
                h = pPointList.mPoints[p].mPolygons[g];
                
                if( pPolygonList.mPolygons[h] == polygon ) 
                    continue;

                if ( polygon->mSmoothingGroupIndex != pPolygonList.mPolygons[h]->mSmoothingGroupIndex )
                    continue;
                
                angle = Maths::ACos( polygon->mNormal dot pPolygonList.mPolygons[h]->mNormal );
                
                if( angle > polygon->mSurface->mMaxSmoothingAngle ) 
                    continue;

                polygon->mVertex[ n ].mNormal += pPolygonList.mPolygons[ h ]->mNormal;
            }

            polygon->mVertex[n].mNormal.Normalize();
        }
    }
}

    
void LWObject::GetPointVMaps( LWPointList& pPointList, LWVertexMapList& pVertexMapList )
{
    UInt32                      i, j, n;
    
    LWVertexMapList::iterator   itVertexMap;
    LWVertexMap*                vertexMap;
    Vector<LWIndex>::iterator   itIndex;

    // Count the number of vmap values for each point.
    for( itVertexMap = pVertexMapList.begin(); itVertexMap != pVertexMapList.end(); ++itVertexMap )
    {
        vertexMap = (*itVertexMap);
        if( !vertexMap->mPerPoly )
        {
            for( itIndex = vertexMap->mVertexIndex.begin(); itIndex != vertexMap->mVertexIndex.end(); ++itIndex )
                pPointList.mPoints[(*itIndex)].mVertexMapsCount++;
        }
    }

    // Allocate vmap references for each mapped point.
    for( i = 0; i < pPointList.mPointsCount; i++ )
    {
        if( pPointList.mPoints[i].mVertexMapsCount > 0 )
        {
            pPointList.mPoints[i].mVertexMaps = GD_NEW_ARRAY(LWVMapPt, pPointList.mPoints[i].mVertexMapsCount, this, "VertexMap");
            pPointList.mPoints[i].mVertexMapsCount = 0; // Reset vertex mapping count (see next loop)
        }
    }

    // Fill in vmap references for each mapped point.
    for( itVertexMap = pVertexMapList.begin(); itVertexMap != pVertexMapList.end(); ++itVertexMap )
    {
        vertexMap = (*itVertexMap);
        if( !vertexMap->mPerPoly )
        {
            for( i = 0, itIndex = vertexMap->mVertexIndex.begin(); itIndex != vertexMap->mVertexIndex.end(); ++itIndex, ++i )
            {
                j = (*itIndex);
                n = pPointList.mPoints[j].mVertexMapsCount;

                pPointList.mPoints[j].mVertexMaps[n].mVMap  = vertexMap;
                pPointList.mPoints[j].mVertexMaps[n].mIndex = i;
                pPointList.mPoints[j].mVertexMapsCount++;
            }
        }
    }
}


void LWObject::GetPolyVMaps( LWPolygonList& pPolygonList, LWVertexMapList& pVertexMapList )
{
    UInt32                          i, j;
       
    LWVertexMap*                    vertexMap;
    LWPolygonVertex*                polyVertex;
    LWPolygon*                      polygon;

    LWVertexMapList::iterator       itVertexMap;
    Vector<LWIndex>::iterator       itIndex;
    Vector<LWPolygon*>::iterator    itPolygon;

    // Count the number of vmap values for each polygon vertex.
    for( itVertexMap = pVertexMapList.begin(); itVertexMap != pVertexMapList.end(); ++itVertexMap )
    {
        vertexMap = (*itVertexMap);
        if( vertexMap->mPerPoly )
        {
            for( i = 0, itIndex = vertexMap->mPolygonIndex.begin(); itIndex != vertexMap->mPolygonIndex.end(); ++itIndex, ++i )
            {
                polygon = pPolygonList.mPolygons[(*itIndex)];
                for( j = 0; j < polygon->mVertexCount; j++ )
                {
                    polyVertex = &polygon->mVertex[j];
                    if( vertexMap->mVertexIndex[i] == polyVertex->mIndex )
                    {
                        polyVertex->mVertexMapsCount++;
                        break;
                    }
                }
            }
        }
    }

    // Allocate vmap references for each mapped vertex.      
    for( itPolygon = pPolygonList.mPolygons.begin(); itPolygon != pPolygonList.mPolygons.end(); ++itPolygon ) 
    {
        polygon = (*itPolygon);
        for( i = 0; i < polygon->mVertexCount; i++ )
        {
            polyVertex = &polygon->mVertex[i];
            if( polyVertex->mVertexMapsCount > 0 )
            {
                polyVertex->mVertexMaps      = GD_NEW_ARRAY(LWVMapPt, polyVertex->mVertexMapsCount, this, "VertexMap");
                polyVertex->mVertexMapsCount = 0;   // Reset vertex mapping count (see next loop)
            }
        }
    }

    Bool bFound = false;
    // Fill in vmap references for each mapped point.
    for( itVertexMap = pVertexMapList.begin(); itVertexMap != pVertexMapList.end(); ++itVertexMap )
    {
        vertexMap = (*itVertexMap);
        if( vertexMap->mPerPoly )
        {

            for( i = 0, itIndex = vertexMap->mPolygonIndex.begin(); itIndex != vertexMap->mPolygonIndex.end(); ++itIndex, ++i )
            {
                bFound = false;
                polygon = pPolygonList.mPolygons[(*itIndex)];
                for ( j = 0; j < polygon->mVertexCount; j++ )
                {
                    polyVertex = &polygon->mVertex[j];
                    
                    if ( vertexMap->mVertexIndex[i] == polyVertex->mIndex )
                    {
                        polyVertex->mVertexMaps[polyVertex->mVertexMapsCount].mVMap  = vertexMap;
                        polyVertex->mVertexMaps[polyVertex->mVertexMapsCount].mIndex = i;
                        polyVertex->mVertexMapsCount++;
                        bFound = true;
                        break;
                    }
                }
            }
        }
    }
}
