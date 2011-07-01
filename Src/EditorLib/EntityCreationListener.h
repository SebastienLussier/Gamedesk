/**
 *  @file       EntityCreationListener.h
 *  @brief	    A listener class to receive events about object creation/deletion.
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
#ifndef     _ENTITY_CREATION_LISTENER_H_
#define     _ENTITY_CREATION_LISTENER_H_


class Entity;


/**
 *  EntityCreationListener base class. This listener will receive event from the sender
 *  concerning entity creation or deletion though the method OnEntityCreated and 
 *  OnEntityDeleted respectively.
 *  @brief  EntityCreationListener base class.
 *  @author Marco Arsenault.
 *  @date   02/02/04.
 */
class EDITORLIB_API EntityCreationListener
{
public:
    //! Default constructor.
	EntityCreationListener();

    //! Destructor.
    virtual ~EntityCreationListener();

    //! Called when an object is created.
    virtual void OnEntityCreated(Entity* pEntity) = 0;

    //! Called when an object is deleted.
    virtual void OnEntityDeleted(Entity* pEntity) = 0;
};


#endif  //  _ENTITY_CREATION_LISTENER_H_
