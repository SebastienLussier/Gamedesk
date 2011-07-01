/**
 *  @file       Win32OGLRenderWindowDll.cpp
 *  @brief      Win32 OpenGL window..
 *  @author     Sébastien Lussier.
 *  @date       23/07/04.
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
#include "Win32OGLRenderWindow.h"
#include "Module/ModuleManager.h"
#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Application/Window.h"


namespace Gamedesk {
	
	
IMPLEMENT_MODULE(Win32OGLRenderWindow);
IMPLEMENT_CLASS(Win32OGLRenderWindow);


Win32OGLRenderWindow::Win32OGLRenderWindow()
    : mRC(NULL)
    , mDC(NULL)
    , mWnd(NULL)
    , mInstance(NULL)
{
}

Win32OGLRenderWindow::~Win32OGLRenderWindow()
{
    Destroy();
}

void Win32OGLRenderWindow::Create( const WindowInfo& pWindowInfo, const RenderTarget::Format& pFormat )
{   
    Super::Create( pWindowInfo, pFormat );
    
    mWnd = (HWND)pWindowInfo.mWindowHandle;
    mDC  = GetDC( mWnd );

    if( pWindowInfo.mFullscreen )
    {
        CreateFullscreen( pFormat );
    }
    else
    {
        CreateWindowed( pFormat );
    }
}

void Win32OGLRenderWindow::CreateWindowed( const RenderTarget::Format& pFormat )
{
    PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;

    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

    if( pFormat.mDoubleBuffer )
        pfd.dwFlags |= PFD_DOUBLEBUFFER;
    
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
	pfd.cDepthBits = 16;

	GLuint iPixelFormat = ChoosePixelFormat( mDC, &pfd );

	if( iPixelFormat != 0 )
	{
		PIXELFORMATDESCRIPTOR bestMatch_pfd;
		DescribePixelFormat( mDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd );

		// TO DO: Double-check  the closet match pfd for anything unacceptable...

		if( bestMatch_pfd.cDepthBits < pfd.cDepthBits )
		{
			// POTENTIAL PROBLEM: We need at least a 16-bit z-buffer!
			return;
		}

		if( SetPixelFormat( mDC, iPixelFormat, &pfd) == FALSE )
		{
			TCHAR szBuf[80]; 
            LPVOID lpMsgBuf;
            DWORD dw = GetLastError(); 

            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                dw,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lpMsgBuf,
                0, NULL );

            wsprintf(szBuf, 
                "SetPixelFormat failed with error %d: %s", dw, lpMsgBuf); 
         
            MessageBox(NULL, szBuf, "Error", MB_OK); 

            LocalFree(lpMsgBuf);
		}
	}
	else
	{
		TCHAR szBuf[80]; 
        LPVOID lpMsgBuf;
        DWORD dw = GetLastError(); 

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL );

        wsprintf(szBuf, 
            "ChoosePixelFormat failed with error %d: %s", dw, lpMsgBuf); 
     
        MessageBox(NULL, szBuf, "Error", MB_OK); 

        LocalFree(lpMsgBuf);
		return;
	}

	mRC = wglCreateContext( mDC );
}


void Win32OGLRenderWindow::CreateFullscreen( const RenderTarget::Format& /*pFormat*/ )
{
    // Cache the current display mode so we can switch back when done.
	EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &mOldDevMode );

	//
	// Enumerate Device modes...
	//
	int nMode = 0;
	DEVMODE devMode;
	bool bDesiredDevModeFound = false;

	while( EnumDisplaySettings( NULL, nMode++, &devMode ) )
	{
		// Does this device mode support a 640 x 480 setting?
		if( devMode.dmPelsWidth  != mWidth || devMode.dmPelsHeight != mHeight )
			continue;

		// Does this device mode support 32-bit color?
		if( devMode.dmBitsPerPel != 32 )
			continue;

		// Does this device mode support a refresh rate of 75 MHz?
		if( devMode.dmDisplayFrequency < 60 )
			continue;

		// We found a match, but can it be set without rebooting?
		if( ChangeDisplaySettings( &devMode, CDS_TEST ) == DISP_CHANGE_SUCCESSFUL )
		{
			bDesiredDevModeFound = true;
			break;
		}
	}

	if( bDesiredDevModeFound == false )
	{
		// TO DO: Handle lack of support for desired mode...
		return;
	}

	//
	// Verify hardware support by enumerating pixel formats...
	//
   /* PIXELFORMATDESCRIPTOR pfd;

	int nTotalFormats = DescribePixelFormat( mDC, 1, sizeof(pfd), NULL );

	int nPixelFormat;

	for( nPixelFormat = 1; nPixelFormat <= nTotalFormats; ++nPixelFormat )
	{
		if( DescribePixelFormat( mDC, nPixelFormat, sizeof(pfd), &pfd ) == 0 )
		{
			DWORD dwErrorCode = GetLastError();
			// TO DO: Respond to failure of DescribePixelFormat
			return;
		}

		if( !(pfd.dwFlags & PFD_SUPPORT_OPENGL) )
			continue;

        if( !(pfd.dwFlags & PFD_DRAW_TO_WINDOW) )
            continue;

		if( !(pfd.dwFlags & PFD_DOUBLEBUFFER) )
			continue;

		if( pfd.iPixelType != PFD_TYPE_RGBA )
			continue;
		
		if( pfd.cColorBits != 32 )
			continue;

		if( pfd.cDepthBits != 32 )
			continue;

		// If we made it this far, we found a match!
		break;
	}*/

	//
	// Everything checks out - create the rendering context and 
	// switch the display settings with our new device mode...
	//
    PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		32,										                        // Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		8,																// No Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		32,																// 16Bit Z-Buffer (Depth Buffer)  
		32,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

    int nPixelFormat = ChoosePixelFormat( mDC, &pfd );				// Find A Compatible Pixel Format
	if( SetPixelFormat( mDC, nPixelFormat, &pfd ) == FALSE )
	{
        TCHAR szBuf[80]; 
        LPVOID lpMsgBuf;
        DWORD dw = GetLastError(); 

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL );

        wsprintf(szBuf, 
            "SetPixelFormat failed with error %d: %s", dw, lpMsgBuf); 
     
        MessageBox(NULL, szBuf, "Error", MB_OK); 

        LocalFree(lpMsgBuf);
 		return;
	}

	mRC = wglCreateContext( mDC );
    wglMakeCurrent( mDC, mRC );

    if( ChangeDisplaySettings( &devMode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
	{
		// TO DO: Respond to failure of ChangeDisplaySettings
		return;
	}
}

void Win32OGLRenderWindow::Destroy()
{
    // Are We In Fullscreen Mode?
    if( 0 )			            						
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

    // Do We Have A Rendering Context?
	if( mRC )											
	{
        // Are We Able To Release The DC And RC Contexts?
		if( !wglMakeCurrent(NULL,NULL) )				
		{
            ::MessageBox( NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}

        // Are We Able To Delete The RC?
		if (!wglDeleteContext(mRC))						
		{
            ::MessageBox( NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}
		mRC = NULL;										
	}

    // Are We Able To Release The DC
	if( mDC && !ReleaseDC(mWnd,mDC) )					
	{
		MessageBox( NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		mDC = NULL;										
	}
}

void Win32OGLRenderWindow::MakeCurrent()
{
    wglMakeCurrent( mDC, mRC );
}

void Win32OGLRenderWindow::SwapBuffers()
{
    ::SwapBuffers( mDC );
}


} // namespace Gamedesk
