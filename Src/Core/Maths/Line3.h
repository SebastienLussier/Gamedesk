/**
 *  @file       Line3.h
 *  @brief      Line maths.
 *  @author     Sébastien Lussier.
 *  @date       23/11/03.
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
#ifndef     _LINE3_H_
#define     _LINE3_H_


#include "Vector3.h"
 
 
namespace Gamedesk {


/**
 *  Representation of a ray in 3d.
 */
template <class T>
class Ray3
{
public:
    /**
     *  Constructor.
     */
    Ray3(){}

    /**
     *  Construct a line given an origin and a direction.
     *  @param  pOrigin     The origin of the line.
     *  @param  pDirection  The direction of the line.
     */
    Ray3( const Vector3<T>& pOrigin, const Vector3<T>& pDirection )
        : mOrigin(pOrigin)
        , mDirection(pDirection)
    {
        mInvDirection.x = 1 / pDirection.x;
        mInvDirection.y = 1 / pDirection.y;
        mInvDirection.z = 1 / pDirection.z;
        mSign[0] = mInvDirection.x < 0;
        mSign[1] = mInvDirection.y < 0;
        mSign[2] = mInvDirection.z < 0;
    }
		
    /**
     *  Get the direction of this line.
     *  return The direction of this line.     
     */
    const Vector3<T>& GetOrigin() const
    {
        return mOrigin;
    }

    /**
     *  Get the direction of this line.
     *  return The direction of this line.     
     */
	const Vector3<T>& GetDirection() const
    {
        return mDirection;
    }

    /**
     *  Get the direction of this line.
     *  return The direction of this line.     
     */
	const Vector3<T>& GetInvDirection() const
    {
        return mInvDirection;
    }

    /**
     *  Set the origin of this line.
     *  @param  pOrigin  New origin of this line.
     */
    void SetOrigin( const Vector3<T>& pOrigin )
    {
        mOrigin = pOrigin;
    }

    /**
     *  Set the direction of this line.
     *  @param  pDirection  New direction of this line.
     */
	void SetDirection( const Vector3<T>& pDirection )
    {
        mDirection = pDirection;

        mInvDirection.x = 1 / pDirection.x;
        mInvDirection.y = 1 / pDirection.y;
        mInvDirection.z = 1 / pDirection.z;
        mSign[0] = mInvDirection.x < 0;
        mSign[1] = mInvDirection.y < 0;
        mSign[2] = mInvDirection.z < 0;
    }

    /**
     *  Return the sign of the inverse direction vector element.
     */
    Bool GetSign( UInt32 pElem ) const
    {
        return mSign[pElem];
    }

    /**
     *  Serialize this ray to/from a stream.
     *  @param  pStream Stream used for serialization.
     *  @return The stream object.
     */
    friend Stream& operator << ( Stream& pStream, Ray3<T>& pRay )
    {
        pStream << pRay.mOrigin
                << pRay.mDirection
                << pRay.mInvDirection
                << pRay.mSign[0] << pRay.mSign[1] << pRay.mSign[2];
        return pStream;
    }

public:
    Vector3<T> mOrigin;         //!< Direction of this line.
	Vector3<T> mDirection;      //!< Orientation of this line.
    Vector3<T> mInvDirection;   //!< Inverse of direction.
    Bool       mSign[3];        //!< Sign of inv. direction.
};


typedef Ray3<Float>    Ray3f;
typedef Ray3<Double>   Ray3d;


} // namespace Gamedesk


#endif  //  _LINE3_H_
