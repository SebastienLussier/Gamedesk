/**
 *  @file       EntityCreationSender.h
 *  @brief	    A sender class to notify the listeners about entity creation/deletion.
 *  @author     Marco Arsenault.
 *  @date       02/02/04.
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
#ifndef     _ENTITY_CREATION_SENDER_H_
#define     _ENTITY_CREATION_SENDER_H_


#include "Containers/Containers.h"


class EntityCreationListener;
class Entity;


/**
 *  EntityCreationSender base class.  This class can notify the listeners about
 *  entity creation and deletion by using the FireEntityCreated and
 *  FireEntityDeleted methods.
 *  @brief  EntityCreationSender base class.
 *  @author Marco Arsenault.
 *  @date   02/02/04.
 */
class EDITORLIB_API EntityCreationSender
{
    CLASS_DISABLE_COPY(EntityCreationSender);

public:
    //! Default constructor.
	EntityCreationSender();

    //! Destructor.
    virtual ~EntityCreationSender();

    //! Add an ObjectCreationListener.
    void AddEntityCreationListener(EntityCreationListener* pListener);

    //! Remove an EntityCreationListener.
    void RemoveEntityCreationListener(EntityCreationListener* pListener);

    //! Notify the listeners that an entity has been created.
    void FireEntityCreated(Entity* pEntity);

    //! Notify the listeners that an entity has been deleted.
    void FireEntityDeleted(Entity* pEntity);
    
private:
    std::list<EntityCreationListener*>  mEntityCreationListeners;
};


#endif  //  _ENTITY_CREATION_SENDER_H_
