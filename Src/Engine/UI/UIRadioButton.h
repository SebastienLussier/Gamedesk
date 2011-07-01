#ifndef     _UI_RADIO_BUTTON_H_
#define     _UI_RADIO_BUTTON_H_


#include "UIButton.h"


namespace Gamedesk {


class UIRadioButton : public UIButton
{
    DECLARE_UI_CLASS(UIRadioButton, UIButton);
public:
    UIRadioButton( UIElement* pParent );
    UIRadioButton( const String& pText, UIElement* pParent );
};


} // namespace Gamedesk


#endif  //  _UI_RADIO_BUTTON_H_
