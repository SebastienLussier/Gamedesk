/**
*  @file       TestShaderProgram.cpp
*  @author     Sébastien Lussier.
*  @date       04/06/05.
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

#include "Graphic/Mesh/Mesh.h"
#include "Graphic/Mesh/MeshHdl.h"

#include "Graphic/Shader/ShaderProgram.h"
#include "Graphic/Shader/ShaderObject.h"

#include "World/Entity.h"

#include "World/ParticleEmitter.h"


namespace Gamedesk {
	
	
class ENGINE_API TestShaderProgram : public Entity
{
    DECLARE_CLASS(TestShaderProgram, Entity);

public:
    TestShaderProgram();
    virtual ~TestShaderProgram();

    virtual void Update( Double pElapsedTime );
    virtual void Render() const;

private:
    ShaderObject*   mVertexShader;
    ShaderObject*   mPixelShader;
    ShaderProgram*  mShaderProgram;
    MeshHdl         mMesh;
    HTexture2D      mDiffuseMap;
    HTexture2D      mNormalMap;
    HTexture2D      mSpecularMap;
    Float           mTime;

    ParticleEmitter mEmitter;
};

IMPLEMENT_CLASS(TestShaderProgram);


TestShaderProgram::TestShaderProgram()
{
    mVertexShader  = Cast<ShaderObject>( GraphicSubsystem::Instance()->Create( ShaderObject::StaticClass() ) );
    mPixelShader   = Cast<ShaderObject>( GraphicSubsystem::Instance()->Create( ShaderObject::StaticClass() ) );
    mShaderProgram = Cast<ShaderProgram>( GraphicSubsystem::Instance()->Create( ShaderProgram::StaticClass() ) );

    mVertexShader->Create( ShaderObject::Shader_Vertex );
    mVertexShader->SetShaderTextFile( "Data/Tests/Shaders/TestShader.vs" );
    mVertexShader->Compile();

    mPixelShader->Create( ShaderObject::Shader_Pixel );
    mPixelShader->SetShaderTextFile( "Data/Tests/Shaders/TestShader.ps" );
    mPixelShader->Compile();
    
    mShaderProgram->AddShader( mVertexShader );
    mShaderProgram->AddShader( mPixelShader );
    mShaderProgram->Link();

    mDiffuseMap.GetTexture( "Data/Tests/Textures/tile_d.jpg" );
    mNormalMap.GetTexture( "Data/Tests/Textures/tile_h.png" );
    mSpecularMap.GetTexture( "Data/Tests/Textures/tile_s.png" );

    mNormalMap->GetImage().ToNormalMap();
    mNormalMap->Update();

    mEmitter.SetSizeStart( 7 );
    mEmitter.SetSizeStartRand( 2 );
    mEmitter.SetSizeEnd( 1 );
    mEmitter.SetSizeEnd( 2 );
    mEmitter.SetLife( 1 );
    mEmitter.SetBirthrate( 500 );
    mEmitter.SetMaxParticleCount( 2000 );

    mTime = 0;
}

TestShaderProgram::~TestShaderProgram()
{
    mVertexShader->Kill();
    mPixelShader->Kill();
    mShaderProgram->Kill();

    GD_DELETE(mVertexShader);
    GD_DELETE(mPixelShader);
    GD_DELETE(mShaderProgram);
}

void TestShaderProgram::Update( Double pElapsedTime )
{
    mTime += pElapsedTime * 0.25;

    mEmitter.Update( pElapsedTime );
    mEmitter.SetPosition( Vector3f(30 * cosf(4.23f * mTime), 30 * sinf(2.37f * mTime) * cosf(1.39f * mTime), 30 * sinf(3.12f * mTime)) );
}

void TestShaderProgram::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    Light light;
    light.mPosition = mEmitter.GetPosition();
    light.mAmbient  = Color4f(0.2f,0.2f, 0.2f,1.0f);
    light.mDiffuse  = Color4f(1.0f,0.6f, 0.6f,1.0f);
    light.mSpecular = Color4f(1.0f,0.6f, 0.6f,1.0f);
    light.mType = Renderer::LightPoint;
    renderer->SetRenderState( Renderer::Light_i, true, 0 );
    renderer->SetLight( 0, light );

    renderer->GetTextureStage(0)->SetTexture( *mDiffuseMap );
    renderer->GetTextureStage(1)->SetTexture( *mNormalMap );
    renderer->GetTextureStage(2)->SetTexture( *mSpecularMap );

    Material m;
    m.mDiffuse   = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
    m.mSpecular  = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
    m.mAmbient   = Color4f(0.1f, 0.1f, 0.1f, 1.0f);
    m.mShininess = 128.0f;
    renderer->SetMaterial( m );

    mShaderProgram->Apply();
    mShaderProgram->SetSampler( "DiffuseMap", 0 );
    mShaderProgram->SetSampler( "NormalMap", 1 );
    mShaderProgram->SetSampler( "SpecularMap", 2 );

    Matrix4f worldTransform;
    Matrix4f rotationMatrix;
    Matrix4f translationMatrix(Matrix4f::Translation(mPosition));

    mOrientation.ToMatrix(rotationMatrix);

    worldTransform = rotationMatrix * translationMatrix;
    Matrix4f invWorldTransform = worldTransform.GetAffineInverse();
    
    Vector3f lightPosition_objectSpace = invWorldTransform.Transform( light.mPosition );
    Vector3f eyePos_objectSpace        = invWorldTransform.Transform( renderer->GetViewPos() );

    mShaderProgram->SetUniform( "lightPosition_OS", lightPosition_objectSpace );
    mShaderProgram->SetUniform( "eyePosition_OS", eyePos_objectSpace );
    
    renderer->DrawBox( Vector3f(-10,-10,-10), Vector3f(10,10,10) );

    mShaderProgram->Done();

    Material m2;
    renderer->SetMaterial( m2 );
    renderer->GetTextureStage(0)->ResetTexture();
    renderer->GetTextureStage(1)->ResetTexture();
    renderer->GetTextureStage(2)->ResetTexture();

    renderer->PushMatrix();
    renderer->Translate(mEmitter.GetPosition());
    renderer->Rotate(mEmitter.GetOrientation());
    mEmitter.Render();
    renderer->PopMatrix();
}


} // namespace Gamedesk
