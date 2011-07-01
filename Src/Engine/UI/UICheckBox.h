#ifndef     _UI_CHECK_BOX_H_
#define     _UI_CHECK_BOX_H_


#include "UIButton.h"


namespace Gamedesk {


class UICheckBox : public UIButton
{
    DECLARE_UI_CLASS(UICheckBox, UIButton);
public:
    UICheckBox( UIElement* pParent );
    UICheckBox( const String& pText, UIElement* pParent );

    Bool EventMouseRelease( const UIMouseReleaseEventArgs& pEventArgs );
};


} // namespace Gamedesk


#endif  //  _UI_CHECK_BOX_H_
