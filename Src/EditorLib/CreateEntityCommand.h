/**
 *  @file       CreateEntityCommand.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       12/02/04.
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
#ifndef     _CREATE_ENTITY_COMMAND_H_
#define     _CREATE_ENTITY_COMMAND_H_


#include "Command/Command.h"


class EditorBase;
class Class;
class Entity;


/**
 *  CreateEntityCommand class to implement the entity creation command.
 *  @brief  CreateEntityCommand class.
 *  @author Marco Arsenault.
 *  @date   12/02/04.
 */
class EDITORLIB_API CreateEntityCommand : public Command
{
    CLASS_DISABLE_COPY(CreateEntityCommand);

public:
    /**
     *  Default constructor.
     *  @param  pEditor The editor the command will interact with.
     *  @param  pEntityClass The class of the entity to be created.
     */
	CreateEntityCommand( EditorBase* pEditor, Class* pEntityClass );

    //! Destructor.
    virtual ~CreateEntityCommand();

    //! Do the command.
    virtual void Do();
    
    //! Undo the command.
    virtual void Undo();

private:
    EditorBase*         mEditor;
    Class*              mEntityClass;
    Entity*             mCreatedEntity;
    Bool                mOwnEntity;
};


#endif  //  _CREATE_ENTITY_COMMAND_H_
