/**
 *  @file       Sound3DProperties.h
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
#ifndef     _SOUND3D_PROPERTIES_H_
#define     _SOUND3D_PROPERTIES_H_


#include "EntityProperties.h"


class Sound3D;


/**
 *  Sound3DProperties class.  This object is used as a facade to a Sound3D.
 *  As a PropertiesSender and PropertiesListener, it can be connected with
 *  other Senders/Listeners to keep the data up to date.
 *  @brief  Sound3DProperties class.
 *  @author Marco Arsenault.
 *  @date   23/02/04.
 */
class EDITORLIB_API Sound3DProperties : public EntityProperties
{
    DECLARE_CLASS(Sound3DProperties, EntityProperties);

public:
    //! Constructor.
	Sound3DProperties();
    //! Destructor.
    virtual ~Sound3DProperties();

    //! Initialize all properties.
    virtual void InitProperties();

    void SetPlaying(Bool pPlaying);
    Bool GetPlaying() const;

    void SetMode(UInt32 pMode);
    UInt32 GetMode() const;

    void SetMuted(Bool pMuted);
    Bool GetMuted() const;

    void SetVolume(Int32 pVolume);
    Int32 GetVolume() const;

    void SetAbsoluteVolume(Int32 pVolume);
    Int32 GetAbsoluteVolume() const;

    void SetPan(Int32 pPan);
    Int32 GetPan() const;

    void SetStereoPan(Bool pStereoPan);
    Bool GetStereoPan() const;

    void SetMinDistance(Float pMinDistance);
    Float GetMinDistance() const;

    void SetMaxDistance(Float pMaxDistance);
    Float GetMaxDistance() const;

    void SetFrequency(Int32 pFrequency);
    Int32 GetFrequency() const;

    void SetPriority(Int32 pPriority);
    Int32 GetPriority() const;

private:
    PropertyBool        mPlaying;
    PropertyEnum        mMode;
    PropertyBool        mMuted;
    PropertyInt32       mVolume;
    PropertyInt32       mAbsoluteVolume;
    PropertyInt32       mPan;
    PropertyBool        mStereoPan;
    PropertyFloat       mMinDistance;
    PropertyFloat       mMaxDistance;
    PropertyInt32       mFrequency;
    PropertyInt32       mPriority;
};


#endif  //  _SOUND3D_PROPERTIES_H_