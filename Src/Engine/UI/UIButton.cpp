#include "Engine.h"
#include "UIButton.h"


namespace Gamedesk {
	
	
UIButton::UIButton( UIElement* pParent ) 
    : UIWidget(pParent)
    , mIsDown(false)
    , mIsCheckable(false)
    , mIsChecked(false)
{
}

const String& UIButton::GetText() const
{
    return mText;
}

void UIButton::SetText( const String& pText )
{
    mText = pText;
}

Bool UIButton::IsDown() const
{
    return mIsDown;
}

void UIButton::SetDown( Bool pDown )
{
    mIsDown = pDown;
}

Bool UIButton::IsCheckable() const
{
    return mIsCheckable;
}
void UIButton::SetCheckable( Bool pCheckable )
{
    mIsCheckable = pCheckable;
}

Bool UIButton::IsChecked() const
{
    return mIsChecked;
}
void UIButton::SetChecked( Bool pChecked )
{
    mIsChecked = pChecked;
}

Bool UIButton::EventMousePress( const UIMousePressEventArgs& pEventArgs )
{
    mIsDown = true;
    return Super::EventMousePress( pEventArgs );
}

Bool UIButton::EventMouseRelease( const UIMouseReleaseEventArgs& pEventArgs )
{
    mIsDown = false;
    return Super::EventMouseRelease( pEventArgs );
}

Bool UIButton::EventMouseLeave( const UIMouseLeaveEventArgs& pEventArgs )
{
    mIsDown = false;
    return Super::EventMouseLeave( pEventArgs );
}


} // namespace Gamedesk
