/**
 *  @file       ResourceManagerTool.cpp
 *  @brief      Manage resources visually.
 *  @author     Sébastien Lussier.
 *  @date       23/10/06.
 */
/*
 *  Copyright (C) 2006 Gamedesk
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
#include "ResourceManagerTool.h"
#include "ResourceManagerUI.h"

#include "..\EditorBase.h"


IMPLEMENT_CLASS(ResourceManagerTool);


ResourceManagerTool::ResourceManagerTool() 
    : mManagerUI(NULL)
{
    
}

ResourceManagerTool::~ResourceManagerTool()
{
}

void ResourceManagerTool::Init()
{
    Super::Init();
    mManagerUI->Init();
}

void ResourceManagerTool::Kill()
{
    mManagerUI->Kill();
    Super::Kill();
}


void ResourceManagerTool::LoadSettings( ConfigSection& pSection )
{
    Super::LoadSettings( pSection );

    // #TODO: Load your tool's settings here.
}

void ResourceManagerTool::SaveSettings( ConfigSection& pSection ) const
{
    Super::SaveSettings( pSection );

    // #TODO: Save your tool's settings here.
}

QWidget* ResourceManagerTool::CreateUI()
{
    if( mManagerUI == NULL )
    {
        mManagerUI = new ResourceManagerUI(this);
        mManagerUI->setObjectName( "Resources" );
        mManagerUI->show();                
        SetWindow( mManagerUI );
    }

    return mManagerUI;
}

void ResourceManagerTool::DestroyUI()
{
    if( mManagerUI )
    {
        mManagerUI->hide();
        SetWindow( NULL );
    }    
}
