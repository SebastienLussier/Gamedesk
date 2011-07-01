/**
*  @file       VectorUtils.h
*  @brief      Utility methods for vector maths.
*  @author     Sébastien Lussier.
*  @date       15/06/05.
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
#ifndef     _VECTOR_UTILS_H_
#define     _VECTOR_UTILS_H_


#include "Vector2.h"
#include "Vector3.h"


namespace Gamedesk {


template <typename T>
Vector3<T> GetTangentSpaceVector( const Vector3<T>& position1, const Vector3<T>& position2, const Vector3<T>& position3,
                                  const Vector2<T>& uv1,       const Vector2<T>& uv2,       const Vector2<T>& uv3 )
{
    // side0 is the vector along one side of the triangle of vertices passed in, 
    // and side1 is the vector along another side. Taking the cross product of these returns the normal.
    Vector3<T> side0 = position1 - position2;
    Vector3<T> side1 = position3 - position1;

    // Calculate face normal
    Vector3<T> normal = side1 cross side0;
    normal.Normalize();

    // Now we use a formula to calculate the tangent. 
    Vector2<T> deltaUV0 = uv1 - uv2;
    Vector2<T> deltaUV1 = uv3 - uv1;

    Vector3<T> tangent = side0*deltaUV1.y - side1*deltaUV0.y;
    tangent.Normalize();

    // Calculate binormal
    Vector3<T> binormal = side0*deltaUV1.x - side1*deltaUV0.x;
    binormal.Normalize();

    //Now, we take the cross product of the tangents to get a vector which 
    //should point in the same direction as our normal calculated above. 
    //If it points in the opposite direction (the dot product between the normals is less than zero), 
    //then we need to reverse the s and t tangents. 
    //This is because the triangle has been mirrored when going from tangent space to object space.
    //reverse tangents if necessary
    Vector3<T> tangentCross = tangent cross binormal;
    if( (tangentCross dot normal) < T(0) )
    {
        tangent = -tangent;
    }

    return tangent;
}


} // namespace Gamedesk


#endif  //  _VECTOR_UTILS_H_
