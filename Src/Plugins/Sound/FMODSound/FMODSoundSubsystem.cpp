/**
 *  @file       FMODSoundSubsystem.h
 *  @brief      Fmod Sound Subsystem..
 *  @author     Marco Arsenault.
 *  @date       13/02/04.
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
#include "FMODSoundSubsystem.h"
#include "FMODSound.h"
#include "FMODTables.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(FMODSoundSubsystem);


FMODSoundSubsystem::FMODSoundSubsystem() : 
    mOutputRate(44100),
    mMaxNbSoftwareChannels(32),
    mSpeakerMode(kSpeakerModeStereo)
{
    
    mLastListenerPosition = Vector3f(0, 0, 0);
}

FMODSoundSubsystem::~FMODSoundSubsystem()
{
}

void FMODSoundSubsystem::Init()
{
Core::DebugOut("[FMODSoundSubsystem::Init()...\n");
    Super::Init();
    /*
    SetDriver(SoundSubsystem::kDefaultDriver);
    SetMixerType(SoundSubsystem::kMixerTypeAutoDetect);
    SetOutputType(SoundSubsystem::kOutputTypeDirectSound);
    SetSpeakerMode(SoundSubsystem::kSpeakerModeQuad);
     
    StartSoundSystem();*/
}

void FMODSoundSubsystem::Kill()
{
    Super::Kill();
    StopSoundSystem();
}

Object* FMODSoundSubsystem::Create(Class* pResourceClass)
{
    if(pResourceClass == Sound::StaticClass())
    {
        return GD_NEW(FMODSound, this, "FMODSound");
    }
    
    return Super::Create(pResourceClass);
}

Bool FMODSoundSubsystem::StartSoundSystem()
{
    GD_ASSERT(FSOUND_Init(mOutputRate, mMaxNbSoftwareChannels, 0) != 0);
    SetRolloffFactor(0.5f);
    return true;
}

Bool FMODSoundSubsystem::StopSoundSystem()
{
    FSOUND_Close();
    return true;
}


void FMODSoundSubsystem::SetOutputRate(Int32 pOutputRate)
{
    mOutputRate = pOutputRate;
}

void FMODSoundSubsystem::SetMaxNbSoftwareChannels(Int32 pNbSoftwareChannels)
{
    mMaxNbSoftwareChannels = pNbSoftwareChannels;
}

Bool FMODSoundSubsystem::SetDriver(Int32 pDriverIndex)
{
    return FSOUND_SetDriver(pDriverIndex) != 0;
}

Bool FMODSoundSubsystem::SetOutputType(SoundSubsystem::eOutputType pOutputType)
{
    mOutputType = pOutputType;
    return FSOUND_SetOutput(GDToFMODOutputType[pOutputType]) != 0;
}

Bool FMODSoundSubsystem::SetMinNbHardwareChannels(Int32 pNbHardwareChannels)
{
    return FSOUND_SetMinHardwareChannels(pNbHardwareChannels) != 0;
}

Bool FMODSoundSubsystem::SetMixerType(SoundSubsystem::eMixerType pMixerType)
{
    mMixerType = pMixerType;
    return FSOUND_SetMixer(GDToFMODMixerType[pMixerType]) != 0;
}

void FMODSoundSubsystem::SetSFXMasterVolume(Int32 pVolume)
{
    Super::SetSFXMasterVolume(pVolume);
    FSOUND_SetSFXMasterVolume(pVolume);
}

SoundSubsystem::eSpeakerMode FMODSoundSubsystem::GetSpeakerMode() const
{
    return mSpeakerMode;
}

void FMODSoundSubsystem::SetSpeakerMode(SoundSubsystem::eSpeakerMode pSpeakerMode)
{
    mSpeakerMode = pSpeakerMode;
    FSOUND_SetSpeakerMode(GDToFMODSpeakerMode[mSpeakerMode]);
}

void FMODSoundSubsystem::SetPanSeparation(Float pPanSeparation)
{
    FSOUND_SetPanSeperation(pPanSeparation);
}

Int32 FMODSoundSubsystem::GetNbPlayingChannels()
{
    return FSOUND_GetChannelsPlaying();
}

Int32 FMODSoundSubsystem::GetNbChannels()
{
    return FSOUND_GetMaxChannels();
}

Int32 FMODSoundSubsystem::GetNbDrivers()
{
    return FSOUND_GetNumDrivers();
}

SoundSubsystem::eOutputType FMODSoundSubsystem::GetOutputType()
{
    return mOutputType;
}

SoundSubsystem::eMixerType FMODSoundSubsystem::GetMixerType()
{
    return mMixerType;
}

Int32 FMODSoundSubsystem::GetOutputRate()
{   
    return mOutputRate;
}

Int32 FMODSoundSubsystem::GetDriver()
{
    return FSOUND_GetDriver();
}

String FMODSoundSubsystem::GetDriverName(Int32 pDriverIndex)
{
    return String(FSOUND_GetDriverName(pDriverIndex));
}

void FMODSoundSubsystem::Get3DListenerAttributes(Vector3f& pPosition, 
                                                 Vector3f& pVelocity,
                                                 Vector3f& pForward, 
                                                 Vector3f& pUp)
{
    FSOUND_3D_Listener_GetAttributes(pPosition, pVelocity,
                                     &pForward.x, &pForward.y, &pForward.z,
                                     &pUp.x, &pUp.y, &pUp.z);

    // Invert the z coordinates.
    pPosition.z = -pPosition.z;
    pVelocity.z = -pVelocity.z;
    pForward.z = -pForward.z;
    pUp.z = -pUp.z;
}

void FMODSoundSubsystem::Set3DListenerAttributes(const Vector3f& pPosition, 
                                                 const Vector3f& pVelocity,
                                                 const Vector3f& pForward, 
                                                 const Vector3f& pUp)
{
    // Invert the z coordinates.
    Vector3f position(pPosition.x, pPosition.y, -pPosition.z);
    Vector3f velocity(pVelocity.x, pVelocity.y, -pVelocity.z);
    
    FSOUND_3D_Listener_SetAttributes(position, velocity,
                                     pForward.x, pForward.y, -pForward.z,
                                     pUp.x, pUp.y, -pUp.z);
}

void FMODSoundSubsystem::SetDistanceFactor(Float pScale)
{
    Super::SetDistanceFactor(pScale);
    FSOUND_3D_SetDistanceFactor(pScale);
}

void FMODSoundSubsystem::SetDopplerFactor(Float pScale)
{
    Super::SetDopplerFactor(pScale);
    FSOUND_3D_SetDopplerFactor(pScale);
}

void FMODSoundSubsystem::SetRolloffFactor(Float pScale)
{
    Super::SetRolloffFactor(pScale);
    FSOUND_3D_SetRolloffFactor(pScale);
}

void FMODSoundSubsystem::Update(Double pElapsedTime,
                                const Vector3f& pListenerPosition,
                                const Vector3f& pListenerForward,
                                const Vector3f& pListenerUp)
{
    // Calculate the velocity.
    Vector3f velocity = (pListenerPosition - mLastListenerPosition) / pElapsedTime;
    
    // Update the last listener's position.
    mLastListenerPosition = pListenerPosition;

    // Update the sound system.
    Set3DListenerAttributes(pListenerPosition, velocity, pListenerForward, pListenerUp);
    FSOUND_Update();
}

Int32 FMODSoundSubsystem::GetSFXMasterVolume()
{
    return Super::GetSFXMasterVolume();
}