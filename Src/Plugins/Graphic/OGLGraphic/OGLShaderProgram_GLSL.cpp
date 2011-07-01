/**
*  @file       OGLShaderProgram_GLSL.cpp
*  @brief	   Shader program (vertex and pixel shader) using GLSL.
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
#include "OGLShaderProgram_GLSL.h"
#include "OGLShaderObject_GLSL.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(OGLShaderProgram_GLSL);

OGLShaderProgram_GLSL::OGLShaderProgram_GLSL()
    : mShaderProgram(0)
{
}

void OGLShaderProgram_GLSL::AddShader( ShaderObject* pShader )
{
    GD_ASSERT_M( pShader->IsA(OGLShaderObject_GLSL::StaticClass()), "[OGLShaderProgram_GLSL::AddShader] Invalid shader class provided!" );

    Super::AddShader( pShader );

    if( !mShaderProgram )
        Init();

    glAttachObjectARB( mShaderProgram, ((OGLShaderObject_GLSL*)pShader)->GetShaderID() );
}

#define OGL_DisplayShaderLog( title )                                                               \
    {                                                                                               \
        GLint logLength;                                                                            \
        glGetObjectParameterivARB( mShaderProgram, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength );     \
        if( logLength > 0 )                                                                         \
        {                                                                                           \
            Char* logStr = GD_NEW_ARRAY(Char, logLength, this, "OGLGraphic::OGLShaderProgram_GLSL");\
            glGetInfoLogARB( mShaderProgram, logLength, NULL, logStr );                             \
            MessageBox( NULL, logStr, (title), MB_OK|MB_ICONEXCLAMATION );                          \
            GD_DELETE_ARRAY(logStr);                                                                \
        }                                                                                           \
    }

Bool OGLShaderProgram_GLSL::Link()
{
    GLint bLinked;

    if( !mShaderProgram )
        Init();

    glLinkProgramARB( mShaderProgram );
    glGetObjectParameterivARB( mShaderProgram, GL_OBJECT_LINK_STATUS_ARB, &bLinked );
    if( bLinked == false )
    {
        OGL_DisplayShaderLog( "GLSL Link Error!" )
    }

    return bLinked != 0;
}

void OGLShaderProgram_GLSL::Apply()
{
    if( mShaderProgram )
        glUseProgramObjectARB(mShaderProgram);
}

void OGLShaderProgram_GLSL::Done()
{
    glUseProgramObjectARB(0);
}

void OGLShaderProgram_GLSL::SetUniform( const String& pVariableName, Bool pValue )
{
    glUniform1iARB( glGetUniformLocationARB(mShaderProgram,pVariableName.c_str()), pValue );
}

void OGLShaderProgram_GLSL::SetUniform( const String& pVariableName, Int32 pValue )
{
    glUniform1iARB( glGetUniformLocationARB(mShaderProgram,pVariableName.c_str()), pValue );
}

void OGLShaderProgram_GLSL::SetUniform( const String& pVariableName, Float pValue )
{
    glUniform1fARB( glGetUniformLocationARB(mShaderProgram,pVariableName.c_str()), pValue );
}

void OGLShaderProgram_GLSL::SetUniform( const String& pVariableName, const Vector2f& pValue )
{
    glUniform2fvARB( glGetUniformLocationARB(mShaderProgram,pVariableName.c_str()), 1, pValue );
}

void OGLShaderProgram_GLSL::SetUniform( const String& pVariableName, const Vector3f& pValue )
{
    glUniform3fvARB( glGetUniformLocationARB(mShaderProgram,pVariableName.c_str()), 1, pValue );
}

void OGLShaderProgram_GLSL::SetUniform( const String& pVariableName, const Color4f& pValue )
{
    glUniform4fvARB( glGetUniformLocationARB(mShaderProgram,pVariableName.c_str()), 1, pValue );
}

void OGLShaderProgram_GLSL::SetUniform( const String& pVariableName, const Matrix4f& pValue )
{
    glUniformMatrix4fvARB( glGetUniformLocationARB(mShaderProgram,pVariableName.c_str()), 1, GL_FALSE, pValue );
}

void OGLShaderProgram_GLSL::SetSampler( const String& pVariableName, UInt32 pValue )
{
    glUniform1iARB( glGetUniformLocationARB(mShaderProgram,pVariableName.c_str()), pValue );
}

void OGLShaderProgram_GLSL::Init()
{
    if( mShaderProgram )
        Kill();

    mShaderProgram = glCreateProgramObjectARB();
}

void OGLShaderProgram_GLSL::Kill()
{
    if( mShaderProgram )
        glDeleteObjectARB( mShaderProgram );

    mShaderProgram = 0;
}


} // namespace Gamedesk
