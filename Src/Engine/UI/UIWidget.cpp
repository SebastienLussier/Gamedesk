#include "Engine.h"
#include "UIWidget.h"

#include "UIDesktop.h"


namespace Gamedesk {
	
	
UIWidget::UIWidget()
    : UIElement(NULL)
{
}

UIWidget::UIWidget( UIElement* pParent )
    : UIElement(pParent)
{
}

void UIWidget::Paint()
{
    UIDesktop::Instance()->GetStyle().Draw(this);
}

Bool UIWidget::IsMouseOver() const
{
    return (mFlags & Flag_MouseOver) == Flag_MouseOver;
}

UIWidget* UIWidget::GetParentWidget()
{
    UIElement* parent = mParent;

    while( parent != NULL && !parent->IsA( UIWidget::ClassID()) )
        parent = parent->GetParent();

    return (UIWidget*)parent;
}

const UIWidget* UIWidget::GetParentWidget() const
{
    const UIElement* parent = mParent;

    while( parent != NULL && !parent->IsA( UIWidget::ClassID()) )
        parent = parent->GetParent();

    return (const UIWidget*)parent;
}

Bool UIWidget::EventMouseEnter( const UIMouseEnterEventArgs& /*pEventArgs*/ )
{
    return false;
}

Bool UIWidget::EventMouseLeave( const UIMouseLeaveEventArgs& /*pEventArgs*/ )
{
    return false;
}

Bool UIWidget::EventMousePress( const UIMousePressEventArgs& /*pEventArgs*/ )
{
    return false;
}

Bool UIWidget::EventMouseRelease( const UIMouseReleaseEventArgs& /*pEventArgs*/ )
{
    return false;
}


} // namespace Gamedesk
