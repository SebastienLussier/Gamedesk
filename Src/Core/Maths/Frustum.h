/**
 *  @file       Frustum.h
 *  @brief      Frustum class.
 *  @author     Marco Arsenault
 *  @date       11/04/04
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

#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_


#include "Vector3.h"
#include "Matrix4.h"
#include "Plane3.h"
#include "BoundingBox.h"


namespace Gamedesk {


class CORE_API Frustum  
{
public:
    enum FrustumSide
    {
	    Right	 = 0,		// The RIGHT side of the frustum
	    Left	 = 1,		// The LEFT	 side of the frustum
	    Bottom	 = 2,		// The BOTTOM side of the frustum
	    Top		 = 3,		// The TOP side of the frustum
	    Back	 = 4,		// The BACK	side of the frustum
	    Front	 = 5,		// The FRONT side of the frustum
        NumSides = 6        // Number of sides...
    };  

	//! Default constructor.
	Frustum();
	
    //! Call this to calculte the frustum.
	void CalculateFrustum( const Matrix4f& pProjectionMatrix, 
					       const Matrix4f& pModelViewMatrix );

    //! Return 0 if the sphere is outside the frustum or return the distance + radius 
	Float SphereDistanceInFrustum( const Vector3f& pPos, Float pRadius );

	//! Test if the polygon is inside the frustum.
	Bool PolygonInFrustum( UInt32 pNumPoints, Vector3f* pointlist );

public:
    //! This takes a 3D point and returns true if it's inside of the frustum
    Bool PointInFrustum( const Vector3f& pPos ) const
    {
        return mPlanes[ Right].DistanceTo(pPos) >= 0 &&
               mPlanes[  Left].DistanceTo(pPos) >= 0 &&
               mPlanes[   Top].DistanceTo(pPos) >= 0 &&
               mPlanes[Bottom].DistanceTo(pPos) >= 0 &&
               mPlanes[  Back].DistanceTo(pPos) >= 0 &&
               mPlanes[ Front].DistanceTo(pPos) >= 0;
    }

    //! This takes the center and half the length of the cube.
    Bool CubeInFrustum( const Vector3f& pPos, Float pSize ) const
    {
        // Do a quick test first: if sphere is in frustum, then the cube certainly is.
        if( SphereInFrustum(pPos, pSize) )
            return true;
        
        return PointInFrustum( Vector3f(pPos.x-pSize, pPos.y-pSize, pPos.z-pSize) ) ||
               PointInFrustum( Vector3f(pPos.x+pSize, pPos.y-pSize, pPos.z-pSize) ) ||
               PointInFrustum( Vector3f(pPos.x-pSize, pPos.y+pSize, pPos.z-pSize) ) ||
               PointInFrustum( Vector3f(pPos.x+pSize, pPos.y+pSize, pPos.z-pSize) ) ||
               PointInFrustum( Vector3f(pPos.x-pSize, pPos.y-pSize, pPos.z+pSize) ) ||
               PointInFrustum( Vector3f(pPos.x+pSize, pPos.y-pSize, pPos.z+pSize) ) ||
               PointInFrustum( Vector3f(pPos.x-pSize, pPos.y+pSize, pPos.z+pSize) ) ||
               PointInFrustum( Vector3f(pPos.x+pSize, pPos.y+pSize, pPos.z+pSize) );
    }

    //! This takes a 3D point and a radius and returns true if the sphere is inside of the frustum
    Bool SphereInFrustum( const Vector3f& pPos, Float pRadius ) const
    {
	    for( UInt32 i = 0; i < NumSides; i++ )	
	    {
		    // If the center of the sphere is farther away from the plane than the radius
		    if( mPlanes[i].DistanceTo( pPos ) > pRadius )
	            return false;
	    }
    	
	    // The sphere was inside of the frustum!
	    return true;
    }

    Bool BoxInFrustum( const BoundingBox& pBox ) const
    {
        if( SphereInFrustum( pBox.GetCenter(), pBox.GetExtend() ) )
            return true;

        return PointInFrustum( pBox.Min() ) || 
               PointInFrustum( pBox.Max() ) ||
               PointInFrustum( Vector3f(pBox.Min().x, pBox.Min().y, pBox.Max().z) ) || 
               PointInFrustum( Vector3f(pBox.Max().x, pBox.Min().y, pBox.Min().z) ) || 
               PointInFrustum( Vector3f(pBox.Max().x, pBox.Min().y, pBox.Max().z) ) || 
               PointInFrustum( Vector3f(pBox.Min().x, pBox.Max().y, pBox.Min().z) ) || 
               PointInFrustum( Vector3f(pBox.Min().x, pBox.Max().y, pBox.Max().z) ) || 
               PointInFrustum( Vector3f(pBox.Max().x, pBox.Max().y, pBox.Min().z) );      
    }

    /**
     *  Serialize this frustum to/from a stream.
     *  @param  pStream Stream used for serialization.
     *  @return The stream object.
     */
    friend Stream& operator << ( Stream& pStream, Frustum& pFrustum )
    {
        pStream << pFrustum.mPlanes[ Right]
                << pFrustum.mPlanes[  Left]
                << pFrustum.mPlanes[   Top]
                << pFrustum.mPlanes[Bottom]
                << pFrustum.mPlanes[  Back]
                << pFrustum.mPlanes[ Front];
        return pStream;
    }

private:
	// This holds the A B C and D values for each side of our frustum.
	Plane3f     mPlanes[6];
};


} // namespace Gamedesk


#endif // _FRUSTUM_H_
