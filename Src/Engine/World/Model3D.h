/**
 *  @file       Model3D.h
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
#ifndef     _MODEL3D_H_
#define     _MODEL3D_H_


#include "Maths/Line3.h"
#include "Maths/BoundingBox.h"
#include "World/Entity.h"
#include "Graphic/Mesh/MeshHdl.h"


namespace Gamedesk {


/**
 *  Model3D class used to represent a 3d mesh in a 3d world.
 *  @brief  Model3D class for a mesh in a 3d world.
 *  @author Marco Arsenault.
 *  @date   06/02/04.
 */
class ENGINE_API Model3D : public Entity
{
    DECLARE_CLASS(Model3D, Entity)

public:
    //! Default constructor.
	Model3D();

    //! Destructor.
    virtual ~Model3D();

    //! Update the model (in case it is animated).
    virtual void Update( Double pElapsedTime );

    //! Render the model.
    virtual void Render() const;
	
    void SetMesh( const String& pMeshFileName );

    Bool LineCheck( const Ray3f& pRay ) const;

private:
    MeshHdl			mMesh;
};


} // namespace Gamedesk


#endif  //  _MODEL3D_H_