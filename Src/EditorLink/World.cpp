/**
 *  @file       World.cpp
 *  @brief      A World class to manage the world's data with the editor.
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
#include "stdafx.h"
#include "World.h"


namespace GameEdit {


World::World() :
    mSelection(gcnew List<ObjectLink^>())
{
}

World::~World()
{
}

void World::InsertEntity( ObjectLink^ pEntity )
{
    mWorld->InsertEntity( Gamedesk::Cast<Gamedesk::Entity>(pEntity->mGDObject) );

    // Notice that an entity has been created.
//    FireEntityCreated( pEntity );
}

void World::RemoveEntity( ObjectLink^ pEntity )
{
    mWorld->RemoveEntity( Gamedesk::Cast<Gamedesk::Entity>(pEntity->mGDObject) );

    // Notice that an entity has been created.
//    FireEntityDeleted( pEntity );
}

ObjectLink^ World::DoSelectionTest( const Gamedesk::Vector2f& pSelectionPoint )
{
    Gamedesk::Int32     viewport[4];
    Gamedesk::Renderer* renderer = Gamedesk::GraphicSubsystem::Instance()->GetRenderer();
    renderer->GetViewport(viewport);

    Gamedesk::Vector3f screenRay( pSelectionPoint.x, viewport[3] - pSelectionPoint.y - 1, 0 );
    Gamedesk::Vector3f worldRay = renderer->ScreenToWorld( screenRay );

    // Get the vector from the mouse positions.
    Gamedesk::Vector3f testOrigin = worldRay;    
    Gamedesk::Vector3f testDir = worldRay - mWorld->GetCurrentCamera()->GetPosition();
    testDir.Normalize();

    return ObjectLink::Create(mWorld->LineTrace( testOrigin, testDir ));
}

void World::SetSelection( ObjectLink^ pSelection )
{
    /*if( pAdd )
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
    }*/

    FireSelectionChanged();
}

void World::GetSelection( List<ObjectLink^>^ pSelection )
{
    pSelection = mSelection;
}

void World::DeleteSelection()
{
    // TODO : Delete the selection
    FireSelectionChanged();
}

void World::SetCurrentCamera( UInt32 pCameraIndex )
{
    mWorld->SetCurrentCamera( pCameraIndex );

    // Notice the listener of the new camera.
    FireCameraChanged();
}


void World::SelectionChanged::add(EventHandler^ handler)
{
    mSelectionChanged += handler;
}

void World::SelectionChanged::remove(EventHandler^ handler)
{
    mSelectionChanged -= handler;
}

void World::SelectionChanged::raise(Object^ sender, EventArgs^ e)
{
    // Check if there is any event handler registered.
    if (mSelectionChanged != nullptr) 
    {
        mSelectionChanged->Invoke(sender, e);
    }
}

void World::CameraChanged::add(EventHandler^ handler)
{
    mCameraChanged += handler;
}

void World::CameraChanged::remove(EventHandler^ handler)
{
    mCameraChanged -= handler;
}

void World::CameraChanged::raise(Object^ sender, EventArgs^ e)
{
    // Check if there is any event handler registered.
    if (mCameraChanged != nullptr) 
    {
        mCameraChanged->Invoke(sender, e);
    }
}

void World::FireCameraChanged()
{
    CameraChanged(this, gcnew EventArgs());
}

void World::FireSelectionChanged()
{
    SelectionChanged(this, gcnew EventArgs());
}


} // namespace GameEdit
