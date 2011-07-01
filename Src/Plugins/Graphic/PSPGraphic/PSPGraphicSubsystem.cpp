/**
 *  @file       PSPGraphicSubsystem.cpp
 *  @brief	    PSP graphic subsystem.
 *  @author     Sébastien Lussier.
 *  @date       09/09/2008.
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
#include "PSPGraphic.h"
#include "PSPGraphicSubsystem.h"
#include "PSPRenderer.h"
#include "PSPTexture.h"

#include "Graphic/Buffer/SoftwareIndexBuffer.h"
#include "Graphic/Buffer/SoftwareVertexBuffer.h"
#include "Graphic/Mesh/Mesh.h"
#include "Graphic/Shader/ShaderObject.h"
#include "Graphic/Shader/ShaderProgram.h"
#include "Graphic/RenderTarget/RenderTexture.h"


namespace Gamedesk {


IMPLEMENT_CLASS(PSPGraphicSubsystem);


PSPGraphicSubsystem::PSPGraphicSubsystem()
{
}

PSPGraphicSubsystem::~PSPGraphicSubsystem()
{
}

void PSPGraphicSubsystem::Init()
{
    mRenderer = GD_NEW(PSPRenderer, this, "PSPGraphic");

    Super::Init();
}

void PSPGraphicSubsystem::Kill()
{
    if( mRenderer )
        GD_DELETE(mRenderer);

    Super::Kill();
}

Object* PSPGraphicSubsystem::Create( Class* pResourceClass )
{
    if( pResourceClass == Texture1D::StaticClass() )
    {
        return NULL;
    }

    if( pResourceClass == Texture2D::StaticClass() )
    {
        return GD_NEW(PSPTexture2D, this, "PSPGraphic::PSPTexture2D");
    }

    if( pResourceClass == Texture3D::StaticClass() )
    {
        return NULL;
    }

    if( pResourceClass == Cubemap::StaticClass() )
    {
        return NULL;
    }
    
    if( pResourceClass == Mesh::StaticClass() )
    {
        return GD_NEW(Mesh, this, "PSPGraphic::Mesh");
    }

    if( pResourceClass == VertexBuffer::StaticClass() )
    {
        return GD_NEW(SoftwareVertexBuffer, this, "PSPGraphic::SoftwareVertexBuffer");
    }
    
    if( pResourceClass == IndexBuffer::StaticClass() )
    {
        return GD_NEW(SoftwareIndexBuffer, this, "PSPGraphic::SoftwareIndexBuffer");
    }

    if( pResourceClass == RenderTexture1D::StaticClass() )
    {
        return NULL;
    }

    if( pResourceClass == RenderTexture2D::StaticClass() )
    {
        return NULL;
    }

    if( pResourceClass == RenderCubemap::StaticClass() )
    {
        return NULL;
    }

    if( pResourceClass == ShaderObject::StaticClass() )
    {
        return NULL;
    }

    if( pResourceClass == ShaderProgram::StaticClass() )
    {
        return NULL;
    }

    return Super::Create( pResourceClass );
}


} // namespace Gamedesk
