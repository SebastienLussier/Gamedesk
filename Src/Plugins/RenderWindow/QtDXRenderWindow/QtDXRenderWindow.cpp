/**
 *  @file       QtDXRenderWindow.cpp
 *  @brief      Qt DirectX render window for gamedesk..
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
#include "QtDXRenderWindow.h"
//Added by qt3to4:
#include <QVBoxLayout>
#include <Q3Frame>


namespace Gamedesk {
	
	
IMPLEMENT_MODULE(QtDXRenderWindow);
IMPLEMENT_CLASS(QtDXRenderWindow);


QtDXRenderWindow::QtDXRenderWindow() :
    mRenderer(NULL),
    mSwapChain(NULL),
    mContainerWnd(NULL),
    mWidget(NULL),
    mLayout(NULL),
    mHWND(0)
{
}

QtDXRenderWindow::~QtDXRenderWindow()
{
    if( mSwapChain != NULL )
		mSwapChain->Release();
}

void QtDXRenderWindow::Create( void* pWindow )
{   
    Super::Create( pWindow );

    mContainerWnd = (QWidget*)pWindow;
    mLayout = new QVBoxLayout( mContainerWnd, 1 );
    mWidget = new Q3Frame( mContainerWnd );
    mLayout->addWidget( mWidget );
    mHWND = mWidget->winId();

    mWidth = mWidget->width();
    mHeight = mWidget->height();

    mRenderer = Cast<DX9Renderer>(GraphicSubsystem::Instance()->GetRenderer());
    mRenderer->Init();

    D3DRestore();
}

void QtDXRenderWindow::Destroy()
{
    Super::Destroy();

    D3DRelease();

    if( mWidget )
        mWidget->close( true );
    
    if( mLayout )
        GD_DELETE(mLayout);

    mContainerWnd   = NULL;
    mWidget       = NULL;
    mLayout         = NULL;   
}

void QtDXRenderWindow::D3DRestore()
{
    mSwapChain = mRenderer->RequestSwapChain( mWidth, mHeight );
}

void QtDXRenderWindow::D3DRelease()
{
    if( mSwapChain )
        mSwapChain->Release();

    mSwapChain = NULL;
}

HWND QtDXRenderWindow::GetHWnd() const
{
    return mHWND;
}

HWND QtDXRenderWindow::GetRootHWnd() const
{
    return mWidget->topLevelWidget()->winId();
}

void QtDXRenderWindow::MakeCurrent()
{
    HRESULT hResult = S_OK;
    LPDIRECT3DSURFACE9  backBuffer = NULL;

	hResult = mSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer );
    if( FAILED(hResult) )
        throw D3DException( hResult, Here );

    hResult = mRenderer->GetDevice()->SetRenderTarget( 0, backBuffer );
    if( FAILED(hResult) )
        throw D3DException( hResult, Here );

    backBuffer->Release();
}

void QtDXRenderWindow::SwapBuffers()
{
    HRESULT hResult = S_OK;
    hResult = mSwapChain->Present( NULL, NULL, mHWND, NULL, 0 );    
    if( FAILED(hResult) )
        throw D3DException( hResult, Here );
}

void QtDXRenderWindow::Resize( UInt32 pWidth, UInt32 pHeight )
{
    UInt32 oldWidth  = mWidth;
    UInt32 oldHeight = mHeight;

    Super::Resize( pWidth, pHeight );
    
    if( mWidth != oldWidth || mHeight != oldHeight )
    {
        mRenderer->D3DRelease();
        mRenderer->D3DRestore();
    }
}


} // namespace Gamedesk
