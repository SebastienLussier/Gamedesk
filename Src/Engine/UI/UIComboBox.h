#ifndef     _UI_COMBO_BOX_H_
#define     _UI_COMBO_BOX_H_


#include "UIWidget.h"


namespace Gamedesk {


class UIComboBox : public UIWidget
{
    DECLARE_UI_CLASS(UIComboBox, UIWidget);
public:
    UIComboBox( UIElement* pParent );
};


} // namespace Gamedesk


#endif  //  _UI_COMBO_BOX_H_
