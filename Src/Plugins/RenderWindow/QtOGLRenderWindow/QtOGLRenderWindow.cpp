/**
 *  @file       QtOGLRenderWindow.cpp
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
#include "QtOGLRenderWindow.h"
#include "Module/ModuleManager.h"
#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"


namespace Gamedesk {
	
	
IMPLEMENT_MODULE(QtOGLRenderWindow);
IMPLEMENT_CLASS(QtOGLRenderWindow);


QtOGLRenderWindow::QtOGLRenderWindow() :
    mContainerWnd(NULL),
    mGLWidget(NULL),
    mLayout(NULL)
{
}

QtOGLRenderWindow::~QtOGLRenderWindow()
{
    Destroy();
}

void QtOGLRenderWindow::Create( const WindowInfo& pWindowInfo, const RenderTarget::Format& pFormat )
{   
    Super::Create( pWindowInfo, pFormat );

    // Init options
    QGL::FormatOptions oglFormatOptions = 0;
    if( pFormat.mAlpha )    
        oglFormatOptions |= QGL::AlphaChannel;
    if( pFormat.mDepth )    
        oglFormatOptions |= QGL::DepthBuffer;
    if( pFormat.mStencil )    
        oglFormatOptions |= QGL::StencilBuffer;
    if( pFormat.mStereo )    
        oglFormatOptions |= QGL::StereoBuffers;
    if( pFormat.mDoubleBuffer )    
        oglFormatOptions |= QGL::DoubleBuffer;

    // Create base item for sharing the contexts
    static QGLWidget* ShareContextWidget = new QGLWidget(new QGLContext(QGLFormat( oglFormatOptions )));

    mContainerWnd = (QWidget*)pWindowInfo.mWindowInternal;

    mLayout  = new QVBoxLayout( mContainerWnd );
    
    // Create a widget that share it's context with all other widgets
    mGLWidget = new QGLWidget( mContainerWnd, ShareContextWidget );

    GD_ASSERT( mGLWidget->isSharing() );

    mLayout->addWidget( mGLWidget );
    mLayout->setMargin(0);
    mLayout->setSpacing(0);

    mWidth = mGLWidget->width();
    mHeight = mGLWidget->height();    
}

void QtOGLRenderWindow::Destroy()
{
    Super::Destroy();

    if( mGLWidget )
        mGLWidget->close();
    
    mContainerWnd   = NULL;
    mGLWidget       = NULL;
    mLayout         = NULL;
}

void QtOGLRenderWindow::MakeCurrent()
{
    mWidth = mGLWidget->width();
    mHeight = mGLWidget->height();

    mGLWidget->makeCurrent();
}

void QtOGLRenderWindow::SwapBuffers()
{
    mGLWidget->swapBuffers();
}


} // namespace Gamedesk
