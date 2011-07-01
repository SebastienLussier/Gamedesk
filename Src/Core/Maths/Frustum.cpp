/**
 *  @file       Frustum.cpp
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
#include "Core.h"
#include "Frustum.h"


namespace Gamedesk {
	
	
Frustum::Frustum()
{
}

void Frustum::CalculateFrustum( const Matrix4f& pProjectionMatrix, 
							    const Matrix4f& pModelViewMatrix )
{    
	// Now that we have our modelview and projection matrix, if we combine these 2 matrices,
	// it will give us our clipping planes.  To combine 2 matrices, we multiply them.
    Matrix4f m(pModelViewMatrix * pProjectionMatrix);

	// Now we actually want to get the sides of the frustum.  To do this we take
	// the clipping planes we received above and extract the sides from them.
    mPlanes[Left]   = Plane3f( Vector3f(-m[ 3] - m[ 0], -m[ 7] - m[ 4], -m[11] - m[ 8]),  m[15] + m[12]);
	mPlanes[Right]  = Plane3f( Vector3f( m[ 0] - m[ 3],  m[ 4] - m[ 7],  m[ 8] - m[11]), -m[12] + m[15]);
	mPlanes[Bottom] = Plane3f( Vector3f(-m[ 3] - m[ 1], -m[ 7] - m[ 5], -m[11] - m[ 9]),  m[15] + m[13]);
	mPlanes[Top]    = Plane3f( Vector3f( m[ 1] - m[ 3],  m[ 5] - m[ 7],  m[ 9] - m[11]), -m[13] + m[15]);
	mPlanes[Front]  = Plane3f( Vector3f(-m[ 3] - m[ 2], -m[ 7] - m[ 6], -m[11] - m[10]),  m[15] + m[14]);
    mPlanes[Back]   = Plane3f( Vector3f( m[ 2] - m[ 3],  m[ 6] - m[ 7],  m[10] - m[11]), -m[14] + m[15]);
	
    // Normalize the planes.
    mPlanes[Right].Normalize();
    mPlanes[Left].Normalize();
    mPlanes[Top].Normalize();
    mPlanes[Bottom].Normalize();
    mPlanes[Back].Normalize();
    mPlanes[Front].Normalize();
}

Float Frustum::SphereDistanceInFrustum( const Vector3f& pPos, Float pRadius )
{
   Float d = 0;

   for( UInt32 i = 0; i < NumSides; i++ )
   {
      d = mPlanes[i].DistanceTo( pPos );
      if( d > pRadius )
         return 0;
   }

   return d + pRadius;
}

Bool Frustum::PolygonInFrustum( UInt32 pNumPoints, Vector3f* pPointList )
{
   for( UInt32 iPoint = 0; iPoint < pNumPoints; iPoint++ )
   {
      if( PointInFrustum( pPointList[iPoint] ) )
          return true;
   }

   return false;
}


} // namespace Gamedesk
