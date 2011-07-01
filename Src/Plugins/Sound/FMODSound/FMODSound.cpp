/**
 *  @file       FMODSound.cpp
 *  @brief	    Resource data for a fmod sound.
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
#include "FMODSoundDll.h"
#include "FMODSound.h"
#include "FMODTables.h"
#include "Sound/SoundData.h"
#include "Sound/SoundSubsystem.h"


namespace Gamedesk {
	

IMPLEMENT_CLASS(FMODSound);


FMODSound::FMODSound() : 
    mFMODSample(0),
    mChannel(-1)
{
}

FMODSound::~FMODSound()
{
}

void FMODSound::Create(SoundData* pSoundData, Bool pIs3DSound)
{
    Sound::Create(pSoundData);

    GD_ASSERT(pSoundData);

	// Build the sound flags.
	UInt32 soundFlags = FSOUND_LOADMEMORY;
	if(pIs3DSound)
	{
		soundFlags = soundFlags | FSOUND_HW3D | FSOUND_FORCEMONO;
	}
    else
    {
        soundFlags = soundFlags | FSOUND_HW2D;
    }

    mFMODSample = FSOUND_Sample_Load(FSOUND_FREE, pSoundData->GetFileData(), 
                                     soundFlags, 0, pSoundData->GetFileDataSize());

    // Set the frequency
    Sound::SetFrequency(pSoundData->GetSampleRate());
    GD_ASSERT(mFMODSample);
}

void FMODSound::Init()
{
    Super::Init();
}

void FMODSound::Kill()
{
    Super::Kill();
    
    if(mFMODSample)
		FSOUND_Sample_Free(mFMODSample);
}

Bool FMODSound::Play()
{
    Sound::Play();

    if(mFMODSample)
    {
        // Set the default properties.
        UChar result = FSOUND_Sample_SetDefaults(mFMODSample, GetFrequency(), 
                                                              GetVolume(),
                                                              GetPan(),
                                                              GetPriority());
        if(result == 0)
            return false;

        // Play the sound.
		mChannel = FSOUND_PlaySound(FSOUND_FREE, mFMODSample);
        if(mChannel == -1)  
            return false;

        // Set the position.
        Bool success = SetAttributes(GetPosition(), Vector3f(0, 0, 0));
        if(!success)
            return false;

        // Mute the sound if needed.
        success = SetMute(IsMuted());
        if(!success)
            return false;
    }

    return true;
}

Bool FMODSound::Stop()
{
    Sound::Stop();

    if(mChannel != -1)
    {
        Int32 channel = mChannel;
        mChannel = -1;
        return FSOUND_StopSound(channel) != 0;
    }

    return false;
}

Bool FMODSound::IsPlaying()
{
    Bool isPlaying = Sound::IsPlaying();
    
    if(mChannel != -1)
        GD_ASSERT(isPlaying == (FSOUND_IsPlaying(mChannel) != 0));

    return isPlaying;
}

Bool FMODSound::SetPaused(Bool pPaused)
{
    Sound::SetPaused(pPaused);
    if(mChannel != -1)
        return FSOUND_SetPaused(mChannel, pPaused) != 0;

        return false;
}

Bool FMODSound::IsPaused() const
{
    Bool isPaused = Sound::IsPaused();

    if(mChannel != -1)
        GD_ASSERT(isPaused == (FSOUND_GetPaused(mChannel) != 0));

    return isPaused;
}

Bool FMODSound::SetMode(eSoundMode pMode)
{
    Sound::SetMode(pMode);

    if(mFMODSample)
        return FSOUND_Sample_SetMode(mFMODSample, GDToFMODSoundMode[pMode]) != 0;

    return false;
}

Sound::eSoundMode FMODSound::GetMode() const
{
    return Sound::GetMode();
}


Bool FMODSound::SetMute(Bool pMute)
{
    Sound::SetMute(pMute);

    if(mChannel != -1)
        return FSOUND_SetMute(mChannel, pMute) != 0;

    return false;
}

Bool FMODSound::IsMuted() const
{
    Bool isMuted = Sound::IsMuted();

    if(mChannel != -1)
        GD_ASSERT(isMuted == (FSOUND_GetMute(mChannel) != 0));

    return isMuted;
}

Bool FMODSound::SetVolume(Int32 pVolume)
{
    Sound::SetVolume(pVolume);

    if(mChannel != -1)
        return FSOUND_SetVolume(mChannel, pVolume) != 0;

    return false;
}

Int32 FMODSound::GetVolume() const
{
    return Sound::GetVolume();
}

Bool FMODSound::SetAbsoluteVolume(Int32 pVolume)
{
    Sound::SetAbsoluteVolume(pVolume);

    if(mChannel != -1)
        return FSOUND_SetVolumeAbsolute(mChannel, pVolume) != 0;

    return false;
}

Int32 FMODSound::GetAbsoluteVolume() const
{
    return Sound::GetAbsoluteVolume();
}

Bool FMODSound::SetPan(Int32 pPan)
{
    Sound::SetPan(pPan);

    if(mChannel != -1)
        return FSOUND_SetPan(mChannel, pPan) != 0;

    return false;
}

Int32 FMODSound::GetPan() const
{
    return Sound::GetPan();
}

Bool FMODSound::SetStereoPan()
{
    Sound::SetStereoPan();

    if(mChannel != -1)
        return FSOUND_SetPan(mChannel, FSOUND_STEREOPAN) != 0;

    return false;
}

Bool FMODSound::IsStereoPan() const
{
    return Sound::IsStereoPan();
}

Bool FMODSound::SetMinMaxDistance(Float pMininumDistance, Float pMaximumDistance)
{
    Sound::SetMinMaxDistance(pMininumDistance, pMaximumDistance);

    if(mFMODSample)
    {
        Bool isPlaying = IsPlaying();
        if(isPlaying)
            Stop();

        Bool result = FSOUND_Sample_SetMinMaxDistance(mFMODSample, 
							                           pMininumDistance, 
									                   pMaximumDistance) != 0;

        if( result && isPlaying )
            Play();

		return result;
    }

    return false;
}

Bool FMODSound::SetMinDistance(Float pMininumDistance)
{
    return Sound::SetMinDistance(pMininumDistance);
}

Float FMODSound::GetMinDistance() const
{
    return Sound::GetMinDistance();
}

Bool FMODSound::SetMaxDistance(Float pMaximumDistance)
{
    return Sound::SetMaxDistance(pMaximumDistance);
}

Float FMODSound::GetMaxDistance() const
{
    return Sound::GetMaxDistance();
}

Bool FMODSound::SetAttributes(const Vector3f& pPosition, const Vector3f& pVelocity)
{
    Sound::SetAttributes(pPosition, pVelocity);

    if(mChannel != -1)
    {
        // Invert the z coordinates cause FMOD is left-handed.
        Vector3f position(pPosition.x, pPosition.y, -pPosition.z);
        Vector3f velocity(pVelocity.x, pVelocity.y, -pVelocity.z);
        return FSOUND_3D_SetAttributes(mChannel, position, velocity) != 0;
    }
    
    return false;
}

Bool FMODSound::GetAttributes(Vector3f& pPosition, Vector3f& pVelocity)
{
    if(mChannel != -1)
    {
        Bool result = FSOUND_3D_GetAttributes(mChannel, pPosition, pVelocity) != 0;
        // Invert the z coordinates cause FMOD is left-handed.
        pPosition.z = -pPosition.z;
        pVelocity.z = -pVelocity.z;
        return result;
    }

    return false;
}

Bool FMODSound::SetFrequency(Int32 pFrequency)
{
    Sound::SetFrequency(pFrequency);

    if(mChannel != -1)
    {
        return FSOUND_SetFrequency(mChannel, pFrequency * SoundSubsystem::Instance()->GetSpeedRatio()) != 0;
    }

    return false;
}

Int32 FMODSound::GetFrequency() const
{
    return Sound::GetFrequency();
}

Bool FMODSound::SetPriority(Int32 pPriority)
{
    Sound::SetPriority(pPriority);

    if(mChannel != -1)
        return FSOUND_SetPriority(mChannel, pPriority) != 0;

    return false;
}

Int32 FMODSound::GetPriority() const
{
    return Sound::GetPriority();
}


} // namespace Gamedesk
