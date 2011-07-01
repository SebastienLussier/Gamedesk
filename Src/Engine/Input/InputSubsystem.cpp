/**
 *  @file       InputSubsystem.cpp
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       05/01/04.
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
#include "Engine.h"
#include "InputSubsystem.h"


namespace Gamedesk {
	
	
IMPLEMENT_ABSTRACT_CLASS(InputSubsystem);
IMPLEMENT_ABSTRACT_SINGLETON(InputSubsystem);


InputSubsystem::InputSubsystem()
{
    mInstance = this;
}

InputSubsystem::~InputSubsystem()
{
    mInstance = NULL;
}

Mouse& InputSubsystem::GetMouse()
{
    if( mInstance )
        return mInstance->mMouse;
    else
        throw NullPointerException("mInstance",Here);
}

Keyboard& InputSubsystem::GetKeyboard()
{
    if( mInstance )
        return mInstance->mKeyboard;
    else
        throw NullPointerException("mInstance",Here);
}

void InputSubsystem::SetKeyboardState( Keyboard::Key pKey, Keyboard::KeyState pState )
{
    mKeyboard.mKeysStates[pKey] = pState;
}

void InputSubsystem::SetMouseButtonState( Mouse::Button pButton, Mouse::ButtonState pState )
{
    mMouse.mButtonsStates[pButton] = pState;
}

void InputSubsystem::SetMouseAxisValue( Mouse::Axis pAxis, Int32 pValue )
{
    mMouse.mAxis[pAxis] = pValue;
}

void InputSubsystem::SetMousePosition( Int32 pPosX, Int32 pPosY )
{
    mMouse.mPosition[0] = pPosX;
    mMouse.mPosition[1] = pPosY;
}

void InputSubsystem::FireEvents()
{
    mKeyboard.FireEvents();
    mMouse.FireEvents();
}


} // namespace Gamedesk
