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
#ifndef     _KDOP_TREE_H_
#define     _KDOP_TREE_H_


#include "Maths/Vector3.h"
#include "Maths/BoundingBox.h"
#include "Maths/Line3.h"


namespace Gamedesk {


// #TODO: Move this elsewhere...
class ENGINE_API Trianglef
{
public:
    UInt16  mIndices[3];
};


class ENGINE_API kDOPTree
{
private:
    class kDOPNode
    {
    public:
        BoundingBox     mBoundingBox;
        UInt32          mChildsIndex;
        Vector<UInt16>  mTriangleIndices;
    };

public:
    void    BuildTree( Vector<Trianglef>& pTriangleSoup, Vector<Vector3f>& pVertices );
    void    DrawLevel( UInt32 pLevel );
    Bool    LineCheck( const Ray3f& pRay ) const;
 
private:
    enum Axis
    {
        Axis_X,
        Axis_Y,
        Axis_Z,
        Axis_Num
    };

    void    BuildNode( kDOPNode* pNode );
    void    FindBoundingBox( kDOPNode* pNode );
    Axis    SelectSplitAxis( Vector<UInt16>& pTriangleSoup, Float& mean );
    void    SplitTriangles( kDOPNode* pNode, Axis splitAxis, Float axisMean );
    Bool    LineCheck( kDOPNode* pNode, const Ray3f& pRay ) const;

#ifdef GD_DEBUG
    void    DrawLevel( kDOPNode* pNode, UInt32 pLevel );
#endif

private:
    UInt32              mNumNodes;
    UInt32              mNumLeaves;
    UInt32              mMaxDepth;
    UInt32              mDepth;

    Vector<kDOPNode*>   mNodes;
    Vector<Vector3f>    mVertices;
    Vector<Trianglef>   mTriangles;

    // Temporary when building tree
    Vector<Vector3f>    mTrianglesCentroids;
};


} // namespace Gamedesk


#endif  //  _KDOP_TREE_H_
