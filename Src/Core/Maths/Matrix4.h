/**
 *  @file       Matrix4.h
 *  @brief      Matrix4 class (4x4).
 *  @author     Sebastien Lussier.
 *  @date       19/01/02.
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
#ifndef     _MATRIX4_H_
#define     _MATRIX4_H_


#define OPENGL_MATRIX


#include "Maths.h"
#include "Vector3.h"


namespace Gamedesk {


/**
 *  Matrix class used in 3d computations.  Has all the normal operations needed.
 *  If you run into an operation wich is not included in the class, add it in there.
 *  @brief  4x4 Matrix class.
 *  @author Sebastien Lussier.
 *  @date   19/01/02.
 *  @todo   Add these methods : Ortho(), Reflect(plane), RotateQuaternion(),
 *          RotateYawPitchRoll(), Shadow()
 */
template <class T>
class Matrix4
{
public:
    /**
     *  Will not initialize the matrix elements.
     *  @brief  Default constructor.
     */
    Matrix4()
    {
    }

    /**
     *  Constructor taking all the elements of the matrix as parameters.
     *  @brief  Constructor.
     */
    Matrix4( const T& p1, const T& p2, const T& p3, const T& p4,
             const T& p5, const T& p6, const T& p7, const T& p8,
             const T& p9, const T& p10, const T& p11, const T& p12,
             const T& p13, const T& p14, const T& p15, const T& p16 )
    {
        elem16[0] = p1;     elem16[1] = p2;     elem16[2] = p3;     elem16[3] = p4;
        elem16[4] = p5;     elem16[5] = p6;     elem16[6] = p7;     elem16[7] = p8;
        elem16[8] = p9;     elem16[9] = p10;    elem16[10] = p11;   elem16[11] = p12;
        elem16[12] = p13;   elem16[13] = p14;   elem16[14] = p15;   elem16[15] = p16;
    }

    /**
     *  Copy constructor taking another matrix in parameter.
     *  @brief  Copy constructor.
     *  @param  pMatrix   a matrix, to be copied to this new one.
     */
    Matrix4( const Matrix4& pMatrix ) :
#ifdef OPENGL_MATRIX
        _11(pMatrix._11), _21(pMatrix._21), _31(pMatrix._31), _41(pMatrix._41),
        _12(pMatrix._12), _22(pMatrix._22), _32(pMatrix._32), _42(pMatrix._42),
        _13(pMatrix._13), _23(pMatrix._23), _33(pMatrix._33), _43(pMatrix._43),
        _14(pMatrix._14), _24(pMatrix._24), _34(pMatrix._34), _44(pMatrix._44)
#else
	    _11(pMatrix._11), _12(pMatrix._12), _13(pMatrix._13), _14(pMatrix._14),
        _21(pMatrix._21), _22(pMatrix._22), _23(pMatrix._23), _24(pMatrix._24),
        _31(pMatrix._31), _32(pMatrix._32), _33(pMatrix._33), _34(pMatrix._34),
        _41(pMatrix._41), _42(pMatrix._42), _43(pMatrix._43), _44(pMatrix._44)
#endif
    {
    }

	/**
     *  Assignment operator. Set this matrix to the content of another matrix.
     *  @brief  Assignment operator.
     *  @param  pMatrix   a matrix, to be copied to this new one.
     *  @return A reference to this matrix.
     */
    const Matrix4& operator = ( const Matrix4& pMatrix )
    {
        memcpy( elem16, pMatrix.elem16, sizeof(elem16) );
        return *this;
    }

    /**
     *  Equal operator. Test if this matrix is equal to another matrix.
     *  @brief  Equal operator.
     *  @param  pMatrix   a matrix, to be tested for equality with this one.
     *  @return A boolean, \b true if the two matrix are equal (all members are equals), or \b false otherwise.
     */
    Bool operator == ( const Matrix4& pMatrix ) const
    {
       return(  _11 == pMatrix._11 && _12 == pMatrix._12 && _13 == pMatrix._13 && _14 == pMatrix._14 &&
                _21 == pMatrix._21 && _22 == pMatrix._22 && _23 == pMatrix._23 && _24 == pMatrix._24 &&
                _31 == pMatrix._31 && _32 == pMatrix._32 && _33 == pMatrix._33 && _34 == pMatrix._34 &&
                _41 == pMatrix._41 && _42 == pMatrix._42 && _43 == pMatrix._43 && _44 == pMatrix._44 );
    }

    /**
     *  Not equal operator. Test if this matrix is not equal to another matrix.
     *  @brief  Not equal operator.
     *  @param  pMatrix   a matrix, to be tested for !equality with this one.
     *  @return A boolean, \b false if the two matrix are equal (all members are equals), or \b true otherwise.
     */
    Bool operator != ( const Matrix4& pMatrix ) const
    {
       return(  _11 != pMatrix._11 || _12 != pMatrix._12 || _13 != pMatrix._13 || _14 != pMatrix._14 ||
                _21 != pMatrix._21 || _22 != pMatrix._22 || _23 != pMatrix._23 || _24 != pMatrix._24 ||
                _31 != pMatrix._31 || _32 != pMatrix._32 || _33 != pMatrix._33 || _34 != pMatrix._34 ||
                _41 != pMatrix._41 || _42 != pMatrix._42 || _43 != pMatrix._43 || _44 != pMatrix._44 );
    }

    /**
     *  Addition operator. Add a matrix to this matrix.
     *  @brief  Addition operator.
     *  @param  pMatrix   a matrix, to be added to this matrix.
     *  @return A new matrix, the result of the addition of the two matrix.
     */
    Matrix4 operator + ( const Matrix4& pMatrix ) const
    {
        return Matrix4( _11 + pMatrix._11, _12 + pMatrix._12, _13 + pMatrix._13, _14 + pMatrix._14,
                        _21 + pMatrix._21, _22 + pMatrix._22, _23 + pMatrix._23, _24 + pMatrix._24,
                        _31 + pMatrix._31, _32 + pMatrix._32, _33 + pMatrix._33, _34 + pMatrix._34,
                        _41 + pMatrix._41, _42 + pMatrix._42, _43 + pMatrix._43, _44 + pMatrix._44 );
    }

    /**
     *  Substraction operator. Substract a matrix to this matrix.
     *  @brief  Substraction operator.
     *  @param  pMatrix   a matrix, to be substracted to this matrix.
     *  @return A new matrix, the result of the substraction of the two matrix.
     */
    Matrix4 operator - ( const Matrix4& pMatrix ) const
    {
        return Matrix4( _11 - pMatrix._11, _12 - pMatrix._12, _13 - pMatrix._13, _14 - pMatrix._14,
                        _21 - pMatrix._21, _22 - pMatrix._22, _23 - pMatrix._23, _24 - pMatrix._24,
                        _31 - pMatrix._31, _32 - pMatrix._32, _33 - pMatrix._33, _34 - pMatrix._34,
                        _41 - pMatrix._41, _42 - pMatrix._42, _43 - pMatrix._43, _44 - pMatrix._44 );
    }

    /**
     *  Unary minus operator. Invert the sign of the matrix.
     *  @brief  Unary minus operator.
     *  @return A new matrix, same as this one but with the signs of all the elements inverted.
     */
    Matrix4 operator - () const
    {
        return Matrix4( - _11, - _12, - _13, - _14,
                        - _21, - _22, - _23, - _24,
                        - _31, - _32, - _33, - _34,
                        - _41, - _42, - _43, - _44 );
    }

    /**
     *  Multiply all the elements of this matrix by a factor given in parameter.
     *  @brief  Multiplication operator.
     *  @param  pFactor     a T, the factor by wich we want to multiply this matrix.
     *  @return A new matrix, same as this one but with all the elements multiplied by \a fFactor.
     */
    Matrix4 operator * ( const T& pFactor ) const
    {
        return Matrix4( _11 * pFactor, _12 * pFactor, _13 * pFactor, _14 * pFactor,
                        _21 * pFactor, _22 * pFactor, _23 * pFactor, _24 * pFactor,
                        _31 * pFactor, _32 * pFactor, _33 * pFactor, _34 * pFactor,
                        _41 * pFactor, _42 * pFactor, _43 * pFactor, _44 * pFactor );
    }

    friend Vector3<T> operator * ( const Vector3<T>& v, const Matrix4<T>& m  )
	{
		return( Vector3<T>( m._11*v.x + m._12*v.y + m._13*v.z + m._14,
							m._21*v.x + m._22*v.y + m._23*v.z + m._24,
							m._31*v.x + m._32*v.y + m._33*v.z + m._34 ) );
	}

    /**
     *  Addition assigment operator. Add a matrix to this matrix.
     *  @brief  Addition assigment operator.
     *  @param  pMatrix   a matrix, to be added to this matrix.
     *  @return A const reference to this matrix.
     */
    const Matrix4& operator += ( const Matrix4& pMatrix )
    {
        _11 += pMatrix._11; _12 += pMatrix._12; _13 += pMatrix._13; _14 += pMatrix._14;
        _21 += pMatrix._21; _22 += pMatrix._22; _23 += pMatrix._23; _24 += pMatrix._24;
        _31 += pMatrix._31; _32 += pMatrix._32; _33 += pMatrix._33; _34 += pMatrix._34;
        _41 += pMatrix._41; _42 += pMatrix._42; _43 += pMatrix._43; _44 += pMatrix._44;
        return *this;
    }

    /**
     *  Substract a matrix to this matrix.
     *  @brief  Substraction assigment operator.
     *  @param  pMatrix   a matrix, to be substracted to this matrix.
     *  @return A const reference to this matrix.
     */
    const Matrix4& operator -= ( const Matrix4& pMatrix )
    {
        _11 -= pMatrix._11; _12 -= pMatrix._12; _13 -= pMatrix._13; _14 -= pMatrix._14;
        _21 -= pMatrix._21; _22 -= pMatrix._22; _23 -= pMatrix._23; _24 -= pMatrix._24;
        _31 -= pMatrix._31; _32 -= pMatrix._32; _33 -= pMatrix._33; _34 -= pMatrix._34;
        _41 -= pMatrix._41; _42 -= pMatrix._42; _43 -= pMatrix._43; _44 -= pMatrix._44;
        return *this;
    }

    /**
     *  Multiply all the elements of this matrix by a factor given in parameter.
     *  @brief  Multiplication assigment operator using a factor.
     *  @param  pFactor     a T, the factor by wich we want to multiply this matrix.
     *  @return A new matrix, same as this one but with all the elements multiplied by \a fFactor.
     */
    const Matrix4& operator *= ( const T& pFactor )
    {
        _11 *= pFactor; _12 *= pFactor; _13 *= pFactor; _14 *= pFactor;
        _21 *= pFactor; _22 *= pFactor; _23 *= pFactor; _24 *= pFactor;
        _31 *= pFactor; _32 *= pFactor; _33 *= pFactor; _34 *= pFactor;
        _41 *= pFactor; _42 *= pFactor; _43 *= pFactor; _44 *= pFactor;
        return *this;
    }

    /**
     *  Matrix multiplication operator. Multiply this matrix by another matrix.
     *  @brief  Matrix multiplication operator using a matrix.
     *  @param  pMatrix     a matrix, the matrix to be multiplied with this one.
     *  @return A new matrix, the result of the multiplication of the two matrix.
     */
    Matrix4 operator * ( const Matrix4& pMatrix ) const
    {
        return Matrix4( M[0][0] * pMatrix(0,0) + M[0][1] * pMatrix(1,0) + M[0][2] * pMatrix(2,0) + M[0][3] * pMatrix(3,0),
                        M[0][0] * pMatrix(0,1) + M[0][1] * pMatrix(1,1) + M[0][2] * pMatrix(2,1) + M[0][3] * pMatrix(3,1),
                        M[0][0] * pMatrix(0,2) + M[0][1] * pMatrix(1,2) + M[0][2] * pMatrix(2,2) + M[0][3] * pMatrix(3,2),
                        M[0][0] * pMatrix(0,3) + M[0][1] * pMatrix(1,3) + M[0][2] * pMatrix(2,3) + M[0][3] * pMatrix(3,3),
                        M[1][0] * pMatrix(0,0) + M[1][1] * pMatrix(1,0) + M[1][2] * pMatrix(2,0) + M[1][3] * pMatrix(3,0),
                        M[1][0] * pMatrix(0,1) + M[1][1] * pMatrix(1,1) + M[1][2] * pMatrix(2,1) + M[1][3] * pMatrix(3,1),
                        M[1][0] * pMatrix(0,2) + M[1][1] * pMatrix(1,2) + M[1][2] * pMatrix(2,2) + M[1][3] * pMatrix(3,2),
                        M[1][0] * pMatrix(0,3) + M[1][1] * pMatrix(1,3) + M[1][2] * pMatrix(2,3) + M[1][3] * pMatrix(3,3),
                        M[2][0] * pMatrix(0,0) + M[2][1] * pMatrix(1,0) + M[2][2] * pMatrix(2,0) + M[2][3] * pMatrix(3,0),
                        M[2][0] * pMatrix(0,1) + M[2][1] * pMatrix(1,1) + M[2][2] * pMatrix(2,1) + M[2][3] * pMatrix(3,1),
                        M[2][0] * pMatrix(0,2) + M[2][1] * pMatrix(1,2) + M[2][2] * pMatrix(2,2) + M[2][3] * pMatrix(3,2),
                        M[2][0] * pMatrix(0,3) + M[2][1] * pMatrix(1,3) + M[2][2] * pMatrix(2,3) + M[2][3] * pMatrix(3,3),
                        M[3][0] * pMatrix(0,0) + M[3][1] * pMatrix(1,0) + M[3][2] * pMatrix(2,0) + M[3][3] * pMatrix(3,0),
                        M[3][0] * pMatrix(0,1) + M[3][1] * pMatrix(1,1) + M[3][2] * pMatrix(2,1) + M[3][3] * pMatrix(3,1),
                        M[3][0] * pMatrix(0,2) + M[3][1] * pMatrix(1,2) + M[3][2] * pMatrix(2,2) + M[3][3] * pMatrix(3,2),
                        M[3][0] * pMatrix(0,3) + M[3][1] * pMatrix(1,3) + M[3][2] * pMatrix(2,3) + M[3][3] * pMatrix(3,3) );
    }

    /**
     *  Matrix multiplication assigment operator. Multiply this matrix by another matrix.
     *  @brief  Matrix multiplication assigment operator.
     *  @param  pMatrix     a matrix, the matrix to be multiplied with this one.
     *  @return A reference to this matrix.
     */
    const Matrix4& operator *= ( const Matrix4& pMatrix )
    {
        Matrix4 matTemp( *this );
        Int32   row, col;

        for( row = 0; row < 4; row++ )
            for( col = 0; col < 4; col++ )
                M[row][col] = matTemp(row,0) * pMatrix(0,col) +
                              matTemp(row,1) * pMatrix(1,col) +
                              matTemp(row,2) * pMatrix(2,col) +
                              matTemp(row,3) * pMatrix(3,col);

        return *this;
    }

    /**
     *  Cast to a T pointer operator. Cast the matrix to an array of T.
     *  Useful for OpenGL calls.
     *  @brief  Cast the matrix to a T pointer (const).
     *  @return A pointer to an array of 16 T.
     */
    operator const T*() const
    {
        return elem16;
    }

    /**
     *  Cast to a T pointer operator. Cast the matrix to an array of T.
     *  Useful for OpenGL calls.
     *  @brief  Cast the matrix to a T pointer.
     *  @return A pointer to an array of 16 T.
     */
    operator T*()
    {
        return elem16;
    }

    /**
     *  Get the element a position <b>(iRow,iCol)</b> in the matrix.
     *  @brief  Get the element a position (iRow,iCol) in the matrix.
     *  @param  pRow    an integer, the row index in the matrix.
     *  @param  pCol    an integer, the column index in the matrix.
     *  @return A T reference, the element at position <b>(iRow,iCol)</b>.
     */
    INLINE T& GetElem( Int32 pRow, Int32 pCol )
    {
        return M[pRow][pCol];
    }

    /**
     *  Get the element a position <b>(iRow,iCol)</b> in the matrix.
     *  @brief  Get the element a position (iRow,iCol) in the matrix.
     *  @param  pRow    an integer, the row index in the matrix.
     *  @param  pCol    an integer, the column index in the matrix.
     *  @return A const T reference, the element at position <b>(iRow,iCol)</b>.
     */
    INLINE const T& GetElem( Int32 pRow, Int32 pCol ) const
    {
        return M[pRow][pCol];
    }

    /**
     *  Get the element a position <b>(iRow,iCol)</b> in the matrix.
     *  @brief  Get the element a position (iRow,iCol) in the matrix.
     *  @param  pRow    an integer, the row index in the matrix.
     *  @param  pCol    an integer, the column index in the matrix.
     *  @return A T reference, the element at position <b>(iRow,iCol)</b>.
     */
    T& operator () ( Int32 pRow, Int32 pCol )
    {
        return M[pRow][pCol];
    }

    /**
     *  Get the element a position <b>(iRow,iCol)</b> in the matrix.
     *  @brief  Get the element a position (iRow,iCol) in the matrix.
     *  @param  pRow    an integer, the row index in the matrix.
     *  @param  pCol    an integer, the column index in the matrix.
     *  @return A const T reference, the element at position <b>(iRow,iCol)</b>.
     */
    const T& operator () ( Int32 pRow, Int32 pCol ) const
    {
        return M[pRow][pCol];
    }

	//! Get the element at position pIndex.
	T& operator()(Int32 pIndex)
	{
		return elem16[pIndex];
	}

	//! Get the element at position pIndex.
	const T& operator()(Int32 pIndex) const
	{
		return elem16[pIndex];
	}

    /**
     *  Obtain the transpose of this matrix.
     *  @brief  Matrix4 transposition.
     *  @return A new matrix, wich is the transpose of this matrix.
     */
    Matrix4 GetTranspose() const
    {
        return Matrix4( M[0][0], M[1][0], M[2][0], M[3][0],
                        M[0][1], M[1][1], M[2][1], M[3][1],
                        M[0][2], M[1][2], M[2][2], M[3][2],
                        M[0][3], M[1][3], M[2][3], M[3][3] );
    }

    /**
     *  Set this matrix to the transpose of this matrix.
     *  @brief  Matrix4 transposition.
     *  @return A const reference to this matrix.
     */
    const Matrix4& Transpose()
    {
        Matrix4 matTemp(*this);
        Int32   row, col;

        for( row = 0; row < 4; row++ )
            for( col = 0; col < 4; col++ )
                M[row][col] = matTemp(col, row);

        return *this;
    }

    /**
     *  Find the inverse of a matrix that is made up of only scales, rotations,
     *  and translations. Do not use to get the inverse of a projection matrix...
     *  Generally speaking, a matrix doesn't need to be bigger than 4x3, but
     *  since this code is to work with GL and it uses 4x4 matrices...
     *  @brief  Obtain the affine inverse of this matrix.
     *  @return A new matrix, wich is the affine inverse of this matrix.
     */
    Matrix4 GetAffineInverse() const
    {
        Matrix4  matInverse;
        T   Tx, Ty, Tz;

        // The rotational part of the matrix is simply the transpose of the
        // original matrix.
        matInverse(0,0) = M[0][0];
        matInverse(1,0) = M[0][1];
        matInverse(2,0) = M[0][2];

        matInverse(0,1) = M[1][0];
        matInverse(1,1) = M[1][1];
        matInverse(2,1) = M[1][2];

        matInverse(0,2) = M[2][0];
        matInverse(1,2) = M[2][1];
        matInverse(2,2) = M[2][2];

        // The right column vector of the matrix should always be [ 0 0 0 1 ]
        // In most cases. . . you don't need this column at all because it'll
        // never be used in the program, but since this code is used with GL
        // and it does consider this column, it is here.
        matInverse(0,3) = matInverse(1,3) = matInverse(2,3) = 0;
        matInverse(3,3) = 1;

        // The translation components of the original matrix.
        Tx = M[3][0];
        Ty = M[3][1];
        Tz = M[3][2];

        // Result = -(Tm * Rm) to get the translation part of the inverse
        matInverse(3,0) = -( M[0][0] * Tx + M[0][1] * Ty + M[0][2] * Tz );
        matInverse(3,1) = -( M[1][0] * Tx + M[1][1] * Ty + M[1][2] * Tz );
        matInverse(3,2) = -( M[2][0] * Tx + M[2][1] * Ty + M[2][2] * Tz );

        return matInverse;
    }

    INLINE Float Determinant() const
	{
		return	M[0][0] * (
					M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
					M[2][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
					M[3][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
					) -
				M[1][0] * (
					M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
					M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
					M[3][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])
					) +
				M[2][0] * (
					M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
					M[1][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
					M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
					) -
				M[3][0] * (
					M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
					M[1][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
					M[2][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
					);
	}

    Matrix4 Inverse() const
    {
        Matrix4 matInverse;

		Float	Det = Determinant();

		if( Det == 0.0f )
			return Matrix4::IDENTITY;

		Float	invDet = 1.0f / Det;

		matInverse(0,0) = invDet * (
				M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				M[2][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
				M[3][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
				);

		matInverse(0,1) = -invDet * (
				M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])
				);
		matInverse(0,2) = invDet * (
				M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
				M[1][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
				);
		matInverse(0,3) = -invDet * (
				M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
				M[1][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
				M[2][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
				);

		matInverse(1,0) = -invDet * (
				M[1][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				M[2][0] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
				M[3][0] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
				);
		matInverse(1,1) = invDet * (
				M[0][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
				M[2][0] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][0] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])
				);
		matInverse(1,2) = -invDet * (
				M[0][0] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
				M[1][0] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
				M[3][0] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
				);
		matInverse(1,3) = invDet * (
				M[0][0] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
				M[1][0] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
				M[2][0] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
				);

		matInverse(2,0) = invDet * (
				M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
				M[2][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) +
				M[3][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1])
				);
		matInverse(2,1) = -invDet * (
				M[0][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
				M[2][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
				M[3][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1])
				);
		matInverse(2,2) = invDet * (
				M[0][0] * (M[1][1] * M[3][3] - M[1][3] * M[3][1]) -
				M[1][0] * (M[0][1] * M[3][3] - M[0][3] * M[3][1]) +
				M[3][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
				);
		matInverse(2,3) = -invDet * (
				M[0][0] * (M[1][1] * M[2][3] - M[1][3] * M[2][1]) -
				M[1][0] * (M[0][1] * M[2][3] - M[0][3] * M[2][1]) +
				M[2][0] * (M[0][1] * M[1][3] - M[0][3] * M[1][1])
				);

		matInverse(3,0) = -invDet * (
				M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
				M[2][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) +
				M[3][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1])
				);
		matInverse(3,1) = invDet * (
				M[0][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
				M[2][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
				M[3][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1])
				);
		matInverse(3,2) = -invDet * (
				M[0][0] * (M[1][1] * M[3][2] - M[1][2] * M[3][1]) -
				M[1][0] * (M[0][1] * M[3][2] - M[0][2] * M[3][1]) +
				M[3][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
				);
		matInverse(3,3) = invDet * (
				M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
				M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
				M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])
				);

        return matInverse;
    }

    /**
     *  Set this matrix to the identity matrix.
     *  @brief  Set this matrix to the identity matrix.
     *  @return A const reference to this matrix.
     */
    const Matrix4& SetIdentity()
    {
        return *this = Matrix4::IDENTITY;
    }

    /**
     *  Set this matrix to a null matrix (full of 0's).
     *  @brief  Set all the elements in this matrix to 0.
     *  @return A const reference to this matrix.
     */
    const Matrix4& SetNull()
    {
        return *this = Matrix4::ZERO;
    }

    /**
     *  Verify if this matrix is equal to the identity matrix.
     *  @brief  Is this matrix equal to the identity matrix ?
     *  @return \b True if this matrix is equal to the identity matrix, \b false otherwise.
     */
    Bool IsIdentity() const
    {
        return( *this == Matrix4::IDENTITY );
    }

    /**
     *  Verify if this matrix is equal to a null matrix.
     *  @brief  Is this matrix equal to a null matrix ?
     *  @return \b True if this matrix is equal to a null matrix, \b false otherwise.
     */
    Bool IsNull() const
    {
        return( *this == Matrix4::ZERO );
    }

    //! Create a "lookat" matrix.
    const Matrix4& LookAt( const Vector3<T>& pFrom, const Vector3<T>& pAt, const Vector3<T>& pUp )
    {
        Matrix4 rot;

        Vector3<T> view = pAt - pFrom;
        view.Normalize();

        Vector3<T> side = view cross pUp.GetNormalized();
        side.Normalize();

        Vector3<T> up = side cross view;
        up.Normalize();

        rot._11 = side.x;
        rot._21 = side.y;
        rot._31 = side.z;
        rot._41 = 0.0;

        rot._12 = up.x;
        rot._22 = up.y;
        rot._32 = up.z;
        rot._42 = 0.0;

        rot._13 = -view.x;
        rot._23 = -view.y;
        rot._33 = -view.z;
        rot._43 = 0.0;

        rot._14 = 0.0f;
        rot._24 = 0.0f;
        rot._34 = 0.0f;
        rot._44 = 1.0f;

        *this = Matrix4::Translation(-pFrom.x, -pFrom.y, -pFrom.z) * rot;

        return *this;
    }

    /**
     *  Build a perpsective projection matrix from the field-of-view (fov, in y),
     *  aspect ratio, near plane (D), and far plane (F).
     *  Note that the projection matrix is normalized for element [3][4] to be 1.0.
     *  This is performed so that W-based range fog will work correctly.
     *  @brief  Build a projection matrix.
     *  @param  pFOV        the field of view in radians.
     *  @param  pAspect     the aspect ration wanted (lW/lH).
     *  @param  pNearPlane  the near plane distance from the camera.
     *  @param  pFarPlane   the far plane distance from the camera.
     *  @return A const reference to this matrix.
     */
    const Matrix4& ProjectionMatrix( T pFOV, T pAspect, T pNearPlane, T pFarPlane )
    {
        if( Maths::Abs(pFarPlane-pNearPlane) < 0.01 )
            return Matrix4::ZERO;    // TODO : throw Exception

        if( Maths::Abs(Maths::Sin(pFOV/2.0)) < 0.01 )
            return Matrix4::ZERO;        // TODO : throw Exception

        T w = pAspect * ( Maths::Cos(pFOV/2.0)/Maths::Sin(pFOV/2.0) );
        T h = 1.0 * ( Maths::Cos(pFOV/2.0)/Maths::Sin(pFOV/2.0) );
        T q = pFarPlane / ( pFarPlane - pNearPlane );

        SetNull();

        _11 = w;
        _22 = h;
        _33 = q;
        _34 = 1.0;
        _43 = -q*pNearPlane;

        return *this;
    }

    /**
     *  Build a transformation that produces a parallel projection
     *  @param  pLeft           the coordinate for the left vertical clipping plane.
     *  @param  pRight          the coordinate for the right vertical clipping plane.
     *  @param  pBottom         the coordinate for the bottom horizontal clipping plane.
     *  @param  pTop            the coordinate for the top horizontal clipping plane.
     *  @param  pNearVal        the distance to the nearer depth clipping plane. This value is negative if the plane is to be behind the viewer.
     *  @param  pFarVal         the distance to the farther depth clipping plane. This value is negative if the plane is to be behind the viewer.
     */
    const Matrix4& Ortho(T pLeft, T pRight, T pBottom, T pTop, T pNearVal, T pFarVal)
    {
        M[0,0] = T(2) / (pRight-pLeft);
        M[0,1] = T(0);
        M[0,2] = T(0);
        M[0,3] = -(pRight + pLeft) / (pRight - pLeft);

        M[1,0] = T(0);
        M[1,1] = T(2) / (pTop - pBottom);
        M[1,2] = T(0);
        M[1,3] = -(pTop + pBottom) / (pTop - pBottom);

        M[2,0] = T(0);
        M[2,1] = T(0);
        M[2,2] = T(-2) / (pFarVal- pNearVal);
        M[2,3] = -(pFarVal + pNearVal) / (pFarVal - pNearVal);

        M[3,0] = T(0);
        M[3,1] = T(0);
        M[3,2] = T(0);
        M[3,3] = T(1);

        return *this;
    }

    /**
     *  Rotate the matrix around the X axis.
     *  @param  pRads   the rotation angle around the x axis, in radians.
     *  @return A const reference to this matrix.
     */
    const Matrix4& RotateX( T pRads )
    {
        return *this *= Matrix4::RotationX( pRads );
    }

    /**
     *  Rotate the matrix around the Y axis.
     *  @param  pRads   the rotation angle around the y axis, in radians.
     *  @return A const reference to this matrix.
     */
    const Matrix4& RotateY( T pRads )
    {
        return *this *= Matrix4::RotationY( pRads );
    }

    /**
     *  Rotate the matrix around the Z axis.
     *  @param  pRads   the rotation angle around the z axis, in radians.
     *  @return A const reference to this matrix.
     */
    const Matrix4& RotateZ( T pRads )
    {
        return *this *= Matrix4::RotationZ( pRads );
    }

    /**
     *  Translate the current matrix.
     *  @param  pX      the translation on the x axis.
     *  @param  pY      the translation on the y axis.
     *  @param  pZ      the translation on the z axis.
     *  @return A const reference to this matrix.
     */
    INLINE const Matrix4& Translate( T pX, T pY, T pZ )
    {
        return *this *= Matrix4::Translation( pX, pY, pZ );
    }

    /**
     *  Translate the current matrix.
     *  @param  pTranslation    a vector, the translation vector.
     *  @return A const reference to this matrix.
     */
    INLINE const Matrix4& Translate( const Vector3<T>& pTranslation )
    {
        return Translate( pTranslation.x, pTranslation.y, pTranslation.z );
    }

    /**
     *  Scale the current matrix.
     *  @param  pX      the scaling on the x axis.
     *  @param  pY      the scaling on the y axis.
     *  @param  pZ      the scaling on the z axis.
     *  @return A const reference to this matrix.
     */
    const Matrix4& Scale( T pX, T pY, T pZ )
    {
        return *this *= Matrix4::Scaling( pX, pY, pZ );
    }

    /**
     *  Scale the current matrix.
     *  @param  pScaling    a vector, the scaling vector.
     *  @return A const reference to this matrix.
     */
    const Matrix4& Scale( const Vector3<T>& pScaling )
    {
        return Scale( pScaling.x, pScaling.y, pScaling.z );
    }

    //! Create a translation matrix.
    static INLINE Matrix4 Translation( T pX, T pY, T pZ )
    {
        return Matrix4( 1,   0,  0,  0,
                        0,   1,  0,  0,
                        0,   0,  1,  0,
                        pX, pY, pZ,  1 );
    }

    //! Create a translation matrix.
    static INLINE Matrix4 Translation( const Vector3<T>& v )
    {
        return Translation( v.x, v.y, v.z );
    }

    //! Create a rotation matrix (around X).
    static INLINE Matrix4 RotationX( T pRad )
    {
        T s = Maths::Sin( pRad );
        T c = Maths::Cos( pRad );

        return Matrix4( 1,  0,  0,  0,
                        0,  c,  s,  0,
                        0, -s,  c,  0,
                        0,  0,  0,  1 );
    }

    //! Create a rotation matrix (around Y).
    static INLINE Matrix4 RotationY( T pRad )
    {
        T s = Maths::Sin( pRad );
        T c = Maths::Cos( pRad );

        return Matrix4( c,  0, -s,  0,
                        0,  1,  0,  0,
                        s,  0,  c,  0,
                        0,  0,  0,  1 );
    }

    //! Create a rotation matrix (around Z).
    static INLINE Matrix4 RotationZ( T pRad )
    {
        T s = Maths::Sin( pRad );
        T c = Maths::Cos( pRad );

        return Matrix4(  c,  s,  0,  0,
                        -s,  c,  0,  0,
                         0,  0,  1,  0,
                         0,  0,  0,  1 );
    }

    //! Create a rotation matrix around a given axis.
    static INLINE Matrix4 AxisRotation( T pRad, T pX, T pY, T pZ )
    {
        return AxisRotation( pRad, Vector3<T>( pX, pY, pZ ) );
    }

    //! Create a rotation matrix around a given axis.
    static INLINE Matrix4 AxisRotation( T pRad, const Vector3<T>& pAxis )
    {
        Vector3<T>  a( pAxis.GetNormalized() );
        T           sin  = Maths::Sin( pRad );
        T           cos  = Maths::Cos( pRad );
        T           icos = 1-cos;

        return Matrix4(       a.x * a.x * icos + cos,  a.y * a.x * icos + (a.z*sin),  a.z * a.x * icos - (a.y*sin),   0,
                        a.x * a.y * icos - (a.z*sin),  a.y * a.y * icos + cos,        a.z * a.y * icos + (a.x*sin),   0,
                        a.x * a.z * icos + (a.y*sin),  a.y * a.z * icos - (a.x*sin),  a.z * a.z * icos + cos,         0,
                                                   0,                             0,                             0,   1 );
    }

    //! Create a scaling matrix.
    static INLINE Matrix4 Scaling( T pX, T pY, T pZ )
    {
        return Matrix4( pX,  0,  0,  0,
                         0, pY,  0,  0,
                         0,  0, pZ,  0,
                         0,  0,  0,  1 );
    }

    //! Create a scaling matrix.
    static INLINE Matrix4 Scaling( const Vector3<T>& pScaling )
    {
        return Scaling( pScaling.x, pScaling.y, pScaling.z );
    }

 	//! Transform the given vector.
	Vector3<T> Transform(const Vector3<T>& pVector)
	{
		return Vector3<T>( _11*pVector.x + _12*pVector.y + _13*pVector.z + _14,
                           _21*pVector.x + _22*pVector.y + _23*pVector.z + _24,
                           _31*pVector.x + _32*pVector.y + _33*pVector.z + _34 );
	}

    //! Transform the given vector.
	Vector3<T> Transform(const Vector3<T>& pVector, T w )
	{
		return Vector3<T>( _11*pVector.x + _12*pVector.y + _13*pVector.z + w*_14,
                           _21*pVector.x + _22*pVector.y + _23*pVector.z + w*_24,
                           _31*pVector.x + _32*pVector.y + _33*pVector.z + w*_34 );
	}

    /**
     *  Serialize this matrix to/from a stream.
     *  @param  pStream Stream used for serialization.
     *  @return The stream object.
     */
    friend Stream& operator << ( Stream& pStream, Matrix4<T>& pMatrix )
    {
        pStream << pMatrix.M[0][0] << pMatrix.M[0][1] << pMatrix.M[0][2] << pMatrix.M[0][3]
                << pMatrix.M[1][0] << pMatrix.M[1][1] << pMatrix.M[1][2] << pMatrix.M[1][3]
                << pMatrix.M[2][0] << pMatrix.M[2][1] << pMatrix.M[2][2] << pMatrix.M[2][3]
                << pMatrix.M[3][0] << pMatrix.M[3][1] << pMatrix.M[3][2] << pMatrix.M[3][3];
        return pStream;
    }

    static const Matrix4     ZERO;           //!<    A zero matrix (filled up with zeros).
    static const Matrix4     IDENTITY;       //!<    An identity matrix.

private:
    union
    {
        T M[4][4];
        T elem16[16];
        struct
        {
#ifdef OPENGL_MATRIX
            T _11, _21, _31, _41, _12, _22, _32, _42, _13, _23, _33, _43, _14, _24, _34, _44;
#else
            T _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44;
#endif
        };
    };
};


typedef Matrix4<Float>   Matrix4f;
typedef Matrix4<Double>  Matrix4d;


// Constants
template<class T> const Matrix4<T> Matrix4<T>::ZERO( T(0), T(0), T(0), T(0),
													 T(0), T(0), T(0), T(0),
													 T(0), T(0), T(0), T(0),
													 T(0), T(0), T(0), T(0) );

template<class T> const Matrix4<T> Matrix4<T>::IDENTITY( T(0), T(0), T(0), T(0),
								 						 T(0), T(0), T(0), T(0),
								 						 T(0), T(0), T(0), T(0),
								 						 T(0), T(0), T(0), T(0) );


} // namespace Gamedesk


#endif  //  _MATRIX4_H_
