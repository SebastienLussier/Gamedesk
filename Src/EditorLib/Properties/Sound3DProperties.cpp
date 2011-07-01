/**
 *  @file       Sound3DProperties.cpp
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       23/02/04.
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
#include "Sound3DProperties.h"
#include "World/Sound3D.h"


ImplementProperty( Sound3DProperties, mPlaying, Bool, GetPlaying, SetPlaying );
ImplementProperty( Sound3DProperties, mMode, UInt32, GetMode, SetMode );
ImplementProperty( Sound3DProperties, mMuted, Bool, GetMuted, SetMuted );
ImplementProperty( Sound3DProperties, mVolume, Int32, GetVolume, SetVolume );
ImplementProperty( Sound3DProperties, mAbsoluteVolume, Int32, GetAbsoluteVolume, SetAbsoluteVolume );
ImplementProperty( Sound3DProperties, mPan, Int32, GetPan, SetPan );
ImplementProperty( Sound3DProperties, mStereoPan, Bool, GetStereoPan, SetStereoPan );
ImplementProperty( Sound3DProperties, mMinDistance, Float, GetMinDistance, SetMinDistance );
ImplementProperty( Sound3DProperties, mMaxDistance, Float, GetMaxDistance, SetMaxDistance );
ImplementProperty( Sound3DProperties, mFrequency, Int32, GetFrequency, SetFrequency );
ImplementProperty( Sound3DProperties, mPriority, Int32, GetPriority, SetPriority );


Sound3DProperties::Sound3DProperties()
{
}

Sound3DProperties::~Sound3DProperties()
{
}

void Sound3DProperties::InitProperties()
{
    Super::InitProperties();

    RemoveProperty( &mOrientation );
    RegisterProperty( Sound3DProperties, "Playing", mPlaying );
    RegisterProperty( Sound3DProperties, "Mode", mMode );
    RegisterProperty( Sound3DProperties, "Mute", mMuted );
    RegisterProperty( Sound3DProperties, "Volume", mVolume );
    //RegisterProperty( Sound3DProperties, "Absolute volume", mAbsoluteVolume );
    RegisterProperty( Sound3DProperties, "Pan", mPan );
    RegisterProperty( Sound3DProperties, "Use stereo pan", mStereoPan );
    RegisterProperty( Sound3DProperties, "Min distance", mMinDistance );
    RegisterProperty( Sound3DProperties, "Max distance", mMaxDistance );
    RegisterProperty( Sound3DProperties, "Frequency", mFrequency );
    RegisterProperty( Sound3DProperties, "Priority", mPriority );

    mMode.SetEnumValue(0, "One shot");
    mMode.SetEnumValue(1, "Loop");

    mVolume.SetMinimum(0);
    mVolume.SetMaximum(255);
    mVolume.UseRangeValidation(true);
    mVolume.UseSlider(true);

    mAbsoluteVolume.SetMinimum(0);
    mAbsoluteVolume.SetMaximum(255);
    mAbsoluteVolume.UseRangeValidation(true);
    mAbsoluteVolume.UseSlider(true);

    mPan.SetMinimum(0);
    mPan.SetMaximum(255);
    mPan.UseRangeValidation(true);
    mPan.UseSlider(true);

    mFrequency.SetMinimum(-705600);
    mFrequency.SetMaximum(705600);
    mFrequency.UseRangeValidation(true);

    mPriority.SetMinimum(0);
    mPriority.SetMaximum(255);
    mPriority.UseRangeValidation(true);
    mPriority.UseSlider(true);
}

void Sound3DProperties::SetPlaying(Bool pPlaying)
{
    if(pPlaying)
        Cast<Sound3D>(GetObject())->GetSoundHandle()->Play();
    else
        Cast<Sound3D>(GetObject())->GetSoundHandle()->Stop();
    
    FirePropertyChanged(&mPlaying);
}

Bool Sound3DProperties::GetPlaying() const
{
    return Cast<Sound3D>(GetObject())->GetSoundHandle()->IsPlaying();
}

void Sound3DProperties::SetMode(UInt32 pMode)
{
    Sound::eSoundMode mode = static_cast<Sound::eSoundMode>(pMode);
    Cast<Sound3D>(GetObject())->GetSoundHandle()->SetMode(mode);
    FirePropertyChanged(&mMode);
}

UInt32 Sound3DProperties::GetMode() const
{
    Sound::eSoundMode mode = Cast<Sound3D>(GetObject())->GetSoundHandle()->GetMode();
    UInt32 modeIndex = static_cast<UInt32>(mode);
    return modeIndex;
}

void Sound3DProperties::SetMuted(Bool pMuted)
{
    Cast<Sound3D>(GetObject())->GetSoundHandle()->SetMute(pMuted);
    FirePropertyChanged(&mMuted);
}

Bool Sound3DProperties::GetMuted() const
{
    return Cast<Sound3D>(GetObject())->GetSoundHandle()->IsMuted();
}

void Sound3DProperties::SetVolume(Int32 pVolume)
{
    Cast<Sound3D>(GetObject())->GetSoundHandle()->SetVolume(pVolume);
    FirePropertyChanged(&mVolume);
}

Int32 Sound3DProperties::GetVolume() const
{
    return Cast<Sound3D>(GetObject())->GetSoundHandle()->GetVolume();
}

void Sound3DProperties::SetAbsoluteVolume(Int32 pVolume)
{
    Cast<Sound3D>(GetObject())->GetSoundHandle()->SetAbsoluteVolume(pVolume);
    FirePropertyChanged(&mAbsoluteVolume);
}

Int32 Sound3DProperties::GetAbsoluteVolume() const
{
    return Cast<Sound3D>(GetObject())->GetSoundHandle()->GetAbsoluteVolume();
}

void Sound3DProperties::SetPan(Int32 pPan)
{
    Cast<Sound3D>(GetObject())->GetSoundHandle()->SetPan(pPan);
    FirePropertyChanged(&mPan);
}

Int32 Sound3DProperties::GetPan() const
{
    return Cast<Sound3D>(GetObject())->GetSoundHandle()->GetPan();
}

void Sound3DProperties::SetStereoPan(Bool pStereoPan)
{
    if(pStereoPan)
        Cast<Sound3D>(GetObject())->GetSoundHandle()->SetStereoPan();
    else
        Cast<Sound3D>(GetObject())->GetSoundHandle()->SetPan(128);
    FirePropertyChanged(&mStereoPan);
}

Bool Sound3DProperties::GetStereoPan() const
{
    return Cast<Sound3D>(GetObject())->GetSoundHandle()->IsStereoPan();
}

void Sound3DProperties::SetMinDistance(Float pMinDistance)
{
    Cast<Sound3D>(GetObject())->GetSoundHandle()->SetMinDistance(pMinDistance);
    FirePropertyChanged(&mMinDistance);
}

Float Sound3DProperties::GetMinDistance() const
{
    return Cast<Sound3D>(GetObject())->GetSoundHandle()->GetMinDistance();
}

void Sound3DProperties::SetMaxDistance(Float pMaxDistance)
{
    Cast<Sound3D>(GetObject())->GetSoundHandle()->SetMaxDistance(pMaxDistance);
    FirePropertyChanged(&mMaxDistance);
}

Float Sound3DProperties::GetMaxDistance() const
{
    return Cast<Sound3D>(GetObject())->GetSoundHandle()->GetMaxDistance();
}

void Sound3DProperties::SetFrequency(Int32 pFrequency)
{
    Cast<Sound3D>(GetObject())->GetSoundHandle()->SetFrequency(pFrequency);
    FirePropertyChanged(&mFrequency);
}

Int32 Sound3DProperties::GetFrequency() const
{
    return Cast<Sound3D>(GetObject())->GetSoundHandle()->GetFrequency();
}

void Sound3DProperties::SetPriority(Int32 pPriority)
{
    Cast<Sound3D>(GetObject())->GetSoundHandle()->SetPriority(pPriority);
    FirePropertyChanged(&mPriority);
}

Int32 Sound3DProperties::GetPriority() const
{
    return Cast<Sound3D>(GetObject())->GetSoundHandle()->GetPriority();
}


IMPLEMENT_CLASS(Sound3DProperties);
