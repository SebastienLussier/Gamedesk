/**
 *  @file       CreateEntityCommand.cpp
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
#include "EditorLib.h"
#include "CreateEntityCommand.h"
#include "EditorBase.h"
#include "WorldManager.h"
#include "World/Entity.h"


CreateEntityCommand::CreateEntityCommand(EditorBase* pEditor, Class* pObjectClass) : 
    Command("Create Entity"),
    mEditor(pEditor),
    mEntityClass(pObjectClass),
    mCreatedEntity(NULL),
    mOwnEntity(true)
{
    GD_ASSERT(mEditor);
    GD_ASSERT(mEntityClass->IsDerivedFrom(Entity::StaticClass()));
}

CreateEntityCommand::~CreateEntityCommand()
{
    if( mOwnEntity && mCreatedEntity )
    {
        mCreatedEntity->Kill();
        GD_DELETE(mCreatedEntity);
        mCreatedEntity = NULL;
    }

    mEditor = NULL;
    mEntityClass = NULL;
}

void CreateEntityCommand::Do()
{
    if( !mCreatedEntity )
        mCreatedEntity = mEditor->GetWorldManager().SpawnEntity( mEntityClass );
    else
        mEditor->GetWorldManager().InsertEntity( mCreatedEntity );

    mOwnEntity = false;
}

void CreateEntityCommand::Undo()
{
    GD_ASSERT(mCreatedEntity);
    mEditor->GetWorldManager().RemoveEntity( mCreatedEntity );
    mOwnEntity = true;
}
