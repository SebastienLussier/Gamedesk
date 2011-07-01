#include "Engine.h"
#include "UICheckBox.h"


namespace Gamedesk {
	
	
UICheckBox::UICheckBox( UIElement* pParent )
    : UIButton( pParent )
{
}

UICheckBox::UICheckBox( const String& pText, UIElement* pParent )
    : UIButton( pParent )
{
    SetText( pText );
}

Bool UICheckBox::EventMouseRelease( const UIMouseReleaseEventArgs& pEventArgs )
{
    SetChecked( !IsChecked() );
    return Super::EventMouseRelease( pEventArgs );
}


} // namespace Gamedesk
