/**
 *  @file       MatrixStack.h
 *  @brief      MatrixStack class.
 *  @author     Sebastien Lussier.
 *  @date       10/09/2008.
 */
/*
 *  Copyright (C) 2008 Gamedesk
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
#ifndef     _MATRIX_STACK_H_
#define     _MATRIX_STACK_H_


#include "Maths\Vector.h"
#include "Maths\Matrix.h"
#include "Maths\Quaternion.h"


namespace Gamedesk {


template <class T, int MAX_DEPTH>
class MatrixStack
{
public:
	MatrixStack();
	MatrixStack(UInt32 pMaxDepth);
	MatrixStack(const MatrixStack& pMatrixStack);

	~MatrixStack();

	void Init(UInt32 pMaxDepth);

    void LoadIdentity();
    void Load(const Matrix4<T>& pMatrix);
    
	Matrix4<T>& GetTop();
	const Matrix4<T>& GetTop() const;

    void PushMatrix();
    void PopMatrix();
    
    void Mult(const Matrix4<T>& pMatrix);
    void Rotate(const Quaternion<T>& pRotation);
    void Scale(const Vector3<T>& pScaling);
    void Translate(const Vector3<T>& pTranslation);
    
    void Ortho(T left, T right, T bottom, T top, T nearVal, T farVal);
    void LookAt(const Vector3<T>& pFrom, const Vector3<T>& pAt, const Vector3<T>& pUp);
    void Perspective(T pFOV, T pAspect, Matrix4 pNearPlane, Matrix4 pFarPlane);

private:
    Matrix4<T>*     mStack;					//!< Dynamically allocated array of (mMaxDepth) matrices
    Matrix4<T>*     mTop;                   //!< Points into mStack
    UInt32          mDepth;                 //!< 0 <= mDepth < mMaxDepth
	UInt32			mMaxDepth;				//!< Maximum depth of this stack
};

///////////////////////////////////////////////////////////////////////////////

MatrixStack::MatrixStack() : mStack(0), mTop(0), mDepth(0), mMaxDepth(0)
{
}

MatrixStack::MatrixStack(UInt32 pMaxDepth) : mStack(0), mTop(0), mDepth(0), mMaxDepth(0)
{
	Init(pMaxDepth);
	mTop = &mStack[mDepth];
}

MatrixStack::MatrixStack(const MatrixStack& pMatrixStack)
{
	Init(pMatrixStack.mMaxDepth);

	memcpy(mStack, pMatrixStack.mStack, sizeof(Matrix4<T>) * mMaxDepth);
	mDepth = pMatrixStack.mDepth;
	mTop = &mStack[mDepth];
}

MatrixStack::~MatrixStack()
{
	GD_DELETE_ARRAY(mStack);
}

void MatrixStack::Init(UInt32 pMaxDepth)
{
	GD_DELETE_ARRAY(mStack);

	mMaxDepth = pMaxDepth;
	mStack = GD_NEW_ARRAY(Matrix4<T>, mMaxDepth, this, "MatrixStack::Stack");
}

void MatrixStack::LoadIdentity()
{
	GD_ASSERT(mTop);
    mTop->SetIdentity();
}

void MatrixStack::Load(const Matrix4<T>& pMatrix)
{
	GD_ASSERT(mTop);
    (*mTop) = pMatrix;
}

Matrix4<T>& MatrixStack::GetTop()
{
	return (*mTop);
}

const Matrix4<T>& MatrixStack::GetTop() const
{
	return (*mTop);
}

void MatrixStack::PushMatrix()
{
	GD_ASSERT(mTop);
    GD_ASSERT(mDepth + 1 < MAX_DEPTH);
        
    mStack[mDepth + 1] = mStack[mDepth];
    mDepth++;
    mTop = &mStack[mDepth];
}

void MatrixStack::PopMatrix()
{
	GD_ASSERT(mTop);
    GD_ASSERT(mDepth != 0);
    
    mDepth--;
    mTop = &mStack[mDepth];
}

void MatrixStack::Mult(const Matrix4<T>& pMatrix)
{
	GD_ASSERT(mTop);
    (*mTop) *= pMatrix;
}

void MatrixStack::Rotate(const Quaternion<T>& pRotation)
{
	GD_ASSERT(mTop);

    Matrix4<T> mat;
    pRotation.ToMatrix( mat );
    (*mTop) *= mat;
}

void MatrixStack::Scale(const Vector3<T>& pScaling)
{
	GD_ASSERT(mTop);
    (*mTop).Scale(pScaling);
}

void MatrixStack::Translate(const Vector3<T>& pTranslation)
{
	GD_ASSERT(mTop);
    (*mTop).Translate(pTranslation);
}

void MatrixStack::Ortho(T pLeft, T pRight, T pBottom, T pTop, T pNearVal, T pFarVal)
{
	GD_ASSERT(mTop);
    Matrix4<T> mat;
    mat.Ortho(pLeft, pRight, pBottom, pTop, pNearVal, pFarVal);
    (*mTop) *= mat;
}

void MatrixStack::LookAt(const Vector3<T>& pFrom, const Vector3<T>& pAt, const Vector3<T>& pUp)
{
	GD_ASSERT(mTop);
    Matrix4<T> mat;
    mat.LookAt(pFrom, pAt, pUp);
    (*mTop) *= mat;
}

void MatrixStack::Perspective(T pFOV, T pAspect, Matrix4 pNearPlane, Matrix4 pFarPlane)
{
	GD_ASSERT(mTop);
    Matrix4<T> mat;
    mat.ProjectionMatrix(pFOV, pAspect, pNearPlane, pFarPlane);
    (*mTop) *= mat;
}


} // namespace Gamedesk


#endif  //  _MATRIX_STACK_H_
