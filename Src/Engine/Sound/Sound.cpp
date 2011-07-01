/**
 *  @file       Sound.cpp
 *  @brief	    Resource data for a sound.
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
#include "Sound.h"
#include "SoundData.h"


namespace Gamedesk {
	
	
IMPLEMENT_ABSTRACT_CLASS(Sound);


Sound::Sound() : 
    mPlaying(false),
    mPaused(false),
    mMode(kSoundModeOneShot),
    mMuted(false),
    mVolume(255),
    mAbsoluteVolume(255),
    mPan(128),
    mStereoPan(false),
    mMinDistance(1.0f),
    mMaxDistance(1000000000.0f),
    mFrequency(-1),
    mPriority(128),
    mPosition(0, 0, 0)
{
}

Sound::~Sound()
{
}

void Sound::Create(SoundData* pSoundData, Bool /*pIs3DSound*/)
{
    mSoundData = pSoundData;
}

void Sound::Init()
{
    Super::Init();
}

void Sound::Kill()
{
    Super::Kill();
}

SoundData& Sound::GetSoundData()
{
    return *mSoundData;
}

Bool Sound::Play()
{
    mPlaying = true;
    mPaused = false;
    return true;
}

Bool Sound::Stop()
{
    mPlaying = false;
    mPaused = false;

    return true;
}

Bool Sound::IsPlaying()
{
    return mPlaying;
}

Bool Sound::SetPaused(Bool pPaused)
{
    mPaused = pPaused;
    mPlaying = !pPaused;
    return true;
}

Bool Sound::IsPaused() const
{
    return mPaused;
}

Bool Sound::SetMode(eSoundMode pMode)
{
    mMode = pMode;
    return true;
}

Sound::eSoundMode Sound::GetMode() const
{
    return mMode;
}

Bool Sound::SetMute(Bool pMute)
{
    mMuted = pMute;
    return true;
}

Bool Sound::IsMuted() const
{
    return mMuted;
}

Bool Sound::SetVolume(Int32 pVolume)
{
    mVolume = pVolume;
    mMuted = false;
    return true;
}

Int32 Sound::GetVolume() const
{
    return mVolume;
}

Bool Sound::SetAbsoluteVolume(Int32 pVolume)
{
    mAbsoluteVolume = pVolume;
    mMuted = false;
    return true;
}

Int32 Sound::GetAbsoluteVolume() const
{
    return mAbsoluteVolume;
}

Bool Sound::SetPan(Int32 pPan)
{
    mPan = pPan;
    mStereoPan = false;
    return true;
}

Int32 Sound::GetPan() const
{
    return mPan;
}

Bool Sound::SetStereoPan()
{
    mStereoPan = true;
    return true;
}

Bool Sound::IsStereoPan() const
{
    return mStereoPan;
}

Bool Sound::SetMinMaxDistance(Float pMininumDistance, Float pMaximumDistance)
{
    mMinDistance = pMininumDistance;
    mMaxDistance = pMaximumDistance;
    return true;
}

Bool Sound::SetMinDistance(Float pMininumDistance)
{
    return SetMinMaxDistance(pMininumDistance, mMaxDistance);
}

Float Sound::GetMinDistance() const
{
    return mMinDistance;
}

Bool Sound::SetMaxDistance(Float pMaximumDistance)
{
    return SetMinMaxDistance(mMinDistance, pMaximumDistance);
}

Float Sound::GetMaxDistance() const
{
    return mMaxDistance;
}

Bool Sound::SetAttributes(const Vector3f& pPosition, const Vector3f& /*pVelocity*/)
{
    mPosition = pPosition;
    return true;
}

Bool Sound::GetAttributes(Vector3f& /*pPosition*/, Vector3f& /*pVelocity*/)
{
    return true;
}

Vector3f Sound::GetPosition() const
{
    return mPosition;
}

Bool Sound::SetFrequency(Int32 pFrequency)
{
    mFrequency = pFrequency;
    return true;
}

Int32 Sound::GetFrequency() const
{
    return mFrequency;
}

Bool Sound::SetPriority(Int32 pPriority)
{
    mPriority = pPriority;
    return true;
}

Int32 Sound::GetPriority() const
{
    return mPriority;
}


} // namespace Gamedesk
