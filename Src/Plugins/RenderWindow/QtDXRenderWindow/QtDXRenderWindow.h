/**
 *  @file       QtDXRenderWindow.h
 *  @brief      Qt DirectX render window for gamedesk.
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
#ifndef     _QTDXRENDERWINDOW_H_
#define     _QTDXRENDERWINDOW_H_


#ifndef QTDXRENDERWINDOW_API
#define QTDXRENDERWINDOW_API DLL_IMPORT
#endif


// TODO: Include headers from other libraries here.
#include <d3d9.h>
#include <qwidget.h>
#include <qlayout.h>
#include <q3frame.h>
//Added by qt3to4:
#include <QVBoxLayout>


#include "Engine.h"
#include "DX9Graphic.h"


namespace Gamedesk {


class QTDXRENDERWINDOW_API QtDXRenderWindow : public RenderWindow, public D3DObject
{
    DECLARE_CLASS(QtDXRenderWindow,RenderWindow);

public:
    QtDXRenderWindow();
    virtual ~QtDXRenderWindow();

    virtual void Create( void* pWindow );
    virtual void Destroy();

    virtual HWND GetHWnd() const;
    virtual HWND GetRootHWnd() const;

    virtual void MakeCurrent();
    virtual void SwapBuffers();

    virtual void Resize( UInt32 pWidth, UInt32 pHeight );

public:
    virtual void D3DRestore();
    virtual void D3DRelease();

private:
    DX9Renderer*            mRenderer;

    LPDIRECT3DSWAPCHAIN9    mSwapChain;
    QWidget*                mContainerWnd;
    Q3Frame*                 mWidget;
    QVBoxLayout*            mLayout;
    HWND                    mHWND;
};


} // namespace Gamedesk


#endif  //  _QTDXRENDERWINDOW_H_
