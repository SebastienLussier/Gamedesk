/**
 *  @file       PropertyEditorTool.cpp
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
#include "PropertyEditorDll.h"
#include "PropertyEditorTool.h"

#include "PropertyEditorUI.h"

#include "EditorBase.h"
#include "WorldManager.h"


IMPLEMENT_CLASS(PropertyEditorTool);


PropertyEditorTool::PropertyEditorTool() :
    mPropertyEditorWindow(NULL)
{
}

PropertyEditorTool::~PropertyEditorTool()
{
}

void PropertyEditorTool::LoadSettings( ConfigSection& pSection )
{
    Super::LoadSettings( pSection );

    // #TODO: Load your tool's settings here.
}

void PropertyEditorTool::SaveSettings( ConfigSection& pSection ) const
{
    Super::SaveSettings( pSection );

    // #TODO: Save your tool's settings here.
}

QWidget* PropertyEditorTool::CreateUI()
{
    if( mPropertyEditorWindow == NULL )
    {
        mPropertyEditorWindow = new PropertyEditorUI(this);
        mPropertyEditorWindow->setObjectName( "Property Editor" );
        mPropertyEditorWindow->show();                
        SetWindow( mPropertyEditorWindow );

        GetEditor()->GetWorldManager().AddSelectionListener(this);
    }

    return mPropertyEditorWindow;
}

void PropertyEditorTool::DestroyUI()
{
    if( mPropertyEditorWindow )
    {
        mPropertyEditorWindow->hide();
        SetWindow( NULL );

        GetEditor()->GetWorldManager().RemoveSelectionListener(this);
    }    
}

void PropertyEditorTool::OnSelectionChanged(const std::list<Entity*>& pSelection)
{
    if( mPropertyEditorWindow )
    {
        if( pSelection.size() == 1 )
            mPropertyEditorWindow->SetSelection( (Object*)pSelection.front() );
        else
            mPropertyEditorWindow->SetSelection(NULL);
    }
}
