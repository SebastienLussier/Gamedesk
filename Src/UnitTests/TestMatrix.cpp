/**
 *  @file       TestMatrix.cpp
 *  @brief      Tests for the Matrix class.
 *  @author     Sébastien Lussier.
 *  @date       09/04/04.
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
#include "UnitTests.h"
#include "Test/TestCase.h"


#include <gl/gl.h>
#include <gl/glut.h>
#include "Maths/Matrix4.h"


class UNITTESTS_API MatrixTest : public TestCase
{
    DECLARE_CLASS( MatrixTest, TestCase );

public:
    MatrixTest()
    {
    }

    Bool EqualEps( const Matrix4f& pFirst, const Matrix4f& pSecond ) const
    {
        if( Maths::Abs(pFirst(0,0) - pSecond(0,0)) < Maths::EPSILON &&
            Maths::Abs(pFirst(0,1) - pSecond(0,1)) < Maths::EPSILON &&
            Maths::Abs(pFirst(0,2) - pSecond(0,2)) < Maths::EPSILON &&
            Maths::Abs(pFirst(0,3) - pSecond(0,3)) < Maths::EPSILON &&
            Maths::Abs(pFirst(1,0) - pSecond(1,0)) < Maths::EPSILON &&
            Maths::Abs(pFirst(1,1) - pSecond(1,1)) < Maths::EPSILON &&
            Maths::Abs(pFirst(1,2) - pSecond(1,2)) < Maths::EPSILON &&
            Maths::Abs(pFirst(1,3) - pSecond(1,3)) < Maths::EPSILON &&
            Maths::Abs(pFirst(2,0) - pSecond(2,0)) < Maths::EPSILON &&
            Maths::Abs(pFirst(2,1) - pSecond(2,1)) < Maths::EPSILON &&
            Maths::Abs(pFirst(2,2) - pSecond(2,2)) < Maths::EPSILON &&
            Maths::Abs(pFirst(2,3) - pSecond(2,3)) < Maths::EPSILON &&
            Maths::Abs(pFirst(3,0) - pSecond(3,0)) < Maths::EPSILON &&
            Maths::Abs(pFirst(3,1) - pSecond(3,1)) < Maths::EPSILON &&
            Maths::Abs(pFirst(3,2) - pSecond(3,2)) < Maths::EPSILON &&
            Maths::Abs(pFirst(3,3) - pSecond(3,3)) < Maths::EPSILON )
            return true;
        else
            return false;
    }

    virtual void SetUp()
    {
        Int32 argc = 1;
        char* argv[] = { "OpenGL context for matrix test" };

        glutInit( &argc, argv );
	    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );  
	    glutInitWindowSize(500,500);
	    glutInitWindowPosition(0,0);
    	glutCreateWindow("OpenGL context for matrix test"); 
    }

    virtual void Run()
    {
        Matrix4f glMatrix;
        Matrix4f gdMatrix;
        Matrix4f gdMatrix2;

        glMatrixMode( GL_MODELVIEW_MATRIX );

        // Identity
        glLoadIdentity();
        glGetFloatv( GL_MODELVIEW_MATRIX, glMatrix );
        gdMatrix.SetIdentity();        
        TestAssert( EqualEps(glMatrix,gdMatrix) );

        // Translation
        glLoadIdentity();
        glTranslatef( 1, 2, 3 );
        glGetFloatv( GL_MODELVIEW_MATRIX, glMatrix );
        TestAssert( EqualEps(glMatrix,Matrix4f::Translation(1,2,3)) );

        // Rotation X
        glLoadIdentity();
        glRotatef( 10, 1, 0, 0 );
        glGetFloatv( GL_MODELVIEW_MATRIX, glMatrix );
        TestAssert( EqualEps(glMatrix,Matrix4f::RotationX(Maths::ToRadians(10.0f))) );

        // Rotation Y
        glLoadIdentity();
        glRotatef( 10, 0, 1, 0 );
        glGetFloatv( GL_MODELVIEW_MATRIX, glMatrix );
        TestAssert( EqualEps(glMatrix,Matrix4f::RotationY(Maths::ToRadians(10.0f))) );

        // Rotation Z
        glLoadIdentity();
        glRotatef( 10, 0, 0, 1 );
        glGetFloatv( GL_MODELVIEW_MATRIX, glMatrix );
        TestAssert( EqualEps(glMatrix,Matrix4f::RotationZ(Maths::ToRadians(10.0f))) );

        // Rotation Axis
        glLoadIdentity();
        glRotatef( 10, 1, 2, 3 );
        glGetFloatv( GL_MODELVIEW_MATRIX, glMatrix );
        TestAssert( EqualEps(glMatrix,Matrix4f::AxisRotation(Maths::ToRadians(10.0f), 1, 2, 3)) );

        // Scale
        glLoadIdentity();
        glScalef( 10, 11, 12 );
        glGetFloatv( GL_MODELVIEW_MATRIX, glMatrix );
        TestAssert( EqualEps(glMatrix,Matrix4f::Scaling(10, 11, 12)) );

        // Translation * Rotation
        glLoadIdentity();
        glRotatef( 10, 1, 0, 0 );
        glTranslatef( 1, 2, 3 );
        glGetFloatv( GL_MODELVIEW_MATRIX, glMatrix );
        gdMatrix = Matrix4f::Translation( 1,2,3 );  
        gdMatrix.RotateX( Maths::ToRadians(10.0f) );        
        TestAssert( EqualEps(glMatrix,gdMatrix) );

        // Rotation * Translation
        glLoadIdentity();
        glTranslatef( 1, 2, 3 );
        glRotatef( 10, 1, 0, 0 );        
        glGetFloatv( GL_MODELVIEW_MATRIX, glMatrix );
        gdMatrix = Matrix4f::RotationX( Maths::ToRadians(10.0f) );  
        gdMatrix.Translate( 1,2,3 );
        TestAssert( EqualEps(glMatrix,gdMatrix) );
    }

    virtual void TearDown()
    {
    }
};

IMPLEMENT_CLASS( MatrixTest );
