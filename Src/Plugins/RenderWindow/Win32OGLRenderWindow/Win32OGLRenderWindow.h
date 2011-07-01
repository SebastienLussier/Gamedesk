/**
 *  @file       Win32OGLRenderWindowDll.h
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
#ifndef     _WIN32OGLRENDERWINDOW_DLL_H_
#define     _WIN32OGLRENDERWINDOW_DLL_H_


#ifndef WIN32OGLRENDERWINDOW_API
    #define WIN32OGLRENDERWINDOW_API DLL_IMPORT
#endif


#include <GL/glew.h>
#include "Engine.h"
#include "Graphic/RenderTarget/RenderWindow.h"


namespace Gamedesk {


class WIN32OGLRENDERWINDOW_API Win32OGLRenderWindow : public RenderWindow
{
    DECLARE_CLASS(Win32OGLRenderWindow,RenderWindow);

public:
    Win32OGLRenderWindow();
    virtual ~Win32OGLRenderWindow();

    virtual void Create( const WindowInfo& pWindowInfo, const RenderTarget::Format& pFormat );
    virtual void Destroy();

    virtual void MakeCurrent();
    virtual void SwapBuffers();

private:
    void CreateWindowed( const RenderTarget::Format& pFormat );
    void CreateFullscreen( const RenderTarget::Format& pFormat );

private:
    HGLRC       mRC;		    //!< Permanent Rendering Context.
    HDC         mDC;		    //!< Private GDI Device Context.
    HWND        mWnd;	    	//!< Holds Our Window Handle.
    HINSTANCE   mInstance;  	//!< Holds The Instance Of The Application.
    DEVMODE     mOldDevMode;    //!< Use this to restore device mode when exiting fullscreen.
};


} // namespace Gamedesk


#endif  //  _WIN32OGLRENDERWINDOW_DLL_H_
