/**
 *  @file       WorldManager.cpp
 *  @brief      A WorldManager class to manage the world's data with the editor.
 *  @author     Marco Arsenault.
 *  @date       31/01/04.
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
#include "WorldManager.h"

#include "EditorBase.h"
#include "Viewer.h"

#include "Module/ModuleManager.h"

#include "World/Entity.h"
#include "World/Camera.h"

#include "System/SystemInfo.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/RenderTarget.h"

#include "Maths/Frustum.h"
#include "Input/InputSubsystem.h"


IMPLEMENT_CLASS(WorldManager);


WorldManager::WorldManager() :
    mSelection(0)
{
}

WorldManager::~WorldManager()
{
}

void WorldManager::Init()
{
    Super::Init();
    ModuleManager::Instance()->LoadModule("Space");
}

void WorldManager::InsertEntity( Entity* pEntity )
{
    Super::InsertEntity( pEntity );

    // Notice that an entity has been created.
    FireEntityCreated( pEntity );
}

void WorldManager::RemoveEntity( Entity* pEntity )
{
    Super::RemoveEntity( pEntity );

    // Notice that an entity has been created.
    FireEntityDeleted( pEntity );
}

Entity* WorldManager::DoSelectionTest( const Vector2f& pSelectionPoint )
{
    Int32     viewport[4];
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    renderer->GetViewport(viewport);

    Vector3f screenRay( pSelectionPoint.x, viewport[3] - pSelectionPoint.y - 1, 0 );
    Vector3f worldRay = renderer->ScreenToWorld( screenRay );

    // Get the vector from the mouse positions.
    Vector3f testOrigin = worldRay;    
    Vector3f testDir = worldRay - GetCurrentCamera()->GetPosition();
    testDir.Normalize();

    return LineTrace( testOrigin, testDir );
}

void WorldManager::SetSelection( Entity* pSelection, Bool pAdd )
{
    if( pAdd )
    {
        if( pSelection )
        {
            Bool removedFromList = false;
    
            std::list<Entity*>::iterator itSelection;
            for( itSelection = mSelection.begin(); itSelection != mSelection.end(); ++itSelection )
            {
                if( *itSelection == pSelection )
                {
                    pSelection->Select( false );
                    mSelection.erase(itSelection);
                    removedFromList = true;
                    break;
                }
            }

            if( !removedFromList )
            {
                pSelection->Select( true );
                mSelection.push_back( pSelection );
            }
        }
    }
    else
    {
        std::list<Entity*>::iterator itSelection;
        for( itSelection = mSelection.begin(); itSelection != mSelection.end(); ++itSelection )
            (*itSelection)->Select( false );
       
        mSelection.clear();

        if( pSelection )
        {
            pSelection->Select( true );
            mSelection.push_back( pSelection );
        }
    }

    FireSelectionChanged( mSelection );
}

void WorldManager::GetSelection( std::list<Entity*>& pSelection ) const
{
    pSelection = mSelection;
}

void WorldManager::DeleteSelection()
{
    // TODO : Delete the selection
    FireSelectionChanged( mSelection );
}

void WorldManager::SetCurrentCamera( UInt32 pCameraIndex )
{
    Super::SetCurrentCamera( pCameraIndex );

    // Notice the listener of the new camera.
    FireCameraChanged( GetCurrentCamera() );
}
