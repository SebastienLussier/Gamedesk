 /**
 *  @file       Plane3.h
 *  @brief      Ininite plane maths.
 *  @author     Sébastien Lussier.
 *  @date       25/05/02.
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
#ifndef     _PLANE3_H_
#define     _PLANE3_H_


#include "Maths.h"
#include "Line3.h"
#include "Vector3.h"


namespace Gamedesk {


/**
 *  Representation of an infinite plane in 3D space.
 *  This class keeps the plane information as a plane formula (A,B,C,D),
 *  where ABC is the normal and D the constant. 
 */
template <class T> 
class Plane3
{
public:
    //! Side of the plane.
    enum Side
    {
        NoSide,            //!< The plane itself.
        PositiveSide,      //!< Side of the plane where the normal is facing.
        NegativeSide       //!< Side of the plane where the normal is facing away.   
    };

    //! Constructor.
    Plane3() {}

    //! Copy constructor.
    Plane3( const Plane3& pOther )  { memcpy( mABCD, pOther.mABCD, 4*sizeof(T) ); }

    /**
     *  Construct a plane using the plane normal and the constant.
     *  @param  pNormal     The plane normal.
     *  @param  pConstant   The plane constant.
     */
    Plane3( const Vector3<T>& pNormal, T pConstant )
    {
        mABCD[0] = pNormal.x;
        mABCD[1] = pNormal.y;
        mABCD[2] = pNormal.z;
        mABCD[3] = pConstant;
    }

    /**
     *  Construct a plane using the plane normal and a point lying on the plane.
     *  @param  pNormal The plane normal.
     *  @param  pPoint  A point on the plane.
     */
    Plane3( const Vector3<T>& pNormal, const Vector3<T>& pPoint )
    {
        mABCD[0] = pNormal.x;
        mABCD[1] = pNormal.y;
        mABCD[2] = pNormal.z;
        mABCD[3] = pNormal dot pPoint;
    }

    /**
     *  Construct a plane using 3 unit length vectors where 
     *  (pPoint2 - pPoint1) cross (pPoint3 - pPoint1) is the plane normal and
     *  normal dot pPoint1 is the constant.
     *  @param  pPoint1     First vector (must be unit length).
     *  @param  pPoint2     Second vector (must be unit length).
     *  @param  pPoint3     Third vector (must be unit length).
     */
    Plane3( const Vector3<T>& pPoint1, const Vector3<T>& pPoint2, const Vector3<T>& pPoint3 )
    {
        Vector3<T> normal = (pPoint2 - pPoint1) cross (pPoint3 - pPoint1);
        mABCD[0] = normal.x;
        mABCD[1] = normal.y;
        mABCD[2] = normal.z;
        mABCD[3] = normal dot pPoint1;
    }
   
    /**
     *	Set the plane normal.
     *  @parame pNormal The new plane normal.
     */
    void SetNormal( const Vector3<T>& pNormal )
    {
        mABCD[0] = pNormal.x; mABCD[1] = pNormal.y; mABCD[2] = pNormal.z;
    }

    /**
     *	Get the plane normal.
     *  @return The plane normal.
     */
    Vector3<T> GetNormal() const
    {
        return Vector3<T>( mABCD[0], mABCD[1], mABCD[2] );
    }

    /**
     *	Set the plane constant.
     *  @param  pConstant   The plane constant.
     */
    void SetConstant( T pConstant ) { mABCD[3] = pConstant; }

    /**
     *	Get the plane constant.
     *  @return The plane constant.
     */
    T    GetConstant() const        { return mABCD[3]; }
    
    /**
     *  Get a pointer to the internal representation of the plane.
     *  @brief  Indirection operator (const).
     *  @return A pointer to a T array.
     */
    operator const T* () const
    {
        return mABCD;
    }
    
    /**
     *  Get a pointer to the internal representation of the plane.
     *  @brief  Indirection operator.
     *  @return A pointer to a T array.
     */
    operator T* ()
    {
        return mABCD;
    }

    /**
     *  Access the plane equation as an array of T.
     *	@brief  Subscript operator (const).
     *  @param  pIndex  Index of the element (between [0,3]).
     *  @return The element of the equation at position pIndex.
     */
    const T& operator () ( UInt32 pIndex ) const
    {
        GD_ASSERT( pIndex < 4 );
        return mABCD[pIndex];
    }

    /**
     *  Access the plane equation as an array of T.
     *	@brief  Subscript operator.
     *  @param  pIndex  Index of the element (between [0,3]).
     *  @return The element of the equation at position pIndex.
     */
    T& operator () ( UInt32 pIndex )
    {
        GD_ASSERT( pIndex < 4 );
        return mABCD[pIndex];
    }

    /**
     *  Assignment operator.	
     *  @param  pOther  Value to be assigned to this plane.
     *  @return A reference to this plance.
     */
    const Plane3& operator = ( const Plane3& pOther )
    {
        memcpy( mABCD, pOther.mABCD, 4*sizeof(T) );
        return *this;
    }

    /**
     *  Return the side where a given point lies in relation to this plane.
     *  @param  pPoint  The point used for the test.
     *  @return The side where the point is located.
     */
    Side WhichSide( const Vector3<T>& pPoint ) const
    {
        T distance = DistanceTo(pPoint);

        if ( distance < (T)0.0 )
            return Plane3<T>::NegativeSide;

        if ( distance > (T)0.0 )
            return Plane3<T>::PositiveSide;

        return Plane3<T>::NoSide;
    }

    /**
     *  Obtain the distance between a point and this plane.
     *  It's in fact a peusdodistance, as it will be negative if the point is on
     *  the negative side of the plane.  The absolute value of the return value
     *  is the true distance only when the plane normal is a unit length vector.
     *  @param  pPoint  The point used for the test.
     *  @return The pseudodistance between the point and this plane.
     */
    T DistanceTo( const Vector3<T>& pPoint ) const
    {
        return mABCD[0]*pPoint.x + mABCD[1]*pPoint.y + mABCD[2]*pPoint.z - mABCD[3];
    }

    /**
     *  Get a normalized version of this plane.
     *  @return A new normalized plane.
     */
    Plane3 GetNormalized() const
    {
        T sqrLength = mABCD[0]*mABCD[0] + mABCD[1]*mABCD[1] + mABCD[2]*mABCD[2];
        T invLength = 1.0 / Maths::Sqrt(sqrLength);

        return Plane3( Vector3<T>(mABCD[0]*invLength, mABCD[1]*invLength, mABCD[2]*invLength), mABCD[3]*invLength );
    }

    /**
     *  Normalize this plane.
     */
    void Normalize()
    {
        T sqrLength = mABCD[0]*mABCD[0] + mABCD[1]*mABCD[1] + mABCD[2]*mABCD[2];
        T invLength = 1.0 / Maths::Sqrt(sqrLength);

        mABCD[0] *= invLength;
        mABCD[1] *= invLength;
        mABCD[2] *= invLength;
        mABCD[3] *= invLength;
    }

    /**
     *  Serialize this plane to/from a stream.
     *  @param  pStream Stream used for serialization.
     *  @return The stream object.
     */
    friend Stream& operator << ( Stream& pStream, Plane3<T>& pPlane )
    {
        pStream << pPlane.mABCD[0] << pPlane.mABCD[1] << pPlane.mABCD[2] << pPlane.mABCD[3];
        return pStream;
    }
    
protected:
    T   mABCD[4];       //!< The equation of this plane as an array: [0]=A,[1]=B,[2]=C,[3]=D
};


typedef Plane3<Float>   Plane3f;
typedef Plane3<Double>  Plane3d;


} // namespace Gamedesk


#endif  //  _PLANE3_H_
