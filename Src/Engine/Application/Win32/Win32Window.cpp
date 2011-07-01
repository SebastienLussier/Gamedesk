/**
 *  @file       Win32Window.cpp
 *  @brief      Win32 window class.
 *  @author     Sébastien Lussier.
 *  @date       20/11/04.
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
#include "Win32Window.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Win32Window);


WNDCLASS						Win32Window::mWindowClass;
Bool							Win32Window::mWindowClassInitialized = false;
std::map<HWND,Win32Window*>		Win32Window::mWindowsMap;


void Win32Window::InitWindowClass()
{
    if( mWindowClassInitialized )
        return;

    memset( &mWindowClass, 0, sizeof(mWindowClass) );
	
    // Fill all the info for our window class.
    mWindowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	mWindowClass.lpfnWndProc	= Win32Window::WindowProc;
	mWindowClass.hInstance		= (HINSTANCE)GetModuleHandle(NULL);
    mWindowClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	mWindowClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	mWindowClass.lpszMenuName	= MAKEINTRESOURCE(NULL);
	mWindowClass.lpszClassName	= "GAMEDESK_WINDOW_CLASS";
    // TODO: Icon -> LoadIcon(hInstance, MAKEINTRESOURCE(ICON));

	// Register our window class.
	GD_ASSERT( RegisterClass(&mWindowClass) );

    mWindowClassInitialized = true;
}

void Win32Window::Init( const String& pTitle, Int32 pWidth, Int32 pHeight, Bool pVisible, Bool pFullscreen )
{
    Bool result;

    Super::Init(pTitle, pWidth, pHeight, pVisible, pFullscreen);
    
    InitWindowClass();

    DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// Define Our Window Style
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style

    RECT rcPos;

    SetRect(&rcPos, 0, 0, mSize.x, mSize.y);
    result = AdjustWindowRectEx( &rcPos,
                                 windowStyle,
                                 GetMenu(mHWnd) != NULL,
                                 windowExtendedStyle ) == TRUE;

	// Create a window using our window class.
    mHWnd = CreateWindowEx( windowExtendedStyle,
                            mWindowClass.lpszClassName,
						    "",	 
						    (mIsFullscreen ?  (WS_POPUP | WS_VISIBLE) : WS_OVERLAPPEDWINDOW ),
						    CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            rcPos.right-rcPos.left,	    // Calculate Window Width
                            rcPos.bottom-rcPos.top,	    // Calculate Window Height
						    HWND_DESKTOP,
                            NULL,
                            (HINSTANCE)GetModuleHandle(NULL),
						    this );
    GD_ASSERT(mHWnd);

    result = SetWindowText( mHWnd, mTitle.c_str() ) == TRUE;
    GD_ASSERT(result);

    if( mIsVisible )
        ShowWindow( mHWnd, SW_SHOW );
}

void Win32Window::Init( HWND pWindowHandle )
{
    mHWnd = pWindowHandle;

    char buff[256];
    RECT rect;

    ::GetWindowText(mHWnd, buff, 256);
    ::GetWindowRect(mHWnd, &rect);

    Super::Init(buff, rect.right - rect.left, rect.bottom - rect.top, true, false);

	mWindowsMap[mHWnd] = this;
	mHijackedWindowProc = (WNDPROC)::SetWindowLong(mHWnd, GWL_WNDPROC, (long)WindowProc);
}

LRESULT CALLBACK Win32Window::WindowProc( HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam ) 
{
    static Win32Window* currentWindow = NULL;
    
    if( !currentWindow || currentWindow->mHWnd != hWnd )
    {
        std::map<HWND,Win32Window*>::iterator it = mWindowsMap.find(hWnd);
        if( it != mWindowsMap.end() )
            currentWindow = it->second;
    }

    if( currentWindow == NULL && iMsg != WM_NCCREATE )
        return DefWindowProc(hWnd, iMsg, wParam, lParam);

	bool bProcessed = false;

	switch(iMsg)
	{	
    // Sent prior to the WM_CREATE message when a window is first created.
    case WM_NCCREATE:
        currentWindow = (Win32Window*)((CREATESTRUCT*)lParam)->lpCreateParams;
        mWindowsMap[hWnd] = currentWindow;
        return 1;

    // Sent when an application requests that a window be created.
    case WM_CREATE: 
		currentWindow->OnCreate();
        bProcessed = true;
		break;

    // Paint the window's client area. 
	case WM_PAINT:
		currentWindow->OnPaint();
        break;

    // Sent after a window has been moved.
    case WM_MOVE:
		currentWindow->OnMove( (Int16)LOWORD(lParam), (Int16)HIWORD(lParam) );
        bProcessed = true;

    // Sent to a window after its size has changed.
    case WM_SIZE:
    {
        switch( wParam )
        {
        case SIZE_MINIMIZED:    
            currentWindow->OnMinimize();    
            break;
        case SIZE_MAXIMIZED:    
            currentWindow->OnMaximize();    
            break;
        case SIZE_MAXSHOW:      
            Core::DebugOut("SIZE_MAXSHOW\n");
            break;
        case SIZE_MAXHIDE:      
            Core::DebugOut("SIZE_MAXHIDE\n");
            break;
        case SIZE_RESTORED:     
            break;
        }                        

        currentWindow->OnResize( (Int16)LOWORD(lParam), (Int16)HIWORD(lParam) );
        bProcessed = true;
    }

    // Sent to both the window being activated and the window being deactivated.
    case WM_ACTIVATE:
	    currentWindow->OnFocus( LOWORD(wParam) != WA_INACTIVE );
        bProcessed = true;

    // Sent when the window is about to be hidden or shown.
    case WM_SHOWWINDOW:
        currentWindow->OnShow( wParam == TRUE );
        bProcessed = true;

    // Sent just before a window is destroyed.
    // Informs a window that its nonclient area is being destroyed.
    // Window is about to be destroyed, clean up window-specific data objects. 
	case WM_DESTROY: 
		currentWindow->OnDestroy();
        bProcessed = true;

    // Sent as a signal that a window or an application should terminate.
    case WM_CLOSE:
        if( !currentWindow->OnClose() )
            bProcessed = true;
    }

    // If the window proc was hijacked, always forward messages to the original WindowProc
	if( currentWindow->mHijackedWindowProc != NULL )
	{
		return CallWindowProc(currentWindow->mHijackedWindowProc, hWnd, iMsg, wParam, lParam);
	}
	else
	{
		// If processed, return 0, otherwise, send all the other messages to the default WindowProc.
		if( bProcessed )
			return 0;
		else 
			return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}
}


} // namespace Gamedesk
