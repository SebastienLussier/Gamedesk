/**
 *  @file       InputState.cpp
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       29/01/04.
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
#include "InputState.h"


namespace Gamedesk {
	
	
InputState::InputState(const Keyboard::Key& pKey, const Keyboard::KeyState& pKeyState,
                       const Mouse::Button& pButton, const Mouse::ButtonState& pButtonState) : 
    mKey(pKey),
    mKeyState(pKeyState),
    mButton(pButton),
    mButtonState(pButtonState)
{
}

InputState::InputState(const InputState& pOther) : 
    mKey(pOther.mKey),
    mKeyState(pOther.mKeyState),
    mButton(pOther.mButton),
    mButtonState(pOther.mButtonState)
{
}

InputState::~InputState()
{}

const InputState& InputState::operator=(const InputState& pOther)
{
    mKey = pOther.mKey;
    mKeyState = pOther.mKeyState;
    mButton = pOther.mButton;
    mButtonState = pOther.mButtonState;

    return *this;
}

const Keyboard::Key& InputState::GetKey() const
{
    return mKey;
}

const Keyboard::KeyState& InputState::GetKeyState() const
{
    return mKeyState;
}

const Mouse::Button& InputState::GetButton() const
{
    return mButton;
}

const Mouse::ButtonState& InputState::GetButtonState() const
{
    return mButtonState;
}


} // namespace Gamedesk
