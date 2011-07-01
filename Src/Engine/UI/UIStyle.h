#ifndef     _UI_STYLE_H_
#define     _UI_STYLE_H_


#include "UIBase.h"
#include "UIPainter.h"


namespace Gamedesk {


class UIWidget;


class UIStyle
{
public:
    UIStyle();

    UIPainter& GetPainter();
    const UIPainter& GetPainter() const;

    virtual void Draw( UIWidget* pWidget ) = 0;

    virtual UIPalette GetDefaultPalette() const = 0;

protected:
    UIPainter*  mPainter;
    UIPalette   mPalette;
};


} // namespace Gamedesk


#endif  //  _UI_STYLE_H_
