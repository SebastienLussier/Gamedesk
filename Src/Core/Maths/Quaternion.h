/**
 *  @file       Quaternion.h
 *  @brief      Quaternion class.
 *  @author     Marco Arsenault
 *  @date       09/11/03
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

//-----------------------------------------------------------------------------
//
//  Tech notes :
//
//      You can get a 180 degree rotation of a quaternion by simply inverting w
//      Only unit quaternions can be used for representing orientations.
//      q1 * q2 != q2 * q1
//      i*i = j*j = k*k = ijk = -1
//      i*j = -j*i = k
//      j*k = -k*j = i
//      k*i = -i*k = j
//      MULT_IDENTITY = [1,(0, 0, 0)]
//      ADD_IDENTITY = [0,(0, 0, 0)]
//      http://gamedev.net/reference/articles/article1095.asp
//      http://www.gamasutra.com/features/19980703/quaternions_01.htm
//      http://gamedev.net/reference/articles/article1691.asp
//      http://freefall.freehosting.net/articles/quaternions_1.html
//      http://www.daimi.au.dk/~mbl/cgcourse/wiki/_files/shoemake94.pdf
//
//-----------------------------------------------------------------------------
#ifndef     _QUATERNION_H_
#define     _QUATERNION_H_


#include "Maths.h"
#include "Vector3.h"
#include "Matrix4.h"


namespace Gamedesk {


/**
 *  Quaternion class used in 3d computations.  Has all the normal operations needed.
 *  If you run into an operation wich is not included in the class, add it in there.
 *  @brief  A quaternion class.
 *  @author Marco Arsenault.
 *  @date   24/11/03.
 */
template<class T> class Quaternion
{
public:
    /**
     *  Default constructor.
     *  @brief  Default constructor.
     */
    Quaternion() {}

    /**
     *  Copy constructor taking another quaterion in parameter.
     *  @brief  Copy constructor.
     *  @param  pQuat   the quaternion to be copied.
     */
    Quaternion(const Quaternion<T>& pQuat) :
        w(pQuat.w),
        x(pQuat.x),
        y(pQuat.y),
        z(pQuat.z)
    {
    }

    /**
     *  Constructor taking the 4 parameters w,x,y,z of a quaternion.
     */
    Quaternion(T pW, T pX, T pY, T pZ) :
        w(pW),
        x(pX),
        y(pY),
        z(pZ)
    {
    }

    /**
     *  Constructor taking a column-major 4 rotation matrix.
     */
    Quaternion(const Matrix4<T>& pMatrix)
    {
        FromMatrix(pMatrix);
    }

    /**
     *  Constructor taking an axis and a rotation angle (in radian) around that axis.
     */
    Quaternion(const Vector3<T>& pAxis, T pAngle)
    {
        FromAxisRotation(pAxis, pAngle);
    }

    /**
     *  Constructor taking the 3 euler rotation angles in radian.
     */
    Quaternion(T pPitch, T pYaw, T pRoll)
    {
        FromEuler(pPitch, pYaw, pRoll);
    }

    /**
     *  Returns true if the quaternion is pure, false otherwise.
     */
    Bool IsPure() const
    {
        return w == 0;
    }

    /**
     *  Returns true if the quaternion is an identity quaternion, false otherwise.
     */
    Bool IsIdentity() const
    {
        return  w == 1 &&
                x == 0 &&
                y == 0 &&
                z == 0;
    }

    /**
     *  Set the quaternion to identity.
     */
    void SetIdentity()
    {
        w = 1;
        x = 0;
        y = 0;
        z = 0;
    }

    /**
     *  Build the quaternion with an axis and a rotation angle (in radian) around that axis.
     *  @brief  Build the quaternion with an axis and the angle around that axis.
     */
    void FromAxisRotation(const Vector3<T>& pAxis, T pAngle)
    {
        //GD_ASSERT(pAxis.IsNormalized());

        T scalar = Maths::Sin(pAngle / 2.0);

        w = Maths::Cos(pAngle / 2.0);
        x = pAxis.x * scalar;
        y = pAxis.y * scalar;
        z = pAxis.z * scalar;
    }

    /**
     *  Convert the quaternion to an axis and a rotation angle (in radian) around that axis.
     *  @brief  Convert the quaternion to an axis and a rotation angle around that axis.
     */
    void ToAxisRotation(Vector3<T>& pAxis, T& pAngle) const
    {
        T scale = x*x + y*y + z*z;

        if(scale == 0)
        {
            pAngle = 0;

            pAxis.x = 1;
            pAxis.y = 0;
            pAxis.z = 0;
        }
        else
        {
            pAngle = 2 * Maths::Cos(w);

            pAxis.x = x / scale;
            pAxis.y = y / scale;
            pAxis.z = z / scale;
        }

        pAxis.Normalize();
    }

    /**
     *  Builds the quaternion from a 4x4 rotation matrix.
     *  Note : Code snippet from game programming gems.
     *  @brief  Builds the quaternion from a 4x4 rotation matrix.
     */
    void FromMatrix(const Matrix4<T>& pMatrix)
    {
        // This code can be optimized for pMatrix(3, 3) = 1, which
        // should always be true.  This optimization is excluded
        // here for clarity.

        T tr = pMatrix(0, 0) + pMatrix(1, 1) + pMatrix(2, 2) + pMatrix(3, 3);
        T fourD;

        // w >= 0.5 ?
        if (tr >= 1)
        {
            fourD = 2.0 * Maths::Sqrt(tr);
            w = fourD / 4.0;
            x = (pMatrix(2, 1) - pMatrix(1, 2)) / fourD;
            y = (pMatrix(0, 2) - pMatrix(2, 0)) / fourD;
            z = (pMatrix(1, 0) - pMatrix(0, 1)) / fourD;
        }
        else
        {
            Int32 matrixIndex;

            if (pMatrix(0, 0) > pMatrix(1, 1))
            {
                fourD = 2.0 * Maths::Sqrt(pMatrix(0, 0) - pMatrix(1, 1) - pMatrix(2, 2) + 1.0);

                x = fourD / 4.0;
                y = (pMatrix(0, 1) + pMatrix(1, 0)) / fourD;
                z = (pMatrix(0, 2) + pMatrix(2, 0)) / fourD;
                w = (pMatrix(2, 1) - pMatrix(1, 2)) / fourD;

                matrixIndex = 0;
            }
            else
            {
                fourD = 2.0 * Maths::Sqrt(pMatrix(1, 1) - pMatrix(2, 2) - pMatrix(0, 0) + 1.0);

                x = (pMatrix(1, 0) + pMatrix(0, 1)) / fourD;
                y = fourD / 4.0;
                z = (pMatrix(1, 2) + pMatrix(2, 1)) / fourD;
                w = (pMatrix(0, 2) - pMatrix(2, 0)) / fourD;

                matrixIndex = 1;

            }

            if (pMatrix(2, 2) > pMatrix(matrixIndex, matrixIndex))
            {
                fourD = 2.0 * Maths::Sqrt(pMatrix(2, 2) - pMatrix(0, 0) - pMatrix(1, 1) + 1.0);

                x = (pMatrix(2, 0) + pMatrix(0, 2)) / fourD;
                y = (pMatrix(2, 1) + pMatrix(1, 2)) / fourD;
                z = fourD / 4.0;
                w = (pMatrix(1, 0) - pMatrix(0, 1)) / fourD;
            }
        }
    }


    /**
     *  Convert the quaternion to a 4x4 rotation matrix.
     *  Note : Code snippet from game programming gems.
     *  @brief  Convert the quaternion to a 4x4 rotation matrix
     */
    const Matrix4<T>& ToMatrix( Matrix4<T>& pResultMatrix ) const
    {
        // if q is guaranteed to be a unit quaternion, s will always
        // be 1.  In that case, this calculation can be optimized out.
        T norm = GetNormSquare();
        T scalar = norm > 0 ? 2.0 / norm : 0;

        // Precalculate coordinate products
        T xx = x * x * scalar;
        T yy = y * y * scalar;
        T zz = z * z * scalar;
        T xy = x * y * scalar;
        T xz = x * z * scalar;
        T yz = y * z * scalar;
        T wx = w * x * scalar;
        T wy = w * y * scalar;
        T wz = w * z * scalar;

        // Calculate 3x3 matrix from orthonormal basis
        // x axis
        pResultMatrix(0, 0) = 1.0 - (yy + zz);
        pResultMatrix(1, 0) = xy + wz;
        pResultMatrix(2, 0) = xz - wy;

        // y axis
        pResultMatrix(0, 1) = xy - wz;
        pResultMatrix(1, 1) = 1.0 - (xx + zz);
        pResultMatrix(2, 1) = yz + wx;

        // z axis
        pResultMatrix(0, 2) = xz + wy;
        pResultMatrix(1, 2) = yz - wx;
        pResultMatrix(2, 2) = 1.0 - (xx + yy);

        /*  4th row and column of 4x4 matrix
            Translation and scale are not stored in quaternions, so these
            values are set to default (no scale, no translation).
            For systems where m comes pre-loaded with scale and translation
            factors, this code can be excluded.*/
        pResultMatrix(0, 3) = 0;
        pResultMatrix(1, 3) = 0;
        pResultMatrix(2, 3) = 0;
        pResultMatrix(3, 0) = 0;
        pResultMatrix(3, 1) = 0;
        pResultMatrix(3, 2) = 0;
        pResultMatrix(3, 3) = 1;

        return pResultMatrix;
    }

    /**
     *  Build the quaternion from the 3 euler angles (in radian).
     */
    void FromEuler(T pPitch, T pYaw, T pRoll)
    {
        T sy = Maths::Sin(pYaw / T(2));
        T sp = Maths::Sin(pPitch / T(2));
        T sr = Maths::Sin(pRoll / T(2));
        T cy = Maths::Cos(pYaw / T(2));
        T cp = Maths::Cos(pPitch / T(2));
        T cr = Maths::Cos(pRoll / T(2));

        x = cy * sp * cr + sy * cp * sr;
        y = sy * cp * cr - cy * sp * sr;
        z = cy * cp * sr - sy * sp * cr;
        w = cy * cp * cr + sy * sp * sr;

    }

    /**
     *  Convert the quaternion to 3 euler angles (in radian).
     */
    void ToEuler(T& pPitch, T& pYaw, T& pRoll) const
    {
        T ww = w * w;
        T xx = x * x;
        T yy = y * y;
        T zz = z * z;

        pYaw = Maths::ATan(2.0 * (x * y + z * w) / (xx - yy - zz + ww));
        pPitch = Maths::ATan(2.0 * (y * z + x * w) / (-xx - yy + zz + ww));
        pRoll = Maths::ASin(-2.0 * (x * z - y * w));
    }

    /**
     *  Returns the inverted quaternion.
     *  @return The inverted quaternion.
     */
    Quaternion<T> GetInverted() const
    {
        Quaternion<T> invertedQuat;
        invertedQuat = GetConjugate() / GetNormSquare();

        return invertedQuat;
    }

    /**
     *  Invert the quaternion.
     */
    void Invert()
    {
        Quaternion invertedQuat;
        invertedQuat = GetConjugate() / GetNormSquare();

        *this = invertedQuat;
    }

    /**
     *  Returns the normalized quaterion.
     */
    Quaternion GetNormalized() const
    {
        Quaternion<T> normalizedQuat;
        T norm = GetNorm();

        normalizedQuat.w = w / norm;
        normalizedQuat.x = x / norm;
        normalizedQuat.y = y / norm;
        normalizedQuat.z = z / norm;

        return normalizedQuat;
    }

    /**
     *  Returns the square norm of the quaternion.
     */
    T GetNormSquare() const
    {
        return w*w + x*x + y*y + z*z;
    }

    /**
     *  Returns the norm of the quaternion.
     */
    T GetNorm() const
    {
        return Maths::Sqrt(w*w + x*x + y*y + z*z);
    }

    /**
     *  Normalize the quaternion.
     */
    void Normalize()
    {
        T norm = GetNorm();
        w /= norm;
        x /= norm;
        y /= norm;
        z /= norm;
    }

    /**
     *  Returns true if the quaternion is normalized, false otherwise.
     */
    Bool IsNormalized() const
    {
        return (w*w + x*x + y*y + z*z) == 1;
    }

    /**
     *  Returns the conjugate quaternion.
     */
    Quaternion<T> GetConjugate() const
    {
        Quaternion<T> conjugateQuat;

        conjugateQuat.w = w;
        conjugateQuat.x = -x;
        conjugateQuat.y = -y;
        conjugateQuat.z = -z;

        return conjugateQuat;
    }

    /**
     *  Conjugate the quaternion.
     */
    void Conjugate()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    /**
     *  Returns the dot product.
     */
    T operator |(const Quaternion<T>& pQuat) const
    {
        return w*pQuat.w + x*pQuat.x + y*pQuat.y + z*pQuat.z;
    }

    /**
     *  Assignment operator.
     */
    Quaternion<T>& operator=(const Quaternion<T>& pQuat)
    {
        w = pQuat.w;
        x = pQuat.x;
        y = pQuat.y;
        z = pQuat.z;

        return *this;
    }

    /**
     *  Quaternion multiplication operator.
     */
    Quaternion<T> operator*(const Quaternion<T>& pQuat) const
    {
        Quaternion<T> resultQuat;

        resultQuat.w = w*pQuat.w - x*pQuat.x - y*pQuat.y - z*pQuat.z;
        resultQuat.x = w*pQuat.x + x*pQuat.w + y*pQuat.z - z*pQuat.y;
        resultQuat.y = w*pQuat.y - x*pQuat.z + y*pQuat.w + z*pQuat.x;
        resultQuat.z = w*pQuat.z + x*pQuat.y - y*pQuat.x + z*pQuat.w;

        return resultQuat;
    }

    /**
     *  Quaternion multiplication assignment operator.
     */
    Quaternion<T>& operator*=(const Quaternion<T>& pQuat)
    {
        w = w*pQuat.w - x*pQuat.x - y*pQuat.y - z*pQuat.z;
        x = w*pQuat.x + x*pQuat.w + y*pQuat.z - z*pQuat.y;
        y = w*pQuat.y - x*pQuat.z + y*pQuat.w + z*pQuat.x;
        z = w*pQuat.z + x*pQuat.y - y*pQuat.x + z*pQuat.w;

        return *this;
    }

    Vector3<T> operator * ( const Vector3<T>& v )
    {
        Quaternion<T> qTmp( -(x * v.x + y * v.y + z * v.z),
	                          w * v.x + y * v.z - z * v.y,
	                          w * v.y + z * v.x - x * v.z,
	                          w * v.z + x * v.y - y * v.x );

        qTmp *= GetConjugate();
        return Vector3<T>( qTmp.x, qTmp.y, qTmp.z );   
    }

    /**
     *  Scalar multiplication operator.
     */
    Quaternion<T> operator*(T pScalar) const
    {
        Quaternion<T> resultQuat;

        resultQuat.w = w * pScalar;
        resultQuat.x = x * pScalar;
        resultQuat.y = y * pScalar;
        resultQuat.z = z * pScalar;

        return resultQuat;
    }

    /**
     *  Scalar multiplication assignment operator.
     */
    Quaternion<T>& operator*=(T pScalar)
    {
        w *= pScalar;
        x *= pScalar;
        y *= pScalar;
        z *= pScalar;

        return *this;
    }

    /**
     *  Quaternion division operator.
     */
    Quaternion<T> operator/(const Quaternion<T>& pQuat) const
    {
        Quaternion<T> resultQuat;

        resultQuat = pQuat.GetInverted() * (*this);

        return resultQuat;
    }

    /**
     *  Quaternion division assignment operator.
     */
    Quaternion& operator/=(const Quaternion& pQuat)
    {
        *this = pQuat.GetInverted() * (*this);

        return *this;
    }

    /**
     *  Scalar division operator.
     */
    Quaternion<T> operator/(T pScalar) const
    {
        Quaternion<T> resultQuat;

        resultQuat.w = w / pScalar;
        resultQuat.x = x / pScalar;
        resultQuat.y = y / pScalar;
        resultQuat.z = z / pScalar;

        return resultQuat;
    }

    /**
     *  Scalar division assignment operator.
     */
    Quaternion<T>& operator/=(T pScalar)
    {
        w /= pScalar;
        x /= pScalar;
        y /= pScalar;
        z /= pScalar;

        return *this;
    }

    /**
     *  Quaternion addition operator.
     */
    Quaternion<T> operator+(const Quaternion<T>& pQuat) const
    {
        Quaternion<T> resultQuat;

        resultQuat.w = w + pQuat.w;
        resultQuat.x = x + pQuat.x;
        resultQuat.y = y + pQuat.y;
        resultQuat.z = z + pQuat.z;

        return resultQuat;
    }

    /**
     *  Quaternion addition assignment operator.
     */
    Quaternion<T>& operator+=(const Quaternion<T>& pQuat)
    {
        w += pQuat.w;
        x += pQuat.x;
        y += pQuat.y;
        z += pQuat.z;

        return *this;
    }

    /**
     *  Quaternion subtraction operator.
     */
    Quaternion<T> operator-(const Quaternion<T>& pQuat) const
    {
        Quaternion<T> resultQuat;

        resultQuat.w = pQuat.w - w;
        resultQuat.x = x - pQuat.x;
        resultQuat.y = y - pQuat.y;
        resultQuat.z = z - pQuat.z;

        return resultQuat;
    }

    /**
     *  Quaternion subtraction assignment operator.
     */
    Quaternion<T>& operator-=(const Quaternion<T>& pQuat)
    {
        w = pQuat.w - w;
        x -= pQuat.x;
        y -= pQuat.y;
        z -= pQuat.z;

        return *this;
    }

    /**
     *  Quaternion equality operator.
     */
    Bool operator==(const Quaternion<T>& pQuat) const
    {
        return  w == pQuat.w &&
                x == pQuat.x &&
                y == pQuat.y &&
                z == pQuat.z;
    }

    /**
     *  Quaternion inequality operator.
     */
    Bool operator!=(const Quaternion<T>& pQuat) const
    {
        return  w != pQuat.w ||
                x != pQuat.x ||
                y != pQuat.y ||
                z != pQuat.z;
    }

    /**
     *  Returns the logarithm of a quaternion.
     */
    Quaternion<T> Log() const
    {
        Quaternion<T> resultQuat;

        T arcCosValue = Maths::ACos(w);
        T sinusValue = Maths::Sin(arcCosValue);

        resultQuat.w = 0;
        if(sinusValue > 0)
        {
            resultQuat.x = arcCosValue * x / sinusValue;
            resultQuat.y = arcCosValue * y / sinusValue;
            resultQuat.z = arcCosValue * z / sinusValue;
        }
        else
        {
            resultQuat.x = 0;
            resultQuat.y = 0;
            resultQuat.z = 0;
        }

        return resultQuat;
    }

    /**
     *  Returns the exponential for this quaternion.
     *  (i.e. e exp quaternion)
     *  @brief  Returns the exponential for this quaternion.
     */
    Quaternion<T> Exp() const
    {
        Quaternion<T> resultQuat;

        T a = Maths::Sqrt(x*x + y*y + z*z);
        T sina = Maths::Sin(a);
        T cosa = Maths::Cos(a);

        resultQuat.w = cosa;
        if(a > 0)
        {
            resultQuat.x = sina * x / a;
            resultQuat.y = sina * y / a;
            resultQuat.z = sina * z / a;
        }
        else
        {
            resultQuat.x = 0;
            resultQuat.y = 0;
            resultQuat.z = 0;
        }

        return resultQuat;
    }

    /**
     *  Returns the linear interpolation with the given quaternion at time pTime.
     *  Note : Code snippet from the game programming gems.
     *  @brief  Returns the linear interpolation with the given quaternion at time pTime.
     *  @param  pQuat the quaternion to interpolate with.
     *  @param  pTime the interpolation time (ranged [0, 1]).
     *  @return The interpolated quaternion.
     */
    Quaternion<T> Lerp(const Quaternion<T>& pQuat, float pTime) const
    {
        Quaternion<T> resultQuat;

        resultQuat = (*this) + ((pQuat - (*this)) * pTime);
        resultQuat.Normalize();

        return resultQuat;
    }

    /**
     *  Returns the spherical linear interpolation with the given quaternion at time pTime.
     *  This method might invert the second quatertion to reduce the spinning.
     *  Note : Code snippet from the game programming gems.
     *  @brief  Returns the spherical linear interpolation with the given quaternion at time pTime.
     *  @param  pQuat the quaternion to interpolate with.
     *  @param  pTime the interpolation time (ranged [0, 1]).
     *  @return The interpolated quaternion.
     */
    Quaternion<T> Slerp(const Quaternion<T>& pQuat, float pTime) const
    {
        Quaternion<T> tempQuat;

        T dotValue = (*this) | pQuat;

        // dot = cos(theta)
        // if (dot < 0), q1 and q2 are more than 90 degrees apart,
        // so we can invert one to reduce spinning
        if(dotValue < 0)
        {
            dotValue = -dotValue;
            tempQuat = pQuat * -1;
        }
        else
        {
            tempQuat = pQuat;
        }

        if(dotValue < static_cast<T>(0.95))
        {
            T angle = Maths::ACos(dotValue);
            T sina = Maths::Sin(angle);
            T sinat = Maths::Sin(angle * pTime);
            T sinaomt = Maths::Sin(angle * (1 - pTime));

            return ((*this) * sinaomt + tempQuat * sinat) / sina;
        }
        //if the angle is small, use linear interpolation
        else
        {
            return Lerp(tempQuat, pTime);
        }
    }

    /**
     *  Returns the spherical linear interpolation with the given quaternion at time pTime.
     *  This method won't invert the second quatertion to reduce the spinning.
     *  Note : Code snippet from the game programming gems.
     *  @brief  Returns the spherical interpolation with the given quaternion at time pTime.
     *  @param  pQuat the quaternion to interpolate with.
     *  @param  pTime the interpolation time (ranged [0, 1]).
     *  @return The interpolated quaternion.
     */
    Quaternion<T> SlerpNoInvert(const Quaternion<T>& pQuat, float pTime) const
    {
        T dotValue = x*pQuat.x + y*pQuat.y + z*pQuat.z + w*pQuat.w;

        T threshold = static_cast<T>(0.95);
        if(dotValue > -threshold && dotValue < threshold)
        {
            T angle = Maths::ACos(dotValue);
            T sina = Maths::Sin(angle);
            T sinat = Maths::Sin(angle * pTime);
            T sinaomt = Maths::Sin(angle * (1 - pTime));

            return ((*this) * sinaomt + pQuat * sinat) / sina;
        }
        // if the angle is small, use linear interpolation
        else
        {
            return Lerp(pQuat, pTime);
        }
    }

    /**
     *  Returns the spherical cubic interpolation with the given quaternion,
     *  controls points and time pTime.
     *  Note : Code snippet from the game programming gems.
     *  @brief  Returns the quadratic interpolation with the given quaternion at time pTime.
     *  @param  pQuat the quaternion to interpolate with.
     *  @param  pControlPoint1 the first control point.
     *  @param  pControlPoint2 the second control point.
     *  @param  pTime the interpolation time (ranged [0, 1]).
     *  @return The interpolated quaternion.
     */
    Quaternion<T> Squad(const Quaternion<T>& pQuat,
                        const Quaternion<T>& pControlPoint1,
                        const Quaternion<T>& pControlPoint2,
                        float pTime) const
    {
        Quaternion<T> c = SlerpNoInvert(pQuat, pTime);
        Quaternion<T> d = pControlPoint1.SlerpNoInvert(pControlPoint2, pTime);

        return c.SlerpNoInvert(d, 2 * pTime * (1-pTime));
    }

    /**
     *  Returns the quaternion control point to be used in a spline interpolation.
     *  The control point is calculated with 3 quaternions : q - 1, q and q + 1
     *  Note : Code snippet from the game programming gems.
     *  @brief  Returns the control point calculated from 3 quaternions.
     *  @param  pQuatA the quaternion q - 1.
     *  @param  pQuatB the quaternion q.
     *  @param  pQuatC the quaternion q + 1.
     *  @return The calculated control point.
     */
    Quaternion<T> Spline(const Quaternion<T>& pQuatA,
                         const Quaternion<T>& pQuatB,
                         const Quaternion<T>& pQuatC) const
    {
        Quaternion<T> tempQuat;

        tempQuat.x = -pQuatB.x;
        tempQuat.y = -pQuatB.y;
        tempQuat.z = -pQuatB.z;
        tempQuat.w = pQuatB.w;

        Quaternion<T> logQuatA = (tempQuat * pQuatA).Log();
        Quaternion<T> logQuatB = (tempQuat * pQuatC).Log();
        Quaternion<T> expQuat = (logQuatA + logQuatB) / -4;

        return pQuatB * expQuat.Exp();
    }

    /**
     *  Returns the quaternion that represents the rotation arc between the two
     *  given vectors.
     *  Note : Code snippet from the game programming gems.
     *  @brief  Returns the quaternion representing the rotation arc between 2 vectors.
     *  @param  pVectorA The first vector.
     *  @param  pVectorB The second vector.
     *  @return The quaternion representing the rotation arc between the 2 vectors.
     */
    Quaternion<T> GetRotationArc(const Vector3<T>& pVectorA, const Vector3<T>& pVectorB) const
    {
        Quaternion<T> resultQuat;

        Vector3<T> crossProduct = pVectorA ^ pVectorB;
        T dotProduct = pVectorA | pVectorB;
        T square = Maths::Sqrt((1 + dotProduct) * 2);
        resultQuat.x = crossProduct.x / square;
        resultQuat.y = crossProduct.y / square;
        resultQuat.z = crossProduct.z / square;
        resultQuat.w = square / 2.0;

        return resultQuat;
    }

    /**
     *  Serialize this quaternion to/from a stream.
     *  @param  pStream Stream used for serialization.
     *  @return The stream object.
     */
    friend Stream& operator << ( Stream& pStream, Quaternion<T>& pQuat )
    {
        pStream << pQuat.w << pQuat.x << pQuat.y << pQuat.z;
        return pStream;
    }

    // Quaternion data.
    T w, x, y, z;
};


typedef Quaternion<Float>   Quaternionf;
typedef Quaternion<Double>  Quaterniond;


} // namespace Gamedesk


#endif  //  _QUATERNION_H_
