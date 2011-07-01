/**
 *  @file       SoundSubsystemProperties.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       14/04/04.
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
#ifndef     _SOUND_SUBSYSTEM_PROPERTIES_H_
#define     _SOUND_SUBSYSTEM_PROPERTIES_H_


#include "Properties/ObjectProperties.h"


class EDITORLIB_API SoundSubsystemProperties : public ObjectProperties
{
    DECLARE_CLASS(SoundSubsystemProperties, ObjectProperties);

public:
    //! Constructor.
	SoundSubsystemProperties();

    //! Destructor.
    virtual ~SoundSubsystemProperties();

    //! Initialize all properties.
    virtual void InitProperties();

    void SetSpeakerMode(UInt32 pSpeakerMode);
    UInt32 GetSpeakerMode() const;

    void SetDistanceFactor(Float pDistanceFactor);
    Float GetDistanceFactor() const;

    void SetDopplerFactor(Float pDopplerFactor);
    Float GetDopplerFactor() const;

    void SetRolloffFactor(Float pRolloffFactor);
    Float GetRolloffFactor() const;

    void SetSFXMasterVolume(Float pSFXMasterVolume);
    Float GetSFXMasterVolume() const;

    void SetSpeedRatio(Float pSpeedRatio);
    Float GetSpeedRatio() const;
    
private:
    PropertyEnum    mSpeakerMode;
    PropertyFloat   mDistanceFactor;
    PropertyFloat   mDopplerFactor;
    PropertyFloat   mRolloffFactor;
    PropertyFloat   mSFXMasterVolume;
    PropertyFloat   mSpeedRatio;
};


#endif  //  _SOUND_SUBSYSTEM_PROPERTIES_H_
