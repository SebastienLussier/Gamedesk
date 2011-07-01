/**
 *  @file       TestRenderTexture.cpp
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
#include "Graphic/Renderer.h"
#include "Graphic/Texture/Texture.h"
#include "Graphic/Texture/TextureHdl.h"
#include "Graphic/RenderTarget/RenderTexture.h"

#include "World/Entity.h"


namespace Gamedesk {
	
	
class ENGINE_API TestRenderTexture : public Entity
{
    DECLARE_CLASS(TestRenderTexture, Entity);

public:
    TestRenderTexture();
    virtual ~TestRenderTexture();

    virtual void Update( Double pElapsedTime );
    virtual void Render() const;

private:
    Float               mSize;
    Float               mRotation;

    RenderTexture2D*    mRenTex;
    HTexture2D          mTestTex;
};

IMPLEMENT_CLASS(TestRenderTexture);


TestRenderTexture::TestRenderTexture()
{
    mSize = 10.0f;
    mRotation = 0;

    mBoundingBox.SetMin( Vector3f(-mSize, -mSize, -mSize) );
    mBoundingBox.SetMax( Vector3f(mSize, mSize, mSize) );

    mTestTex.GetTexture( "Data/Tests/Textures/tile_d.jpg" );

    UInt32 width  = 256;
    UInt32 height = 256;

    // Create render texture
    mRenTex = Cast<RenderTexture2D>( GraphicSubsystem::Instance()->Create( RenderTexture2D::StaticClass() ) );
    mRenTex->Create( width, height, Image::Format_R8G8B8A8 );

    // Initialize render texture context
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    RenderTarget* bckTarget = renderer->GetRenderTarget();
    renderer->SetRenderTarget( mRenTex );
    {
        renderer->SetViewport( 0, 0, width, height);
        renderer->SetClearColor( Color4f(0.0f, 0.0f, 0.0f, 1.0f) );
        renderer->SetRenderState( Renderer::DepthTest, true );
        renderer->SetMatrixMode( Renderer::ProjectionMatrix );
        renderer->LoadIdentity();
	    renderer->Perspective( 45.0f, 1, 0.1f, 10.0f );	   
        renderer->SetMatrixMode( Renderer::ModelViewMatrix );
    }
    renderer->SetRenderTarget( bckTarget );
}

TestRenderTexture::~TestRenderTexture()
{
}

void TestRenderTexture::Update( Double pElapsedTime )
{
    mRotation += Maths::ToRadians(360.0f) * pElapsedTime;
}

void TestRenderTexture::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetRenderState( Renderer::Lighting, false );

    Quaternionf rot;
    rot.FromAxisRotation( Vector3f(1.0, 1.0f, -1.0f), mRotation );

    // Render a cube into texture
    RenderTarget* bckTarget = renderer->GetRenderTarget();
    renderer->SetRenderTarget( mRenTex );
    renderer->Clear( Renderer::ColorBuffer | Renderer::DepthBuffer );
    renderer->LoadIdentity();
    renderer->Translate( Vector3f(0.0f, 0.0f, -5.0f) );
    renderer->Rotate(rot);
    renderer->GetTextureStage(0)->SetTexture( *mTestTex );   
    renderer->DrawBox( Vector3f(-1,-1,-1), Vector3f(1,1,1) );
    renderer->GetTextureStage(0)->ResetTexture();
            
    // Render a cube to main window
    Float size = mSize * 0.5f;
    renderer->SetRenderTarget( bckTarget );
    renderer->GetTextureStage(0)->SetTexture( *mRenTex );    
    renderer->DrawBox( Vector3f(-size,-size,-size), Vector3f(size,size,size) );
    
    renderer->SetRenderState( Renderer::Lighting, true );
    renderer->GetTextureStage(0)->ResetTexture();
    renderer->SetColor( Color4f(1.0f, 1.0f, 1.0f, 1.0f) );    
}


} // namespace Gamedesk
