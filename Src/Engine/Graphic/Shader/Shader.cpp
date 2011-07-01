#include "Engine.h"
#include "Graphic/Shader/Shader.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/Texture/Texture.h"


namespace Gamedesk {
	
	
/**
 *	Base Shader class.
 */
Shader::~Shader()
{
}

void Shader::Prepare()
{
}

void Shader::Done()
{
}

Bool Shader::NeedBlending()
{
    return false;
}


/**
 *  Texture shader (with material)
 */
TextureShader::~TextureShader()
{
}

UInt32 TextureShader::GetPassCount()
{
    return 1;
}

void TextureShader::Apply( UInt32 /*iPass*/ )
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
        
    renderer->SetMaterial( mMaterial );
    
    if( mTexture )
        renderer->GetTextureStage(0)->SetTexture( *mTexture );
}

void TextureShader::Done()
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    renderer->GetTextureStage(0)->ResetTexture();
    renderer->SetMaterial( Material() );
}

TransparencyTextureShader::~TransparencyTextureShader()
{
}

void TransparencyTextureShader::Prepare()
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();

    renderer->SetRenderState( Renderer::DepthMask, false );
    renderer->SetRenderState( Renderer::Blend, true );
    renderer->SetBlendFunc( Renderer::BlendSrcAlpha, Renderer::BlendInvSrcAlpha );
}

void TransparencyTextureShader::Done()
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    renderer->SetRenderState( Renderer::Blend, false );
    renderer->SetRenderState( Renderer::DepthMask, true );

    TextureShader::Done();
}

Bool TransparencyTextureShader::NeedBlending()
{
    return true;
}

/**
 *  EnvMapShader,
 */
EnvMapShader::~EnvMapShader()
{
}

UInt32 EnvMapShader::GetPassCount()
{
    return 1;
}

void EnvMapShader::Apply( UInt32 /*iPass*/ )
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();

    renderer->GetTextureStage(0)->SetTextureCoordGenMode( TextureStage::Gen_ReflectionMap );
    renderer->SetRenderState( Renderer::TextureGenS, true );
    renderer->SetRenderState( Renderer::TextureGenT, true );
    renderer->SetRenderState( Renderer::TextureGenR, true );

    Matrix4f pView;
    Vector3f pos(0,0,0);
    Vector3f dir = renderer->GetViewDir();
    Vector3f up = renderer->GetViewUp();

    pView.LookAt( pos, pos+dir, up );
    
    renderer->SetMatrixMode( Renderer::TextureMatrix );
    renderer->PushMatrix();
    renderer->LoadIdentity();
    renderer->MultMatrix( pView );
    renderer->SetMatrixMode( Renderer::ModelViewMatrix );
                            
    renderer->GetTextureStage(0)->SetTexture( *mEnvironment );
}

void EnvMapShader::Done()
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    
    renderer->GetTextureStage(0)->SetTextureCoordGenMode( TextureStage::Gen_Disabled );
    renderer->SetRenderState( Renderer::TextureGenS, false );
    renderer->SetRenderState( Renderer::TextureGenT, false );
    renderer->SetRenderState( Renderer::TextureGenR, false );
    

    renderer->GetTextureStage(0)->ResetTexture();

    renderer->SetMatrixMode( Renderer::TextureMatrix );
    renderer->PopMatrix();
    renderer->SetMatrixMode( Renderer::ModelViewMatrix );
}


} // namespace Gamedesk
