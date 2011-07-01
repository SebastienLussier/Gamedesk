/**
*  @file       TestUI.cpp
*  @author     Sébastien Lussier.
*  @date       10/03/05.
*/
/*
*  Copyright (C) 2005 Gamedesk
*
*  This program is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*  Gamedesk
*  http://gamedesk.type-cast.com
*
*/
#include "Engine.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Shader/ShaderProgram.h"
#include "Graphic/Shader/ShaderObject.h"
#include "Graphic/Renderer.h"
#include "Graphic/Font/Font.h"
#include "Graphic/Font/FontHdl.h"

#include "World/Entity.h"

#include "UI/UIPushButton.h"
#include "UI/UICheckBox.h"
#include "UI/UILabel.h"
#include "UI/UIDesktop.h"


namespace Gamedesk {
	
	
class ENGINE_API TestUI : public Entity
{
    DECLARE_CLASS(TestUI, Entity);

public:
    TestUI();
    virtual ~TestUI();

    virtual void Update( Double pElapsedTime );
    virtual void Render() const;
    
    void TestRender();

private:
    FontHdl         test;

    ShaderObject*   mVertexShader;
    ShaderObject*   mPixelShader;
    ShaderProgram*  mShaderProgram;
};

IMPLEMENT_CLASS(TestUI);


TestUI::TestUI()
{
    /*test.GetFont( "Data\\Fonts\\Arial.ttf", 14 );

    mVertexShader  = Cast<ShaderObject>( GraphicSubsystem::Instance()->Create( ShaderObject::StaticClass() ) );
    mPixelShader   = Cast<ShaderObject>( GraphicSubsystem::Instance()->Create( ShaderObject::StaticClass() ) );
    mShaderProgram = Cast<ShaderProgram>( GraphicSubsystem::Instance()->Create( ShaderProgram::StaticClass() ) );

    mVertexShader->Create( ShaderObject::Shader_Vertex );
    mVertexShader->SetShaderTextFile( "Data/Tests/Shaders/Gradient.vs" );
    mVertexShader->Compile();

    mPixelShader->Create( ShaderObject::Shader_Pixel );
    mPixelShader->SetShaderTextFile( "Data/Tests/Shaders/Gradient.ps" );
    mPixelShader->Compile();

    mShaderProgram->AddShader( mVertexShader );
    mShaderProgram->AddShader( mPixelShader );
    mShaderProgram->Link();*/
/*
    UIElement* elem = GD_NEW(UIElement, this, "Engine::World::TestUI")(&mUI);
    elem->SetSize( UIPoint(100, 30) );
    elem->AttachParent( UIElement::Right, UIElement::Right, -10 );
    elem->AttachParent( UIElement::Top, UIElement::Top, -10 );

    UIElement* elem2 = GD_NEW(UIElement, this, "Engine::World::TestUI")(&mUI);
    elem2->SetSize( UIPoint(100, 30) );

    elem2->Attach( UIElement::Right, elem, UIElement::Left, -10 );
    elem2->Attach( UIElement::Top, elem, UIElement::Top, 0 );


    // Resizable
    UIElement* elem3 = GD_NEW(UIElement, this, "Engine::World::TestUI")(&mUI);
    elem3->SetHeight( 30 );
    elem3->Attach( UIElement::Right, elem2, UIElement::Left, -10 );
    elem3->Attach( UIElement::Top, elem2, UIElement::Top, 0 );
    elem3->AttachParent( UIElement::Left, UIElement::Left, 10 );
*/

    // Simple test
    /*{
        UIPushButton* A = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("Y", UIDesktop::Instance());
        UIPushButton* B = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("Z", UIDesktop::Instance());

        A->SetHeight( 30 );
        A->AttachParent( UI::Top, UI::Top, -200 );
        A->AttachParent( UI::Left, UI::Left, 10 );
        A->SetWidth( 200 );

        B->SetHeight( 30 );
        B->AttachParent( UI::Top, UI::Top, -200 );
        B->Attach( UI::Left, A, UI::Right, 10 );
        B->SetWidth( 50 );
    }*/

    // Simple test 2
    // Left widget is fixed, right is variable
    {
        UIPushButton* A = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("A", UIDesktop::Instance());
        UIPushButton* B = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("B", UIDesktop::Instance());
        
        A->SetHeight( 30 );
        A->AttachParent( UI::Top, UI::Top, -250 );

        B->SetHeight( 30 );
        B->AttachParent( UI::Top, UI::Top, -250 );

        A->AttachParent( UI::Left, UI::Left, 10 );
        A->SetWidth( 200 );        

        B->Attach( UI::Left, A, UI::Right, 10 );
        B->AttachParent( UI::Right, UI::Right, -10 );
    }

    // Simple test 3
    // Left widget is variable, right is fixed
    {
        UIPushButton* A = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("C", UIDesktop::Instance());
        UIPushButton* B = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("D", UIDesktop::Instance());
        
        A->SetHeight( 30 );
        A->AttachParent( UI::Top, UI::Top, -200 );

        B->SetHeight( 30 );
        B->AttachParent( UI::Top, UI::Top, -200 );

        A->AttachParent( UI::Left, UI::Left, 10 );
        A->Attach( UI::Right, B, UI::Left, -10 );        

        B->SetWidth( 200 );
        B->AttachParent( UI::Right, UI::Right, -10 );
    }

    // Simple test 3
    /*{
        UIPushButton* A = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("M", UIDesktop::Instance());
        UIPushButton* B = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("N", UIDesktop::Instance());
        UIPushButton* C = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("O", UIDesktop::Instance());

        A->SetHeight( 30 );
        A->AttachParent( UI::Top, UI::Top, -300 );
        A->AttachParent( UI::Left, UI::Left, 10 );
        A->Attach( UI::Right, B, UI::Left, -10 );

        B->SetHeight( 30 );
        B->SetWidth( 200 );
        B->AttachParent( UI::Top, UI::Top, -300 );
        B->Attach( UI::Right, C, UI::Left, -10 );

        C->SetHeight( 30 );
        C->SetWidth( 200 );
        C->AttachParent( UI::Top, UI::Top, -300 );
        C->AttachParent( UI::Right, UI::Right, -10 );
    }*/

    // Complex test
    {
        UIPushButton* A = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("A", UIDesktop::Instance());
        UIPushButton* B = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("B", UIDesktop::Instance());
        UIPushButton* C = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("C", UIDesktop::Instance());
        UIPushButton* D = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("D", UIDesktop::Instance());
        UIPushButton* E = GD_NEW(UIPushButton, this, "Engine::World::TestUI")("E", UIDesktop::Instance());

        UILabel*    label    = GD_NEW(UILabel, this, "Engine::World::TestUI")("This is a text label", UIDesktop::Instance());
        UICheckBox* checkBox = GD_NEW(UICheckBox, this, "Engine::World::TestUI")( "Checkbox", UIDesktop::Instance());

        // Variable size, attach to left
        A->SetHeight( 30 );
        A->AttachParent( UI::Top, UI::Top, -50 );
        A->AttachParent( UI::Left, UI::Left, 10 );
        A->Attach( UI::Right, B, UI::Left, 10 );
        
        // Fixed width, attach to left
        B->SetHeight( 30 );
        B->AttachParent( UI::Top, UI::Top, -50 );
        B->Attach( UI::Left, A, UI::Right, 10 );
        B->SetWidth( 50 );
        
        // Variable size, attach to left & right
        C->SetHeight( 30 );
        C->AttachParent( UI::Top, UI::Top, -50 );
        C->Attach( UI::Left, B, UI::Right, 10 );
        C->Attach( UI::Right, D, UI::Left, -10 );

        // Fixed width, attach to right
        D->SetHeight( 30 );
        D->AttachParent( UI::Top, UI::Top, -50 );
        D->Attach( UI::Right, E, UI::Left, -10 );
        D->SetWidth( 50 );
       
        // Variable size, attach to right
        E->SetHeight( 30 );
        E->AttachParent( UI::Top, UI::Top, -50 );
        E->Attach( UI::Left, D, UI::Right, -10 );
        E->AttachParent( UI::Right, UI::Right, -10 );

        // 
        label->SetHeight( 50 );
        label->AttachParent( UI::Left, UI::Left, 10 );
        label->Attach( UI::Top, A, UI::Bottom, -10 );
        label->SetWidth( 200 );        

        checkBox->SetHeight( 20 );
        checkBox->SetWidth( 100 );
        checkBox->AttachParent( UI::Right, UI::Right, -10 );
        checkBox->AttachParent( UI::Bottom,UI::Bottom, 10 );
    }

    
    //elem2->AttachParent( UIElement::Left, UIElement::Left, 10 );
    //elem->Attach( UIElement::Left, elem2, UIElement::Right, -10 );
}

TestUI::~TestUI()
{
}

void TestUI::Update( Double /*pElapsedTime*/ )
{
}

void TestUI::Render() const
{
    ((TestUI*)this)->TestRender();
}

void TestUI::TestRender()
{
    /*mUIRenderer.Begin();

    UIPen pen;
    pen.SetStyle( UIPen::STYLE_Solid );
    pen.SetColor( Color4f(0.5019f,0.5176f,0.5019f,1.0f) );
    pen.SetLineWidth( 1.0f );
    mUIRenderer.SetPen( pen );

    UIBrush brush;
    brush.SetColor( Color4f((Byte)216, (Byte)220, (Byte)224, (Byte)255) );
    brush.SetStyle( UIBrush::STYLE_Solid );
    mUIRenderer.SetBrush( brush );

    mUIRenderer.DrawLine( UILine( UIPoint( 100, 100 ), UIPoint( 400, 100 ) ) );

    Vector<UIPoint> lineStrip;
    lineStrip.push_back( UIPoint( 300, 234 ) );
    lineStrip.push_back( UIPoint( 330, 239 ) );
    lineStrip.push_back( UIPoint( 340, 220 ) );
    lineStrip.push_back( UIPoint( 270, 250 ) );
    mUIRenderer.DrawLineStrip( lineStrip );


    mUIRenderer.DrawRect( UIRect( 600, 400, 100, 30 ) );
    pen.SetColor( Color4f(0.0f,0.0f,0.0f,1.0f) );
    pen.MakeCurrent();
    Vector2i sz = test->GetStringSize( "Button Test" );    
    test->DrawString( 600 + (100 - sz.x) / 2, 400 + (30 - sz.y) / 2, "Button Test" );
*/
/*

    mShaderProgram->Apply();
    mShaderProgram->SetUniform( "GradientStart", Vector2f(400,300) );
    mShaderProgram->SetUniform( "GradientEnd",   Vector2f(600,300) );
    mShaderProgram->SetUniform( "ViewportSize",  Vector2f(800,600) );

    mShaderProgram->SetUniform( "Key_Num", 1 );
    mShaderProgram->SetUniform( "Key_Start", Color4f(1.0f,0.0f,0.0f,1.0f) );
    mShaderProgram->SetUniform( "Key1_T", 0.5f );
    mShaderProgram->SetUniform( "Key1_Color", Color4f(0.0f,1.0f,0.0f,1.0f) );
    mShaderProgram->SetUniform( "Key_End", Color4f(0.0f,0.0f,0.0f,1.0f) );

    mUIRenderer.DrawRect( UIRect( 400, 300, 200, 200 ) );
    mShaderProgram->Done();


    mUIRenderer.End();
    */

    UIDesktop::Instance()->Draw();
}


} // namespace Gamedesk
