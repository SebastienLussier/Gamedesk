/**
 *  @file       InputState.h
 *  @brief	    
 *  @author     Marco Arsenault
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
#ifndef     _INPUT_STATE_H_
#define     _INPUT_STATE_H_


#include "Keyboard.h"
#include "Mouse.h"


 namespace Gamedesk {


/**
 *  InputState object.  Represent a state of the mouse and keyboard input.
 *  @brief  InputState object.
 *  @author Marco Arsenault.
 *  @date   29/01/04.
 */
class ENGINE_API InputState
{
public:
    //! Default constructor.
    InputState(const Keyboard::Key& pKey, const Keyboard::KeyState& pKeyState,
               const Mouse::Button& pButton, const Mouse::ButtonState& pButtonState);
    //! Copy constructor.
    InputState(const InputState& pOther);
    //! Destructor.
    ~InputState();

    //! Assignement operator.
    const InputState& operator=(const InputState& pOther);

    //! Returns the key.
    const Keyboard::Key& GetKey() const;
    //! Returns the key state.
    const Keyboard::KeyState& GetKeyState() const;
    //! Returns the button.
    const Mouse::Button& GetButton() const;
    //! Returns the button state.
    const Mouse::ButtonState& GetButtonState() const;

private:
    Keyboard::Key       mKey;
    Keyboard::KeyState  mKeyState;
    Mouse::Button       mButton;
    Mouse::ButtonState  mButtonState;
};


} // namespace Gamedesk


#endif  //  _INPUT_STATE_H_
