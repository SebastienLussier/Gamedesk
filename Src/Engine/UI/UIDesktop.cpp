/**
*  @file       UIDesktop.cpp
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
#include "Engine.h"
#include "UIDesktop.h"

#include "UIDefaultStyle.h"
#include "Graphic/Renderer.h"

#include "Application/Application.h"
#include "Input/InputSubsystem.h"


namespace Gamedesk {
	
	
UIDesktop::UIDesktop()
{
    mStyle = GD_NEW(UIDefaultStyle, this, "Engine::UI::UIDesktop");

    InputSubsystem::Instance()->GetMouse().AddMoveListener( this );
    InputSubsystem::Instance()->GetMouse().AddButtonListener( this, Mouse::Button_Left );
    InputSubsystem::Instance()->GetMouse().AddButtonListener( this, Mouse::Button_Right );
    InputSubsystem::Instance()->GetMouse().AddButtonListener( this, Mouse::Button_Middle );
}

UIDesktop::~UIDesktop()
{
    GD_DELETE(mStyle);

    InputSubsystem::Instance()->GetMouse().RemoveMoveListener( this );
    InputSubsystem::Instance()->GetMouse().RemoveButtonListener( this, Mouse::Button_Left );
    InputSubsystem::Instance()->GetMouse().RemoveButtonListener( this, Mouse::Button_Right );
    InputSubsystem::Instance()->GetMouse().RemoveButtonListener( this, Mouse::Button_Middle );
}

void UIDesktop::UpdateLayout()
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    UIScalar viewWidth  = renderer->GetRenderTarget()->GetWidth();
    UIScalar viewHeight = renderer->GetRenderTarget()->GetHeight();

    if( !HasFlags(Flag_ValidPos) || GetWidth() != viewWidth || GetHeight() != viewHeight )
    {
        mRect = UIRect(0,0,viewWidth,viewHeight);
        SetFlags( Flag_ValidPos );

        UIElement::UpdateLayout();
    }
}

void UIDesktop::Draw()
{
    UpdateLayout();

    GetStyle().GetPainter().Begin();
    UIElement::Draw();
    GetStyle().GetPainter().End();
}

void UIDesktop::Paint()
{
}

UIStyle& UIDesktop::GetStyle()
{
    return *mStyle;
}

const UIStyle& UIDesktop::GetStyle() const
{
    return *mStyle;
}

// Mouse listener
void UIDesktop::OnMouseMove( Int32 /*pRelX*/, Int32 /*pRelY*/ )
{
    UIPoint oldMousePos = mMousePosition;
    UIPoint newMousePos = InputSubsystem::Instance()->GetMouse().GetPos();
    UIPoint windowPos   = Application::Instance()->GetWindow()->GetClientPos();

    newMousePos -= windowPos;
    newMousePos.y = mRect.Top() - newMousePos.y;
    
    UIWidget* oldWidgetUnder = WidgetAt(oldMousePos);
    UIWidget* newWidgetUnder = WidgetAt(newMousePos);
    UIWidget* widget;

    // Clear mouse over flag and send mouse leave events.
    widget = oldWidgetUnder;
    while( widget != NULL )
    {
        if( !widget->GetRect().Contains(newMousePos) )
        {
            if( widget->HasFlags(Flag_MouseOver) )
            {
                widget->ClearFlags(Flag_MouseOver);
                widget->EventMouseLeave( UIMouseLeaveEventArgs(newMousePos) );
            }
        }

        widget = widget->GetParentWidget();
    }

    // Set mouse over flag and send mouse enter events.
    widget = newWidgetUnder;
    while( widget != NULL )
    {
        if( widget->GetRect().Contains(newMousePos) )
        {
            if( !widget->HasFlags(Flag_MouseOver) )
            {
                widget->SetFlags(Flag_MouseOver);
                widget->EventMouseEnter( UIMouseEnterEventArgs(newMousePos) );
            }
        }

        widget = widget->GetParentWidget();
    }
        
    mMousePosition = newMousePos;
}

void UIDesktop::OnMouseButtonPressed( const Mouse::Button& pButton )
{
    UI::MouseButton button;

    switch( pButton )
    {
    case Mouse::Button_Left: 
        button = UI::MouseButton_Left;      
        break;

    case Mouse::Button_Right: 
        button = UI::MouseButton_Right;   
        break;

    case Mouse::Button_Middle: 
        button = UI::MouseButton_Middle;  
        break;

    default:    
        return; // Button not handled by UI.
    }


    Bool        bHandled = false;
    UIWidget*   widget = WidgetAt( mMousePosition );

    while( !bHandled && widget )
    {
        bHandled = widget->EventMousePress( UIMousePressEventArgs( button, mMousePosition ) );

        if( !bHandled )
            widget = widget->GetParentWidget();
    }
}

void UIDesktop::OnMouseButtonReleased( const Mouse::Button& pButton )
{
    UI::MouseButton button;

    switch( pButton )
    {
    case Mouse::Button_Left: 
        button = UI::MouseButton_Left;      
        break;

    case Mouse::Button_Right: 
        button = UI::MouseButton_Right;   
        break;

    case Mouse::Button_Middle: 
        button = UI::MouseButton_Middle;  
        break;

    default:    
        return; // Button not handled by UI.
    }

    Bool        bHandled = false;
    UIWidget*   widget = WidgetAt( mMousePosition );

    while( !bHandled && widget )
    {
        bHandled = widget->EventMouseRelease( UIMouseReleaseEventArgs( button, mMousePosition ) );

        if( !bHandled )
            widget = widget->GetParentWidget();
    }
}

UIWidget* UIDesktop::WidgetAt( const UIPoint& /*pPos*/ )
{
    UIWidget* widget = this;
    Bool updated = true;

    while( updated )
    {
        updated = false;

        for( UIWidget::ChildIterator it = widget->BeginChilds(); it; ++it )
        {
            if( (*it)->GetRect().Contains(mMousePosition) )
            {
                widget = (*it);
                updated = true;
                break;
            }
        }
    }

    return widget;
}

const UIWidget* UIDesktop::WidgetAt( const UIPoint& /*pPos*/ ) const
{
    const UIWidget* widget = this;
    Bool updated = true;

    while( updated )
    {
        updated = false;

        for( UIWidget::ConstChildIterator it = widget->BeginChilds(); it; ++it )
        {
            if( (*it)->GetRect().Contains(mMousePosition) )
            {
                widget = (*it);
                updated = true;
                break;
            }
        }
    }

    return widget;
}


} // namespace Gamedesk
