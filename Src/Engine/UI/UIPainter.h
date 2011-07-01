#ifndef     _UI_PAINTER_H_
#define     _UI_PAINTER_H_


#include "Maths/Maths.h"

#include "Graphic/Image/Image.h"
#include "Graphic/Texture/TextureHdl.h"
#include "Graphic/Font/FontHdl.h"

#include "UI/UIBase.h"


namespace Gamedesk {


class UIGradient
{
public:
    enum Style
    {
        STYLE_Linear,
        STYLE_Radial,
        STYLE_NUM
    };

    Style GetStyle() const                      { return mStyle;   }
    void SetStyle( Style pStyle )               { mStyle = pStyle; }

private:
    struct Entry
    {
        Float   mPercent;
        UIColor mColor;
    };

    Style           mStyle;
    Vector<Entry>   mColors;
};


class UIBrush
{
public:
    enum Style
    {
        STYLE_Empty,
        STYLE_Solid, 
        STYLE_Gradient,
        STYLE_NUM
    };

    UIBrush()
        : mStyle(STYLE_Solid)
        , mColor(1.0f, 1.0f, 1.0f, 1.0f)
    {
    }

    UIBrush( const UIColor& pColor )
        : mStyle(STYLE_Solid)
        , mColor(pColor)
    {
    }

    UIBrush( const UIBrush& pBrush )
        : mStyle(pBrush.mStyle)
        , mColor(pBrush.mColor)
    {
    }

    const UIBrush& operator = ( const UIBrush& pBrush )
    {
        mStyle = pBrush.mStyle;
        mColor = pBrush.mColor;

        return *this;
    }

    Style GetStyle() const                      { return mStyle;   }
    void SetStyle( Style pStyle )               { mStyle = pStyle; }

    const UIColor& GetColor() const             { return mColor;   }
    void SetColor( const UIColor& pColor )      { mColor = pColor; }

    void MakeCurrent();

private:
    Style       mStyle;
    UIColor     mColor;
};


class UIPen
{
public:
    enum Style
    {
        STYLE_Empty,
        STYLE_Solid,
        STYLE_Dash,
        STYLE_Dot,
        STYLE_DashDot,
        STYLE_DashDotDot,
        STYLE_NUM
    };

    UIPen()
        : mStyle(STYLE_Solid)
        , mLineWidth(1.0f)
    {
    }

    UIPen( UIColor pColor )
        : mStyle(STYLE_Solid)
        , mLineWidth(1.0f)
        , mBrush(pColor)
    {
    }

    UIPen( const UIPen& pPen )
        : mStyle(pPen.mStyle)
        , mLineWidth(pPen.mLineWidth)
        , mBrush(pPen.mBrush)
    {
    }

    const UIPen& operator = ( const UIPen& pPen )
    {
        mStyle = pPen.mStyle;
        mLineWidth = pPen.mLineWidth;
        mBrush = pPen.mBrush;

        return *this;
    }

    Style GetStyle() const                      { return mStyle;   }
    void SetStyle( Style pStyle )               { mStyle = pStyle; }

    const UIBrush& GetBrush() const             { return mBrush;   }
    void SetBrush( const UIBrush& pBrush )      { mBrush = pBrush; }

    const UIColor& GetColor() const             { return mBrush.GetColor();  }
    void SetColor( const UIColor& pColor )      { mBrush.SetColor( pColor ); }

    UIReal GetLineWidth() const                 { return mStyle != STYLE_Empty ? mLineWidth : 0; }
    void SetLineWidth( UIReal pLineWidth )      { mLineWidth = pLineWidth; }

    void MakeCurrent();

private:
    Style       mStyle;
    UIReal      mLineWidth;
    UIBrush     mBrush;
};


class UIFont
{
public:
    FontHdl mFont;
};


class UIPalette
{
public:
    UIPalette() :
        mCurrentGroup(Color_Inactive)
    {
    }

    enum ColorGroup
    { 
        Color_Active,
        Color_Disabled,
        Color_Inactive,
        ColorGroup_NUM
    };

    enum ColorType
    { 
        Color_Base,
        Color_AlternateBase,
        Color_Background,
        Color_Foreground,
        Color_Button,
        Color_Light,
        Color_Midlight,
        Color_Mid,
        Color_Dark,        
        Color_Text,
        Color_BrightText,
        Color_ButtonText,
        Color_HighlightedText,
        Color_Highlight,
        Color_Shadow,
        Color_Link,
        Color_LinkVisited,

        ColorType_NUM
    };

    const UIBrush& Base() const                 { return mBrush[mCurrentGroup][Color_Base]; }
    const UIBrush& AlternateBase() const        { return mBrush[mCurrentGroup][Color_AlternateBase]; }
    const UIBrush& Background () const          { return mBrush[mCurrentGroup][Color_Background]; }
    const UIBrush& Foreground() const           { return mBrush[mCurrentGroup][Color_Foreground]; }
    const UIBrush& Button() const               { return mBrush[mCurrentGroup][Color_Button]; }
    const UIBrush& Light() const                { return mBrush[mCurrentGroup][Color_Light]; }
    const UIBrush& Midlight() const             { return mBrush[mCurrentGroup][Color_Midlight]; }
    const UIBrush& Mid() const                  { return mBrush[mCurrentGroup][Color_Mid]; }
    const UIBrush& Dark() const                 { return mBrush[mCurrentGroup][Color_Dark]; }
    const UIBrush& Text() const                 { return mBrush[mCurrentGroup][Color_Text]; }
    const UIBrush& BrightText () const          { return mBrush[mCurrentGroup][Color_BrightText]; }
    const UIBrush& ButtonText() const           { return mBrush[mCurrentGroup][Color_ButtonText]; }
    const UIBrush& HighlightedText() const      { return mBrush[mCurrentGroup][Color_HighlightedText]; }
    const UIBrush& Highlight() const            { return mBrush[mCurrentGroup][Color_Highlight]; }
    const UIBrush& Shadow() const               { return mBrush[mCurrentGroup][Color_Shadow]; }
    const UIBrush& Link() const                 { return mBrush[mCurrentGroup][Color_Link]; }
    const UIBrush& LinkVisited() const          { return mBrush[mCurrentGroup][Color_LinkVisited]; }
    
    ColorGroup CurrentColorGroup() const { return mCurrentGroup; }
    void SetCurrentColorGroup( ColorGroup pGroup ) { mCurrentGroup = pGroup; }

    void SetBrush( ColorType pType, const UIBrush& pBrush )
    {
        mBrush[mCurrentGroup][pType] = pBrush;
    }

    void SetBrush( ColorGroup pGroup, ColorType pType, const UIBrush& pBrush )
    {
        mBrush[pGroup][pType] = pBrush;
    }

    void SetColor( ColorType pType, const UIColor& pColor )
    {
        mBrush[mCurrentGroup][pType].SetColor(pColor);
    }

    void SetColor( ColorGroup pGroup, ColorType pType, const UIColor& pColor )
    {
        mBrush[pGroup][pType].SetColor(pColor);
    }
    
 /*   Bool operator == ( const UIPalette& pOther ) const;
    Bool operator != ( const UIPalette& pOther ) const;
   */ 
   
    UIPalette& operator = ( const UIPalette& pOther )
    {
        for( UInt32 i = 0; i < ColorGroup_NUM; i++ )
        {
            for( UInt32 j = 0; j < ColorType_NUM; j++ )
                mBrush[i][j] = pOther.mBrush[i][j];
        }

        return *this;
    }
    
    const UIBrush& Brush( ColorType pType ) const                       { return mBrush[mCurrentGroup][pType]; }
    const UIBrush& Brush( ColorGroup pGroup, ColorType pType ) const    { return mBrush[pGroup][pType]; }

    const UIColor& Color( ColorType pType ) const                       { return mBrush[mCurrentGroup][pType].GetColor(); }
    const UIColor& Color( ColorGroup pGroup, ColorType pType ) const    { return mBrush[pGroup][pType].GetColor(); }
    
private:
    UIBrush     mBrush[ColorGroup_NUM][ColorType_NUM];
    ColorGroup  mCurrentGroup;
};


class UIPainter
{
public:
    UIPainter();

    void Begin();
    void End();

    void DrawPoint( UIScalar pX, UIScalar pY );
    void DrawPoint( const UIPoint& pPoint );
    void DrawPoints( const Vector<UIPoint>& pPoints );
    void DrawPoints( const UIPoint* pPoints, UIScalar pCount );
    
    void DrawLine( UIScalar pLeft, UIScalar pBottom, UIScalar pRight, UIScalar pTop );
    void DrawLine( const UILine& pLine );
    void DrawLineList( const Vector<UILine>& pLines );
    void DrawLineList( const UILine* pLines, UIScalar pCount );
    void DrawLineLoop( const Vector<UIPoint>& pPoints );
    void DrawLineLoop( const UIPoint* pPoints, UIScalar pCount );

    void DrawRect( const UIRect& pRect );
    void DrawRects( const Vector<UIRect>& pRects );

    void DrawGradient( const UILine& pLine, const UIColor& pColorStart, const UIColor& pColorEnd );
    void DrawGradient( const UIRect& pRect, const UIColor& pColorStart, const UIColor& pColorEnd, UI::Direction pDirection );

    void DrawText( const String& pString, const UIPoint& pPos );
    void DrawText( const String& pString, const UIRect& pRect, UIAlignment pAlignemnt );

    void DrawTexture( const UIPoint& pPos, const HTexture1D& pTexture );
    void DrawTexture( const UIRect& pDestRect, const HTexture1D& pTexture, const UIRect& pSrcRect );
    void DrawTextureTiled( const UIRect& pDestRect, const HTexture1D& pTexture );
    
    void DrawTexture( const UIPoint& pPos, Texture2D* pTexture );
    void DrawTexture( const UIRect& pDestRect, const HTexture2D& pTexture, const UIRect& pSrcRect );
    void DrawTextureTiled( const UIRect& pDestRect, const HTexture2D& pTexture );

    void DrawImage( const UIPoint& pPos, const Image& pImage );
    void DrawImage( const UIRect& pDestRect, const Image& pImage, const UIRect& pSrcRect );
    void DrawImageTiled( const UIRect& pDestRect, const Image& pImage );

    void DrawArc( const UIRect& pRect, Float pStartAngle, Float pEndAngle );
    void DrawPie( const UIRect& pRect, Float pStartAngle, Float pEndAngle );
    void DrawChord( const UIRect& pRect, Float pStartAngle, Float pEndAngle );

    void DrawPolygon( const Vector<UIPoint>& pPoints );
    void DrawElipse( const UIRect& pRect );
 
    const UIPen& GetPen() const             { return mPen; }
    void SetPen( const UIPen& pPen )        { mPen = pPen; }

    const UIBrush& GetBrush() const         { return mBrush;   }
    void SetBrush( const UIBrush& pBrush )  { mBrush = pBrush; }

    const UIFont& GetFont() const           { return mFont;  }
    void SetFont( const UIFont& pFont )     { mFont = pFont; }

    //! Test if the given rect is visible taking into account clipping.
    Bool IsVisible( const UIRect& pRect );

    void PushClipRect( const UIRect& pClipRect );
    void PopClipRect();

private:
    class Renderer* mRenderer;
    UIFont          mFont;
    UIBrush         mBrush;
    UIPen           mPen;

    Vector<UIRect>  mClipRects;
};


} // namespace Gamedesk


#endif  //  _UI_PAINTER_H_
