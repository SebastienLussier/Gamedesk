/**
 *  @file       MDLModel.h
 *  @brief      Defines data structures to store MDL model info.
 *  @author     Sébastien Lussier.
 *  @date       30/01/04.
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
#include "MDLModel.h"

namespace NWN
{


class Vertex
{
public:
    Vertex()
    {
    }

    Vertex( const Vector3f pPos, Vector2f pTexCoord, Int32 pGroup ) :
        mPos(pPos),
        mTexCoord(pTexCoord),
        mGroup(pGroup)
    {
    }

    Vertex( const Vertex& pOther ) :
        mPos(pOther.mPos),
        mTexCoord(pOther.mTexCoord),
        mGroup(pOther.mGroup)
    {
    }

    Bool operator == ( const Vertex& pOther ) const
    {
        return mPos.EqualEps( pOther.mPos ) && mTexCoord.EqualEps( pOther.mTexCoord ) && mGroup == pOther.mGroup;
    }
    
    const Vertex& operator = ( const Vertex& pOther )
    {
        mPos = pOther.mPos;
        mTexCoord = pOther.mTexCoord;
        mGroup = pOther.mGroup;
        return *this;
    }

public:
    Vector3f    mPos;
    Vector2f    mTexCoord;
    Int32       mGroup;
};


Int32 Find( const std::list<Vertex>& listVertex, const Vertex& pVertex )
{
    Int32 index = -1;

    Int32 i = 0;
    for( std::list<Vertex>::const_iterator itList = listVertex.begin(); itList != listVertex.end(); ++itList, i++ )
    {
        if( pVertex == (*itList) )
            return i;       
    }    

    return index;
}


void TrimeshGeometryNode::Indexify()
{
    std::list<Vertex>       listVertex;
    
    Vector<Vector3f>     newVerts;
    Vector<Vector2f>     newTexCoords;
    Vector<Color4f>      newColors;
    Vector<Float>        newConstraints; 
            
    mNormals.clear();

    std::list<Vector3f>    normalP;
    std::list<Vector3f>    normalV;

    // Temp
    Vector3f a;
    Vector3f b;
    Face*    face = NULL;
    
    Vector3f pos;
    Vector2f tex;
    Vertex   v;
    Int32    index; 

    UInt32   i = 0;
    
    // Remove duplicate and change indices.
    for( Vector<Face>::iterator itFace = mFaces.begin(); itFace != mFaces.end(); ++itFace, i++ )
    {
        face = &(*itFace);

        // compute face normal
        Vector3f p1 = mVerts[face->mV1];
        Vector3f p2 = mVerts[face->mV2];
        Vector3f p3 = mVerts[face->mV3];

        Vector3f normal;

        a = p3 - p2;
        b = p1 - p2;
        normal = a cross b;
        
        if( normal.GetLengthSqr() == 0 )
        {
            normal.x = 1.0f;
            normal.y = 0;
            normal.z = 0;
        }
        else
        {
            normal.Normalize();
        }

        normalP.push_back(p1);
        normalV.push_back(normal);
        normalP.push_back(p2);
        normalV.push_back(normal);
        normalP.push_back(p3);
        normalV.push_back(normal);
        
        // Vertex 1 ///////////////////////////////////////
        pos     = mVerts[face->mV1];
        if( face->mT1 < mTexCoords.size() )
        {
            tex     = mTexCoords[face->mT1];
        }
        else
        {
            tex     = Vector2f( 0, 0 );
        }
        
        v       = Vertex( pos, tex, face->mM1 );
        index   = Find( listVertex, v );
        if( index == -1 )
        {
            listVertex.push_back(v);
            
            newVerts.push_back( pos );
            newTexCoords.push_back( tex );
            mNormals.push_back( normal );

            if( !mColors.empty() )
                newColors.push_back( mColors[face->mV1] );

            if( !mConstraints.empty() )
                newConstraints.push_back( mConstraints[face->mV1] );

            index = listVertex.size() - 1;
        }
        
        face->mV1 = index;
        face->mT1 = index;
        ///////////////////////////////////////////////////

        // Vertex 2 ///////////////////////////////////////
        pos     = mVerts[face->mV2];
        tex     = face->mT2 < mTexCoords.size() ? mTexCoords[face->mT2] : Vector2f( 0, 0 );
        v       = Vertex( pos, tex, face->mM1 );
        index   = Find( listVertex, v );
        if( index == -1 )
        {
            listVertex.push_back(v);
            
            newVerts.push_back( pos );
            newTexCoords.push_back( tex );
            mNormals.push_back( normal );

            if( !mColors.empty() )
                newColors.push_back( mColors[face->mV2] );

            if( !mConstraints.empty() )
                newConstraints.push_back( mConstraints[face->mV2] );

            index = listVertex.size() - 1;
        }
        face->mV2 = index;
        face->mT2 = index;
        ///////////////////////////////////////////////////

        // Vertex 3 ///////////////////////////////////////           
        pos     = mVerts[face->mV3];
        tex     = face->mT3 < mTexCoords.size() ? mTexCoords[face->mT3] : Vector2f( 0, 0 );
        v       = Vertex( pos, tex, face->mM1 );
        index   = Find( listVertex, v );
        if( index == -1 )
        {
            listVertex.push_back(v);
            
            newVerts.push_back( pos );
            newTexCoords.push_back( tex );
            mNormals.push_back( normal );

            if( !mColors.empty() )
                newColors.push_back( mColors[face->mV3] );

            if( !mConstraints.empty() )
                newConstraints.push_back( mConstraints[face->mV3] );

            index = listVertex.size() - 1;
        }
        
        face->mV3 = index;
        face->mT3 = index;
        ///////////////////////////////////////////////////
    }

    // We now have our new indexed list
    mVerts          = newVerts;
    mTexCoords      = newTexCoords;
    mColors         = newColors;
    mConstraints    = newConstraints;
    
    Vector<Vector3f>    foundV;

    // Compute smooth normals
    while( !normalP.empty() )
    {
        foundV.clear();

        Vector3f  p = normalP.back();

        std::list<Vector3f>::iterator  pi = normalP.begin();
        std::list<Vector3f>::iterator  vi = normalV.begin();
        
        while( pi != normalP.end() )
        {
            Vector3f currentP( *pi );
            Vector3f currentV( *vi );

            if( currentP.EqualEps(p) )
            {
                foundV.push_back(currentV);

                pi = normalP.erase( pi );
                vi = normalV.erase( vi );
            }
            else
            {
                ++pi;
                ++vi;
            }            
        }

        Vector3f normal(0,0,0);

        // Compute smooth normal
        for( Vector<Vector3f>::iterator itList = foundV.begin(); itList != foundV.end(); ++itList )
            normal += (*itList);

        if( normal.GetLengthSqr() == 0 )
        {
            normal.x = 1.0f;
            normal.y = 0;
            normal.z = 0;
        }
        else
        {
            normal.Normalize();
        }

        UInt32 index = 0;
        for( Vector<Vector3f>::iterator itList = mVerts.begin(); itList != mVerts.end(); ++itList, index++ )
        {
            if( p.EqualEps( mVerts[index] ) )
                mNormals[index] = normal;
        }
    }
}


}