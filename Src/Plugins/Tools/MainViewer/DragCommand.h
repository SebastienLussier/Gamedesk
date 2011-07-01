/**
 *  @file       DragCommand.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       09/02/04.
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
#ifndef     _DRAG_COMMAND_H_
#define     _DRAG_COMMAND_H_


#include "Command/Command.h"
#include "Maths/Vector3.h"


class Entity;


/**
 *  DragCommand class to implement the dragging command.
 *  @brief  DragCommand class.
 *  @author Marco Arsenault.
 *  @date   09/02/04.
 */
class MAINVIEWER_API DragCommand : public Command
{
    CLASS_DISABLE_COPY(DragCommand);

public:
    /**
     *  Default constructor.
     *  @param  pObjectProperties The object on which the command is applied.
     *  @param  pOldPosition The old position of the object before the drag.
     */
	DragCommand(Entity* pEntity, const Vector3f& pOldPosition);
    
    //! Destructor.
    virtual ~DragCommand();

    //! Do the command.
    virtual void Do();
    
    //! Undo the command.
    virtual void Undo();

private:
    Entity*     mEntity;
    Vector3f    mOldPosition;
    Vector3f    mNewPosition;
};


#endif  //  _DRAG_MANIPULATOR_H_
