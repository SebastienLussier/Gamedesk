/**
 *  @file       Window.h
 *  @brief      Main application class.
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
#ifndef     _WINDOW_H_
#define     _WINDOW_H_


#include "Maths/Vector2.h"


namespace Gamedesk {


/**
 *
 */
class ENGINE_API Window : public Object
{
    DECLARE_ABSTRACT_CLASS(Window,Object);

public:
    class ENGINE_API Listener
    {
    public:
        virtual void OnCreateWindow( const Window& pWindow );
        virtual void OnDestroyWindow( const Window& pWindow );
        virtual Bool OnCloseWindow( const Window& pWindow );
        virtual void OnMoveWindow( const Window& pWindow );
        virtual void OnResizeWindow( const Window& pWindow );
        virtual void OnMinimizeWindow( const Window& pWindow );
        virtual void OnMaximizeWindow( const Window& pWindow );
        virtual void OnMakeWindowFullscreen( const Window& pWindow );           
        virtual void OnWindowGotFocus( const Window& pWindow );
        virtual void OnWindowLostFocus( const Window& pWindow );
    };

public:
    Window()
    {
    }
    
    virtual ~Window()
    {
    }
    
    virtual void Init( const String& pTitle, Int32 pWidth, Int32 pHeight, Bool pVisible, Bool pFullscreen )
    {
        mTitle = pTitle;

        mSize.x = pWidth;
        mSize.y = pHeight;

        mIsVisible = pVisible;
        mIsFullscreen = pFullscreen;
    }

    virtual void Kill()
    {
    }

    virtual void Tick()
    {
    }

    void AddListener( Listener* pListener )
    {
        mListeners.push_back(pListener);
    }

    void RemoveListener( Listener* pListener )
    {
        List<Listener*>::iterator it = std::find( mListeners.begin(), mListeners.end(), pListener );

        if( it != mListeners.end() )
            mListeners.erase(it);
    }

    virtual Handle GetHandle() const = 0;

    virtual void SetPosition( const Vector2i& pPos ) = 0;
    virtual void SetSize( const Vector2i& pSize ) = 0;
    virtual void SetFocus() = 0;
    
    virtual void ShowMinimized() = 0;
    virtual void ShowMaximized() = 0;
    virtual void ShowNormal() = 0;
    virtual void ShowFullscreen() = 0;

    virtual Bool SupportFullScreen() = 0;
    
    const Vector2i& GetPosition() const
    {
        return mPosition;
    }

    const Vector2i& GetSize() const
    {
        return mSize;
    }

    virtual Vector2i GetClientSize() const = 0;
    virtual Vector2i GetClientPos() const = 0;

    Bool HasFocus() const
    {
        return mHasFocus;
    }

    Bool IsMinimized() const
    {
        return mIsMinimized;
    }

    Bool IsMaximized() const
    {
        return mIsMaximized;
    }

    Bool IsVisible() const
    {
        return mIsVisible;
    }

    Bool IsFullScreen() const
    {
        return mIsFullscreen;
    }    

protected:
    void OnCreate()
    {
    }
    
    void OnPaint()
    {
    }
    
    Bool OnClose()
    {
        Bool bShouldClose = true;
        for( List<Listener*>::iterator it = mListeners.begin(); it != mListeners.end() && bShouldClose; ++it )
            bShouldClose = (*it)->OnCloseWindow( *this );

        return bShouldClose;
    }
    
    void OnDestroy()
    {
    }

    void OnMove( Int32 pPosX, Int32 pPosY )
    {
        mPosition.x = pPosX;
        mPosition.y = pPosY;

        for( List<Listener*>::iterator it = mListeners.begin(); it != mListeners.end(); ++it )
            (*it)->OnMoveWindow( *this );
    }

    void OnResize( Int32 pWidth, Int32 pHeight )
    {
        mSize.x = pWidth;
        mSize.y = pHeight;

        for( List<Listener*>::iterator it = mListeners.begin(); it != mListeners.end(); ++it )
            (*it)->OnResizeWindow( *this );
    }

    void OnMinimize()
    {
        mIsMinimized = true;
        mIsMaximized = false;

        for( List<Listener*>::iterator it = mListeners.begin(); it != mListeners.end(); ++it )
            (*it)->OnMinimizeWindow( *this );
    }
    
    void OnMaximize()
    {
        mIsMinimized = true;
        mIsMaximized = false;

        for( List<Listener*>::iterator it = mListeners.begin(); it != mListeners.end(); ++it )
            (*it)->OnMaximizeWindow( *this );
    }

    void OnShow( Bool pShow )
    {
        mIsVisible = pShow;
    }
    
    void OnMakeFullscreen()
    {
        for( List<Listener*>::iterator it = mListeners.begin(); it != mListeners.end(); ++it )
            (*it)->OnMakeWindowFullscreen( *this );
    }
    
    void OnFocus( Bool pHasFocus )
    {
        mHasFocus = pHasFocus;
    }

protected:
    String          mTitle;             //!< Window title bar caption.
    Vector2i        mPosition;          //!< Screen position in pixels (relative to the upper left corner)
    Vector2i        mSize;              //!< Size in pixels
    Bool            mIsMinimized;       //!< Is minimized ?
    Bool            mIsMaximized;       //!< Is maximized ?    
    Bool            mIsVisible;         //!< Is visible ?
    Bool            mIsFullscreen;      //!< Is fullscreen ?
    Bool            mHasFocus;          //!< Has focus ?

    //! Listeners that will be notified of change in the window properties.
    List<Listener*> mListeners;
};


} // namespace Gamedesk


#endif  //  _WINDOW_H_
