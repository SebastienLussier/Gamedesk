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

#include "World/Entity.h"


namespace Gamedesk {
	
	
const UInt32 NUM_TEXTURE_TEST = 2;


class ENGINE_API TestTexture2D : public Entity
{
    DECLARE_CLASS(TestTexture2D, Entity);

public:
    TestTexture2D();
    virtual ~TestTexture2D();

    virtual void Update( Double pElapsedTime );
    virtual void Render() const;

private:
    Float       mTimeElapsed;
    UInt32      mCurrentImage;
    HTexture2D  mTexture2D[NUM_TEXTURE_TEST];
};

IMPLEMENT_CLASS(TestTexture2D);


TestTexture2D::TestTexture2D()
{
    mTimeElapsed  = 0;
    mCurrentImage = 0;
    mTexture2D[0].GetTexture( "Data/Tests/Textures/TestTexture2D_00.bmp" );
    mTexture2D[1].GetTexture( "Data/Tests/Textures/TestTexture2D_01.dds" );
}

TestTexture2D::~TestTexture2D()
{
}

void TestTexture2D::Update( Double pElapsedTime )
{
    mTimeElapsed += pElapsedTime;

    if( mTimeElapsed > 2.0f )
    {
        mTimeElapsed -= 2.0f;
        mCurrentImage++;

        mCurrentImage %= NUM_TEXTURE_TEST;
    }
}

void TestTexture2D::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->GetTextureStage(0)->SetTexture( *mTexture2D[mCurrentImage] );
    renderer->SetRenderState( Renderer::Lighting, false );

    renderer->BeginScene( Renderer::QuadList );
        renderer->SetUV(0.0f, 0.0f);
        renderer->SetVertex(-5.0f, -5.0f, 0.0f);
        
        renderer->SetUV(1.0f, 0.0f);
        renderer->SetVertex(5.0f, -5.0f, 0.0f);
        
        renderer->SetUV(1.0f, 1.0f);
        renderer->SetVertex(5.0f, 5.0f, 0.0f);
        
        renderer->SetUV(0.0f, 1.0f);
        renderer->SetVertex(-5.0f, 5.0f, 0.0f);
    renderer->EndScene();


    renderer->SetRenderState( Renderer::Lighting, true );
    renderer->GetTextureStage(0)->ResetTexture();
}


} // namespace Gamedesk
