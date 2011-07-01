/**
 *  @file       ParticleEmitter.cpp
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       08/04/04.
 */
/*
 *  Copyright (C) 2004 Gamedesk
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
#include "ParticleEmitter.h"

#include "Maths/Quaternion.h"
#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/Texture/Texture.h"
#include "Maths/Number.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(ParticleEmitter);


ParticleEmitter::ParticleEmitter()
{
    mParticleCount      = 0;

    mMaxParticleCount   = 250;
    mEmissionConeAngle  = 20;
    mBirthrate          = 200;

    mLife               = 0.5f;
    mLifeRand           = 0.1f;

    mSizeStart          = 0.7f;
    mSizeStartRand      = 0.2f;

    mSizeEnd            = 0.1f;
    mSizeEndRand        = 0.2f;

    mColorStart         = Color4f( 1.0f, 0.0f, 0.0f, 0.96f );
    mColorEnd           = Color4f( 0.86f, 0.47f, 0.04f, 0.0f );
     
    mGravity            = Vector3f( 0.0f, 0.0f, 0.0f );

    mInitialSpeed       = 2.0f;
    mInitialSpeedRand   = 0.5f;

    mAccel              = 0.1f;

    mOrientation        = Quaternionf( 1,0,1,0 );

    mTexture.GetTexture( "Data/Particles/particle0.bmp" );
    mTexture->SetWrapMode( Texture::Wrap_S, Texture::Wrap_Clamp );
    mTexture->SetWrapMode( Texture::Wrap_T, Texture::Wrap_Clamp );
    mTexture->SetMagFilter( Texture::MagFilter_Linear );
    mTexture->SetMinFilter( Texture::MinFilter_LinearMipmapNearest );

    Particle* particle;
    for( UInt32 i = 0; i < mMaxParticleCount; i++ )
    {
        particle = GD_NEW(Particle, this, "Engine::World::ParticleEmitter");
        particle->mDead = true;

        mParticles.push_back(particle);
        mDeadParticles.push_back(particle);
    }
}

ParticleEmitter::~ParticleEmitter()
{
    List<Particle*>::iterator itPart;
    for( itPart = mParticles.begin(); itPart != mParticles.end(); ++itPart )
    {
        GD_DELETE(*itPart);
    }
}

void ParticleEmitter::Update( Double pElapsedTime )
{
    Super::Update( pElapsedTime );

    List<Particle*>::iterator itPart;
    Particle* particle;
    Float     lifePercent;

    // Build a maximum sized bounding box.
    mBoundingBox = BoundingBox();

    for( itPart = mParticles.begin(); itPart != mParticles.end(); ++itPart )
    {
        particle = (*itPart);

        // Ignore dead particles.
        if( particle->mDead )
            continue;

        // Update life of the particle.
        particle->mLife -= pElapsedTime;
        if( particle->mLife < 0 )
        {
            particle->mDead = true;
            mParticleCount--;
            mDeadParticles.push_back( particle );
            continue;   // Particle is dead... we don't need to update it's properties.
        }

        // Update it's position.
        //particle->mVelocity *= mAccel * pElapsedTime;
        particle->mVelocity += mGravity * pElapsedTime;
        particle->mPosition += particle->mVelocity * pElapsedTime;
        
        mBoundingBox.Grow( particle->mPosition );

        lifePercent = 1.0f - (particle->mLife / particle->mInitialLife);

        // Update it's color.
        particle->mColor = mColorStart*(1-lifePercent) + mColorEnd*lifePercent;

        // Update it's size.
        particle->mSize =  particle->mInitialSize*(1-lifePercent) + particle->mFinalSize*lifePercent;
    }

    UInt32 toBeEmitted = pElapsedTime*mBirthrate;

    if( toBeEmitted > mBirthrate )
        toBeEmitted = mBirthrate;

    while( toBeEmitted > 0 && mParticleCount < mMaxParticleCount )
    {
        EmitParticle();
        toBeEmitted--;
    }

    Float sizeGrow = Maths::Max( mSizeStart + mSizeStartRand, mSizeEnd + mSizeEndRand ) * 0.5f;
    mBoundingBox(0).x -= sizeGrow;
    mBoundingBox(0).y -= sizeGrow;
    mBoundingBox(0).z -= sizeGrow;
    mBoundingBox(1).x += sizeGrow;
    mBoundingBox(1).y += sizeGrow;
    mBoundingBox(1).z += sizeGrow;

    // Take back the bounding box in the object's coordinate.
    /*Matrix4f translation = Matrix4f::Translation(-mPosition);
    Matrix4f rotation;
    (mOrientation.GetInverted()).ToMatrix(rotation);

    Matrix4f result = translation * rotation;
    mBoundingBox.mMin = result.Transform(mBoundingBox.mMin);
    mBoundingBox.mMax = result.Transform(mBoundingBox.mMax);*/
}

void ParticleEmitter::EmitParticle()
{
    // Unable to spawn new particles...
    if( mDeadParticles.empty() )
        return;

    Particle* particle = mDeadParticles.front();
    mDeadParticles.pop_front();

    particle->mDead = false;
    mParticleCount++;

    Float       angle;
    Vector3f    dir;
    mOrientation.ToAxisRotation( dir, angle );

    particle->mVelocity     = GetRandomVectorFromDir( dir, Maths::ToRadians((Float)mEmissionConeAngle) );
    particle->mVelocity    *= mInitialSpeed + Maths::Rand( -mInitialSpeedRand, mInitialSpeedRand );
    particle->mInitialSize  = mSizeStart + Maths::Rand( -mSizeStartRand, mSizeStartRand );
    particle->mFinalSize    = mSizeEnd + Maths::Rand( -mSizeEndRand, mSizeEndRand );
    particle->mSize         = particle->mInitialSize;
    particle->mColor        = mColorStart;
    particle->mInitialLife  = mLife + Maths::Rand( -mLifeRand, mLifeRand );
    particle->mLife         = particle->mInitialLife;
    particle->mPosition     = mPosition;
}

Vector3f ParticleEmitter::GetRandomVectorFromDir( const Vector3f& dir, Float angle )
{
    Vector3f  random;
    Float     cosAngle = Maths::Cos( angle );

    Float u = Maths::Rand( 0.0, 1.0 );
	Float v = Maths::Rand( 0.0, 1.0 );

    // transformation lineaire pour genere le bonne angle phi
    Float a = ( 1 - cosAngle ) / cosAngle;
    Float b = cosAngle;

    v = ( a * ( v * cosAngle ) ) + b;

    // trouve les coordonnées sphériques
    Float theta = 2 * Maths::PI * u;
    Float phi = Maths::ACos( v );

    // transforme les coordonnées sphérique en coordonnées universelle
    random.x = Maths::Cos( theta ) * Maths::Sin( phi );
    random.y = Maths::Sin( theta ) * Maths::Sin( phi );
    random.z = Maths::Cos( phi );

    Vector3f w = Vector3f::Z_AXIS cross dir;
    if( w.GetLengthSqr() == 0 )
    {
        w = Vector3f::Z_AXIS cross Vector3f::X_AXIS;
        if( w.GetLengthSqr() == 0.0 )
        {
            w = Vector3f::Z_AXIS cross Vector3f::Y_AXIS;
        }
    }

    return (random * Matrix4f::AxisRotation( Vector3f::Z_AXIS.GetAngleBetween(dir), w )).GetNormalized();
}

void ParticleEmitter::Render() const
{
    List<Particle*>::const_iterator itPart;
    Particle* particle;
   
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetRenderState( Renderer::Lighting, false );
        
    /*renderer->SetColor(Color4f(0.2f, 0.5f, 1.0f, 1.0f));
    Float size = 0.25f;
    Float sizeDiag = 0.17f;
    renderer->BeginScene(Renderer::LineList);
    renderer->SetVertex(Vector3f(-size, 0, 0));
    renderer->SetVertex(Vector3f( size, 0, 0));
    renderer->SetVertex(Vector3f(0,  size, 0));
    renderer->SetVertex(Vector3f(0, -size, 0));
    renderer->SetVertex(Vector3f(0, 0,  size));
    renderer->SetVertex(Vector3f(0, 0, -size));
    renderer->SetVertex(Vector3f(-sizeDiag, sizeDiag, -sizeDiag));
    renderer->SetVertex(Vector3f( sizeDiag,  -sizeDiag,  sizeDiag));
    renderer->SetVertex(Vector3f(-sizeDiag, sizeDiag, sizeDiag));
    renderer->SetVertex(Vector3f( sizeDiag, -sizeDiag, -sizeDiag));
    renderer->SetVertex(Vector3f( sizeDiag, sizeDiag, -sizeDiag));
    renderer->SetVertex(Vector3f( -sizeDiag, -sizeDiag, sizeDiag));
    renderer->SetVertex(Vector3f( sizeDiag, sizeDiag, sizeDiag));
    renderer->SetVertex(Vector3f(-sizeDiag, -sizeDiag, -sizeDiag));
    renderer->EndScene();*/

    renderer->GetTextureStage( 0 )->SetTexture( *mTexture );

    renderer->SetMatrixMode( Renderer::ModelViewMatrix );
    renderer->PopMatrix();
        
    Matrix4d modelView;
    renderer->GetModelViewMatrix( modelView );

    Vector3f vRight( modelView[0], modelView[4], modelView[8] );
    Vector3f vUp( modelView[1], modelView[5], modelView[9] );

    Vector3f vBottomLeft  = (-vRight - vUp).Normalize() * 0.5f;
    Vector3f vBottomRight = ( vRight - vUp).Normalize() * 0.5f;
    Vector3f vTopLeft     = (-vRight + vUp).Normalize() * 0.5f;
    Vector3f vTopRight    = ( vRight + vUp).Normalize() * 0.5f;

    Vector3f v0, v1, v2, v3;
    
    renderer->SetRenderState( Renderer::DepthMask, false );
    renderer->SetRenderState( Renderer::Lighting, false );
    renderer->SetRenderState( Renderer::Blend, true );
    renderer->SetBlendFunc( Renderer::BlendSrcAlpha, Renderer::BlendOne );

    for( itPart = mParticles.begin(); itPart != mParticles.end(); ++itPart )
    {
        particle = (*itPart);

        // Ignore dead particles.
        if( particle->mDead )
            continue;

        renderer->SetColor( particle->mColor.R, particle->mColor.G, particle->mColor.G, particle->mColor.A );
        v0 = particle->mPosition + vBottomLeft*particle->mSize;
        v1 = particle->mPosition + vBottomRight*particle->mSize;
        v2 = particle->mPosition + vTopRight*particle->mSize;
        v3 = particle->mPosition + vTopLeft*particle->mSize;
        renderer->DrawQuad( v0, v1, v2, v3 );            
    }

    renderer->PushMatrix();

    renderer->GetTextureStage( 0 )->ResetTexture();
    renderer->SetColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    renderer->SetRenderState( Renderer::DepthMask, true );
    renderer->SetRenderState( Renderer::Blend, false );
    renderer->SetRenderState( Renderer::Lighting, true );
}

void ParticleEmitter::RenderSelected() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetMatrixMode( Renderer::ModelViewMatrix );
    renderer->PopMatrix();

    Super::RenderSelected();

    renderer->PushMatrix();
}

void ParticleEmitter::SetBirthrate( UInt32 pBirthrate )
{
    mBirthrate = pBirthrate;
}
UInt32 ParticleEmitter::GetBirthrate() const
{
    return mBirthrate;
}

void ParticleEmitter::SetLife( Float pLife )
{
    mLife = pLife;
}
Float ParticleEmitter::GetLife() const
{
    return mLife;
}

void ParticleEmitter::SetLifeRand( Float pLifeRand )
{
    mLifeRand = pLifeRand;
}
Float ParticleEmitter::GetLifeRand() const
{
    return mLifeRand;
}

void ParticleEmitter::SetSizeStart( Float pSizeStart )
{
    mSizeStart = pSizeStart;
}
void ParticleEmitter::SetSizeStartRand( Float pSizeStartRand )
{
    mSizeStartRand = pSizeStartRand;
}
Float ParticleEmitter::GetSizeStart() const
{
    return mSizeStart;
}
Float ParticleEmitter::GetSizeStartRand() const
{
    return mSizeStartRand;
}


void ParticleEmitter::SetGravity( const Vector3f& pGravity )
{
    mGravity = pGravity;
}

const Vector3f& ParticleEmitter::GetGravity() const
{
    return mGravity;
}


void ParticleEmitter::SetSizeEnd( Float pSizeEnd )
{
    mSizeEnd = pSizeEnd;
}
void ParticleEmitter::SetSizeEndRand( Float pSizeEndRand )
{
    mSizeEndRand = pSizeEndRand;
}
Float ParticleEmitter::GetSizeEnd() const
{
    return mSizeEnd;
}
Float ParticleEmitter::GetSizeEndRand() const
{
    return mSizeEndRand;
}

void ParticleEmitter::SetColorStart( const Color4f& pColorStart )
{
    mColorStart = pColorStart;
}
const Color4f& ParticleEmitter::GetColorStart() const
{
    return mColorStart;
}

void ParticleEmitter::SetColorEnd( const Color4f& pColorEnd )
{
    mColorEnd = pColorEnd;
}
const Color4f& ParticleEmitter::GetColorEnd() const
{
    return mColorEnd;
}

void ParticleEmitter::SetInitialSpeed( Float pSpeed )
{
    mInitialSpeed = pSpeed;
}
Float ParticleEmitter::GetInitialSpeed() const
{
    return mInitialSpeed;
}
void ParticleEmitter::SetInitialSpeedRand( Float pSpeedRand )
{
    mInitialSpeedRand = pSpeedRand;
}
Float ParticleEmitter::GetInitialSpeedRand() const
{
    return mInitialSpeedRand;
}

void ParticleEmitter::SetAccel( Float pAccel )
{
    mAccel = pAccel;
}
Float ParticleEmitter::GetAccel() const
{
    return mAccel;
}

void ParticleEmitter::SetEmissionConeAngle( UInt32 pConeAngle )
{
    mEmissionConeAngle = pConeAngle;
}

UInt32 ParticleEmitter::GetEmissionConeAngle() const
{
    return mEmissionConeAngle;
}

void ParticleEmitter::SetMaxParticleCount( UInt32 pMaxParticleCount )
{
    if( pMaxParticleCount > mMaxParticleCount )
    {
        while( mMaxParticleCount != pMaxParticleCount )
        {
            Particle* particle;
       
            particle = GD_NEW(Particle, this, "Engine::World::ParticleEmitter");
            particle->mDead = true;

            mParticles.push_back(particle);
            mDeadParticles.push_back(particle);

            mMaxParticleCount++;
        }
    }
    else
    {
        while( mMaxParticleCount != pMaxParticleCount )
        {
            Particle* particle = mParticles.front();
            mParticles.pop_front();

            if( particle->mDead )
                mDeadParticles.remove( particle );
            else
                mParticleCount--;

            GD_DELETE(particle);
            mMaxParticleCount--;
        }
    }
}


UInt32 ParticleEmitter::GetMaxParticleCount() const
{
    return mMaxParticleCount;
}

void ParticleEmitter::Serialize( Stream& pStream )
{
    Super::Serialize( pStream );

    UInt32 num = mParticles.size();
    pStream << num;

    if( pStream.In() )
        mParticles.resize(num);
    
    for( List<Particle*>::iterator itPart = mParticles.begin(); itPart != mParticles.end(); ++itPart )
    {
        Particle* p = *itPart;

        pStream << p->mDead;

        if( p->mDead )
            continue;

        pStream << p->mPosition;
        pStream << p->mVelocity;
        pStream << p->mColor;
        pStream << p->mInitialSize;
        pStream << p->mFinalSize;
        pStream << p->mInitialLife;
        pStream << p->mLife;        
    }
        
    pStream << mTexture;
    pStream << mParticleCount;
    pStream << mMaxParticleCount;
    pStream << mBirthrate;
    pStream << mLife;
    pStream << mLifeRand;
    pStream << mSizeStart;
    pStream << mSizeStartRand;
    pStream << mSizeEnd;
    pStream << mSizeEndRand;
    pStream << mColorStart;
    pStream << mColorEnd;
    pStream << mGravity;
    pStream << mInitialSpeed;
    pStream << mInitialSpeedRand;
    pStream << mEmissionConeAngle;
    pStream << mAccel;
}


} // namespace Gamedesk
