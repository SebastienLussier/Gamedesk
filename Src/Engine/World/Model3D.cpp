/**
 *  @file       Model3D.cpp
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       06/02/04.
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
#include "Model3D.h"
#include "Graphic/Mesh/Mesh.h"
#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
Model3D::Model3D()
{
}

Model3D::~Model3D()
{
}

const Model3D& Model3D::operator=(const Model3D& /*pOther*/)
{
    return *this;
}

void Model3D::Update( Double pElapsedTime )
{
    if(mMesh)
    {
        (*mMesh)->Update(pElapsedTime);
        mBoundingBox = (*mMesh)->GetBoundingBox();
    }
}

void Model3D::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

	if(mMesh)
	{
		(*mMesh)->Render();
	}
}

void Model3D::SetMesh(const String& pMeshFileName)
{
    mMesh = MeshHdl(pMeshFileName);
	mBoundingBox = (*mMesh)->GetBoundingBox();
}

Bool Model3D::LineCheck( const Ray3f& pRay ) const
{
    if( mMesh )
	{
        return (*mMesh)->LineCheck( pRay );
    }

    return false;
}


IMPLEMENT_CLASS(Model3D)


} // namespace Gamedesk
