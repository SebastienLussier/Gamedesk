/**
 *  @file       WorldTool.cpp
 *  @brief      Property editor used to edit the selection properties.
 *  @author     Sébastien Lussier.
 *  @date       29/02/04.
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
#include "WorldTool.h"
#include "WorldUI.h"

#include "..\EditorBase.h"
#include "..\WorldManager.h"


IMPLEMENT_CLASS(WorldTool);


WorldTool::WorldTool() 
    : mWorldWindow(NULL)
{
}

WorldTool::~WorldTool()
{
}

void WorldTool::Init()
{
    Super::Init();
    mWorldWindow->Init();
}

void WorldTool::Kill()
{
    mWorldWindow->Kill();
    Super::Kill();
}


void WorldTool::LoadSettings( ConfigSection& pSection )
{
    Super::LoadSettings( pSection );

    // #TODO: Load your tool's settings here.
}

void WorldTool::SaveSettings( ConfigSection& pSection ) const
{
    Super::SaveSettings( pSection );

    // #TODO: Save your tool's settings here.
}

QWidget* WorldTool::CreateUI()
{
    if( mWorldWindow == NULL )
    {
        mWorldWindow = new WorldUI(this);
        mWorldWindow->setObjectName( "World" );
        mWorldWindow->show();                
        SetWindow( mWorldWindow );

        GetEditor()->GetWorldManager().AddSelectionListener(this);
    }

    return mWorldWindow;
}

void WorldTool::DestroyUI()
{
    if( mWorldWindow )
    {
        mWorldWindow->hide();
        SetWindow( NULL );

        GetEditor()->GetWorldManager().RemoveSelectionListener(this);
    }    
}

void WorldTool::OnSelectionChanged(const std::list<Entity*>& pSelection)
{
    if( mWorldWindow )
    {
      /*  if( pSelection.size() == 1 )
            mWorldWindow->SetSelection( (Object*)pSelection.front() );
        else
            mWorldWindow->SetSelection(NULL);
      */
    }
}
