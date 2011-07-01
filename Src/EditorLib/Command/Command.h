/**
 *  @file       Command.h
 *  @brief      Base class for a command.
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
#ifndef     _COMMAND_H_
#define     _COMMAND_H_


#include "Containers/Containers.h"


/**
 *  Command base class. This class must be derived to implement the command
 *  pattern within the editor.
 *  @brief  Command base class.
 *  @author Marco Arsenault.
 *  @date   09/02/04.
 */
class EDITORLIB_API Command
{
    CLASS_DISABLE_COPY(Command);

public:
    //! Constructor taken the name of the command.
    Command(const String& pName);
    //! Destructor.
    virtual ~Command();
   
    //! Do the command, the derived classes must overload this method.
    virtual void Do() = 0;
    //! Undo the command.
    virtual void Undo();

    //! Returns the name of the command.
    const String& GetName() const;

protected:
    String  mName;
    Bool    mUndoCalled;
};



#endif  //  _COMMAND_H_
