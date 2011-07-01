/**
*  @file       OGLShaderObject_GLSL.cpp
*  @brief	   Part of a shader program, using GLSL.
*  @author     Sébastien Lussier.
*  @date       28/05/05.
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
#include "OGLGraphic.h"
#include "OGLShaderObject_GLSL.h"


namespace Gamedesk {
	

IMPLEMENT_CLASS(OGLShaderObject_GLSL);


const GLenum GDToGLShaderType[] =   {
                    GL_VERTEX_SHADER,           // Shader_vertex,
                    GL_FRAGMENT_SHADER,         // Shader_Pixel,
                                    };


OGLShaderObject_GLSL::OGLShaderObject_GLSL()
    : mShaderObject(0)
{
}

void OGLShaderObject_GLSL::Init()
{
    Super::Init();

    mShaderObject = glCreateShaderObjectARB( GDToGLShaderType[mShaderType] );
}

void OGLShaderObject_GLSL::Kill()
{
    if( mShaderObject )
        glDeleteObjectARB( mShaderObject );

    mShaderObject = 0;

    Super::Kill();
}


#define OGL_DisplayShaderLog( title )                                                               \
    {                                                                                               \
        GLint logLength;                                                                            \
        glGetObjectParameterivARB( mShaderObject, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength );      \
        if( logLength > 0 )                                                                         \
        {                                                                                           \
            Char* logStr = GD_NEW_ARRAY(Char, logLength, this, "OGLGraphic::OGLShaderObject_GLSL"); \
            glGetInfoLogARB( mShaderObject, logLength, NULL, logStr );                              \
            MessageBox( NULL, logStr, (title), MB_OK|MB_ICONEXCLAMATION );                          \
            GD_DELETE_ARRAY(logStr);                                                                \
        }                                                                                           \
    }

Bool OGLShaderObject_GLSL::Compile()
{
    GLint bCompiled;
    const Char* shaderStrings[1];

    shaderStrings[0] = GetShaderText().c_str();

    glShaderSourceARB( mShaderObject, 1, shaderStrings, NULL );
    glCompileShaderARB( mShaderObject );
    glGetObjectParameterivARB( mShaderObject, GL_OBJECT_COMPILE_STATUS_ARB, &bCompiled );
    if( !bCompiled )
    {
        OGL_DisplayShaderLog( "GLSL Compile Error!" );
    }

    return bCompiled != 0;
}


} // namespace Gamedesk
