#include "Engine.h"
#include "UIStyle.h"
#include "UIPainter.h"


namespace Gamedesk {
	
	
UIStyle::UIStyle()
{
    mPainter = GD_NEW(UIPainter, this, "Engine::UI::UIStyle");
}

UIPainter& UIStyle::GetPainter()
{
    return *mPainter;
}

const UIPainter& UIStyle::GetPainter() const
{
    return *mPainter;
}


} // namespace Gamedesk
