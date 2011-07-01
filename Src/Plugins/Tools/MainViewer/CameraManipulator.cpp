/**
 *  @file       CameraManipulator.cpp
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       09/01/04.
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
#include "CameraManipulator.h"
#include "World/Camera.h"
#include "Input/InputSubsystem.h"


const Float sgk_movementSpeed(50.0f);
const Float sgk_rotationSpeed(0.3f);


CameraManipulator::CameraManipulator() : 
    mCamera(NULL),
    mActivationInputState(Keyboard::Key_None, Keyboard::Key_Pressed,
                          Mouse::Button_Right, Mouse::Button_Pressed),
    mDeactivationInputState(Keyboard::Key_None, Keyboard::Key_Pressed,
                            Mouse::Button_Right, Mouse::Button_Released)
{
}

CameraManipulator::~CameraManipulator()
{
}

void CameraManipulator::Activate()
{
    Manipulator::Activate();

    Keyboard& keyboard = InputSubsystem::GetKeyboard();
    keyboard.AddKeyListener(this, Keyboard::Key_W, Keyboard::Key_Down);
    keyboard.AddKeyListener(this, Keyboard::Key_S, Keyboard::Key_Down);
    keyboard.AddKeyListener(this, Keyboard::Key_A, Keyboard::Key_Down);
    keyboard.AddKeyListener(this, Keyboard::Key_D, Keyboard::Key_Down);
        
    Mouse& mouse = InputSubsystem::GetMouse();
    mouse.AddMoveListener(this);
}

void CameraManipulator::Deactivate()
{
    Manipulator::Deactivate();

    Keyboard& keyboard = InputSubsystem::GetKeyboard();
    keyboard.RemoveKeyListener(this, Keyboard::Key_W, Keyboard::Key_Down);
    keyboard.RemoveKeyListener(this, Keyboard::Key_S, Keyboard::Key_Down);
    keyboard.RemoveKeyListener(this, Keyboard::Key_A, Keyboard::Key_Down);
    keyboard.RemoveKeyListener(this, Keyboard::Key_D, Keyboard::Key_Down);
        
    Mouse& mouse = InputSubsystem::GetMouse();
    mouse.RemoveMoveListener(this);
}

const InputState& CameraManipulator::GetActivationInputState() const
{
    return mActivationInputState;
}

const InputState& CameraManipulator::GetDeactivationInputState() const
{
    return mDeactivationInputState;
}

Bool CameraManipulator::CanManipulate(Entity* pEntity) const
{
    return pEntity->IsA(Camera::StaticClass());
}

void CameraManipulator::SetManipulatedEntity(Entity* pEntity)
{
    mCamera = Cast<Camera>(pEntity);
}

void CameraManipulator::OnMouseMove( Int32 pRelX, Int32 pRelY )
{
    if(!IsActivated()) 
        return;

    mCamera->Pitch(-pRelY * sgk_rotationSpeed);
    mCamera->Yaw(-pRelX * sgk_rotationSpeed);
}

void CameraManipulator::OnKeyDown( const Keyboard::Key& pKey )
{
    if(!IsActivated()) 
        return;
    
    if(pKey == Keyboard::Key_W)
        mCamera->Move(sgk_movementSpeed);
    else if(pKey == Keyboard::Key_S)
        mCamera->Move(-sgk_movementSpeed);
    
    if(pKey == Keyboard::Key_D)
        mCamera->PanLeftRight(sgk_movementSpeed);
    else if(pKey == Keyboard::Key_A)
        mCamera->PanLeftRight(-sgk_movementSpeed);
}