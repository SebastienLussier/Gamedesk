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
	
	
class ENGINE_API TestCubemap : public Entity
{
    DECLARE_CLASS(TestCubemap, Entity);

public:
    TestCubemap();
    virtual ~TestCubemap();

    virtual void Render() const;

private:
    HCubemap    mCubemap;
};

IMPLEMENT_CLASS(TestCubemap);


TestCubemap::TestCubemap()
{
    mCubemap.GetTexture( "Data/Tests/Textures/TestCubemap.dds" );
}

TestCubemap::~TestCubemap()
{
}

void TestCubemap::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->GetTextureStage(0)->SetTexture( *mCubemap );
    renderer->GetTextureStage(0)->SetTextureCoordGenMode( TextureStage::Gen_ReflectionMap );
    
    renderer->SetRenderState( Renderer::TextureGenS, true );
    renderer->SetRenderState( Renderer::TextureGenT, true );
    renderer->SetRenderState( Renderer::TextureGenR, true );

    renderer->SetRenderState( Renderer::Lighting, false );

    Matrix4f invViewMatrix;
    renderer->GetModelViewMatrix(invViewMatrix);
    invViewMatrix(3,0) = 0;
    invViewMatrix(3,1) = 0;
    invViewMatrix(3,2) = 0;
    invViewMatrix = invViewMatrix.Transpose();

    renderer->SetMatrixMode( Renderer::TextureMatrix );
    renderer->PushMatrix();
    renderer->LoadIdentity();
    renderer->MultMatrix( invViewMatrix );

    renderer->DrawSphere( 10, 50 );

    renderer->PopMatrix();
    renderer->SetMatrixMode( Renderer::ModelViewMatrix );

    renderer->SetRenderState( Renderer::Lighting, true );

    renderer->SetRenderState( Renderer::TextureGenS, false );
    renderer->SetRenderState( Renderer::TextureGenT, false );
    renderer->SetRenderState( Renderer::TextureGenR, false );

    renderer->GetTextureStage(0)->SetTextureCoordGenMode( TextureStage::Gen_Disabled );
    renderer->GetTextureStage(0)->ResetTexture();
}


} // namespace Gamedesk
