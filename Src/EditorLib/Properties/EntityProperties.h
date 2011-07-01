/**
 *  @file       EntityProperties.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       14/04/04.
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
#ifndef     _ENTITY_PROPERTIES_H_
#define     _ENTITY_PROPERTIES_H_


#include "Properties/ObjectProperties.h"
#include "Containers/Containers.h"
#include "Maths/Vector3.h"
#include "Maths/Quaternion.h"


class Entity;


/**
 *  EntityProperties base class.  This object is used as a facade to an entity.
 *  As a PropertiesSender and PropertiesListener, it can be connected with
 *  other Senders/Listeners to keep the data up to date.
 *  @brief  EntityProperties base class.
 *  @author Marco Arsenault.
 *  @date   14/04/04.
 */
class EDITORLIB_API EntityProperties : public ObjectProperties
{
    DECLARE_CLASS(EntityProperties, ObjectProperties);

public:
    //! Constructor.
	EntityProperties();

    //! Destructor.
    virtual ~EntityProperties();

    //! Initialize all properties.
    virtual void InitProperties();

    /**
     *  Set the object's position.
     *  @param  pPosition The new position as a vector3f.
     */
    void SetPosition(const Vector3f& pPosition);
    
    //! Returns the position.
    const Vector3f& GetPosition() const;

	/**
     *  Set the object's orientation.
     *  @param  pOrientation the new orientation as a quaternion.
     */
    void SetOrientation(const Quaternionf& pOrientation);
    
    //! Returns the orientation.
    const Quaternionf& GetOrientation() const;
   
protected:
    PropertyVector3f        mPosition;
    PropertyQuaternionf     mOrientation;
};


#endif  //  _OBJECT3D_PROPERTIES_H_
