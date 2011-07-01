/**
*  @file       ShaderObject.cpp
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
#include "Engine.h"
#include "Graphic/Shader/ShaderObject.h"

#include "FileManager/FileManager.h"


namespace Gamedesk {
	
	
IMPLEMENT_ABSTRACT_CLASS(ShaderObject);


ShaderObject::ShaderObject() 
    : mShaderType(Shader_Vertex)
{
}

void ShaderObject::SetShaderTextFile( const String& pShaderTextFile )
{
    GD_ASSERT_M( FileManager::FileExist(pShaderTextFile), "Shader file could not be found!" );

    FILE* pFile = NULL;
    fopen_s(&pFile, pShaderTextFile.c_str(), "rt");
    GD_ASSERT(pFile);

    Int32 size;

    fseek(pFile, 0, SEEK_END);
    size = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
    
    mShaderText.resize(size);
    fread(&mShaderText[0], size, 1, pFile);

    fclose(pFile);
}


} // namespace Gamedesk
