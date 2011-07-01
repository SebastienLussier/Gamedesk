/**
 *  @file       QtOGLRenderWindow.h
 *  @brief      Qt render window for OpenGL.
 *  @author     Sébastien Lussier.
 *  @date       03/11/03.
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
#ifndef     _QTOGLRENDERWINDOW_H_
#define     _QTOGLRENDERWINDOW_H_


#ifndef QTOGLRENDERWINDOW_API
#define QTOGLRENDERWINDOW_API DLL_IMPORT
#endif

// GLEW...
#include <GL/glew.h>

// Qt...
#pragma warning( push )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4512 )
#include <QGl.h>
#include <QLayout.h>
#include <QVBoxLayout>
#pragma warning( push )


#include "Engine.h"
#include "Graphic/RenderWindow.h"


namespace Gamedesk {


class QTOGLRENDERWINDOW_API QtOGLRenderWindow : public RenderWindow
{
    DECLARE_CLASS(QtOGLRenderWindow,RenderWindow);

public:
    QtOGLRenderWindow();
    virtual ~QtOGLRenderWindow();

    void Create( const WindowInfo& pWindowInfo, const RenderTarget::Format& pFormat );
    void Destroy();

    void MakeCurrent();
    void SwapBuffers();

private:
    QWidget*        mContainerWnd;
    QGLWidget*      mGLWidget;
    QVBoxLayout*    mLayout;
};


} // namespace Gamedesk


#endif  //  _QTOGLRENDERWINDOW_H_
