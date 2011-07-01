/**
 *  @file       Mouse.cpp
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
#include "Mouse.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Mouse);


Mouse::Mouse()
{
    memset( mButtonsStates, Button_Up, sizeof(mButtonsStates) );
    memset( mAxis, 0, sizeof(mAxis) );
}

Mouse::ButtonState Mouse::GetState( const Mouse::Button& pButton ) const
{
    return mButtonsStates[pButton];
}

Bool Mouse::IsDown( const Mouse::Button& pButton ) const
{
    return mButtonsStates[pButton] == Button_Down || mButtonsStates[pButton] == Button_Pressed;
}

Bool Mouse::IsUp( const Mouse::Button& pButton ) const
{
    return mButtonsStates[pButton] == Button_Up || mButtonsStates[pButton] == Button_Released;
}

Bool Mouse::WasPressed( const Mouse::Button& pButton ) const
{
    return mButtonsStates[pButton] == Button_Pressed;
}

Bool Mouse::WasReleased( const Mouse::Button& pButton ) const
{
    return mButtonsStates[pButton] == Button_Released;
}

Int32 Mouse::GetRelX() const
{
    return mAxis[Axis_X];
}

Int32 Mouse::GetRelY() const
{
    return mAxis[Axis_Y];
}

Int32 Mouse::GetRelZ() const
{
    return mAxis[Axis_Z];
}

Int32 Mouse::GetRelAxis( const Mouse::Axis& pAxis ) const
{
    return mAxis[pAxis];
}

const Vector2i& Mouse::GetPos() const
{
    return mPosition;
}

void Mouse::GetPos( Int32& pAxisX, Int32& pAxisY ) const
{
    pAxisX = mPosition.x;
    pAxisY = mPosition.y;
}

// Mouse move listeners management.
void Mouse::AddMoveListener( Mouse::Listener* pListener )
{
    mMoveListeners.push_back( pListener );
}

void Mouse::RemoveMoveListener( Mouse::Listener* pListener )
{
    mMoveListeners.remove( pListener );
}

// Mouse button listeners management.
void Mouse::AddButtonListener( Mouse::Listener* pListener, const Mouse::Button& pButton )
{
    AddButtonListener( pListener, pButton, Button_Up );
    AddButtonListener( pListener, pButton, Button_Pressed );
    AddButtonListener( pListener, pButton, Button_Down );
    AddButtonListener( pListener, pButton, Button_Released );
}

void Mouse::AddButtonListener( Mouse::Listener* pListener, const Mouse::Button& pButton, const Mouse::ButtonState& pState )
{
    mButtonsListeners[pButton][pState].push_back( pListener );
}

void Mouse::RemoveButtonListener( Mouse::Listener* pListener, const Mouse::Button& pButton )
{
    RemoveButtonListener( pListener, pButton, Button_Up );
    RemoveButtonListener( pListener, pButton, Button_Pressed );
    RemoveButtonListener( pListener, pButton, Button_Down );
    RemoveButtonListener( pListener, pButton, Button_Released );
}

void Mouse::RemoveButtonListener( Mouse::Listener* pListener, const Mouse::Button& pButton, const Mouse::ButtonState& pState )
{
    mButtonsListeners[pButton][pState].remove( pListener );
}

void Mouse::FireEvents()
{
    std::list<Listener*>::const_iterator     itList;
    std::list<Listener*>::const_iterator     itListEnd;
    Button                                   button;
    ButtonState                              buttonState;

    // For each button.
    for( UInt32 iButton = 0; iButton < Button_NumButtons; iButton++ )
    {
        // Get the state of the current button.   
        button      = (Button)iButton;
        buttonState = mButtonsStates[button];

        // Get an iterator to the beginning and end of the list.
        itList    = mButtonsListeners[button][buttonState].begin();
        itListEnd = mButtonsListeners[button][buttonState].end();

        switch( buttonState )
        {
        case Button_Up:
            for( ; itList != itListEnd; ++itList )
                (*itList)->OnMouseButtonUp( button );
            break;

        case Button_Pressed:
            for( ; itList != itListEnd; ++itList )
                (*itList)->OnMouseButtonPressed( button );
            break;

        case Button_Down:
            for( ; itList != itListEnd; ++itList )
                (*itList)->OnMouseButtonDown( button );
            break;

        case Button_Released:
            for( ; itList != itListEnd; ++itList )
                (*itList)->OnMouseButtonReleased( button );
            break;

		default:
			break;
        }        
    }

    // If axis X or Y changed, fire OnMove
    if( mAxis[Axis_X] != 0 || mAxis[Axis_Y] != 0 )
    {
        for( std::list<Listener*>::const_iterator itList = mMoveListeners.begin(); itList != mMoveListeners.end(); ++itList )
            (*itList)->OnMouseMove( mAxis[Axis_X], mAxis[Axis_Y] );
    }
}

void Mouse::Listener::OnMouseMove            ( Int32 /*pRelX*/, Int32 /*pRelY*/ ) {}
void Mouse::Listener::OnMouseButtonUp        ( const Mouse::Button& /*pButton*/ ) {}
void Mouse::Listener::OnMouseButtonPressed   ( const Mouse::Button& /*pButton*/ ) {}
void Mouse::Listener::OnMouseButtonDown      ( const Mouse::Button& /*pButton*/ ) {}
void Mouse::Listener::OnMouseButtonReleased  ( const Mouse::Button& /*pButton*/ ) {}

String Mouse::GetButtonName( const Button& pButton ) const
{
    switch( pButton )
    {
        case Button_Left:       return "Left";
        case Button_Right:      return "Right";
        case Button_Middle:     return "Middle";
        case Button_3:          return "3";
        case Button_4:          return "4";
        case Button_5:          return "5";
        case Button_6:          return "6";
        case Button_7:          return "7";
        default:                return "Unknown";
    }
}

String Mouse::GetStateName( const ButtonState& pState ) const
{
    switch( pState )
    {
        case Button_Up:         return "Up";
        case Button_Pressed:    return "Pressed";
        case Button_Down:       return "Down";
        case Button_Released:   return "Released";
        default:                return "Unknown";
    }
}

String Mouse::GetAxisName( const Axis& pAxis ) const
{
    switch( pAxis )
    {
        case Axis_X:            return "X";
        case Axis_Y:            return "Y";
        case Axis_Z:            return "Z";
        default:                return "Unknown";
    }
}


} // namespace Gamedesk
