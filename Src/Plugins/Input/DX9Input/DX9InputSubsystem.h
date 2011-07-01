/**
 *  @file       DX9InputSubsystem.h
 *  @brief      DX9 Input Subsystem.
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
#ifndef     _DX9INPUT_SUBSYSTEM_H_
#define     _DX9INPUT_SUBSYSTEM_H_


#include "Input/InputSubsystem.h"


namespace Gamedesk {


const Int32 DI_KEY_ARRAY_SIZE = 256;


/**
 *  Input subsystem that uses DirectInput to update the devices states.
 */
class DX9INPUT_API DX9InputSubsystem : public InputSubsystem
{
    DECLARE_CLASS(DX9InputSubsystem,InputSubsystem);

public:
    //! Constructor.
    DX9InputSubsystem();
    
    //! Initialize DirectInput and basic device.
    virtual void Init();

    //! Free DirectInput resource.
    virtual void Kill();

    /**
     *  Update the state of each input devices and fire events.
     *  This method has to be overidden in derived classes. You need to 
     *  call \bFireEvents() at the end of it to notify the listeners of 
     *  changes to these devices
     */ 
    virtual void Update();
    
protected:
    //! Initialize DirectInput mouse.
    void InitMouse();

    //! Initialize DirectInput keyboard.
    void InitKeyboard();
    
    //! Update the state of the mouse.
    void UpdateMouse();

    //! Update the state of the keyboard.
    void UpdateKeyboard();

private:
    LPDIRECTINPUT8          mDirectInput;       //!< DirectInput object.
    LPDIRECTINPUTDEVICE8    mKeyboardDevice;    //!< DirectInput device for the keyboard.
    LPDIRECTINPUTDEVICE8    mMouseDevice;       //!< DirectInput device for the mouse.

    DIMOUSESTATE2           mLastMouseState;                        //!< Last state of the mouse.
    Byte                    mLastKeyboardState[DI_KEY_ARRAY_SIZE];  //!< Last state of the keyboard.
};


class DX9INPUT_API DInputException : public Exception
{
    DECLARE_EXCEPTION(DInputException);

public:
    DInputException( HRESULT pResult, CodeLocation pHere );
};


} // namespace Gamedesk


#endif  //  _DX9INPUT_SUBSYSTEM_H_
