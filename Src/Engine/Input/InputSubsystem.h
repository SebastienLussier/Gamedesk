/**
 *  @file       InputSubsystem.h
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
#ifndef     _INPUT_SUBSYSTEM_H_
#define     _INPUT_SUBSYSTEM_H_


#include "Patterns/Singleton.h"
#include "Subsystem/Subsystem.h"

#include "Mouse.h"
#include "Keyboard.h"


namespace Gamedesk {


/**
 *  Manage access to input devices.
 */
class ENGINE_API InputSubsystem : public Subsystem
{
    DECLARE_ABSTRACT_CLASS(InputSubsystem, Subsystem);
    DECLARE_ABSTRACT_SINGLETON(InputSubsystem);

public:
    //! Destructor.
    virtual ~InputSubsystem();

    /**
     *  Update the state of each input devices and fire events.
     *  This method has to be overidden in derived classes. You need to 
     *  call \bFireEvents() at the end of it to notify the listeners of 
     *  changes to these devices
     */
    virtual void Update() = 0;

    /**
     *  Get a reference to the mouse device.
     *  Will thrown an exception if there is no InputSybsystem instance.
     */
    static Mouse&     GetMouse();

    /**
     *  Get a reference to the keyboard device.
     *  Will thrown an exception if there is no InputSybsystem instance.
     */
    static Keyboard&  GetKeyboard();
    
protected:
    /**
     *  Update the state of a given key of the keyboard.
     *  This class is a friend of class Keyboard, so only it can update the keyboard state.
     *  Call this method from your derived InputSubsystem class.
     *  @param  pKey    The key to update.
     *  @param  pState  The new state of the key.
     */
    void SetKeyboardState( Keyboard::Key pKey, Keyboard::KeyState pState );

    /**
     *  Update the state of a given button of the mouse.
     *  This class is a friend of class Mouse, so only it can update the mouse state.
     *  Call this method from your derived InputSubsystem class.
     *  @param  pButton The button to update.
     *  @param  pState  The new state of the button.
     */
    void SetMouseButtonState( Mouse::Button pButton, Mouse::ButtonState pState );
    
    /**
     *  Update the value of a given axis of the mouse.
     *  This class is a friend of class Mouse, so only it can update the mouse state.
     *  Call this method from your derived InputSubsystem class.
     *  @param  pAxis   The axis to update.
     *  @param  pValue  The new value (relative) of the axis.
     */
    void SetMouseAxisValue( Mouse::Axis pAxis, Int32 pValue );

    /**
     *  Update the absolute of the mouse.
     *  This class is a friend of class Mouse, so only it can update the mouse state.
     *  Call this method from your derived InputSubsystem class.
     *  @param  pPosX   The position on axis X.
     *  @param  pPosY   The position on axis Y.
     */
    void SetMousePosition( Int32 pPosX, Int32 pPosY );

    //! Fire events (notify the device listeners of the changes to theses devices).
    void FireEvents();

protected:
    //! Constructor.
    InputSubsystem();

protected:
    Mouse                   mMouse;             //!< Mouse device.
    Keyboard                mKeyboard;          //!< Keyboard device.
    Vector<InputDevice*>    mOtherDevices;      //!< List of other possible devices.
};


} // namespace Gamedesk


#endif  //  _INPUT_SUBSYSTEM_H_
