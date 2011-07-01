/**
 *  @file       FMODSound.h
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
#ifndef     _FMOD_SOUND_H_
#define     _FMOD_SOUND_H_


#include "Sound/Sound.h"


namespace Gamedesk {


class FMODSOUND_API FMODSound : public Sound
{
    DECLARE_CLASS(FMODSound, Sound);

public:
    //! Default constructor.
    FMODSound();
    //! Destructor.
    virtual ~FMODSound();

    //! Creates a sound.
    virtual void Create(SoundData* pSoundData, Bool pIs3DSound = true);
    //! Init the sound.
    virtual void Init();
    //! Kill the sound.
    virtual void Kill();

    /**
     *  Play the sound.
     *  @return true on success and false on failure.
     */
    virtual Bool Play();

    /**
     *  Stop the sound.
     *  @return true on success and false on failure.
     */
    virtual Bool Stop();

    //! Returns if the sound is currently playing or not.
    virtual Bool IsPlaying();
    
    /**
     *  Pause or unpause the sound.
     *  @brief  Pause or unpause the sound.
     *  @param  pPaused true pauses the sound, false unpauses it.
     *  @return true on success and false on failure.
     */
    virtual Bool SetPaused(Bool pPaused);

    //! Returns whether the sound is paused or not.
    virtual Bool IsPaused() const;

    /**
     *  Stop the sound mode.
     *  @return true on success and false on failure.
     */
    virtual Bool SetMode(eSoundMode pMode);

    //! Returns the sound mode.
    virtual eSoundMode GetMode() const;

    /**
     *  Mute/unmute the sound.
     *  @return true on success and false on failure.
     */
    virtual Bool SetMute(Bool pMute);

    //! Returns whether the sound is muted or not.
    virtual Bool IsMuted() const;

    /**
     *  Sets the sound's volume linearly.
     *  This volume IS affected by the SoundSubsystem::SetSFXMasterVolume method.
     *  If you want a volume that IS NOT affected by it, use the SetAbsoluteVolume
     *  method. Valid values are from 0 (silent) to 255 (full volume).
     *  @brief  Sets the sound's volume linearly.
     *  @param  pVolume   The volume to set.
     *  @return true on success and false on failure.
     */
    virtual Bool SetVolume(Int32 pVolume);

    //! Returns the sound's volume.
    virtual Int32 GetVolume() const;

    /**
     *  Sets the sound's absolute volume linearly.
     *  This volume IS NOT affected by the SoundSubsystem::SetSFXMasterVolume method.
     *  If you want a volume that IS affected by it, use the SetVolume method.
     *  This method is used when you want to quiet everything down using 
     *  SoundSubsystem::SetSFXMasterVolume, but make a sound prominent.
     *  A good example of this method is to use it for a game needing a voice over.
     *  If all the background sounds were too loud and drowned out the voice over,
     *  there is no way to feasibly go through all the sounds and lower 
     *  the background noise volumes.  Simply lower the background noise 
     *  with SoundSubsystem::SetSFXMasterVolume and use SetAbsoluteVolume to bring
     *  up the volume of the voice over to full, and you will get one sound
     *  standing out amongst the rest.
     *  Valid values are from 0 (silent) to 255 (full volume).
     *  @brief  Sets the sound's absolute volume linearly.
     *  @param  pVolume   The absolute volume to set.
     *  @return true on success and false on failure.
     */
    virtual Bool SetAbsoluteVolume(Int32 pVolume);

    //! Returns the sound's absolute volume.
    virtual Int32 GetAbsoluteVolume() const;

    /**
     *  Sets a channels pan position linearly.
     *  Values goes from 0 (full left) to 255 (full right).
     *  128 is a 50/50 pan called middle panning so if you want a full volume
     *  for left and right, use the SetStereoPan method.
     *  @brief  Sets a channels pan position linearly.
     *  @param  pPan      The panning position to set.
     *  @return true on success and false on failure.
     */
    virtual Bool SetPan(Int32 pPan);

    //! Returns the sound's pan.
    virtual Int32 GetPan() const;

    /**
     *  Sets a stereo panning.
     *  See SetPan for more explanation.
     *  @brief  Sets a stereo panning.
     *  @return true on success and false on failure.
     */
    virtual Bool SetStereoPan();

    //! Returns whether the sound is using a stereo pan or not.
    virtual Bool IsStereoPan() const;

    /**
     *  Sets the minimum and maximum audible distance for a sound.
     *  MinDistance is the minimum distance that the sound emitter will cease to 
     *  continue growing louder at (as it approaches the listener). Within the 
     *  minDistance it stays at the constant loudest volume possible. Outside of this 
     *  minDistance it begins to attenuate.
     *  MaxDistance is the distance a sound stops attenuating at. Beyond this point 
     *  it will stay at the volume it would be at maxdistance units from the listener
     *  and will not attenuate any more.
     *  MinDistance is useful to give the impression that the sound is loud or soft in 
     *  3d space. An example of this is a small quiet object, such as a bumblebee, 
     *  which you could set a minDistance of to 0.1 for example, which would cause 
     *  it to attenuate quickly and dissapear when only a few meters away from the 
     *  listener.  Another example is a jumbo jet, which you could set to a minDistance
     *  of 100.0, which would keep the sound volume at max until the listener was 
     *  100 meters away, then it would be hundreds of meters more before it would fade 
     *  out.  In summary, increase the mindistance of a sound to make it 'louder' 
     *  in a 3d world, and decrease it to make it 'quieter' in a 3d world.
     *  maxDistance is effectively obsolete unless you need the sound to stop fading 
     *  out at a certain point. Do not adjust this from the default if you dont need to.
     *  Some people have the confusion that maxDistance is the point the sound will 
     *  fade out to, this is not the case.
     *  A 'distance unit' is specified by SoundSubsystem::SetDistanceFactor.
     *  By default this is set to meters which is a distance scale of 1.0.
     *  See SoundSubsystem::SetDistanceFactor for more info on this.
     *  The default units for minimum and maximum distances are 1.0 and 1000000000.0f.
     *  Volume drops off at minDistance / distance.
     *  @brief  Sets the minimum and maximum audible distance for a sound.
     *  @param  pMininumDistance    The minimum volume distance in units.
     *  @param  pMaximumDistance    The maximum volume distance in units.
     *  @return true on success and false on failure.
     */
    virtual Bool SetMinMaxDistance(Float pMininumDistance, Float pMaximumDistance);

    /**
     *  Specialized version of the SetMinMaxDistance method for the minimum value.
     *  @return true on success and false on failure.
     */
    virtual Bool SetMinDistance(Float pMininumDistance);

    //! Returns the sound's minimum distance.
    virtual Float GetMinDistance() const;

    /**
     *  Specialized version of the SetMinMaxDistance method for the maximum value.
     *  @return true on success and false on failure.
     */
    virtual Bool SetMaxDistance(Float pMaximumDistance);

    //! Returns the sound's maximum distance.
    virtual Float GetMaxDistance() const;
 
    /**
     *  Sets the position and velocity of the sound.
     *  A 'distance unit' is specified by SoundSubsystem::SetDistanceFactor.
     *  By default this is set to meters which is a distance scale of 1.0.
     *  See SoundSubsystem::SetDistanceFactor for more info on this.
     *  @brief  Updates the position and velocity of the sound.
     *  @param  pPosition   The new position in world space, measured in distance 'units'.
     *  @param  pVelocity   The new velocity measured in distance 'units' per second.
     *  @return true on success and false on failure.
     */
    virtual Bool SetAttributes(const Vector3f& pPosition, const Vector3f& pVelocity);

    /**
     *  Returns the position and velocity of the sound.
     *  A 'distance unit' is specified by SoundSubsystem::SetDistanceFactor.
     *  By default this is set to meters which is a distance scale of 1.0.
     *  See SoundSubsystem::SetDistanceFactor for more info on this.
     *  @brief  Returns the position and velocity of the sound.
     *  @param  pPosition   A vector reference to store the sound's position in 
     *                      world space, measured in distance 'units'.
     *  @param  pVelocity   A vector reference to store the sound's velocity measured 
     *                      in distance 'units' per second.
     *  @return true on success and false on failure.
     */
    virtual Bool GetAttributes(Vector3f& pPosition, Vector3f& pVelocity);

    /**
     *  Sets the frequency or playback rate, in HZ.
     *  @brief  Sets the frequency or playback rate, in HZ.
     *  @param  pFrequency  The frequency to set. Valid ranges are from 100 to 705600,
     *                      and -100 to -705600.
     *  @return true on success and false on failure.
     */
    virtual Bool SetFrequency(Int32 pFrequency);

    //! Returns the sound's frequency.
    virtual Int32 GetFrequency() const;

    /**
     *  Sets the sound's priority.
     *  Higher priority means it is less likely to get discarded when
     *  all the Sound Subsystem's channels are being used.
     *  Valid priorities are from 0 (lowest) to 255 (highest).
     *  @brief  Sets the sound's priority.
     *  @param  pPriority   The priority to set.
     *  @return true on success and false on failure.
     */
    virtual Bool SetPriority(Int32 pPriority);

    //! Returns the sound's priority.
    virtual Int32 GetPriority() const;
       
private:
	FSOUND_SAMPLE*	mFMODSample;
	Int32           mChannel;
};


} // namespace Gamedesk


#endif  //  _FMOD_SOUND_H_
