#ifndef     _UI_DEFAULT_STYLE_H_
#define     _UI_DEFAULT_STYLE_H_


#include "UIStyle.h"


namespace Gamedesk {


class UIWidget;
    class UIButton;
        class UIPushButton;
        class UIRadioButton;
        class UICheckBox;
    class UILabel;
    class UIComboBox;


class UIDefaultStyle : public UIStyle
{
public:
    UIDefaultStyle();

    void Draw( UIWidget* pWidget );

    UIPalette GetDefaultPalette() const;

private:
    void DrawPushButton( UIPushButton* pPushButton );
    void DrawCheckBox( UICheckBox* pCheckBox );
    void DrawRadioButton( UIRadioButton* pRadioButton );    
    void DrawLabel( UILabel* pLabel );
    void DrawComboBox( UIComboBox* pComboBox );

    void DrawRoundedRect( const UIRect& pRect, const UIColor& pColor );

private:
    Texture2D*  mTex_ComboBoxCheck;
};


}


#endif  //  _UI_DEFAULT_STYLE_H_
