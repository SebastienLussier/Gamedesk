#include "Engine.h"
#include "UIDefaultStyle.h"

#include "UIWidget.h"
    #include "UIButton.h"
        #include "UIPushButton.h"
        #include "UICheckBox.h"
    #include "UILabel.h"

#include "UIDefaultStyleBitmaps.h"


namespace Gamedesk {
	
	
UIDefaultStyle::UIDefaultStyle()
{
    mPalette = GetDefaultPalette();

    mTex_ComboBoxCheck = Cast<Texture2D>(Texture2D::StaticClass()->AllocateNew("DefaultStyle_ComboBox"));
    mTex_ComboBoxCheck->Create( Image(IMG_UICheckBox_Check), false );
    mTex_ComboBoxCheck->Init();
}

UIPalette UIDefaultStyle::GetDefaultPalette() const
{
    UIPalette palette;

    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Foreground,        UIColor(0x808080ff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Button,            UIColor(0xdddfe4ff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Light,             UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Midlight,          UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Dark,              UIColor(0x555555ff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Mid,               UIColor(0xc7c7c7ff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Text,              UIColor(0xc7c7c7ff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_BrightText,        UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_ButtonText,        UIColor(0x808080ff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Base,              UIColor(0xefefefff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Background,        UIColor(0xefefefff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Shadow,            UIColor(0x000000ff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Highlight,         UIColor(0x567594ff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_HighlightedText,   UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_Link,              UIColor(0x0000eeff) );
    palette.SetBrush( UIPalette::Color_Disabled, UIPalette::Color_LinkVisited,       UIColor(0x52188bff) );

    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Foreground,        UIColor(0x000000ff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Button,            UIColor(0xdddfe4ff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Light,             UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Midlight,          UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Dark,              UIColor(0x555555ff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Mid,               UIColor(0xc7c7c7ff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Text,              UIColor(0x000000ff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_BrightText,        UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_ButtonText,        UIColor(0x000000ff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Base,              UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Background,        UIColor(0xefefefff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Shadow,            UIColor(0x000000ff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Highlight,         UIColor(0x678db2ff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_HighlightedText,   UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_Link,              UIColor(0x0000eeff) );
    palette.SetBrush( UIPalette::Color_Active,   UIPalette::Color_LinkVisited,       UIColor(0x52188bff) );

    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Foreground,        UIColor(0x000000ff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Button,            UIColor(0xdddfe4ff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Light,             UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Midlight,          UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Dark,              UIColor(0x555555ff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Mid,               UIColor(0xc7c7c7ff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Text,              UIColor(0x000000ff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_BrightText,        UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_ButtonText,        UIColor(0x000000ff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Base,              UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Background,        UIColor(0xefefefff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Shadow,            UIColor(0x000000ff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Highlight,         UIColor(0x678db2ff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_HighlightedText,   UIColor(0xffffffff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_Link,              UIColor(0x0000eeff) );
    palette.SetBrush( UIPalette::Color_Inactive, UIPalette::Color_LinkVisited,       UIColor(0x52188bff) );

    return palette;
}

void UIDefaultStyle::Draw( UIWidget* pWidget )
{
    if( pWidget->GetClassID() == UIPushButton::ClassID() )
    {
        DrawPushButton( (UIPushButton*)pWidget );
        return;
    }    

    if( pWidget->GetClassID() == UICheckBox::ClassID() )
    {
        DrawCheckBox( (UICheckBox*)pWidget );
        return;
    } 

    if( pWidget->GetClassID() == UILabel::ClassID() )
    {
        DrawLabel( (UILabel*)pWidget );
        return;
    }
}

void UIDefaultStyle::DrawPushButton( UIPushButton* pPushButton )
{
    UIColor borderColor         = mPalette.Background().GetColor().Darken(1.78f);
    UIColor gradientStartColor  = mPalette.Button().GetColor().Darken(1.05f);
    UIColor gradientStopColor   = mPalette.Button().GetColor().Lighten(1.04f);
    
    const UIRect& rect = pPushButton->GetRect();

    DrawRoundedRect( rect, borderColor );

    // Gradient rect
    if( pPushButton->IsDown() )
        mPainter->DrawGradient( rect.Resize(2,2,-2,-2), gradientStopColor, gradientStartColor, UI::Vertical );
    else
        mPainter->DrawGradient( rect.Resize(2,2,-2,-2), gradientStartColor, gradientStopColor, UI::Vertical );

    // Text
    mPainter->SetPen( mPalette.ButtonText().GetColor() );
    mPainter->DrawText( pPushButton->GetText(), pPushButton->GetRect(), AlignCenter );
}

void UIDefaultStyle::DrawCheckBox( UICheckBox* pCheckBox )
{
    UIColor borderColor         = mPalette.Background().GetColor().Darken(1.78f);
    UIColor alphaCornerColor    = borderColor.Blend(mPalette.Background().GetColor());

    UIColor baseGradientStartColor = mPalette.Base().GetColor().Darken(1.06f);
    UIColor baseGradientStopColor = mPalette.Base().GetColor().Darken(1.01f);

    UIColor highlightedGradientStartColor = mPalette.Button().GetColor().Blend(mPalette.Highlight().GetColor(), 0.15f);
    UIColor highlightedGradientStopColor = mPalette.Button().GetColor().Lighten(1.01f);

    UIColor highlightedBaseGradientStartColor = mPalette.Base().GetColor().Darken(1.05f).Blend(mPalette.Highlight().GetColor(), 0.3f);
    UIColor highlightedBaseGradientStopColor = mPalette.Base().GetColor();

    UIColor highlightedDarkInnerBorderColor = mPalette.Button().GetColor().Blend(mPalette.Highlight().GetColor(), 0.42f);
    UIColor highlightedLightInnerBorderColor = mPalette.Button().GetColor().Blend(mPalette.Highlight().GetColor(), 0.65f);


    const UIRect& rect = pCheckBox->GetRect();
    
    UIPoint size = rect.mP2 - rect.mP1;
    UIScalar posY = rect.mP1.y + (size.y - 13) / 2;

    UIRect rectCheck( rect.mP1.x, posY, rect.mP1.x + 13, posY + 13 );

    
    UILine lines[4];
    UIPoint points[4];

    // Border
    mPainter->SetPen( borderColor );
    lines[0] = UILine( rectCheck.Left() + 1,  rectCheck.Bottom(), rectCheck.Right() - 1, rectCheck.Bottom() );
    lines[1] = UILine( rectCheck.Right() - 1,  rectCheck.Bottom() + 1, rectCheck.Right() - 1, rectCheck.Top() - 1 );
    lines[2] = UILine( rectCheck.Right() - 1,  rectCheck.Top() - 1, rectCheck.Left() + 1, rectCheck.Top() - 1 );
    lines[3] = UILine( rectCheck.Left(), rectCheck.Top() - 1, rectCheck.Left(), rectCheck.Bottom() + 1 );
    mPainter->DrawLineList( lines, 4 );

    // Smooth corners
    mPainter->SetPen( alphaCornerColor );
    points[0].x = rectCheck.mP1.x;      points[0].y = rectCheck.mP1.y;
    points[1].x = rectCheck.mP2.x - 1;  points[1].y = rectCheck.mP1.y;
    points[2].x = rectCheck.mP2.x - 1;  points[2].y = rectCheck.mP2.y - 1;
    points[3].x = rectCheck.mP1.x;      points[3].y = rectCheck.mP2.y - 1;
    mPainter->DrawPoints( points, 4 );

    // Gradient background
    if( pCheckBox->IsMouseOver() )
        mPainter->DrawGradient( rectCheck.Resize(1,1,-1,-1), highlightedBaseGradientStartColor, highlightedBaseGradientStopColor, UI::Vertical );
    else
        mPainter->DrawGradient( rectCheck.Resize(1,1,-1,-1), baseGradientStartColor, baseGradientStopColor, UI::Vertical );

    // Highlighted border when hovering
    if( pCheckBox->IsMouseOver() ) 
    {
        mPainter->SetPen(highlightedLightInnerBorderColor);
        mPainter->DrawLine(rectCheck.Left() + 1, rectCheck.Bottom() + 1, rectCheck.Left() + 1, rectCheck.Top() - 2);
        mPainter->DrawLine(rectCheck.Left() + 1, rectCheck.Top() - 2, rectCheck.Right() - 2, rectCheck.Top() - 2);
        
        mPainter->SetPen(highlightedDarkInnerBorderColor);
        mPainter->DrawLine(rectCheck.Left() + 2, rectCheck.Bottom() + 2, rectCheck.Left() + 2, rectCheck.Top() - 3);
        mPainter->DrawLine(rectCheck.Left() + 2, rectCheck.Top() - 3, rectCheck.Right() - 3, rectCheck.Top() - 3);
        
        mPainter->SetPen(highlightedLightInnerBorderColor.Darken(1.10f));
        mPainter->DrawLine(rectCheck.Left() + 2, rectCheck.Bottom() + 1, rectCheck.Right() - 1, rectCheck.Bottom() + 1);
        mPainter->DrawLine(rectCheck.Right() - 2, rectCheck.Bottom() + 1, rectCheck.Right() - 2, rectCheck.Top() - 1);
        
        mPainter->SetPen(highlightedDarkInnerBorderColor.Darken(1.10f));
        mPainter->DrawLine(rectCheck.Left() + 3, rectCheck.Bottom() + 2, rectCheck.Right() - 2, rectCheck.Bottom() + 2);
        mPainter->DrawLine(rectCheck.Right() - 3, rectCheck.Bottom() + 2, rectCheck.Right() - 3, rectCheck.Top() - 2);
    }

    // Check mark
    if( pCheckBox->IsChecked() ) 
    {
        mPainter->SetBrush( borderColor );
        mPainter->DrawTexture( UIPoint(rectCheck.Left() + 2, rectCheck.Bottom() + 2), mTex_ComboBoxCheck );
    }

    // Text!
    UIRect textRect = pCheckBox->GetRect();
    textRect.mP1.x += 16;
    mPainter->SetPen( mPalette.ButtonText().GetColor() );
    mPainter->DrawText( pCheckBox->GetText(), textRect, AlignLeft );
}

void UIDefaultStyle::DrawLabel( UILabel* pLabel )
{
    DrawRoundedRect( pLabel->GetRect(), UIColor(0.0f, 0.0f, 0.0f, 1.0f) );

    // Text!
    mPainter->SetPen( mPalette.Text().GetColor() );
    mPainter->DrawText( pLabel->GetText(), pLabel->GetRect(), AlignLeft );
    
}

void UIDefaultStyle::DrawRoundedRect( const UIRect& pRect, const UIColor& pColor )
{
    static UIPoint points[8];

    // Border
    mPainter->SetPen( pColor );
    points[0].x = pRect.Left()  + 3;     points[0].y = pRect.Bottom();
    points[1].x = pRect.Right() - 3;     points[1].y = pRect.Bottom();
    points[2].x = pRect.Right() - 1;     points[2].y = pRect.Bottom() + 2;
    points[3].x = pRect.Right() - 1;     points[3].y = pRect.Top() - 2;
    points[4].x = pRect.Right() - 2;     points[4].y = pRect.Top() - 1;
    points[5].x = pRect.Left() + 2;      points[5].y = pRect.Top() - 1;
    points[6].x = pRect.Left();          points[6].y = pRect.Top() - 3;
    points[7].x = pRect.Left();          points[7].y = pRect.Bottom() + 3;
    mPainter->DrawLineLoop( points, 8 );

    UIColor alphaColor(pColor);
    alphaColor.A *= 0.5f;

    // Smooth corners
    mPainter->SetPen( alphaColor );
    points[0].x = pRect.Left() + 1;      points[0].y = pRect.Bottom();
    points[1].x = pRect.Left();          points[1].y = pRect.Bottom() + 1;
    points[2].x = pRect.Right() - 2;     points[2].y = pRect.Bottom();
    points[3].x = pRect.Right() - 1;     points[3].y = pRect.Bottom() + 1;
    points[4].x = pRect.Right() - 2;     points[4].y = pRect.Top() - 1;
    points[5].x = pRect.Right() - 1;     points[5].y = pRect.Top() - 2;
    points[6].x = pRect.Left();          points[6].y = pRect.Top() - 2;
    points[7].x = pRect.Left() + 1;      points[7].y = pRect.Top() - 1;
    mPainter->DrawPoints( points, 8 );
}


} // namespace Gamedesk
