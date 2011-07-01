/**
 *  @file       RenderWindow.h
 *  @brief      .
 *  @author     Sébastien Lussier.
 *  @date       20/01/03.
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
#ifndef     _RENDER_WINDOW_H_
#define     _RENDER_WINDOW_H_


#include "RenderTarget.h"
#include "Maths/Vector2.h"


namespace Gamedesk {


class ENGINE_API RenderWindow : public RenderTarget
{
    DECLARE_ABSTRACT_CLASS(RenderWindow,RenderTarget);

public:
    class WindowInfo
    {
    public:
        Handle      mWindowInternal;
        Handle      mWindowHandle;
        Vector2i    mSize;
        Bool        mFullscreen;
    };

public:
    virtual void Create( const WindowInfo& pWindowInfo, const RenderTarget::Format& pFormat );
    virtual void Destroy();
 
    virtual void SwapBuffers() = 0;

protected:
    RenderWindow();
    
private:
    WindowInfo  mWindowInfo;
};


} // namespace Gamedesk


#endif  //  _RENDER_WINDOW_H_
