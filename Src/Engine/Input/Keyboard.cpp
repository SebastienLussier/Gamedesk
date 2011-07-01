/**
 *  @file       Keyboard.cpp
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
#include "Keyboard.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Keyboard);


Keyboard::Keyboard()
{
    memset( mKeysStates, Key_Up, sizeof(KeyState)*Key_NumKeys );
}

Keyboard::KeyState Keyboard::GetKeyState( const Keyboard::Key& pKey ) const
{
 return mKeysStates[pKey];
}

Bool Keyboard::IsDown( const Keyboard::Key& pKey ) const
{
 return mKeysStates[pKey] == Key_Down || mKeysStates[pKey] == Key_Pressed;
}

Bool Keyboard::IsUp( const Keyboard::Key& pKey ) const
{
 return mKeysStates[pKey] == Key_Up || mKeysStates[pKey] == Key_Released;
}

Bool Keyboard::WasPressed( const Keyboard::Key& pKey ) const
{
 return mKeysStates[pKey] == Key_Pressed;
}

Bool Keyboard::WasReleased( const Keyboard::Key& pKey ) const
{
 return mKeysStates[pKey] == Key_Released;
}

Bool Keyboard::IsCtrlDown() const
{
 return IsDown( Key_LeftCtrl ) || IsDown( Key_RightCtrl );
}

Bool Keyboard::IsAltDown() const
{
 return IsDown( Key_LeftAlt ) || IsDown( Key_RightAlt );
}

Bool Keyboard::IsShiftDown() const
{
 return IsDown( Key_LeftShift ) || IsDown( Key_RightShift );
}

void Keyboard::AddKeyListener( Keyboard::Listener* pListener, const Key& pKey )
{
    AddKeyListener( pListener, pKey, Key_Up );
    AddKeyListener( pListener, pKey, Key_Pressed );
    AddKeyListener( pListener, pKey, Key_Down );
    AddKeyListener( pListener, pKey, Key_Released );
}

void Keyboard::AddKeyListener( Keyboard::Listener* pListener, const Key& pKey, const KeyState& pState )
{
    mKeysListeners[pKey][pState].push_back( pListener );
}

void Keyboard::RemoveKeyListener( Keyboard::Listener* pListener, const Key& pKey )
{
    RemoveKeyListener( pListener, pKey, Key_Up );
    RemoveKeyListener( pListener, pKey, Key_Pressed );
    RemoveKeyListener( pListener, pKey, Key_Down );
    RemoveKeyListener( pListener, pKey, Key_Released );
}

void Keyboard::RemoveKeyListener( Keyboard::Listener* pListener, const Key& pKey, const KeyState& pState )
{
    mKeysListeners[pKey][pState].remove( pListener );
}

void Keyboard::FireEvents()
{
    std::list<Listener*>::const_iterator     itList;
    std::list<Listener*>::const_iterator     itListEnd;
    Key                                      key;
    KeyState                                 keyState;

    // For each key.
    for( UInt32 iKey = 0; iKey < Key_NumKeys; iKey++ )
    {
        // Get the state of the current key.   
        key      = (Key)iKey;
        keyState = mKeysStates[key];

        // Get an iterator to the beginning and end of the list.
        itList    = mKeysListeners[key][keyState].begin();
        itListEnd = mKeysListeners[key][keyState].end();

        switch( keyState )
        {
        case Key_Up:
            for( ; itList != itListEnd; ++itList )
                (*itList)->OnKeyUp( key );
            break;

        case Key_Pressed:
            for( ; itList != itListEnd; ++itList )
                (*itList)->OnKeyPressed( key );
            break;

        case Key_Down:
            for( ; itList != itListEnd; ++itList )
                (*itList)->OnKeyDown( key );
            break;

        case Key_Released:
            for( ; itList != itListEnd; ++itList )
                (*itList)->OnKeyReleased( key );
            break;

		default:
			break;
        }        
    }
}

void Keyboard::Listener::OnKeyUp        ( const Keyboard::Key& /*pKey*/ ) {}
void Keyboard::Listener::OnKeyPressed   ( const Keyboard::Key& /*pKey*/ ) {}
void Keyboard::Listener::OnKeyDown      ( const Keyboard::Key& /*pKey*/ ) {}
void Keyboard::Listener::OnKeyReleased  ( const Keyboard::Key& /*pKey*/ ) {}

String Keyboard::GetKeyName( const Key& pKey ) const
{
    switch( pKey )
    {
        case Key_0:             return "0";
        case Key_1:             return "1";
        case Key_2:             return "2";
        case Key_3:             return "3";
        case Key_4:             return "4";
        case Key_5:             return "5";
        case Key_6:             return "6";
        case Key_7:             return "7";
        case Key_8:             return "8";
        case Key_9:             return "9";
        case Key_A:             return "A";
        case Key_B:             return "B";
        case Key_C:             return "C";
        case Key_D:             return "D";
        case Key_E:             return "E";
        case Key_F:             return "F";
        case Key_G:             return "G";
        case Key_H:             return "H";
        case Key_I:             return "I";
        case Key_J:             return "J";
        case Key_K:             return "K";
        case Key_L:             return "L";
        case Key_M:             return "M";    
        case Key_N:             return "N";
        case Key_O:             return "O";
        case Key_P:             return "P";
        case Key_Q:             return "Q";
        case Key_R:             return "R";
        case Key_S:             return "S";
        case Key_T:             return "T";
        case Key_U:             return "U";
        case Key_V:             return "V";
        case Key_W:             return "W";
        case Key_X:             return "X";
        case Key_Y:             return "Y";
        case Key_Z:             return "Z";

        case Key_F1:            return "F1";
        case Key_F2:            return "F2";
        case Key_F3:            return "F3";
        case Key_F4:            return "F4";
        case Key_F5:            return "F5";
        case Key_F6:            return "F6";
        case Key_F7:            return "F7";
        case Key_F8:            return "F8";
        case Key_F9:            return "F9";
        case Key_F10:           return "F10";
        case Key_F11:           return "F11";
        case Key_F12:           return "F12";
        case Key_F13:           return "F13";
        case Key_F14:           return "F14";
        case Key_F15:           return "F15";

        case Key_UpArrow:       return "Up Arrow";
        case Key_DownArrow:     return "Down Arrow";
        case Key_LeftArrow:     return "Left Arrow";
        case Key_RightArrow:    return "Right Arrow";

        case Key_Home:          return "Home";
        case Key_End:           return "End";
        case Key_PageUp:        return "Page Up";
        case Key_PageDown:      return "Page Down";
        case Key_Insert:        return "Insert";
        case Key_Delete:        return "Delete";

        case Key_Escape:        return "Escape";
        
        case Key_NumLock:       return "NumLock";
        case Key_NumPad0:       return "NumPad 0";
        case Key_NumPad1:       return "NumPad 1";
        case Key_NumPad2:       return "NumPad 2";
        case Key_NumPad3:       return "NumPad 3";
        case Key_NumPad4:       return "NumPad 4";
        case Key_NumPad5:       return "NumPad 5";
        case Key_NumPad6:       return "NumPad 6";
        case Key_NumPad7:       return "NumPad 7";
        case Key_NumPad8:       return "NumPad 8";
        case Key_NumPad9:       return "NumPad 9";
        case Key_NumPadEnter:   return "NumPad Enter";
        case Key_NumSubtract:   return "NumPad -";
        case Key_NumAdd:        return "NumPad +";
        case Key_NumMultiply:   return "NumPad *";
        case Key_NumDivide:     return "NumPad /";
        case Key_NumPoint:      return "NumPad .";

        case Key_LeftBracket:   return "[";
        case Key_LeftCtrl:      return "Left Ctrl";
        case Key_LeftAlt:       return "Left Alt";
        case Key_LeftShift:     return "Left Shift";
        case Key_LeftWin:       return "Left Windows Key";

        case Key_RightBracket:  return "]";
        case Key_RightCtrl:     return "Right Ctrl";
        case Key_RightAlt:      return "Right Alt";
        case Key_RightShift:    return "Right Shift";
        case Key_RightWin:      return "Right Windows Key";

        case Key_PrintScreen:   return "Print Screen";
        case Key_ScrollLock:    return "Scroll Lock";
        case Key_Pause:         return "Pause";
        
        case Key_Spacebar:      return "Spacebar";
        case Key_Backspace:     return "Backspace";
        case Key_Enter:         return "Enter";
        case Key_Tab:           return "Tab";

        case Key_Apps:          return "Application Key";
        case Key_CapsLock:      return "Caps Lock";

        case Key_Slash:         return "/";
        case Key_Backslash:     return "\\";
        case Key_Comma:         return ",";
        case Key_Semicolon:     return ";";
        case Key_Period:        return ".";
        case Key_Grave:         return "`";
        case Key_Apostrophe:    return "'";
        case Key_Minus:         return "-";
        case Key_Equals:        return "=";
        default:                return "Unknown";
    }
}

String Keyboard::GetStateName( const KeyState& pState ) const
{
    switch( pState )
    {
        case Key_Up:            return "Up";
        case Key_Pressed:       return "Pressed";
        case Key_Down:          return "Down";
        case Key_Released:      return "Released";
        default:                return "Unknown";
    }
}


} // namespace Gamedesk
