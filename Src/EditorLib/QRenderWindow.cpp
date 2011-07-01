/**
 *  @file       QRenderWindow.cpp
 *  @brief      Qt rendering window using a renderwindow internally.
 *  @author     Sébastien Lussier.
 *  @date       12/12/03.
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
#include "EditorLib.h"
#include "QRenderWindow.h"
//Added by qt3to4:
#include <QTimerEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include "AVIEncoder.h"
#include "Graphic/RenderWindow.h"
#include "Graphic/Renderer.h"
#include "Config/ConfigFile.h"
#include "Module/ModuleManager.h"
#include "Graphic/GraphicSubsystem.h"
#include "System/SystemInfo.h"


Class*       QRenderWindow::mRenderWindowClass          = NULL;
const UInt32 QRenderWindow::REFRESH_FASTEST_POSSIBLE    = 0xFFFFFFFF;
const UInt32 QRenderWindow::DEFAULT_REFRESH_RATE        = 60;
const Double QRenderWindow::FPS_UPDATE_TIME             = 0.5;


QRenderWindow::QRenderWindow( QWidget* parent, RenderListener* pRenderListener ) : 
    QWidget( parent, 0 ),
    mRefreshRate(0),
    mRenderWindow(NULL),
    mRenderListener(pRenderListener),
    mFPS(0),
    mTime(0),
	mLastTime(0),
	mFrameCount(0)
{
    if( !mRenderWindowClass )
    {
        ConfigFile editorConfig( "editor.cfg" );
        editorConfig.Load();

        String renderWndClassName = editorConfig["RenderWindow"]["ClassName"];

        if( renderWndClassName.empty() )
            throw ConfigFileMissingValueException( "editor.cfg", "RenderWindow", "ClassName", Here );
    
        mRenderWindowClass = ModuleManager::Instance()->LoadClass( renderWndClassName.c_str() );
    }

    RenderWindow::WindowInfo windowInfo;
    windowInfo.mWindowInternal  = (Handle)this;
    windowInfo.mWindowHandle    = (Handle)winId();
    windowInfo.mSize.x          = width();
    windowInfo.mSize.y          = height();
    windowInfo.mFullscreen      = false;

    RenderTarget::Format targetFormat;
    targetFormat.mAlpha         = true;
    targetFormat.mDepth         = true;
    targetFormat.mDoubleBuffer  = true;
    targetFormat.mStencil       = true;
    targetFormat.mStereo        = false;

    mRenderWindow = Cast<RenderWindow>( mRenderWindowClass->AllocateNew() );
    mRenderWindow->Create( windowInfo, targetFormat );
    
    mAVIEncoder = GD_NEW(AVIEncoder, this, "EditorLib::Viewer::QRenderWindow")( this );
}

QRenderWindow::~QRenderWindow()
{
    GD_DELETE(mAVIEncoder);
    GD_DELETE(mRenderWindow);
}

void QRenderWindow::Init()
{
    SetRefreshRate( REFRESH_FASTEST_POSSIBLE );
}

void QRenderWindow::Kill()
{
    killTimer(mRefreshRateTimerID);
    mRenderListener = NULL;    
}

RenderWindow* QRenderWindow::GetRenderWindow()
{
    return mRenderWindow;
}

Bool QRenderWindow::StartMovie( const String& pFilename )
{
    mAVIEncoder->Start( pFilename, 25 );
    if( mAVIEncoder->IsRecording() )
        SetRefreshRate( 30 );

    return mAVIEncoder->IsRecording();
}

void QRenderWindow::StopMovie()
{
    mAVIEncoder->Stop();
    SetRefreshRate( REFRESH_FASTEST_POSSIBLE );
}

void QRenderWindow::SetRefreshRate( UInt32 pRefreshRate )
{
    if( pRefreshRate == mRefreshRate )
        return;

    killTimer(mRefreshRateTimerID);

    if( pRefreshRate == 0 )
        return;

    mRefreshRate = pRefreshRate;
    
    if( mRefreshRate == REFRESH_FASTEST_POSSIBLE )
        mRefreshRateTimerID = startTimer( 0 );
    else
        mRefreshRateTimerID = startTimer( 1000.0 / mRefreshRate );
}

void QRenderWindow::paintEvent( QPaintEvent* )
{
    Render();
}

void QRenderWindow::timerEvent( QTimerEvent * )
{
    Render();
}

void QRenderWindow::resizeEvent( QResizeEvent* pEvent )
{
    QWidget::resizeEvent( pEvent );
    mRenderWindow->Resize( pEvent->size().width(), pEvent->size().height() );
}

void QRenderWindow::destroy( bool pDestroyWindow, bool pDestroySubWindows )
{
    QWidget::destroy( pDestroyWindow, pDestroySubWindows );

    killTimer(mRefreshRateTimerID);

    if( mRenderWindow )
        mRenderWindow->Destroy();
}

void QRenderWindow::Render()
{
    // If there is no listener, ignore rendering request...
    if( !mRenderListener )
        return;

    // Wait for the graphic subsystem to be created before rendering!
    if( !GraphicSubsystem::Instance() )
        return;

    UpdateFPS();
 
    GraphicSubsystem::Instance()->GetRenderer()->SetRenderTarget( mRenderWindow );
    
    mRenderListener->Render();
    
    mRenderWindow->SwapBuffers();    

    if( mAVIEncoder->IsRecording() )
        mAVIEncoder->TakeShot();
}

void QRenderWindow::UpdateFPS()
{    
    mTime = SystemInfo::Instance()->GetSeconds();
	mFrameCount++;
		
    if( mTime - mLastTime > FPS_UPDATE_TIME )
	{
		mFPS        = mFrameCount / (mTime-mLastTime);
		mLastTime   = mTime;
		mFrameCount = 0;
	}
}

Float QRenderWindow::GetFPS()
{
    return mFPS;
}

void QRenderWindow::MakeCurrent()
{
	mRenderWindow->MakeCurrent();
}

