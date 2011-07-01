/**
 *  @file       Win32Window.h
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
#ifndef     _WIN32_WINDOW_H_
#define     _WIN32_WINDOW_H_


#include "Application/Window.h"


namespace Gamedesk {


class ENGINE_API Win32Window : public Window
{
    DECLARE_CLASS(Win32Window,Window);

public:
    Win32Window() 
		: mHWnd(0)
		, mHijackedWindowProc(0)
	{
    }

    virtual ~Win32Window()
    {
    }

    virtual void Init( const String& pTitle, Int32 pWidth, Int32 pHeight, Bool pVisible, Bool pFullscreen );
    virtual void Init( HWND pWindowHandle );

    virtual void Kill()
    {
        PostQuitMessage(0);
        Super::Kill();
    }

    void Tick()
    {
        Super::Tick();

        MSG msg;

        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	    { 
		    TranslateMessage( &msg );
		    DispatchMessage( &msg ); 	// Send message to the WindowProc.
	    }
	    /*else if( !mHasFocus )
        {
	        WaitMessage();	            // Wait until we receive another event.
        }*/
    }

    virtual Handle GetHandle() const
    {
        return (Handle)mHWnd;
    }

    virtual void SetPosition( const Vector2i& pPos )
    {
        Bool result = SetWindowPos( mHWnd, HWND_TOP, pPos.x, pPos.y, mSize.x, mSize.y, SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER ) == TRUE;
        GD_ASSERT( result );
    }

    virtual void SetSize( const Vector2i& pSize )
    {
        Bool result = SetWindowPos( mHWnd, HWND_TOP, mPosition.x, mPosition.y, pSize.x, pSize.y, SWP_NOOWNERZORDER|SWP_NOMOVE|SWP_NOZORDER ) == TRUE;
        GD_ASSERT( result );
    }

    Vector2i GetClientSize() const
    {
        Vector2i size;
        RECT rect;

        GetClientRect( mHWnd, &rect );
        size.x = rect.right - rect.left; 
        size.y = rect.bottom - rect.top;

        return size;
    }

    Vector2i GetClientPos() const
    {
        Vector2i   pos;
        WINDOWINFO pwi;

        pwi.cbSize = sizeof(WINDOWINFO);
        GetWindowInfo( mHWnd, &pwi );
        
        pos.x = pwi.rcClient.left; 
        pos.y = pwi.rcClient.top;

        return pos;
    }


    virtual void SetFocus()
    {
    }

    virtual void ShowMinimized()
    {
        if( !mIsMinimized )
            ShowWindow( mHWnd, SW_MINIMIZE );
    }
    
    virtual void ShowMaximized()
    {
        if( !mIsMaximized )
            ShowWindow( mHWnd, SW_MAXIMIZE );
    }

    virtual void ShowNormal()
    {
        if( mIsMinimized || mIsMaximized )
            ShowWindow( mHWnd, SW_RESTORE );
    }

    virtual void ShowFullscreen()
    {
        if( mIsFullscreen )
            return;
    }

    virtual Bool SupportFullScreen()
    {
        return true;
    }

private:
    static void InitWindowClass();
    static LRESULT CALLBACK WindowProc( HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam );

private:
    HWND		mHWnd;
	WNDPROC		mHijackedWindowProc;

    static WNDCLASS mWindowClass;
    static Bool     mWindowClassInitialized;

	static std::map<HWND,Win32Window*>  mWindowsMap;
};


} // namespace Gamedesk


#endif  //  _WIN32_WINDOW_H_
