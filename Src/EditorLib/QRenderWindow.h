/**
 *  @file       QRenderWindow.h
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
#ifndef     _QRENDER_WINDOW_
#define     _QRENDER_WINDOW_


#include "EditorLib.h"
//Added by qt3to4:
#include <QTimerEvent>
#include <QResizeEvent>
#include <QPaintEvent>


class RenderWindow;
class AVIEncoder;


class EDITORLIB_API RenderListener
{
public:
    virtual void Render() = 0;
};


class EDITORLIB_API QRenderWindow : public QWidget
{
public:
    QRenderWindow( QWidget* pParent, RenderListener* pRenderListener );
    virtual ~QRenderWindow();

    virtual void Init();
    virtual void Kill();

    void    SetRefreshRate( UInt32 pRefreshRate = DEFAULT_REFRESH_RATE );
    Float   GetFPS();
	void	MakeCurrent();

    RenderWindow* GetRenderWindow();

    Bool    StartMovie( const String& pName );
    void    StopMovie();
   
    static const UInt32     REFRESH_FASTEST_POSSIBLE;
    static const UInt32     DEFAULT_REFRESH_RATE;
    static const Double     FPS_UPDATE_TIME;
  
private:
    virtual void paintEvent( QPaintEvent* pEvent );
    virtual void resizeEvent( QResizeEvent* pEvent );
    virtual void destroy( bool pDestroyWindow = true, bool pDestroySubWindows = true );

    void UpdateFPS();
    void Render();
    void  timerEvent( QTimerEvent * );
  
private:
    QRenderWindow( const QRenderWindow& pOther );
    const QRenderWindow& operator = ( const QRenderWindow& pOther );

    UInt32          mRefreshRate;
    UInt32          mRefreshRateTimerID;
    
    RenderWindow*   mRenderWindow;
    static Class*   mRenderWindowClass;   
    RenderListener* mRenderListener;
    
    // FPS
    Float           mFPS;
    Double          mTime;
	Double          mLastTime;
	Int32           mFrameCount;

    AVIEncoder*     mAVIEncoder;
};


#endif  //  _QRENDER_WINDOW_
