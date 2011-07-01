/**
*  @file       ShaderProgram.h
*  @brief	   GPU program consisting of pixel/vertex shader.
*  @author     Sébastien Lussier.
*  @date       06/04/05.
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
#ifndef     _SHADER_PROGRAM_H_
#define     _SHADER_PROGRAM_H_


#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Maths/Matrix4.h"
#include "Graphic/Color4.h"


namespace Gamedesk {


class ShaderObject;


class ENGINE_API ShaderProgram : public Object
{
    friend class GraphicSubsystem;
    DECLARE_ABSTRACT_CLASS(ShaderProgram, Object);

public:
    virtual ~ShaderProgram()
    {
        Kill();
    }

    virtual void AddShader( ShaderObject* pShaderObject )
    {
        mShaderObjects.push_back(pShaderObject);
    }

    virtual void Apply();
    virtual void Done();

    virtual Bool Link();

    virtual void SetUniform( const String& pVariableName, Int32 pValue ) = 0;
    virtual void SetUniform( const String& pVariableName, Float pValue ) = 0;
    virtual void SetUniform( const String& pVariableName, const Vector2f& pValue ) = 0;
    virtual void SetUniform( const String& pVariableName, const Vector3f& pValue ) = 0;
    virtual void SetUniform( const String& pVariableName, const Color4f& pValue ) = 0;
    virtual void SetUniform( const String& pVariableName, const Matrix4f& pValue ) = 0;
    virtual void SetSampler( const String& pVariableName, UInt32 pValue ) = 0;

    virtual void Init();
    virtual void Kill();

protected:
    ShaderProgram();
    
protected:
    Vector<ShaderObject*>       mShaderObjects;
};


} // namespace Gamedesk


#endif  //  _SHADER_PROGRAM_H_
