#include "Engine.h"
#include "UIPainter.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/Font/Font.h"


namespace Gamedesk {
	
	
///////////////////////////////////////////////////////////////////////////////
// UIBrush
void UIBrush::MakeCurrent()
{
    GraphicSubsystem::Instance()->GetRenderer()->SetColor( mColor );
}

///////////////////////////////////////////////////////////////////////////////
// UIPen
const UInt16 PEN_PATTERNS[UIPen::STYLE_NUM] = 
                                    {
                                        0x0000,      // STYLE_Empty
                                        0xFFFF,      // STYLE_Solid,
                                        0xFFF0,      // STYLE_Dash,
                                        0xCCCC,      // STYLE_Dot,
                                        0xFC30,      // STYLE_DashDot,
                                        0xFCCC       // STYLE_DashDotDot,
                                    };

void UIPen::MakeCurrent()
{
    if( mStyle != STYLE_Solid )
        GraphicSubsystem::Instance()->GetRenderer()->SetRenderState( Renderer::LineStipple, true );
    else
        GraphicSubsystem::Instance()->GetRenderer()->SetRenderState( Renderer::LineStipple, false );

    GraphicSubsystem::Instance()->GetRenderer()->SetLineStipple( 1, PEN_PATTERNS[mStyle] );
    GraphicSubsystem::Instance()->GetRenderer()->SetLineWidth( mLineWidth );
    mBrush.MakeCurrent();
}


///////////////////////////////////////////////////////////////////////////////
// UIPainter
UIPainter::UIPainter()
{
    mRenderer = GraphicSubsystem::Instance()->GetRenderer();

    mFont.mFont.GetFont( "Data\\Fonts\\Tahoma.ttf", 12 );
}

void UIPainter::Begin()
{
    Int32 viewport[4];
    mRenderer->GetViewport( viewport );
    mRenderer->Begin2DProjection( viewport[0], viewport[2], viewport[1], viewport[3], -1, 1 );
    mRenderer->SetRenderState( Renderer::Lighting, false );
    mRenderer->SetRenderState( Renderer::DepthTest, false );
    mRenderer->SetRenderState( Renderer::DepthMask, false );

    mRenderer->SetRenderState( Renderer::Blend, true );
    mRenderer->SetBlendFunc( Renderer::BlendSrcAlpha, Renderer::BlendInvSrcAlpha );
}

void UIPainter::End()
{
    mRenderer->End2DProjection();
    mRenderer->SetRenderState( Renderer::Lighting, true );
    mRenderer->SetRenderState( Renderer::DepthTest, true );
    mRenderer->SetRenderState( Renderer::DepthMask, true );

    mRenderer->SetColor( 1.0f, 1.0f, 1.0f, 1.0f );
    mRenderer->SetRenderState( Renderer::Blend, false );
}

void UIPainter::DrawPoint( UIScalar pX, UIScalar pY )
{
    DrawPoint( UIPoint(pX, pY) );
}

void UIPainter::DrawPoint( const UIPoint& pPoint )
{
    mPen.MakeCurrent();

    mRenderer->BeginScene( Renderer::PointList );
    mRenderer->SetVertex( pPoint );
    mRenderer->EndScene();
}

void UIPainter::DrawPoints( const Vector<UIPoint>& pPoints )
{ 
    DrawPoints( &pPoints[0], pPoints.size() );
}

void UIPainter::DrawPoints( const UIPoint* pPoints, UIScalar pCount )
{
    GD_ASSERT( pCount > 0 );

    mPen.MakeCurrent();

    const UIPoint* point = pPoints;

    mRenderer->BeginScene( Renderer::PointList );
    for( Int32 i = 0; i < pCount; i++, point++ )
        mRenderer->SetVertex( *point );
    mRenderer->EndScene();
}

void UIPainter::DrawLine( UIScalar pLeft, UIScalar pBottom, UIScalar pRight, UIScalar pTop )
{
    DrawLine( UILine(pLeft, pBottom, pRight, pTop) );
}

void UIPainter::DrawLine( const UILine& pLine )
{
    mPen.MakeCurrent();

    mRenderer->BeginScene( Renderer::LineList );
    mRenderer->SetVertex( pLine.mP1 );
    mRenderer->SetVertex( pLine.mP2 );
    mRenderer->EndScene();
}

void UIPainter::DrawLineList( const Vector<UILine>& pLines )
{
    DrawLineList( &pLines[0], pLines.size() );
}

void UIPainter::DrawLineList( const UILine* pLines, UIScalar pCount )
{
    mPen.MakeCurrent();

    const UILine* line = pLines;

    mRenderer->BeginScene( Renderer::LineList );
    for( Int32 i = 0; i < pCount; i++, line++ )
    {
        mRenderer->SetVertex( line->mP1 );
        mRenderer->SetVertex( line->mP2 );
    }
    mRenderer->EndScene();
}

void UIPainter::DrawLineLoop( const Vector<UIPoint>& pPoints )
{
    DrawLineLoop( &pPoints[0], pPoints.size() );
}

void UIPainter::DrawLineLoop( const UIPoint* pPoints, UIScalar pCount )
{
    GD_ASSERT_M( pCount > 1, "[UIPainter::DrawLineLoop] Array must at least contain 2 points to form a line..." );
    mPen.MakeCurrent();

    const UIPoint* point = pPoints;

    mRenderer->BeginScene( Renderer::LineLoop );
    for( Int32 i = 0; i < pCount; i++, point++ )
        mRenderer->SetVertex( *point );
    mRenderer->EndScene();
}

void UIPainter::DrawRect( const UIRect& pRect )
{
    UIScalar halfPenWidth = mPen.GetLineWidth()/2;
    UIPoint P1 = pRect.mP1 + UIPoint(halfPenWidth, halfPenWidth);
    UIPoint P2 = UIPoint(pRect.mP2.x - mPen.GetLineWidth(), pRect.mP1.y);
    UIPoint P3 = pRect.mP2 - UIPoint(mPen.GetLineWidth(), mPen.GetLineWidth());
    UIPoint P4 = UIPoint(pRect.mP1.x, pRect.mP2.y - mPen.GetLineWidth());

    mBrush.MakeCurrent();
    mRenderer->BeginScene( Renderer::TriangleFan );
        mRenderer->SetVertex( P1 );
        mRenderer->SetVertex( P2 );
        mRenderer->SetVertex( P3 );
        mRenderer->SetVertex( P4 );
    mRenderer->EndScene();

    if( mPen.GetStyle() != UIPen::STYLE_Empty )
    {        
        mPen.MakeCurrent();
        mRenderer->BeginScene( Renderer::LineLoop );
            mRenderer->SetVertex( P1 );
            mRenderer->SetVertex( P2 );
            mRenderer->SetVertex( P3 );
            mRenderer->SetVertex( P4 );
        mRenderer->EndScene();
    }
}

void UIPainter::DrawGradient( const UILine& pLine, const UIColor& pColorStart, const UIColor& pColorEnd )
{
    mRenderer->BeginScene( Renderer::LineList );
        mRenderer->SetColor( pColorStart );
        mRenderer->SetVertex( pLine.mP1 );
        mRenderer->SetColor( pColorEnd );
        mRenderer->SetVertex( pLine.mP2 );
    mRenderer->EndScene();
}

void UIPainter::DrawGradient( const UIRect& pRect, const UIColor& pColorStart, const UIColor& pColorEnd, UI::Direction pDirection )
{
    UIPoint P1 = pRect.mP1;
    UIPoint P2 = UIPoint(pRect.mP2.x, pRect.mP1.y);
    UIPoint P3 = pRect.mP2;
    UIPoint P4 = UIPoint(pRect.mP1.x, pRect.mP2.y);

    mRenderer->BeginScene( Renderer::TriangleFan );
        
        mRenderer->SetColor( pColorStart );
        mRenderer->SetVertex( P1 );

        if( pDirection & UI::Horizontal )
            mRenderer->SetColor( pColorEnd );
        mRenderer->SetVertex( P2 );
        
        mRenderer->SetColor( pColorEnd );
        mRenderer->SetVertex( P3 );
        
        if( pDirection & UI::Horizontal )
            mRenderer->SetColor( pColorStart );
        mRenderer->SetVertex( P4 );
    mRenderer->EndScene();
}

void UIPainter::DrawRects( const Vector<UIRect>& /*pRects*/ )
{
}



void UIPainter::DrawText( const String& pString, const UIPoint& pPos )
{
    mFont.mFont->DrawString( pPos.x, pPos.y, pString.c_str() );
    mRenderer->SetRenderState( Renderer::Blend, true );
}

void UIPainter::DrawText( const String& pString, const UIRect& pRect, UIAlignment pAlignment )
{
    mPen.MakeCurrent();

    UIPoint stringSize = mFont.mFont->GetStringSize( pString.c_str() );
    
    UIScalar availableSpaceH = pRect.mP2.x - pRect.mP1.x;
    UIScalar availableSpaceV = pRect.mP2.y - pRect.mP1.y;

    UIPoint pos;
    
    if( pAlignment & AlignLeft )
        pos.x = pRect.mP1.x;
    else if( pAlignment & AlignRight )
        pos.x = pRect.mP2.x - stringSize.x;
    else
        pos.x = pRect.mP1.x + ((availableSpaceH - stringSize.x) >> 1);

    if( pAlignment & AlignBottom )
        pos.y = pRect.mP1.y;
    else if( pAlignment & AlignTop )
        pos.y = pRect.mP2.y - stringSize.y;
    else
        pos.y = pRect.mP1.y + ((availableSpaceV - stringSize.y) >> 1);

    mFont.mFont->DrawString( pos.x, pos.y, pString.c_str() );

    mRenderer->SetRenderState( Renderer::Blend, true );
}

void UIPainter::DrawTexture( const UIPoint& /*pPos*/, const HTexture1D& /*pTexture*/ )
{
}

void UIPainter::DrawTexture( const UIRect& /*pDestRect*/, const HTexture1D& /*pTexture*/, const UIRect& /*pSrcRect*/ )
{
}

void UIPainter::DrawTextureTiled( const UIRect& /*pDestRect*/, const HTexture1D& /*pTexture*/ )
{
}


void UIPainter::DrawTexture( const UIPoint& pPos, Texture2D* pTexture )
{
    mRenderer->GetTextureStage(0)->SetTexture( *pTexture );

    mBrush.MakeCurrent();
    mRenderer->BeginScene( Renderer::TriangleFan );
        mRenderer->SetUV( 0, 1 );
        mRenderer->SetVertex( pPos );

        mRenderer->SetUV( 1, 1 );
        mRenderer->SetVertex( UIPoint(pPos.x + pTexture->GetWidth(), pPos.y) );

        mRenderer->SetUV( 1, 0 );
        mRenderer->SetVertex( UIPoint(pPos.x + pTexture->GetWidth(), pPos.y + pTexture->GetHeight()) );

        mRenderer->SetUV( 0, 0 );
        mRenderer->SetVertex( UIPoint(pPos.x, pPos.y + pTexture->GetHeight()) );
    mRenderer->EndScene();

    mRenderer->GetTextureStage(0)->ResetTexture();
}

void UIPainter::DrawTexture( const UIRect& /*pDestRect*/, const HTexture2D& /*pTexture*/, const UIRect& /*pSrcRect*/ )
{
}

void UIPainter::DrawTextureTiled( const UIRect& /*pDestRect*/, const HTexture2D& /*pTexture*/ )
{
}


void UIPainter::DrawImage( const UIPoint& /*pPos*/, const Image& /*pImage*/ )
{
}

void UIPainter::DrawImage( const UIRect& /*pDestRect*/, const Image& /*pImage*/, const UIRect& /*pSrcRect*/ )
{
}

void UIPainter::DrawImageTiled( const UIRect& /*pDestRect*/, const Image& /*pImage*/ )
{
}


void UIPainter::DrawArc( const UIRect& /*pRect*/, Float /*pStartAngle*/, Float /*pEndAngle*/ )
{
}

void UIPainter::DrawPie( const UIRect& /*pRect*/, Float /*pStartAngle*/, Float /*pEndAngle*/ )
{
}

void UIPainter::DrawChord( const UIRect& /*pRect*/, Float /*pStartAngle*/, Float /*pEndAngle*/ )
{
}


void UIPainter::DrawPolygon( const Vector<UIPoint>& /*pPoints*/ )
{
}

void UIPainter::DrawElipse( const UIRect& /*pRect*/ )
{
}

Bool UIPainter::IsVisible( const UIRect& /*pRect*/ )
{
    return true;
}

void UIPainter::PushClipRect( const UIRect& /*pClipRect*/ )
{
}

void UIPainter::PopClipRect()
{
}


} // namespace Gamedesk
