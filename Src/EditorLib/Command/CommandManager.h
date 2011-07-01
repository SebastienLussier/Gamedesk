/**
 *  @file       CommandManager.h
 *  @brief      A CommandManager class to manage the editor's commands.
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
#ifndef     _COMMAND_MANAGER_H_
#define     _COMMAND_MANAGER_H_


#include "Containers/Containers.h"


class Command;


/**
 *  CommandManager class. This implement the manager to handle the undo/redo in the 
 *  editor.
 *  @brief  CommandManager class.
 *  @author Marco Arsenault.
 *  @date   09/02/04.
 */
class EDITORLIB_API CommandManager
{
    CLASS_DISABLE_COPY(CommandManager);

public:
    //! Constructor.
	CommandManager();
    //! Destructor.
	virtual ~CommandManager();

    //! Perform a command.
    void PerformCommand(Command* pCommand);
    //! Perform the undo.
    void Undo();
    //! Perform the redo.
    void Redo();

    //! Save the command manager state.
    void Save();
    //! Returns a bool whether the command manager has been saved.
	bool WasSaved() const;

protected:
	//! Store a command.
    void StoreCommand(Command* pCommand);
    //! Clear the given command stack.
    void ClearStack(std::stack<Command*>& pCommandStack);
    
private:
    std::stack<Command*>    mUndos;
    std::stack<Command*>    mRedos;
    Command*                mLastSavePoint;
};

#endif //   _COMMAND_MANAGER_H_