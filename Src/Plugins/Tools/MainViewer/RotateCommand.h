/**
 *  @file       RotateCommand.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       10/02/04.
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
#ifndef     _ROTATE_COMMAND_H_
#define     _ROTATE_COMMAND_H_


#include "Command/Command.h"
#include "Maths/Quaternion.h"


class Entity;


/**
 *  RotateCommand class to implement the rotation command.
 *  @brief  RotateCommand class.
 *  @author Marco Arsenault.
 *  @date   10/02/04.
 */
class MAINVIEWER_API RotateCommand : public Command
{
    CLASS_DISABLE_COPY(RotateCommand);

public:
    /**
     *  Default constructor.
     *  @param  pObjectProperties   The entity on which the command is applied.
     *  @param  pOldOrientation     The initial orientation of the entity before the rotation.
     */
	RotateCommand(Entity* pEntity, const Quaternionf& pOldOrientation);
    
    //! Destructor.
    virtual ~RotateCommand();

    //! Do the command.
    virtual void Do();
    
    //! Undo the command.
    virtual void Undo();

private:
    Entity*     mEntity;
    Quaternionf mOldOrientation;
    Quaternionf mNewOrientation;
};


#endif  //  _ROTATE_COMMAND_H_