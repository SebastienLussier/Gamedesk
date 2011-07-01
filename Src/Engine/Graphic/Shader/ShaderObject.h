/**
*  @file       ShaderObject.h
*  @brief	   Part of a shader program.
*  @author     Sébastien Lussier.
*  @date       04/06/05.
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
#ifndef     _SHADER_OBJECT_H_
#define     _SHADER_OBJECT_H_


#include "Resource/Resource.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {


class ENGINE_API ShaderObject : public Resource
{
    DECLARE_ABSTRACT_CLASS(ShaderObject, Resource);

public:
    enum ShaderType
    {
        Shader_Vertex,
        Shader_Pixel
    };

    virtual ~ShaderObject()
    {
        Kill();
    }

    virtual void Create( ShaderType type )
    {
        mShaderType = type;
        Init();
    }

    void SetShaderText( const String& pShaderText )
    {
        mShaderText = pShaderText;
    }

    void SetShaderTextFile( const String& pShaderTextFile );

    const String& GetShaderText() const
    {
        return mShaderText;
    }

    virtual Bool Compile() = 0;

protected:
    ShaderObject();
    
protected:
    String          mShaderText;
    ShaderType      mShaderType;
};


} // namespace Gamedesk


#endif  //  _SHADER_OBJECT_H_
