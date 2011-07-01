#ifndef     _UI_PUSH_BUTTON_H_
#define     _UI_PUSH_BUTTON_H_


#include "UIButton.h"


namespace Gamedesk {


class UIPushButton : public UIButton
{
    DECLARE_UI_CLASS(UIPushButton, UIButton);
public:
    UIPushButton( UIElement* pParent );
    UIPushButton( const String& pText, UIElement* pParent );

    Bool IsFlat() const;
    void SetFlat( Bool pFlat );

private:
    Bool    mIsFlat;
};


} // namespace Gamedesk


#endif  //  _UI_PUSH_BUTTON_H_
