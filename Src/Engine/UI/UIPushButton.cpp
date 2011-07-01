#include "Engine.h"
#include "UIPushButton.h"


namespace Gamedesk {
	
	
UIPushButton::UIPushButton( UIElement* pParent )
    : UIButton(pParent)
{

}

UIPushButton::UIPushButton( const String& pText, UIElement* pParent )
    : UIButton(pParent)
{
    SetText(pText);
}

Bool UIPushButton::IsFlat() const
{
    return mIsFlat;
}

void UIPushButton::SetFlat( Bool pFlat )
{
    mIsFlat = pFlat;
}


} // namespace Gamedesk
