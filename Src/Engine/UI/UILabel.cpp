#include "Engine.h"
#include "UILabel.h"


namespace Gamedesk {
	
	
UILabel::UILabel( UIElement* pParent )
    : UIWidget(pParent)
{
}

UILabel::UILabel( const String& pText, UIElement* pParent )
    : UIWidget(pParent)
    , mText(pText)
{
}

const String& UILabel::GetText() const
{
    return mText;
}

void UILabel::SetText( const String& pText )
{
    mText = pText;
}


} // namespace Gamedesk
