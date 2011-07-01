/**
 *  @file       Viewer.cpp
 *  @brief      A viewer base class for the editor.
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
#include "Viewer.h"
#include "Manipulator/Manipulator.h"
#include "Input/InputSubsystem.h"


Viewer::Viewer( QWidget* parent, RenderListener* pRenderListener ) : 
    QRenderWindow(parent, pRenderListener),
    mActiveManipulator(0)
{
}

Viewer::~Viewer()
{
}

void Viewer::Init()
{
    QRenderWindow::Init();

    Mouse& mouse = InputSubsystem::GetMouse();
	mouse.AddButtonListener(this, Mouse::Button_Left, Mouse::Button_Pressed);
}

void Viewer::Kill()
{
    QRenderWindow::Kill();

    Mouse& mouse = InputSubsystem::GetMouse();
	mouse.RemoveButtonListener(this, Mouse::Button_Left, Mouse::Button_Pressed);
}

void Viewer::AddManipulator(Manipulator* pManipulator)
{
    mManipulators.push_back(pManipulator);

    // Get the input devices.
    Keyboard& keyboard = InputSubsystem::GetKeyboard();
    Mouse& mouse = InputSubsystem::GetMouse();
    
    // Start listening to the activation input state.
    InputState activationState = pManipulator->GetActivationInputState();
    if(activationState.GetKey() != Keyboard::Key_None)
    {
        keyboard.AddKeyListener(this, activationState.GetKey(), 
                                      activationState.GetKeyState());
    }
    if(activationState.GetButton() != Mouse::Button_None)
    {
        mouse.AddButtonListener(this, activationState.GetButton(), 
                                      activationState.GetButtonState());
    }
    
    // Start listening to the deactivation input state.
    InputState deactivationState = pManipulator->GetDeactivationInputState();
    if(deactivationState.GetKey() != Keyboard::Key_None &&
       (deactivationState.GetKey() !=  activationState.GetKey() ||
        deactivationState.GetKeyState() !=  activationState.GetKeyState()))
    {
        
        keyboard.AddKeyListener(this, deactivationState.GetKey(), 
                                      deactivationState.GetKeyState());
    }
    if(deactivationState.GetButton() != Mouse::Button_None &&
       (deactivationState.GetButton() !=  activationState.GetButton() ||
        deactivationState.GetButtonState() !=  activationState.GetButtonState()))
    {
        mouse.AddButtonListener(this, deactivationState.GetButton(), 
                                      deactivationState.GetButtonState());
    }
}

void Viewer::RemoveManipulator(Manipulator* pManipulator)
{
    mManipulators.remove(pManipulator);

    // Get the input devices.
    Keyboard& keyboard = InputSubsystem::GetKeyboard();
    Mouse& mouse = InputSubsystem::GetMouse();

    // Stop listening to the activation input state.
    InputState activationState = pManipulator->GetActivationInputState();
    if(activationState.GetKey() != Keyboard::Key_None)
    {
        keyboard.RemoveKeyListener(this, activationState.GetKey(), 
                                         activationState.GetKeyState());
    }
    if(activationState.GetButton() != Mouse::Button_None)
    {
        mouse.RemoveButtonListener(this, activationState.GetButton(), 
                                         activationState.GetButtonState());
    }
    
    // Stop listening to the deactivation input state.
    InputState deactivationState = pManipulator->GetDeactivationInputState();
    if(deactivationState.GetKey() != Keyboard::Key_None &&
       (deactivationState.GetKey() !=  activationState.GetKey() ||
        deactivationState.GetKeyState() !=  activationState.GetKeyState()))
    {
        
        keyboard.RemoveKeyListener(this, deactivationState.GetKey(), 
                                         deactivationState.GetKeyState());
    }
    if(deactivationState.GetButton() != Mouse::Button_None &&
       (deactivationState.GetButton() !=  activationState.GetButton() ||
        deactivationState.GetButtonState() !=  activationState.GetButtonState()))
    {
        mouse.RemoveButtonListener(this, deactivationState.GetButton(), 
                                         deactivationState.GetButtonState());
    }
}

Manipulator* Viewer::GetActiveManipulator()
{
    return mActiveManipulator;
}

void Viewer::OnKeyPressed( const Keyboard::Key& pKey )
{
    // Map the mouse position to the viewer widget area.
    Int32 posX, posY;
    InputSubsystem::GetMouse().GetPos(posX, posY);
	mKeyPressedOnViewer = IsPointOnViewer(posX, posY);
	if(!mKeyPressedOnViewer)
		return;

    // Check all the manipulator if one should be activated or deactivated.
    std::list<Manipulator*>::iterator itManip;
    for(itManip = mManipulators.begin(); itManip != mManipulators.end(); itManip++)
    {
        Manipulator* currentManip = *itManip;
        if(currentManip->IsActivated())
        {
            InputState deactivationState = currentManip->GetDeactivationInputState();
            if(deactivationState.GetKeyState() == Keyboard::Key_Pressed && 
               deactivationState.GetKey() == pKey)
            {
                DeactivateManipulator(currentManip);
                break;
            }
        }
        else
        {
            InputState activationState = currentManip->GetActivationInputState();
            if(activationState.GetKeyState() == Keyboard::Key_Pressed && 
               activationState.GetKey() == pKey)
            {
                ActivateManipulator(currentManip);
                break;
            }
        }
    }
}

void Viewer::OnKeyReleased( const Keyboard::Key& pKey )
{
	if(!mKeyPressedOnViewer)
		return;

    // Check all the manipulator if one should be activated or deactivated.
    std::list<Manipulator*>::iterator itManip;
    for(itManip = mManipulators.begin(); itManip != mManipulators.end(); itManip++)
    {
        Manipulator* currentManip = *itManip;
        if(currentManip->IsActivated())
        {
            InputState deactivationState = currentManip->GetDeactivationInputState();
            if(deactivationState.GetKeyState() == Keyboard::Key_Released && 
               deactivationState.GetKey() == pKey)
            {
                DeactivateManipulator(currentManip);
                break;
            }
        }
        else
        {
            InputState activationState = currentManip->GetActivationInputState();
            if(activationState.GetKeyState() == Keyboard::Key_Released && 
               activationState.GetKey() == pKey)
            {
                ActivateManipulator(currentManip);
                break;
            }
        }
    }
}

void Viewer::OnMouseButtonPressed( const Mouse::Button& pButton )
{
	// Map the mouse position to the viewer widget area.
    Int32 posX, posY;
    InputSubsystem::GetMouse().GetPos(posX, posY);
	mButtonPressedOnViewer = IsPointOnViewer(posX, posY);

	// Tell the manipulators if they have the focus. 
	std::list<Manipulator*>::iterator itManip;
    for(itManip = mManipulators.begin(); itManip != mManipulators.end(); itManip++)
    {
        (*itManip)->SetFocus(mButtonPressedOnViewer);
    }

	if(!mButtonPressedOnViewer)
		return;

    // Check all the manipulator if one should be activated or deactivated.
    for(itManip = mManipulators.begin(); itManip != mManipulators.end(); itManip++)
    {
        Manipulator* currentManip = *itManip;
        if(currentManip->IsActivated())
        {
            InputState deactivationState = currentManip->GetDeactivationInputState();
            if(deactivationState.GetButtonState() == Mouse::Button_Pressed && 
               deactivationState.GetButton() == pButton)
            {
                DeactivateManipulator(currentManip);
                break;
            }
        }
        else
        {
            InputState activationState = currentManip->GetActivationInputState();
            if(activationState.GetButtonState() == Mouse::Button_Pressed && 
               activationState.GetButton() == pButton)
            {
                ActivateManipulator(currentManip);
                break;
            }
        }
    }
}

void Viewer::OnMouseButtonReleased( const Mouse::Button& pButton )
{
	if(!mButtonPressedOnViewer)
		return;

    // Check all the manipulator if one should be activated or deactivated.
    std::list<Manipulator*>::iterator itManip;
    for(itManip = mManipulators.begin(); itManip != mManipulators.end(); itManip++)
    {
        Manipulator* currentManip = *itManip;
        if(currentManip->IsActivated())
        {
            InputState deactivationState = currentManip->GetDeactivationInputState();
            if(deactivationState.GetButtonState() == Mouse::Button_Released && 
               deactivationState.GetButton() == pButton)
            {
                DeactivateManipulator(currentManip);
                break;
            }
        }
        else
        {
            InputState activationState = currentManip->GetActivationInputState();
            if(activationState.GetButtonState() == Mouse::Button_Released && 
               activationState.GetButton() == pButton)
            {
                ActivateManipulator(currentManip);
                break;
            }
        }
    }
}

void Viewer::ActivateManipulator(Manipulator* pManipulator)
{
    if(mActiveManipulator)
    {
        mActiveManipulator->Deactivate();
    }
    mActiveManipulator = pManipulator;
    mActiveManipulator->Activate();
}

void Viewer::DeactivateManipulator(Manipulator* pManipulator)
{
    pManipulator->Deactivate();
    if(mActiveManipulator == pManipulator)
    {
        mActiveManipulator = 0;
    }
}

Bool Viewer::IsPointOnViewer(Int32 pX, Int32 pY) const
{
	QPoint widgetPoint = mapFromGlobal(QPoint(pX, pY));
		
	return widgetPoint.x() >= 0 && widgetPoint.x() < width() &&
		   widgetPoint.y() >= 0 && widgetPoint.y() < height();
}
