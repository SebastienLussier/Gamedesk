/**
*  @file       OGLShaderProgram_GLSL.h
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
#ifndef     _OGL_SHADER_PROGRAM_GLSL_H_
#define     _OGL_SHADER_PROGRAM_GLSL_H_


#include "Graphic/Shader/ShaderProgram.h"


namespace Gamedesk {


class OGLGRAPHIC_API OGLShaderProgram_GLSL : public ShaderProgram
{
    friend class OGLGraphicSubsystem;
    DECLARE_CLASS(OGLShaderProgram_GLSL, ShaderProgram);

public:
    void AddShader( ShaderObject* pVertexShader );

    Bool Link();

    void Apply();
    void Done();

    void SetUniform( const String& pVariableName, Bool pValue );
    void SetUniform( const String& pVariableName, Int32 pValue );
    void SetUniform( const String& pVariableName, Float pValue );
    void SetUniform( const String& pVariableName, const Vector2f& pValue );
    void SetUniform( const String& pVariableName, const Vector3f& pValue );
    void SetUniform( const String& pVariableName, const Color4f& pValue );
    void SetUniform( const String& pVariableName, const Matrix4f& pValue );

    void SetSampler( const String& pVariableName, UInt32 pValue );
    
private:
    OGLShaderProgram_GLSL();

    virtual void Init();
    virtual void Kill();

private:
    GLuint  mShaderProgram;
};


} // namespace Gamedesk


#endif  //  _OGL_SHADER_PROGRAM_GLSL_H_
