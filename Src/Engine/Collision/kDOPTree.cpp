/**
 *  @file       kDOPTree.h
 *  @brief	    k-DOP tree (k = 6) data structure.
 *  @author     Sébastien Lussier.
 *  @date       13/03/05.
 */
/*
 *  Copyright (C) 2005 Gamedesk
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
#include "kDOPTree.h"

#include "Maths/Intersection.h"


namespace Gamedesk {
	
	
kDOPTree::Axis kDOPTree::SelectSplitAxis( Vector<UInt16>& pTriangleSoup, Float& mean )
{
    Float       variances[Axis_Num];
    Float       means[Axis_Num];
    Float       sum[Axis_Num];
    Float       invNumTriangle = 1.0f / pTriangleSoup.size();

    // Compute the mean for each axis
    sum[Axis_X] = sum[Axis_Y] = sum[Axis_Z] = 0;
    for( UInt32 i = 0; i < pTriangleSoup.size(); i++ )
    {
        sum[Axis_X] += mTrianglesCentroids[pTriangleSoup[i]] dot Vector3f::X_AXIS;
        sum[Axis_Y] += mTrianglesCentroids[pTriangleSoup[i]] dot Vector3f::Y_AXIS;
        sum[Axis_Z] += mTrianglesCentroids[pTriangleSoup[i]] dot Vector3f::Z_AXIS;
    }
    means[Axis_X] = sum[Axis_X] * invNumTriangle;
    means[Axis_Y] = sum[Axis_Y] * invNumTriangle;
    means[Axis_Z] = sum[Axis_Z] * invNumTriangle;

    // Compute the variance for each axis
    sum[Axis_X] = sum[Axis_Y] = sum[Axis_Z] = 0;
    for( UInt32 i = 0; i < pTriangleSoup.size(); i++ )
    {
        sum[Axis_X] += Maths::Square( (mTrianglesCentroids[pTriangleSoup[i]] dot Vector3f::X_AXIS) - means[Axis_X] );
        sum[Axis_Y] += Maths::Square( (mTrianglesCentroids[pTriangleSoup[i]] dot Vector3f::Y_AXIS) - means[Axis_Y] );
        sum[Axis_Z] += Maths::Square( (mTrianglesCentroids[pTriangleSoup[i]] dot Vector3f::Z_AXIS) - means[Axis_Z] );
    }
    variances[Axis_X] = sum[Axis_X] * invNumTriangle;
    variances[Axis_Y] = sum[Axis_Y] * invNumTriangle;
    variances[Axis_Z] = sum[Axis_Z] * invNumTriangle;

    // We'll split on the axis having the greater variance.
    Axis splitAxis;

    if( variances[Axis_X] > variances[Axis_Y] && variances[Axis_X] > variances[Axis_Z] )
        splitAxis = Axis_X;
    else if( variances[Axis_Y] > variances[Axis_Z] )
        splitAxis = Axis_Y;
    else
        splitAxis = Axis_Z;

    mean = means[splitAxis];

    return splitAxis;
}

void kDOPTree::SplitTriangles( kDOPTree::kDOPNode* pNode, kDOPTree::Axis splitAxis, Float axisMean )
{
    Vector<UInt32> onAxisMean;

    kDOPNode* leftNode  = mNodes[pNode->mChildsIndex + 0];
    kDOPNode* rightNode = mNodes[pNode->mChildsIndex + 1];

    UInt32 index;
    for( UInt32 i = 0; i < pNode->mTriangleIndices.size(); i++ )
    {
        index = pNode->mTriangleIndices[i];

        if( mTrianglesCentroids[index][splitAxis] < axisMean )
            leftNode->mTriangleIndices.push_back(index);
        else if( mTrianglesCentroids[index][splitAxis] > axisMean )
            rightNode->mTriangleIndices.push_back(index);
        else
            onAxisMean.push_back(index);
    }

    // Distribute the points lying exactly on the axis
    for( UInt32 i = 0; i < onAxisMean.size(); i++ )
    {
        if( leftNode->mTriangleIndices.size() < rightNode->mTriangleIndices.size() )
            leftNode->mTriangleIndices.push_back( onAxisMean[i] );
        else
            rightNode->mTriangleIndices.push_back( onAxisMean[i] );
    }
}

void kDOPTree::BuildTree( Vector<Trianglef>& pTriangleSoup, Vector<Vector3f>& pVertices )
{
    // Keep a copy of the vertices and triangles.
    mVertices.resize( pVertices.size() );
    for( UInt32 i = 0; i < pVertices.size(); i++ )
        mVertices[i] = pVertices[i];
            
    mTriangles.resize( pTriangleSoup.size() );
    for( UInt32 i = 0; i < pTriangleSoup.size(); i++ )
    {
        mTriangles[i].mIndices[0] = pTriangleSoup[i].mIndices[0];
        mTriangles[i].mIndices[1] = pTriangleSoup[i].mIndices[1];
        mTriangles[i].mIndices[2] = pTriangleSoup[i].mIndices[2];
    }
    
    mTrianglesCentroids.resize( pTriangleSoup.size() );

    // Find each triangle center.
    for( UInt32 i = 0; i < pTriangleSoup.size(); i++ )
    {
        mTrianglesCentroids[i]  = mVertices[mTriangles[i].mIndices[0]];
        mTrianglesCentroids[i] += mVertices[mTriangles[i].mIndices[1]];
        mTrianglesCentroids[i] += mVertices[mTriangles[i].mIndices[2]];
        mTrianglesCentroids[i] *= (1.0f / 3.0f);
    }

    mNumNodes  = 1;
    mNumLeaves = 0;
    mMaxDepth  = 0;
    mDepth     = 0;

    // Create the root node.
    kDOPNode* root = GD_NEW(kDOPNode, this, "Engine::Collision::kDOPTree");
    mNodes.push_back( root );
    
    // Insert the triangles indices in the root node list.
    root->mTriangleIndices.resize( pTriangleSoup.size() );
    for( UInt32 i = 0; i < pTriangleSoup.size(); i++ )
    {
        root->mTriangleIndices[i] = i;
    }

    // Recursively build kDOP tree.
    BuildNode( root );

    // No need to store centroids.
    mTrianglesCentroids.clear();

/*
#ifdef GD_DEBUG
    // 
    // Report memory usage
    UInt32 memUsage = 0;
    memUsage += (mNodes.size() * 4) + (mNodes.size() * sizeof(kDOPNode));
    memUsage += mVertices.size() * sizeof(mVertices[0]);
    memUsage += mTriangles.size() * sizeof(mTriangles[0]);
    printf( "kDOPTree mem usage = %08d bytes", memUsage );
#endif
*/
}

void kDOPTree::FindBoundingBox( kDOPTree::kDOPNode* pNode )
{
    for( UInt32 i = 0; i < pNode->mTriangleIndices.size(); i++ )
    {
        pNode->mBoundingBox.Grow( mVertices[mTriangles[pNode->mTriangleIndices[i]].mIndices[0]] );
        pNode->mBoundingBox.Grow( mVertices[mTriangles[pNode->mTriangleIndices[i]].mIndices[1]] );
        pNode->mBoundingBox.Grow( mVertices[mTriangles[pNode->mTriangleIndices[i]].mIndices[2]] );
    }
}

void kDOPTree::BuildNode( kDOPTree::kDOPNode* pNode )
{
    mDepth++;
    
    if( mDepth > mMaxDepth )
        mMaxDepth = mDepth;

    FindBoundingBox( pNode );

    // Test if we're creating a leaf or a node.
    if( pNode->mTriangleIndices.size() > 5 )
    {
        mNumNodes++;

        Float axisMean;
        Axis splitAxis = SelectSplitAxis( pNode->mTriangleIndices, axisMean );

        // Add two nodes.
        kDOPNode* leftNode  = GD_NEW(kDOPNode, this, "Engine::Collision::kDOPTree");
        kDOPNode* rightNode = GD_NEW(kDOPNode, this, "Engine::Collision::kDOPTree");

        pNode->mChildsIndex = mNodes.size();
        
        mNodes.push_back( leftNode );
        mNodes.push_back( rightNode );

        // Split triangles into two list.
        SplitTriangles( pNode, splitAxis, axisMean );

        // No need to store this in nodes.
        pNode->mTriangleIndices.clear();

        // Recursively call build tree.
        BuildNode( leftNode );
        BuildNode( rightNode );
    }
    else
    {
        // No work to do for leaves!
        mNumLeaves++;
    }

    mDepth--;
}

Bool kDOPTree::LineCheck( const Ray3f& pRay ) const
{
    if( mNodes.size() > 0 )
        return LineCheck( mNodes[0], pRay );

    return false;
}

Bool kDOPTree::LineCheck( kDOPNode* pNode, const Ray3f& pRay ) const
{
    if( Intersect( pRay, pNode->mBoundingBox ) )
    {
        // At the root.
        if( pNode->mTriangleIndices.size() != 0 )
        {
            // Check each triangles.
            const Trianglef* tri;
            for( UInt32 i = 0; i < pNode->mTriangleIndices.size(); i++ )
            {
                tri = &mTriangles[pNode->mTriangleIndices[i]];
                if( IntersectTriangle( mVertices[tri->mIndices[0]], mVertices[tri->mIndices[1]], mVertices[tri->mIndices[2]], pRay ) )
                    return true;
            }

            return false;
        }
        else
        {
            // Check childs.
            return LineCheck( mNodes[pNode->mChildsIndex + 0], pRay ) ||
                   LineCheck( mNodes[pNode->mChildsIndex + 1], pRay );
        }
    }

    return false;
}

void kDOPTree::DrawLevel( UInt32 pLevel )
{
#ifdef GD_DEBUG
    mDepth = 0;
    if( mNodes.size() > 0 )
        DrawLevel( mNodes[0], pLevel );
#endif
}


} // namespace Gamedesk


#ifdef GD_DEBUG

#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"

void Gamedesk::kDOPTree::DrawLevel( kDOPTree::kDOPNode* pNode, UInt32 pLevel )
{
    mDepth++;

    if( mDepth == pLevel ||
        pNode->mTriangleIndices.size() != 0 )
    {
        Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
        GD_ASSERT(renderer);

	    // Render the bounding box.
	    renderer->SetRenderState( Renderer::Lighting, false );
        renderer->SetPolygonMode( Renderer::BothFace, Renderer::FillWireframe );
        renderer->SetCulling( Renderer::NoCulling );
        
        renderer->SetColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
        renderer->DrawBox( pNode->mBoundingBox.Min(), pNode->mBoundingBox.Max() );

        renderer->SetRenderState( Renderer::Lighting, true );
        renderer->SetPolygonMode( Renderer::BothFace, Renderer::FillSolid );
        renderer->SetCulling( Renderer::CullBackFace );
    }
    else
    {        
        kDOPNode* leftNode  = mNodes[pNode->mChildsIndex + 0];
        kDOPNode* rightNode = mNodes[pNode->mChildsIndex + 1];

        DrawLevel( leftNode, pLevel );
        DrawLevel( rightNode, pLevel );
    }

    mDepth--;
}

#endif



