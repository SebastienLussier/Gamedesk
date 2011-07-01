/**
 *  @file       OGLGraphicSubsystem.cpp
 *  @brief	    OpenGL graphic subsystem.
 *  @author     Sébastien Lussier.
 *  @date       04/12/03.
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
#include "OGLGraphic.h"
#include "OGLGraphicSubsystem.h"
#include "OGLRenderer.h"

#include "OGLTexture.h"
#include "OGLRenderTexture_ARB.h"

#include "OGLVertexBuffer.h"
#include "OGLIndexBuffer.h"

#include "OGLShaderObject_GLSL.h"
#include "OGLShaderProgram_GLSL.h"

#include "Graphic/Buffer/SoftwareIndexBuffer.h"
#include "Graphic/Buffer/SoftwareVertexBuffer.h"
#include "Graphic/Mesh/Mesh.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(OGLGraphicSubsystem);


OGLGraphicSubsystem::OGLGraphicSubsystem()
{
}

OGLGraphicSubsystem::~OGLGraphicSubsystem()
{
}

void OGLGraphicSubsystem::Init()
{
    mRenderer = GD_NEW(OGLRenderer, this, "OGLGraphic");

    Super::Init();
}

void OGLGraphicSubsystem::Kill()
{
    if( mRenderer )
        GD_DELETE(mRenderer);

    Super::Kill();
}

Object* OGLGraphicSubsystem::Create( Class* pResourceClass )
{
    if( pResourceClass == Texture1D::StaticClass() )
    {
        return GD_NEW(OGLTexture1D, this, "OGLGraphic::OGLTexture1D");
    }

    if( pResourceClass == Texture2D::StaticClass() )
    {
        return GD_NEW(OGLTexture2D, this, "OGLGraphic::OGLTexture2D");
    }

    if( pResourceClass == Texture3D::StaticClass() )
    {
        return GD_NEW(OGLTexture3D, this, "OGLGraphic::OGLTexture3D");
    }

    if( pResourceClass == Cubemap::StaticClass() )
    {
        return GD_NEW(OGLCubemap, this, "OGLGraphic::OGLCubemap");
    }
    
    if( pResourceClass == Mesh::StaticClass() )
    {
        return GD_NEW(Mesh, this, "OGLGraphic::Mesh");
    }

    if( pResourceClass == VertexBuffer::StaticClass() )
    {
        // ARB_vertex_buffer_object
        if( GLEW_ARB_vertex_buffer_object )
            return GD_NEW(OGLHWVertexBuffer, this, "OGLGraphic::OGLHWVertexBuffer");

        return GD_NEW(SoftwareVertexBuffer, this, "OGLGraphic::SoftwareVertexBuffer");
    }
    
    if( pResourceClass == IndexBuffer::StaticClass() )
    {
        // ARB_vertex_buffer_object
        if( GLEW_ARB_vertex_buffer_object )
            return GD_NEW(OGLHWIndexBuffer, this, "OGLGraphic::OGLHWIndexBuffer");
        
        return GD_NEW(SoftwareIndexBuffer, this, "OGLGraphic::SoftwareIndexBuffer");
    }

    if( pResourceClass == RenderTexture1D::StaticClass() )
    {
        // ARB_render_texture
        if( WGLEW_ARB_render_texture )
            return GD_NEW(OGLRenderTexture1D_ARB, this, "OGLGraphic::OGLRenderTexture1D_ARB");

        return NULL;
    }

    if( pResourceClass == RenderTexture2D::StaticClass() )
    {
        // ARB_render_texture
        if( WGLEW_ARB_render_texture )
            return GD_NEW(OGLRenderTexture2D_ARB, this, "OGLGraphic::OGLRenderTexture2D_ARB");

        return NULL;
    }

    if( pResourceClass == RenderCubemap::StaticClass() )
    {
        // ARB_render_texture
        if( WGLEW_ARB_render_texture )
            return GD_NEW(OGLRenderCubemap_ARB, this, "OGLGraphic::OGLRenderCubemap_ARB");

        return NULL;
    }

    if( pResourceClass == ShaderObject::StaticClass() )
    {
        // ARB_shader_objects
        //if( WGLEW_ARB_shader_objects )
            return GD_NEW(OGLShaderObject_GLSL, this, "OGLGraphic::OGLShaderObject_GLSL");

        //return NULL;
    }

    if( pResourceClass == ShaderProgram::StaticClass() )
    {
        // ARB_shader_objects
        //if( WGLEW_ARB_shader_objects )
            return GD_NEW(OGLShaderProgram_GLSL, this, "OGLGraphic::OGLShaderProgram_GLSL");

        //return NULL;
    }

    return Super::Create( pResourceClass );
}


} // namespace Gamedesk
