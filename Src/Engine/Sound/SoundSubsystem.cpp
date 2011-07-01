/**
 *  @file       SoundSubsystem.cpp
 *  @brief      The SoundSubsystem base class.
 *  @author     Marco Arsenault.
 *  @date       21/02/04.
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
#include "SoundSubsystem.h"


namespace Gamedesk {
	
	
IMPLEMENT_ABSTRACT_CLASS(SoundSubsystem);
IMPLEMENT_ABSTRACT_SINGLETON(SoundSubsystem);


const Int32 SoundSubsystem::kDefaultDriver = 0;
const Int32 SoundSubsystem::kNoSoundDriver = -1;


SoundSubsystem::SoundSubsystem()
{
    mInstance = this;
}

SoundSubsystem::~SoundSubsystem()
{
    mInstance = 0;
}

void SoundSubsystem::Init()
{
    Super::Init();

    mDistanceFactor = 1.0f;
    mDopplerFactor = 1.0f;
    mRolloffFactor = 1.0f;
    mSFXMasterVolume = 255;
    mListenerPosition = Vector3f(0, 0, 0);
    mSpeedRatio = 1.0f;
}

void SoundSubsystem::Kill()
{
    Super::Kill();
}

void SoundSubsystem::SetOutputRate(Int32 /*pOutputRate*/)
{
}

void SoundSubsystem::SetMaxNbSoftwareChannels(Int32 /*pNbSoftwareChannels*/)
{
}

Bool SoundSubsystem::SetDriver(Int32 /*pDriverIndex*/)
{
    return false;
}

Bool SoundSubsystem::SetOutputType(eOutputType /*pOutputType*/)
{
    return false;
}

Bool SoundSubsystem::SetMinNbHardwareChannels(Int32 /*pNbHardwareChannels*/)
{
    return false;
}

Bool SoundSubsystem::SetMixerType(eMixerType /*pMixerType*/)
{
    return false;
}

void SoundSubsystem::SetSFXMasterVolume(Int32 pVolume)
{
    mSFXMasterVolume = pVolume;
}

SoundSubsystem::eSpeakerMode SoundSubsystem::GetSpeakerMode() const
{
    return kSpeakerModeMono;
}

void SoundSubsystem::SetSpeakerMode(eSpeakerMode /*pSpeakerMode*/)
{
}

void SoundSubsystem::SetPanSeparation(Float /*pPanSeparation*/)
{
}

Int32 SoundSubsystem::GetNbPlayingChannels()
{
    return -1;
}

Int32 SoundSubsystem::GetNbChannels()
{
    return -1;
}

Int32 SoundSubsystem::GetNbDrivers()
{
    return -1;
}

SoundSubsystem::eOutputType SoundSubsystem::GetOutputType()
{
    return kOutputTypeAutoDetect;
}

SoundSubsystem::eMixerType SoundSubsystem::GetMixerType()
{
    return kMixerTypeAutoDetect;
}

Int32 SoundSubsystem::GetOutputRate()
{   
    return -1;
}

Int32 SoundSubsystem::GetDriver()
{
    return -1;
}

String SoundSubsystem::GetDriverName(Int32 /*pDriverIndex*/)
{
    return "";
}

void SoundSubsystem::Get3DListenerAttributes(Vector3f& /*pPosition*/, 
                                             Vector3f& /*pVelocity*/,
                                             Vector3f& /*pForward*/, 
                                             Vector3f& /*pUp*/)
{
}

void SoundSubsystem::Set3DListenerAttributes(const Vector3f& pPosition, 
                                             const Vector3f& /*pVelocity*/,
                                             const Vector3f& /*pForward*/, 
                                             const Vector3f& /*pUp*/)
{
    mListenerPosition = pPosition;
}

void SoundSubsystem::SetDistanceFactor(Float pScale)
{
    mDistanceFactor = pScale;
}

Float SoundSubsystem::GetDistanceFactor() const
{
    return mDistanceFactor;
}

void SoundSubsystem::SetDopplerFactor(Float pScale)
{
    mDopplerFactor = pScale;
}

Float SoundSubsystem::GetDopplerFactor() const
{
    return mDopplerFactor;
}

void SoundSubsystem::SetRolloffFactor(Float pScale)
{
    mRolloffFactor = pScale;
}

Float SoundSubsystem::GetRolloffFactor() const
{
    return mRolloffFactor;
}

void SoundSubsystem::Update(Double /*pElapsedTime*/,
                            const Vector3f& /*pListenerPosition*/,
                            const Vector3f& /*pListenerForward*/,
                            const Vector3f& /*pListenerUp*/)
{
}

Int32 SoundSubsystem::GetSFXMasterVolume()
{
    return mSFXMasterVolume;
}

void SoundSubsystem::SetSpeedRatio(Float pSpeedRatio)
{
    mSpeedRatio = pSpeedRatio;
}

Float SoundSubsystem::GetSpeedRatio() const
{
    return mSpeedRatio;
}


} // namespace Gamedesk
