/**
 *  @file       Keyboard.h
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
#ifndef     _KEYBOARD_H_
#define     _KEYBOARD_H_


#include "InputDevice.h"


namespace Gamedesk {


/**
 *  Access information about the keyboard state.
 */
class ENGINE_API Keyboard : public InputDevice
{
    DECLARE_CLASS(Keyboard,InputDevice);
    friend class InputSubsystem;

public:

    //! Key enumeration.
    enum Key
    {
        Key_0,
        Key_1,
        Key_2,
        Key_3,
        Key_4,
        Key_5,
        Key_6,
        Key_7,
        Key_8,
        Key_9,
        
        Key_A,
        Key_B,
        Key_C,
        Key_D,
        Key_E,
        Key_F,
        Key_G,
        Key_H,
        Key_I,
        Key_J,
        Key_K,
        Key_L,
        Key_M,    
        Key_N,
        Key_O,
        Key_P,
        Key_Q,
        Key_R,
        Key_S,
        Key_T,
        Key_U,
        Key_V,
        Key_W,
        Key_X,
        Key_Y,
        Key_Z,

        Key_F1,
        Key_F2,
        Key_F3,
        Key_F4,
        Key_F5,
        Key_F6,
        Key_F7,
        Key_F8,
        Key_F9,
        Key_F10,
        Key_F11,
        Key_F12,
        Key_F13,
        Key_F14,
        Key_F15,

        Key_UpArrow,
        Key_DownArrow,
        Key_LeftArrow,
        Key_RightArrow,

        Key_Home,
        Key_End,
        Key_PageUp,
        Key_PageDown,
        Key_Insert,
        Key_Delete,

        Key_Escape,
        
        Key_NumLock,
        Key_NumPad0,
        Key_NumPad1,
        Key_NumPad2,
        Key_NumPad3,
        Key_NumPad4,
        Key_NumPad5,
        Key_NumPad6,
        Key_NumPad7,
        Key_NumPad8,
        Key_NumPad9,
        Key_NumPadEnter,
        Key_NumSubtract,        // (-) on numeric keypad 
        Key_NumAdd,             // (+) on numeric keypad 
        Key_NumMultiply,        // (*) on numeric keypad 
        Key_NumDivide,          // (/) on numeric keypad 
        Key_NumPoint,           // PERIOD (decimal point) on numeric keypad 

        Key_LeftBracket,        // Left square bracket [ 
        Key_LeftCtrl,           // Left CTRL         
        Key_LeftAlt,            // Left ALT 
        Key_LeftShift,          // Left SHIFT 
        Key_LeftWin,            // Left Microsoft® Windows® logo key 

        Key_RightBracket,       // Right square bracket ] 
        Key_RightCtrl,          // Right CTRL 
        Key_RightAlt,           // Right ALT 
        Key_RightShift,         // Right SHIFT 
        Key_RightWin,           // Right Windows logo key 

        Key_PrintScreen,
        Key_ScrollLock,
        Key_Pause,
        
        Key_Spacebar,
        Key_Backspace,
        Key_Enter,              // ENTER on main keyboard 
        Key_Tab,            

        Key_Apps,               // Application key 
        Key_CapsLock,

        Key_Slash,              // (/) On main keyboard 
        Key_Backslash,          // (\) 
        Key_Comma,              // (,)
        Key_Semicolon,          // (;)
        Key_Period,             // (.) On main keyboard 
        Key_Grave,              // (`) Grave accent 
        Key_Apostrophe,         // (')
        Key_Minus,              // (-) On main keyboard 
        Key_Equals,             // (=) On main keyboard 

        Key_NumKeys,
        Key_MaxValue = 0xFFFFFFFE,
        Key_None
    };

    //! State of a keyboard key.
    enum KeyState
    {
        Key_Up,                 //!< Key is up.
        Key_Pressed,            //!< Key was up and is now down.
        Key_Down,               //!< Key is down.
        Key_Released,           //!< Key was down and is now up.
        
        Key_NumStates,
        KeyState_MaxValue = 0xFFFFFFFF
    };

    /**
     *  Listener class used by other classes that wish to be notified of changes
     *  in the keyboard status.
     */
    class ENGINE_API Listener
    {
    public:
        //! Called when the given key is up.
        virtual void OnKeyUp( const Key& pKey );

        //! Called when the given key is pressed.
        virtual void OnKeyPressed( const Key& pKey );

        //! Called when the given key is down.
        virtual void OnKeyDown( const Key& pKey );

        //! Called when the given key is released.
        virtual void OnKeyReleased( const Key& pKey );
    };

public:
    //! Get the state of a given key.
    KeyState GetKeyState( const Key& pKey ) const;
    
    //! Return \btrue if the given key is down (or was pressed), \bfalse otherwise.
    Bool IsDown( const Key& pKey ) const;

    //! Return \btrue if the given key is up (or was released), \bfalse otherwise.
    Bool IsUp( const Key& pKey ) const;

    //! Return \btrue if the given key was pressed, \bfalse otherwise.
    Bool WasPressed( const Key& pKey ) const;

    //! Return \btrue if the given key was released, \bfalse otherwise.
    Bool WasReleased( const Key& pKey ) const;

    //! Return \true if one of the two Ctrl key is down (or was pressed).
    Bool IsCtrlDown() const;

    //! Return \true if one of the two Alt key is down (or was pressed).
    Bool IsAltDown() const;

    //! Return \true if one of the two Shift key is down (or was pressed).
    Bool IsShiftDown() const;

    //! Add a key listener that will be notified of a given key state.
    void AddKeyListener( Listener* pListener, const Key& pKey );

    //! Add a key listener that will be notified when a given key is in a given state.
    void AddKeyListener( Listener* pListener, const Key& pKey, const KeyState& pState );

    //! Remove a key listener from the list of listeners.
    void RemoveKeyListener( Listener* pListener, const Key& pKey );

    //! Remove a key listener from the list of listeners.
    void RemoveKeyListener( Listener* pListener, const Key& pKey, const KeyState& pState );

    //! Get the name of a key as a string.
    String GetKeyName( const Key& pKey ) const;

    //! Get the name of a state as a string.
    String GetStateName( const KeyState& pState ) const;
  
private:
    //! Private constructor.  Only the InputSubsystem is allowed to create a Keyboard object.
    Keyboard();

    //! Fire events (notify listeners of the changes in the mouse state).
    void FireEvents();

private:
    KeyState                    mKeysStates[Key_NumKeys];                       //!< State of each key.

    //! Key listeners list for each key possible state.
    std::list<Listener*>        mKeysListeners[Key_NumKeys][Key_NumStates];     
};


} // namespace Gamedesk


#endif  //  _KEYBOARD_H_
