/**
 *  @file       Entity.cpp
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       20/12/03.
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
#include "Engine.h"
#include "Entity.h"
#include "Maths/Maths.h"
#include "Maths/Vector3.h"
#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Entity);


Entity::Entity() : 
	mPosition(0, 0, 0),
    mOrientation(1, 0, 0, 0),
    mWorld(NULL),
    mSelected(false)
{
}

Entity::~Entity()
{
}

void Entity::Update( Double /*pElapsedTime*/ )
{
    
}

void Entity::Render() const
{
}

void Entity::RenderSelected() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

	// Render the bounding box.
	renderer->SetRenderState( Renderer::Lighting, false );
    renderer->SetPolygonMode( Renderer::BothFace, Renderer::FillWireframe );
    renderer->SetCulling( Renderer::NoCulling );
    
    renderer->SetColor(Color4f(1.0f, 0.0f, 0.0f, 1.0f));
    renderer->DrawBox( mBoundingBox.Min(), mBoundingBox.Max() );
    renderer->SetColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f));

    renderer->SetRenderState( Renderer::Lighting, true );
    renderer->SetPolygonMode( Renderer::BothFace, Renderer::FillSolid );
    renderer->SetCulling( Renderer::CullBackFace );
}

void Entity::SetPosition(const Vector3f& pPosition)
{
    mPosition = pPosition;
}

const Vector3f& Entity::GetPosition() const
{
    return mPosition;
}

void Entity::SetOrientation(const Quaternionf& pOrientation)
{
    mOrientation = pOrientation;
}

const Quaternionf& Entity::GetOrientation() const
{
    return mOrientation;
}

const BoundingBox& Entity::GetBoundingBox() const
{
    return mBoundingBox;
}

void Entity::Serialize( Stream& pStream )
{
    Super::Serialize( pStream );

    pStream << mPosition;
    pStream << mOrientation;
    pStream << mBoundingBox;
    pStream << mSelected;
}


} // namespace Gamedesk
