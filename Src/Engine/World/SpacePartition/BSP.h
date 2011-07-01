/**
 *  @file       BSP.h
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       02/25/05.
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
#ifndef     _BSP_H_
#define     _BSP_H_


#include "Containers/Bitset.h"
#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Maths/Plane3.h"
#include "Maths/BoundingBox.h"
#include "Resource/Resource.h"
#include "Graphic/Texture/TextureHdl.h"
#include "Graphic/Texture/PackedTexture.h"


namespace Gamedesk {


class VertexBuffer;
class IndexBuffer;


class ENGINE_API Bsp : public Resource
{
	DECLARE_CLASS(Bsp, Resource);

private:
    static const UInt32 MAX_MAP_LEAFS       = 065536;
    static const UInt32 NOT_VISIBLE         = 0xFFFF;

    static const UInt32 SURF_PLANEBACK		= 0x02;
    static const UInt32 SURF_DRAWSKY		= 0x04;
    static const UInt32 SURF_DRAWTURB		= 0x10;
    static const UInt32 SURF_DRAWBACKGROUND	= 0x40;
    static const UInt32 SURF_UNDERWATER		= 0x80;

    static const UInt32 SURF_LIGHT		    = 0x01;	// value will hold the light strength
    static const UInt32 SURF_SLICK		    = 0x02;	// effects game physics
    static const UInt32 SURF_SKY		    = 0x04;	// don't draw, but add to skybox
    static const UInt32 SURF_WARP		    = 0x08;	// turbulent water warp
    static const UInt32 SURF_TRANS33	    = 0x10;
    static const UInt32 SURF_TRANS66	    = 0x20;
    static const UInt32 SURF_FLOWING	    = 0x40;	// scroll towards angle
    static const UInt32 SURF_NODRAW		    = 0x80; // don't bother referencing the texture

public:
	//! Default constructor.
	Bsp();
   
    //! Destructor.
    virtual ~Bsp();

	//! Render the model.
    virtual void Render();

	virtual void Init();

public:
	class BSPEdge
	{
	public:
		UInt32		mPoints[2];
	};

	class BSPNode
	{
	public:
        UInt32      mVisibleFrame;

		UInt32		mPlaneIndex;
		
		Int32		mFrontChild;
		Int32		mBackChild;

		BoundingBox	mBBox;

		UInt32		mFirstFaceIndex;
		UInt32		mNumFaces;
	};

	class BSPLeaf
	{
	public:
        UInt32      mVisibleFrame;

        UInt32      mCluster;
		BoundingBox	mBBox;

		UInt32		mFirstFaceIndex;
		UInt32		mNumFaces;
	};

	class BSPFace
	{
	public:
		UInt32		mPlaneIndex;
        Bool        mPlaneSide;
		UInt32		mFirstEdgeIndex;
		UInt16		mNumEdges;

        UInt16      mTextureInfo;
        Byte        mLightmapStyles[4];
        UInt32      mLightmapOffset;

        PackedTexture::Info mLightmapInfo;

        Vector2<Int16>  mTextureMin;
	    Vector2<Int16>  mExtent;
	};

	class BSPCluster
	{
	public:
        Bitset      mPVS;
        UInt32      mPHS;
	};

    class BSPTexInfo
    {
    public:
        Vector3f    mAxisU;
        Float       mOffsetU;
   
        Vector3f    mAxisV;
        Float       mOffsetV;

        UInt32      mFlags;
        UInt32      mValue;

        HTexture2D  mTexture;
        UInt32      mNextTexInfo;
    };

private:
    void            CalculateFaceExtent( UInt32 iFace );
    void            CreateFaceLightmap( UInt32 iFace );

    void            MarkVisibleLeafs( UInt32 pFromCluster );
    Bool            IsPotentiallyVisible( UInt32 pFromCluster, UInt32 pTestCluster ) const;
    const BSPLeaf&  FindLeafContaining( const Vector3f& pPoint ) const;

public:
	Vector<Vector3f>	mVertices;
	Vector<BSPEdge>		mEdges;
	Vector<Int32>		mFacesEdge;
	Vector<BSPNode>		mNodes;
	Vector<BSPLeaf>		mLeaves;
	Vector<BSPFace>		mFaces;
	Vector<UInt32>		mLeafFaces;
	Vector<Plane3f>		mPlanes;
    Vector<BSPCluster>  mClusters;
    Vector<BSPTexInfo>  mTextureInfo;

    VertexBuffer*       mBufPositions;
    VertexBuffer*       mBufNormals;
    IndexBuffer*        mBufIndices;

    Vector<Byte>        mLightmapData;

    Bitset              mFacesDrawn;

    PackedTexture       mPackedLightmaps;
};


} // namespace Gamedesk


#endif	//	_BSP_H_
