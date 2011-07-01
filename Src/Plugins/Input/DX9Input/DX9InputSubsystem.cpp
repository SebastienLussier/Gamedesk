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
#include "DX9Input.h"
#include "DX9InputSubsystem.h"
#include "DX9InputKeyTable.h"

#include "Application/Application.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(DX9InputSubsystem);


DX9InputSubsystem::DX9InputSubsystem() : 
    mDirectInput(NULL),
    mKeyboardDevice(NULL),
    mMouseDevice(NULL)
{
    // Reset last mouse state.
    mLastMouseState.lX = 0;
    mLastMouseState.lY = 0;
    mLastMouseState.lZ = 0;
    memset( &mLastMouseState.rgbButtons, 0, sizeof(mLastMouseState.rgbButtons) );
    
    // Reset las keyboard state.
    memset( &mLastKeyboardState, 0, sizeof(mLastKeyboardState) );

    InitDInputToGamedeskKeyTable();
}

void DX9InputSubsystem::Init()
{
    Super::Init();

    HRESULT hResult; 

    // Create the DirectInput object.
    hResult = DirectInput8Create( gDLLInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, NULL ); 
    if( FAILED(hResult) )
        throw DInputException(hResult,Here);

    // Create 2 basic devices, the mouse and the keyboard.
    InitKeyboard();
    InitMouse();    
}

void DX9InputSubsystem::InitKeyboard()
{
    HRESULT hResult;

    hResult = mDirectInput->CreateDevice( GUID_SysKeyboard, &mKeyboardDevice, NULL ); 
    if( FAILED(hResult) )
    {
        Kill();
        throw DInputException(hResult,Here);
    }

    hResult = mKeyboardDevice->SetDataFormat( &c_dfDIKeyboard ); 
    if( FAILED(hResult) )
    { 
        Kill();
        throw DInputException(hResult,Here);
    } 

    // Set the cooperative level for the keyboard
    hResult = mKeyboardDevice->SetCooperativeLevel( (HWND)Application::Instance()->GetWindow()->GetHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ); 
    if( FAILED(hResult) )
    { 
        Kill();
        throw DInputException(hResult,Here);
    } 

    if( mKeyboardDevice )
        mKeyboardDevice->Acquire();
}

void DX9InputSubsystem::InitMouse()
{
    HRESULT hResult;

    hResult = mDirectInput->CreateDevice( GUID_SysMouse, &mMouseDevice, NULL ); 
    if( FAILED(hResult) )
    {
        Kill();
        throw DInputException(hResult,Here);
    }

    hResult = mMouseDevice->SetDataFormat( &c_dfDIMouse2 ); 
    if( FAILED(hResult) )
    { 
        Kill();
        throw DInputException(hResult,Here);
    } 

    // Set the cooperative level for the mouse
    hResult = mMouseDevice->SetCooperativeLevel( (HWND)Application::Instance()->GetWindow()->GetHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ); 
    if( FAILED(hResult) )
    { 
        Kill();
        throw DInputException(hResult,Here);
    } 

    mMouseDevice->Acquire();
}

void DX9InputSubsystem::Kill()
{
    if( mKeyboardDevice )
    {
        mKeyboardDevice->Unacquire();
        mKeyboardDevice->Release();
        mKeyboardDevice = NULL;
    }

    if( mMouseDevice )
    {
        mMouseDevice->Unacquire();
        mMouseDevice->Release();
        mMouseDevice = NULL;
    }

    if( mDirectInput )
    {
        mDirectInput->Release();
        mDirectInput = NULL;
    }

    Super::Kill();
}

void DX9InputSubsystem::Update()
{
    UpdateMouse();
    UpdateKeyboard();

    FireEvents();
}

void DX9InputSubsystem::UpdateMouse()
{
    HRESULT         hResult;
    DIMOUSESTATE2   mouseState;     // DirectInput mouse state structure

    // Get mouse state
    memset( &mouseState, 0, sizeof(mouseState) );
    hResult = mMouseDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &mouseState );
    if( FAILED(hResult) ) 
    {
        // DirectInput may be telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done.
        // We just re-acquire and try again.
        
        // If input is lost then acquire and keep trying 
        hResult = mMouseDevice->Acquire();
        while( hResult == DIERR_INPUTLOST ) 
            hResult = mMouseDevice->Acquire();

        // Update the dialog text 
        if( hResult == DIERR_OTHERAPPHASPRIO || hResult == DIERR_NOTACQUIRED ) 
        {
            // #TODO: Clear input here ?
        }
         
        // hResult may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
    }

    // Store new buttons states
    for( Int32 iButton = 0; iButton < Mouse::Button_NumButtons; iButton++ )
    {
        if( (mouseState.rgbButtons[iButton] & 0x80) )
        {
            if( mLastMouseState.rgbButtons[iButton] & 0x80 )
                SetMouseButtonState( (Mouse::Button)iButton, Mouse::Button_Down );
            else
                SetMouseButtonState( (Mouse::Button)iButton, Mouse::Button_Pressed );
        }
        else
        {
            if( mLastMouseState.rgbButtons[iButton] & 0x80 )
                SetMouseButtonState( (Mouse::Button)iButton, Mouse::Button_Released );
            else
                SetMouseButtonState( (Mouse::Button)iButton, Mouse::Button_Up );
        }
    }

    // Update mouse relative mouvement.
    SetMouseAxisValue( Mouse::Axis_X, mouseState.lX );
    SetMouseAxisValue( Mouse::Axis_Y, mouseState.lY );
    SetMouseAxisValue( Mouse::Axis_Z, mouseState.lZ );

    POINT pos;
    GetCursorPos( &pos );
    SetMousePosition( pos.x, pos.y );

    // Now make this new state the last obtained state.
    memcpy( &mLastMouseState, &mouseState, sizeof(mouseState) );
}

void DX9InputSubsystem::UpdateKeyboard()
{
    HRESULT         hResult;
    static Byte     keyState[DI_KEY_ARRAY_SIZE];  // DirectInput keyboard state buffer 

    // Get keyboard state
    memset( keyState, 0, DI_KEY_ARRAY_SIZE );
    hResult = mKeyboardDevice->GetDeviceState( sizeof(keyState), keyState );
    if( FAILED(hResult) ) 
    {
        // DirectInput may be telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done.
        // We just re-acquire and try again.
        
        // If input is lost then acquire and keep trying 
        hResult = mKeyboardDevice->Acquire();
        while( hResult == DIERR_INPUTLOST ) 
            hResult = mKeyboardDevice->Acquire();

        if( hResult == DIERR_OTHERAPPHASPRIO || hResult == DIERR_NOTACQUIRED ) 
        {
            // #TODO: Clear input here ?
        }

        // hResult may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
    }    

    Keyboard::Key currentKey;
    for( Int32 iKey = 0; iKey < DI_KEY_ARRAY_SIZE; iKey++ )
    {
        // Ignore unused keys
        if( gDInputToGamedeskKeyTable[iKey] == -1 )
            continue;
        else 
            currentKey = (Keyboard::Key)gDInputToGamedeskKeyTable[iKey];

        if( keyState[iKey] & 0x80 )
        {
            if( mLastKeyboardState[iKey] & 0x80 )
                Super::SetKeyboardState( currentKey, Keyboard::Key_Down );
            else
                Super::SetKeyboardState( currentKey, Keyboard::Key_Pressed );
        }
        else
        {
            if( mLastKeyboardState[iKey] & 0x80 )
                Super::SetKeyboardState( currentKey, Keyboard::Key_Released );
            else
                Super::SetKeyboardState( currentKey, Keyboard::Key_Up );
        }
    }

    // Now make this new state the last obtained state.
    memcpy( &mLastKeyboardState, &keyState, sizeof(keyState) );
}

///////////////////////////////////////////////////////////////////////////////
// DInputException
DInputException::DInputException( HRESULT pResult, CodeLocation pHere ) : Exception(pHere)
{
    mMessage  = "(";
    mMessage += DXGetErrorString( pResult );
    mMessage += ") : ";
    mMessage += DXGetErrorDescription( pResult );
    mMessage += ".";
    DebugOut();
}


} // namespace Gamedesk
