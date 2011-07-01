/**
 *  @file       BoundingBox.h
 *  @brief
 *  @author     Marco Arsenault.
 *  @date       01/02/04.
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
#ifndef     _BOUNDING_BOX_H_
#define     _BOUNDING_BOX_H_


#include "Vector3.h"
#include "Number.h"


namespace Gamedesk {


/**
 *  BoundingBox class.
 *  @brief  BoundingBox class.
 *  @author Marco Arsenault.
 *  @date   01/02/04.
 */
class CORE_API BoundingBox
{
public:
    //! Default constructor.
    BoundingBox()
    {
        mBounds[0] = Vector3f( Number<Float>::Max, Number<Float>::Max, Number<Float>::Max );
        mBounds[1] = Vector3f( Number<Float>::Min, Number<Float>::Min, Number<Float>::Min );
    }

    //! Constructor taking a min and max vector.
    BoundingBox( const Vector3f& pMin, const Vector3f& pMax )
    {
        mBounds[0] = pMin;
        mBounds[1] = pMax;
    }

    //! Copy constructor.
    BoundingBox( const BoundingBox& pOther )
    {
        mBounds[0] = pOther.mBounds[0];
        mBounds[1] = pOther.mBounds[1];
    }

    //! Assignment operator.
    const BoundingBox& operator = ( const BoundingBox& pOther )
    {
        mBounds[0] = pOther.mBounds[0];
        mBounds[1] = pOther.mBounds[1];
        return *this;
    }

    //! Grow the bounding box so that it contains the given point.
    void Grow( const Vector3f& pPoint )
    {
        // Min
        if( pPoint.x < mBounds[0].x )
            mBounds[0].x = pPoint.x;

        if( pPoint.y < mBounds[0].y )
            mBounds[0].y = pPoint.y;

        if( pPoint.z < mBounds[0].z )
            mBounds[0].z = pPoint.z;

        // Max
        if( pPoint.x > mBounds[1].x )
            mBounds[1].x = pPoint.x;
        
        if( pPoint.y > mBounds[1].y  )
            mBounds[1].y = pPoint.y;

        if( pPoint.z > mBounds[1].z)
            mBounds[1].z = pPoint.z;        
    }

    //! Grow the bounding box with another bounding box.
    void Grow( const BoundingBox& pBox )
    {
        // Min vs Min
        if( pBox.mBounds[0].x < mBounds[0].x )
            mBounds[0].x = pBox.mBounds[0].x;

        if( pBox.mBounds[0].y < mBounds[0].y )
            mBounds[0].y = pBox.mBounds[0].y;

        if( pBox.mBounds[0].z < mBounds[0].z )
            mBounds[0].z = pBox.mBounds[0].z;

        // Max vs Max
        if( pBox.mBounds[1].x > mBounds[1].x )
            mBounds[1].x = pBox.mBounds[1].x;

        if( pBox.mBounds[1].y > mBounds[1].y )
            mBounds[1].y = pBox.mBounds[1].y;

        if( pBox.mBounds[1].z > mBounds[1].z )
            mBounds[1].z = pBox.mBounds[1].z;
    }

    //! Returns a bool whether the given point is inside the bounding volume.
    Bool Contains( const Vector3f& pPoint ) const
    {
        return mBounds[0].x <= pPoint.x && mBounds[0].y <= pPoint.y && mBounds[0].z <= pPoint.z &&
               mBounds[1].x >= pPoint.x && mBounds[1].y >= pPoint.y && mBounds[1].z >= pPoint.z;
    }

    Bool Contains( const BoundingBox& pBox ) const
    {
		return Contains(pBox.mBounds[0]) && Contains(pBox.mBounds[1]);
    }

    Vector3f GetCenter() const
    {
        return mBounds[0] + (mBounds[1] - mBounds[0]) * 0.5f;
    }
    
    Float GetExtend() const
    {
        return (mBounds[1] - mBounds[0]).GetLength() * 0.5f;
    }

    const Vector3f& Min() const
    {
        return mBounds[0];
    }

    const Vector3f& Max() const
    {
        return mBounds[1];
    }

    void SetMin( const Vector3f& pMin )
    {
        mBounds[0] = pMin;
    }

    void SetMax( const Vector3f& pMax )
    {
        mBounds[1] = pMax;
    }

    const Vector3f& operator () ( UInt32 pElem ) const
    {
        return mBounds[pElem];
    }

    Vector3f& operator () ( UInt32 pElem )
    {
        return mBounds[pElem];
    }

    /**
     *  Serialize this bounding box to/from a stream.
     *  @param  pStream Stream used for serialization.
     *  @return The stream object.
     */
    friend Stream& operator << ( Stream& pStream, BoundingBox& pBBox )
    {
        pStream << pBBox.mBounds[0] << pBBox.mBounds[1];
        return pStream;
    }

protected:
    Vector3f    mBounds[2];
};


} // namespace Gamedesk


#endif  //  _BOUNDING_BOX_H_
