#ifndef     _UI_BUTTON_H_
#define     _UI_BUTTON_H_


#include "UIWidget.h"


namespace Gamedesk {


class UIButton : public UIWidget
{
    DECLARE_UI_CLASS(UIButton, UIWidget);

public:
    UIButton( UIElement* pParent );

    const String& GetText() const;
    void SetText( const String& pText );

    Bool IsDown() const;
    void SetDown( Bool pDown );

    Bool IsCheckable() const;
    void SetCheckable( Bool pCheckable );

    Bool IsChecked() const;
    void SetChecked( Bool pChecked );


    virtual Bool EventMousePress( const UIMousePressEventArgs& pEventArgs );
    virtual Bool EventMouseRelease( const UIMouseReleaseEventArgs& pEventArgs );
    virtual Bool EventMouseLeave( const UIMouseLeaveEventArgs& pEventArgs );
    
private:
    String  mText;
    Bool    mIsDown;
    Bool    mIsCheckable;
    Bool    mIsChecked;    
};


}


#endif  //  _UI_BUTTON_H_
