/**
 *  @file       CommandManager.cpp
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
#include "EditorLib.h"
#include "CommandManager.h"
#include "Command.h"


CommandManager::CommandManager() : 
    mLastSavePoint(0)
{
}

CommandManager::~CommandManager()
{
    ClearStack(mUndos); 
    ClearStack(mRedos);
}

void CommandManager::PerformCommand(Command* pCommand)
{
    pCommand->Do();
    StoreCommand(pCommand);
}

void CommandManager::StoreCommand(Command* pCommand)
{
    mUndos.push(pCommand);
    ClearStack(mRedos);        
}

void CommandManager::Undo()
{
    if(!mUndos.empty())
    {
        mUndos.top()->Undo();
        mRedos.push(mUndos.top());
        mUndos.pop();
    }
}

void CommandManager::Redo()
{
    if( !mRedos.empty() )
    {
        mRedos.top()->Do();
        mUndos.push(mRedos.top());
        mRedos.pop();
    }
}

void CommandManager::ClearStack(std::stack<Command*>& pCommandStack)
{
	Command* command;
    while( !pCommandStack.empty() )
    {
		command = pCommandStack.top();

		if( command == mLastSavePoint )
			mLastSavePoint = 0;

        GD_DELETE(command);
        pCommandStack.pop();
    }        
}

void CommandManager::Save()
{
	if(!mUndos.empty())
		mLastSavePoint = mUndos.top();
	else
		mLastSavePoint = 0;
}

bool CommandManager::WasSaved() const
{
	if(mUndos.empty() && mLastSavePoint == 0)
	    return true;
	else if(mUndos.top() == mLastSavePoint)
		return true;

	return false;
}