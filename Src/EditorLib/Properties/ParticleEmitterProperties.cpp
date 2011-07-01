/**
 *  @file       ParticleEmitterProperties.cpp
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       31/03/04.
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
#include "EditorLib.h"
#include "ParticleEmitterProperties.h"
#include "World/ParticleEmitter.h"


ImplementProperty( ParticleEmitterProperties, mMaxParticleCount, UInt32, GetMaxParticleCount, SetMaxParticleCount );
ImplementProperty( ParticleEmitterProperties, mEmissionConeAngle, UInt32, GetEmissionConeAngle, SetEmissionConeAngle );
ImplementProperty( ParticleEmitterProperties, mBirthrate, UInt32, GetBirthrate, SetBirthrate );
ImplementProperty( ParticleEmitterProperties, mLife, Float, GetLife, SetLife );
ImplementProperty( ParticleEmitterProperties, mLifeRand, Float, GetLifeRand, SetLifeRand );
ImplementProperty( ParticleEmitterProperties, mSizeStart, Float, GetSizeStart, SetSizeStart );
ImplementProperty( ParticleEmitterProperties, mSizeStartRand, Float, GetSizeStartRand, SetSizeStartRand );
ImplementProperty( ParticleEmitterProperties, mSizeEnd, Float, GetSizeEnd, SetSizeEnd );
ImplementProperty( ParticleEmitterProperties, mSizeEndRand, Float, GetSizeEndRand, SetSizeEndRand );
ImplementProperty( ParticleEmitterProperties, mColorStart, Color4f, GetColorStart, SetColorStart );
ImplementProperty( ParticleEmitterProperties, mColorEnd, Color4f, GetColorEnd, SetColorEnd );
ImplementProperty( ParticleEmitterProperties, mInitialSpeed, Float, GetInitialSpeed, SetInitialSpeed );
ImplementProperty( ParticleEmitterProperties, mInitialSpeedRand, Float, GetInitialSpeedRand, SetInitialSpeedRand );
ImplementProperty( ParticleEmitterProperties, mAccel, Float, GetAccel, SetAccel );
ImplementProperty( ParticleEmitterProperties, mGravity, Vector3f, GetGravity, SetGravity );


ParticleEmitterProperties::ParticleEmitterProperties()
{
}

ParticleEmitterProperties::~ParticleEmitterProperties()
{
}

void ParticleEmitterProperties::InitProperties()
{
    Super::InitProperties();

    RegisterProperty( ParticleEmitterProperties, "Max Particle Count", mMaxParticleCount );
    RegisterProperty( ParticleEmitterProperties, "Emission Cone Cutoff", mEmissionConeAngle );
    RegisterProperty( ParticleEmitterProperties, "Birthrate", mBirthrate );
    RegisterProperty( ParticleEmitterProperties, "Life", mLife );
    RegisterProperty( ParticleEmitterProperties, "Life Random", mLifeRand );
    RegisterProperty( ParticleEmitterProperties, "Size Start", mSizeStart );
    RegisterProperty( ParticleEmitterProperties, "Size Start Random", mSizeStartRand );    
    RegisterProperty( ParticleEmitterProperties, "Size End", mSizeEnd );
    RegisterProperty( ParticleEmitterProperties, "Size End Random", mSizeEndRand );
    RegisterProperty( ParticleEmitterProperties, "Color Start", mColorStart );
    RegisterProperty( ParticleEmitterProperties, "Color End", mColorEnd );    
    RegisterProperty( ParticleEmitterProperties, "Initial Speed", mInitialSpeed );
    RegisterProperty( ParticleEmitterProperties, "Initial Speed Random", mInitialSpeedRand );
    RegisterProperty( ParticleEmitterProperties, "Acceleration", mAccel );
    RegisterProperty( ParticleEmitterProperties, "Gravity", mGravity );
   

    mMaxParticleCount.SetMinimum( 0 );
    mMaxParticleCount.SetMaximum( 10000 );
    mMaxParticleCount.UseRangeValidation( true );
    mMaxParticleCount.UseSlider( true );

    mEmissionConeAngle.SetMinimum( 1 );
    mEmissionConeAngle.SetMaximum( 180 );
    mEmissionConeAngle.UseRangeValidation( true );
    mEmissionConeAngle.UseSlider( true );
    
    mBirthrate.SetMinimum( 0 );
    mBirthrate.SetMaximum( 10000 );
    mBirthrate.UseRangeValidation( true );
    mBirthrate.UseSlider( true, 100 );

    mLife.SetMinimum( 0.1f );
    mLife.SetMaximum( 10.0f );
    mLife.UseRangeValidation( true );
    mLife.UseSlider( true, 10 );

    mLifeRand.SetMinimum( 0.0f );
    mLifeRand.SetMaximum( 10.0f );
    mLifeRand.UseRangeValidation( true );
    mLifeRand.UseSlider( true, 10 );

    mSizeStart.SetMinimum( 0.1f );
    mSizeStart.SetMaximum( 10.0f );
    mSizeStart.UseRangeValidation( true );
    mSizeStart.UseSlider( true, 10 );

    mSizeStartRand.SetMinimum( 0.0f );
    mSizeStartRand.SetMaximum( 10.0f );
    mSizeStartRand.UseRangeValidation( true );
    mSizeStartRand.UseSlider( true, 10 );

    mSizeEnd.SetMinimum( 0.1f );
    mSizeEnd.SetMaximum( 10.0f );
    mSizeEnd.UseRangeValidation( true );
    mSizeEnd.UseSlider( true, 10 );

    mSizeEndRand.SetMinimum( 0.0f );
    mSizeEndRand.SetMaximum( 10.0f );
    mSizeEndRand.UseRangeValidation( true );
    mSizeEndRand.UseSlider( true, 10 );

    mInitialSpeed.SetMinimum( 0.1f );
    mInitialSpeed.SetMaximum( 10.0f );
    mInitialSpeed.UseRangeValidation( true );
    mInitialSpeed.UseSlider( true, 10 );

    mInitialSpeedRand.SetMinimum( 0.0f );
    mInitialSpeedRand.SetMaximum( 10.0f );
    mInitialSpeedRand.UseRangeValidation( true );
    mInitialSpeedRand.UseSlider( true, 10 );

    mAccel.SetMinimum( 0.1f );
    mAccel.SetMaximum( 10.0f );
    mAccel.UseRangeValidation( true );
    mAccel.UseSlider( true, 10 );
}

void ParticleEmitterProperties::SetBirthrate( UInt32 pBirthrate )
{
    Cast<ParticleEmitter>(GetObject())->SetBirthrate(pBirthrate);
    FirePropertyChanged(&mBirthrate);
}
UInt32 ParticleEmitterProperties::GetBirthrate() const
{
    return Cast<ParticleEmitter>(GetObject())->GetBirthrate();
}

void ParticleEmitterProperties::SetLife( Float pLife )
{
    Cast<ParticleEmitter>(GetObject())->SetLife(pLife);
    FirePropertyChanged(&mLife);
}
Float ParticleEmitterProperties::GetLife() const
{
    return Cast<ParticleEmitter>(GetObject())->GetLife();
}

void ParticleEmitterProperties::SetLifeRand( Float pLifeRand )
{
    Cast<ParticleEmitter>(GetObject())->SetLifeRand(pLifeRand);
    FirePropertyChanged(&mLifeRand);
}
Float ParticleEmitterProperties::GetLifeRand() const
{
    return Cast<ParticleEmitter>(GetObject())->GetLifeRand();
}

void ParticleEmitterProperties::SetSizeStart( Float pSizeStart )
{
    Cast<ParticleEmitter>(GetObject())->SetSizeStart(pSizeStart);
    FirePropertyChanged(&mSizeStart);
}

void ParticleEmitterProperties::SetSizeStartRand( Float pSizeStartRand )
{
    Cast<ParticleEmitter>(GetObject())->SetSizeStartRand(pSizeStartRand);
    FirePropertyChanged(&mSizeStartRand);
}

Float ParticleEmitterProperties::GetSizeStart() const
{
    return Cast<ParticleEmitter>(GetObject())->GetSizeStart();
}
Float ParticleEmitterProperties::GetSizeStartRand() const
{
    return Cast<ParticleEmitter>(GetObject())->GetSizeStartRand();
}

void ParticleEmitterProperties::SetSizeEnd( Float pSizeEnd )
{
    Cast<ParticleEmitter>(GetObject())->SetSizeEnd(pSizeEnd);
    FirePropertyChanged(&mSizeEnd);
}

void ParticleEmitterProperties::SetSizeEndRand( Float pSizeEndRand )
{
    Cast<ParticleEmitter>(GetObject())->SetSizeEndRand(pSizeEndRand);
    FirePropertyChanged(&mSizeEndRand);
}

Float ParticleEmitterProperties::GetSizeEnd() const
{
    return Cast<ParticleEmitter>(GetObject())->GetSizeEnd();
}
Float ParticleEmitterProperties::GetSizeEndRand() const
{
    return Cast<ParticleEmitter>(GetObject())->GetSizeEndRand();
}

void ParticleEmitterProperties::SetColorStart( const Color4f& pColorStart )
{
    Cast<ParticleEmitter>(GetObject())->SetColorStart(pColorStart);
    FirePropertyChanged(&mColorStart);
}

const Color4f& ParticleEmitterProperties::GetColorStart() const
{
    return Cast<ParticleEmitter>(GetObject())->GetColorStart();
}

void ParticleEmitterProperties::SetColorEnd( const Color4f& pColorEnd )
{
    Cast<ParticleEmitter>(GetObject())->SetColorEnd(pColorEnd);
    FirePropertyChanged(&mColorEnd);
}

const Color4f& ParticleEmitterProperties::GetColorEnd() const
{
    return Cast<ParticleEmitter>(GetObject())->GetColorEnd();
}

void ParticleEmitterProperties::SetInitialSpeed( Float pSpeed )
{
    Cast<ParticleEmitter>(GetObject())->SetInitialSpeed(pSpeed);
    FirePropertyChanged(&mInitialSpeed);
}

Float ParticleEmitterProperties::GetInitialSpeed() const
{
    return Cast<ParticleEmitter>(GetObject())->GetInitialSpeed();
}

void ParticleEmitterProperties::SetInitialSpeedRand( Float pSpeedRand )
{
    Cast<ParticleEmitter>(GetObject())->SetInitialSpeedRand(pSpeedRand);
    FirePropertyChanged(&mInitialSpeedRand);
}

Float ParticleEmitterProperties::GetInitialSpeedRand() const
{
    return Cast<ParticleEmitter>(GetObject())->GetInitialSpeedRand();
}

void ParticleEmitterProperties::SetAccel( Float pAccel )
{
    Cast<ParticleEmitter>(GetObject())->SetAccel(pAccel);
    FirePropertyChanged(&mAccel);
}

Float ParticleEmitterProperties::GetAccel() const
{
    return Cast<ParticleEmitter>(GetObject())->GetAccel();
}

void ParticleEmitterProperties::SetGravity( const Vector3f& pGravity )
{
    Cast<ParticleEmitter>(GetObject())->SetGravity(pGravity);
    FirePropertyChanged(&mGravity);
}

const Vector3f& ParticleEmitterProperties::GetGravity() const
{
    return Cast<ParticleEmitter>(GetObject())->GetGravity();
}

void ParticleEmitterProperties::SetEmissionConeAngle( UInt32 pConeAngle )
{
    Cast<ParticleEmitter>(GetObject())->SetEmissionConeAngle(pConeAngle);
    FirePropertyChanged(&mEmissionConeAngle);
}

UInt32 ParticleEmitterProperties::GetEmissionConeAngle() const
{
    return Cast<ParticleEmitter>(GetObject())->GetEmissionConeAngle();
}

void ParticleEmitterProperties::SetMaxParticleCount( UInt32 pMaxParticleCount )
{
    Cast<ParticleEmitter>(GetObject())->SetMaxParticleCount(pMaxParticleCount);
    FirePropertyChanged(&mMaxParticleCount);
}

UInt32 ParticleEmitterProperties::GetMaxParticleCount() const
{
    return Cast<ParticleEmitter>(GetObject())->GetMaxParticleCount();
}


IMPLEMENT_CLASS(ParticleEmitterProperties);
