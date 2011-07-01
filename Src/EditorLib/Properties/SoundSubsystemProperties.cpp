/**
 *  @file       SkyDomeProperties.cpp
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       12/02/04.
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
#include "SoundSubsystemProperties.h"
#include "Sound/SoundSubsystem.h"


ImplementProperty( SoundSubsystemProperties, mSpeakerMode, UInt32, GetSpeakerMode, SetSpeakerMode );
ImplementProperty( SoundSubsystemProperties, mDistanceFactor, Float, GetDistanceFactor, SetDistanceFactor );
ImplementProperty( SoundSubsystemProperties, mDopplerFactor, Float, GetDopplerFactor, SetDopplerFactor );
ImplementProperty( SoundSubsystemProperties, mRolloffFactor, Float, GetRolloffFactor, SetRolloffFactor );
ImplementProperty( SoundSubsystemProperties, mSFXMasterVolume, Float, GetSFXMasterVolume, SetSFXMasterVolume );
ImplementProperty( SoundSubsystemProperties, mSpeedRatio, Float, GetSpeedRatio, SetSpeedRatio );


SoundSubsystemProperties::SoundSubsystemProperties()
{
    InitProperties();
}

SoundSubsystemProperties::~SoundSubsystemProperties()
{
}

void SoundSubsystemProperties::InitProperties()
{
    Super::InitProperties();

    RegisterProperty(SoundSubsystemProperties, "Speaker mode", mSpeakerMode);
    RegisterProperty(SoundSubsystemProperties, "Distance factor", mDistanceFactor);
    RegisterProperty(SoundSubsystemProperties, "Doppler factor", mDopplerFactor);
    RegisterProperty(SoundSubsystemProperties, "Rolloff factor", mRolloffFactor);
    RegisterProperty(SoundSubsystemProperties, "SFX Master Volume", mSFXMasterVolume);
    RegisterProperty(SoundSubsystemProperties, "Simulation speed", mSpeedRatio);

    mSpeakerMode.SetEnumValue( SoundSubsystem::kSpeakerModeMono,         "Mono" );
    mSpeakerMode.SetEnumValue( SoundSubsystem::kSpeakerModeHeadphone,    "Headphone" );
    mSpeakerMode.SetEnumValue( SoundSubsystem::kSpeakerModeStereo,       "Stereo" );
    mSpeakerMode.SetEnumValue( SoundSubsystem::kSpeakerModeQuad,         "Quad" );
    mSpeakerMode.SetEnumValue( SoundSubsystem::kSpeakerModeSurround,     "Surround" );
    mSpeakerMode.SetEnumValue( SoundSubsystem::kSpeakerModeDolbyDigital, "Dolby Digital" );

    mDistanceFactor.SetMinimum(0.00001f);
    mDistanceFactor.UseRangeValidation(true);

    mDopplerFactor.SetMinimum(0.00001f);
    mDopplerFactor.UseRangeValidation(true);

    mRolloffFactor.SetMinimum(0);
    mRolloffFactor.SetMaximum(10);
    mRolloffFactor.UseRangeValidation(true);
    mRolloffFactor.UseSlider(true);

    mSFXMasterVolume.SetMinimum(0);
    mSFXMasterVolume.SetMaximum(255);
    mSFXMasterVolume.UseRangeValidation(true);
    mSFXMasterVolume.UseSlider(true);

    mSpeedRatio.SetMinimum(0.1f);
    mSpeedRatio.SetMaximum(2.0f);
    mSpeedRatio.UseRangeValidation(true);
    mSpeedRatio.UseSlider(true, 100);
}

void SoundSubsystemProperties::SetSpeakerMode(UInt32 pSpeakerMode)
{
    SoundSubsystem::Instance()->SetSpeakerMode( (SoundSubsystem::eSpeakerMode)pSpeakerMode );
    FirePropertyChanged(&mSpeakerMode);
}
UInt32 SoundSubsystemProperties::GetSpeakerMode() const
{
    return SoundSubsystem::Instance()->GetSpeakerMode();
}

void SoundSubsystemProperties::SetDistanceFactor(Float pDistanceFactor)
{
    SoundSubsystem::Instance()->SetDistanceFactor(pDistanceFactor);
    FirePropertyChanged(&mDistanceFactor);
}

Float SoundSubsystemProperties::GetDistanceFactor() const
{
    return SoundSubsystem::Instance()->GetDistanceFactor();
}

void SoundSubsystemProperties::SetDopplerFactor(Float pDopplerFactor)
{
    SoundSubsystem::Instance()->SetDopplerFactor(pDopplerFactor);
    FirePropertyChanged(&mDopplerFactor);
}

Float SoundSubsystemProperties::GetDopplerFactor() const
{
    return SoundSubsystem::Instance()->GetDopplerFactor();
}

void SoundSubsystemProperties::SetRolloffFactor(Float pRolloffFactor)
{
    SoundSubsystem::Instance()->SetRolloffFactor(pRolloffFactor);
    FirePropertyChanged(&mRolloffFactor);
}

Float SoundSubsystemProperties::GetRolloffFactor() const
{
    return SoundSubsystem::Instance()->GetRolloffFactor();
}

void SoundSubsystemProperties::SetSFXMasterVolume(Float pSFXMasterVolume)
{   
    SoundSubsystem::Instance()->SetSFXMasterVolume(pSFXMasterVolume);
    FirePropertyChanged(&mSFXMasterVolume);
}

Float SoundSubsystemProperties::GetSFXMasterVolume() const
{
    return SoundSubsystem::Instance()->GetSFXMasterVolume();
}

void SoundSubsystemProperties::SetSpeedRatio(Float pSpeedRatio)
{
    SoundSubsystem::Instance()->SetSpeedRatio(pSpeedRatio);
    FirePropertyChanged(&mSpeedRatio);
}

Float SoundSubsystemProperties::GetSpeedRatio() const
{
    return SoundSubsystem::Instance()->GetSpeedRatio();
}

IMPLEMENT_CLASS(SoundSubsystemProperties);
