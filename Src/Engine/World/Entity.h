/**
 *  @file       Entity.h
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
#ifndef     _ENTITY_H_
#define     _ENTITY_H_


#include "Maths/Quaternion.h"
#include "Maths/BoundingBox.h"
#include "World.h"


namespace Gamedesk {


/**
 *  Entity class used as a base class to represent an object in a 3D world.
 *  @brief  Entity class to represent object in a 3d world.
 *  @author Marco Arsenault.
 *  @date   24/11/03.
 */
class ENGINE_API Entity : public Object
{
    DECLARE_CLASS(Entity, Object);

public:
	/**
     *  Default constructor.
     */
	Entity();

	/**
     *  Destructor.
     */
	virtual ~Entity();

	/**
     *  Update the object.
     *  @param  pElapsedTime the time elapsed since the last update in seconds.
     */
	virtual void Update( Double pElapsedTime );

    //! Render the object.
	virtual void Render() const;

	//! Render the object as a selection.
	virtual void RenderSelected() const;

    /**
     *  Set the pointer to the world in which the entity is.
     *  @param  pWorld The world in which the entity is.
     */
    void SetWorld(World* pWorld)
    {
        mWorld = pWorld;
    }
    
    /**
     *  Set the object's position.
     *  @param  pPosition The new position as a vector3f.
     */
    virtual void SetPosition(const Vector3f& pPosition);

    /**
     *  Returns the position of the object as a vector3f.
     */
    const Vector3f& GetPosition() const;

	/**
     *  Set the object's orientation.
     *  @param  pOrientation the new orientation as a quaternion.
     */
    virtual void SetOrientation(const Quaternionf& pOrientation);

	/**
     *  Returns the orientation of the object as a quaternion.
     */
    const Quaternionf& GetOrientation() const;

    //! Returns the bounding box.
    virtual const BoundingBox& GetBoundingBox() const;

    Bool IsSelected()
    {
        return mSelected;
    }

    void Select( Bool pSelect )
    {
        mSelected = pSelect;
    }

    virtual void Serialize( Stream& pStream );
	    
properties:
    //! Position of the entity in the world.
    Vector3f mPosition;
    
    //! Orientation of the entity in the world.
	Quaternionf	mOrientation;

protected:
	World*          mWorld;
    BoundingBox     mBoundingBox;
    Bool            mSelected;
    Vector3f        mVelocity;
};      


} // namespace Gamedesk


#endif  //  _ENTITY_H_
