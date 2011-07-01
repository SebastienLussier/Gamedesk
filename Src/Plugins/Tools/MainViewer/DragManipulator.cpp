/**
 *  @file       DragManipulator.cpp
 *  @brief
 *  @author     Marco Arsenault.
 *  @date       04/02/04.
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
#include "MainViewerDll.h"
#include "DragManipulator.h"
#include "DragCommand.h"
#include "Input/InputSubsystem.h"
#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "EditorBase.h"
#include "Viewer.h"

#include "WorldManager.h"
#include "World/Entity.h"
#include "World/Camera.h"
#include "World/Terrain.h"
#include "World/SkyDome.h"


DragManipulator::DragManipulator(EditorBase* pEditor) :
    mActivationInputState(Keyboard::Key_T, Keyboard::Key_Pressed,
                          Mouse::Button_None, Mouse::Button_Pressed),
    mDeactivationInputState(Keyboard::Key_T, Keyboard::Key_Pressed,
                            Mouse::Button_None, Mouse::Button_Pressed),
    mEditor(pEditor)
{
}

DragManipulator::~DragManipulator()
{
}

void DragManipulator::Activate()
{
    Manipulator::Activate();

    Mouse& mouse = InputSubsystem::GetMouse();
    mouse.AddMoveListener(this);
    mouse.AddButtonListener(this, Mouse::Button_Left, Mouse::Button_Pressed);
    mouse.AddButtonListener(this, Mouse::Button_Left, Mouse::Button_Released);

    UpdateOriginPositions();
}

void DragManipulator::Deactivate()
{
    Manipulator::Deactivate();

    Mouse& mouse = InputSubsystem::GetMouse();
    mouse.RemoveMoveListener(this);
    mouse.RemoveButtonListener(this, Mouse::Button_Left, Mouse::Button_Pressed);
    mouse.RemoveButtonListener(this, Mouse::Button_Left, Mouse::Button_Released);
}

const InputState& DragManipulator::GetActivationInputState() const
{
    return mActivationInputState;
}

const InputState& DragManipulator::GetDeactivationInputState() const
{
    return mDeactivationInputState;
}

Bool DragManipulator::CanManipulate(const std::list<Entity*>& pEntities) const
{
    Bool canManipulateAll = true;

    // Check if all objects are supported,
    std::list<Entity*>::const_iterator itEntity;
    for(itEntity = pEntities.begin(); itEntity != pEntities.end(); itEntity++)
    {
        if(!(*itEntity)->IsA(Entity::StaticClass()) ||
            (*itEntity)->IsA(Terrain::StaticClass()) ||
            (*itEntity)->IsA(SkyDome::StaticClass()))
        {
            canManipulateAll = false;
            break;
        }
    }

    return canManipulateAll;
}

Bool DragManipulator::CanManipulate(Entity* pEntity) const
{
    return pEntity->IsA(Entity::StaticClass());
}

void DragManipulator::SetManipulatedEntity(Entity* pEntity)
{
    mEntities.clear();

    if(pEntity)
        mEntities.push_back(pEntity);
}

void DragManipulator::SetManipulatedEntity(const std::list<Entity*>& pEntities)
{
    mEntities = pEntities;
}

void DragManipulator::OnMouseMove( Int32 /*pRelX*/, Int32 /*pRelY*/ )
{
    if(!IsActivated() || mEntities.size() == 0 || !HasFocus() ||
        InputSubsystem::GetMouse().IsUp(Mouse::Button_Left))
    {
        return;
    }

	mEditor->GetMainView()->MakeCurrent();
   
    // Get the current camera.
    Camera* camera = mEditor->GetWorldManager().GetCurrentCamera();

    // Get the renderer.
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    // Get the new widget and screen position.
    Int32 x, y;
    InputSubsystem::GetMouse().GetPos(x, y);
    Vector2f newScreenPos(x, y);

    QPoint widgetPos = mEditor->GetMainView()->mapFromGlobal(QPoint(x, y));
    Vector2f newWidgetPos(widgetPos.x(), widgetPos.y());

    // Calculate the origin screen position.
    Vector3f originWindowPos = renderer->WorldToScreen( mWorldPosition );

    // Calculate the right offset.
    Vector3f rightWorldOffet = mWorldPosition + camera->GetRight();
    Vector3f rightWindowOffset = renderer->WorldToScreen( rightWorldOffet );

    // Calculate the up offset.
    Vector3f upWorldOffet = mWorldPosition + camera->GetUp();
    Vector3f upWindowOffset = renderer->WorldToScreen( upWorldOffet );

	// Get the offsets for 1 pixel.
	Float nbPixelRight = rightWindowOffset.x - originWindowPos.x;
	Float nbPixelUp = upWindowOffset.y - originWindowPos.y;
    
	// Get the nb. of pixels between the current mouse position and the click position.
	Float screenOffsetRight = newScreenPos.x - mScreenClickPos.x;
	Float screenOffsetUp = newScreenPos.y - mScreenClickPos.y;

    GD_ASSERT(mEntities.size() == mOriginPositions.size());
    std::list<Entity*>::iterator itEntity = mEntities.begin();
	std::list<Vector3f>::iterator itOriginPos = mOriginPositions.begin();
	while(itEntity != mEntities.end())
	{
		Vector3f newPosition = *itOriginPos + (camera->GetRight() * screenOffsetRight / nbPixelRight)
									        + (camera->GetUp() * screenOffsetUp / -nbPixelUp);
		Vector3f oldPosition = (*itEntity)->GetPosition();

		(*itEntity)->SetPosition(newPosition);

        ++itEntity;
        ++itOriginPos;
    }
}

void DragManipulator::OnMouseButtonPressed(const Mouse::Button& pButton)
{
    if(pButton != Mouse::Button_Left || !HasFocus())
        return;

    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    // Get the position of the mouse in the screen and widget.
    Int32 x, y;
    InputSubsystem::GetMouse().GetPos(x, y);
    mScreenClickPos = Vector2f(x, y);

    QPoint widgetPos = mEditor->GetMainView()->mapFromGlobal(QPoint(x, y));
    mClickPos = Vector2f(widgetPos.x(), widgetPos.y());

    // Calculate the center position of the manipulated objects.
    mWorldPosition = GetCenterPosition();

    UpdateOriginPositions();
}

void DragManipulator::OnMouseButtonReleased(const Mouse::Button& pButton)
{
    if(pButton != Mouse::Button_Left || !HasFocus())
        return;

    if(GetCenterPosition() != mWorldPosition)
    {
        /*DragCommand* dragCommand = new DragCommand(mObjectProperties,
                                                   mOldObjectPosition);
        mEditor->GetCommandManager().PerformCommand(dragCommand);*/
    }
}

Vector3f DragManipulator::GetCenterPosition() const
{
	// Calculate the center of the group of object.
	Vector3f centerPosition(0, 0, 0);

    std::list<Entity*>::const_iterator itEntity;
	for(itEntity = mEntities.begin(); itEntity != mEntities.end(); ++itEntity)
		centerPosition += (*itEntity)->GetPosition();
	
	return centerPosition / mEntities.size();
}

void DragManipulator::UpdateOriginPositions()
{
	mOriginPositions.clear();
	std::list<Entity*>::iterator itEntity;
	for(itEntity = mEntities.begin(); itEntity != mEntities.end(); ++itEntity)
	{
		mOriginPositions.push_back((*itEntity)->GetPosition());
	}
}