#ifndef     _UI_LABEL_H_
#define     _UI_LABEL_H_


#include "UIWidget.h"


namespace Gamedesk {


class UILabel : public UIWidget
{
    DECLARE_UI_CLASS(UILabel, UIWidget);

public:
    UILabel( UIElement* pParent );
    UILabel( const String& pText, UIElement* pParent );

    const String& GetText() const;
    void SetText( const String& pText );

private:
    String  mText;
};


} // namespace Gamedesk


#endif  //  _UI_LABEL_H_
