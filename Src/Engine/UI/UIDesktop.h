/**
*  @file       UIDesktop.h
*  @brief	   Top level UIElement.
*  @author     Sébastien Lussier.
*  @date       11/07/05.
*/
/*
*  Copyright (C) 2005 Gamedesk
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
#ifndef     _UI_DESKTOP_H_
#define     _UI_DESKTOP_H_


#include "UIWidget.h"
#include "UIStyle.h"

#include "Input/Mouse.h"
#include "Input/Keyboard.h"


namespace Gamedesk {


class UIDesktop : public UIWidget,
                  public Mouse::Listener
{
    DECLARE_UI_CLASS(UIDesktop, UIWidget);

public:
    UIDesktop();
    virtual ~UIDesktop();
    
    void UpdateLayout();
    void Draw();

    UIStyle& GetStyle();
    const UIStyle& GetStyle() const;

    static UIDesktop* Instance()
    {
        static UIDesktop instance;
        return &instance;
    }

    UIWidget* WidgetAt( const UIPoint& pPos );
    const UIWidget* WidgetAt( const UIPoint& pPos ) const;

private:
    // Mouse listener
    void OnMouseMove( Int32 pRelX, Int32 pRelY );
    void OnMouseButtonPressed( const Mouse::Button& pButton );
    void OnMouseButtonReleased( const Mouse::Button& pButton );

private:
    virtual void Paint();

private:
    UIStyle*    mStyle;
    UIPoint     mMousePosition;
};


} // namespace Gamedesk


#endif  //  _UI_DESKTOP_H_
