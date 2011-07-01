/**
 *  @file       SoundSubsystem.h
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
#ifndef     _SOUND_SUBSYSTEM_H_
#define     _SOUND_SUBSYSTEM_H_


#include "Subsystem/Subsystem.h"
#include "Patterns/Singleton.h"
#include "Maths/Vector3.h"


namespace Gamedesk {
	

class Resource;
class Sound;


class ENGINE_API SoundSubsystem : public Subsystem
{
    DECLARE_ABSTRACT_CLASS(SoundSubsystem, Subsystem);
    DECLARE_ABSTRACT_SINGLETON(SoundSubsystem);

public:

    //! The speaker mode enum.
    enum eSpeakerMode
    {
        kSpeakerModeDolbyDigital,   //!< Dolby Digital speaker output (XBOX or PC only).
        kSpeakerModeHeadphone,      //!< Headphones speakers.
        kSpeakerModeMono,           //!< Mono speakers.
        kSpeakerModeQuad,           //!< Quadraphonic speakers.
        kSpeakerModeStereo,         //!< Stereo speakers.
        kSpeakerModeSurround        //!< Surround speakers.
    };

    //! The output type enum.
    enum eOutputType
    {
        kOutputTypeAutoDetect,  //!< The output system is autodetected based on the O/S.
        kOutputTypeNoSound,     //!< NoSound driver, all calls to this succeed but do nothing.
        kOutputTypeWinMM,       //!< Windows Multimedia driver.
        kOutputTypeDirectSound, //!< DirectSound driver. You need this to get EAX or EAX2 support.
        kOutputTypeA3D,         //!< A3D driver. You need this to get geometry support.
        kOutputTypeOSS,         //!< Linux/Unix OSS (Open Sound System) driver, i.e. the kernel sound drivers.
        kOutputTypeESD,         //!< Linux/Unix ESD (Enlightment Sound Daemon) driver.
        kOutputTypeAlsa         //!< Linux Alsa driver.
    };

    //! The mixer type enum.
    enum eMixerType
    {
        kMixerTypeAutoDetect,        //!< Enables autodetection of the fastest mixer based on your cpu.
        kMixerTypeBlendMode,         //!< Enables the standard non mmx, blendmode mixer.
        kMixerTypeMMXP5,             //!< Enables the mmx, pentium optimized blendmode mixer.
        kMixerTypeMMXP6,             //!< Enables the mmx, ppro/p2/p3 optimized mixer.
        kMixerTypeQualityAutoDetect, //!< Enables the interpolating/volume ramping fastest mixer.
        kMixerTypeQualityFPU,        //!< Enables the interpolating/volume ramping FPU mixer.
        kMixerTypeQualityMMXP5,      //!< Enables the interpolating/volume ramping p5 MMX mixer.
        kMixerTypeQualityMMXP6       //!< Enables the interpolating/volume ramping ppro/p2/p3+ MMX mixer.
    };
 
    static const Int32 kDefaultDriver; //!< Default sound driver.
    static const Int32 kNoSoundDriver; //!< No sound driver.

    //! Destructor.
    virtual ~SoundSubsystem();
    
    //! Initialize the sound subsystem with default values.
    virtual void Init();
    //! Kill the sound subsystem.
    virtual void Kill();

    //! Start the sound system.
    virtual Bool StartSoundSystem() = 0;
    //! Stop the sound system.
    virtual Bool StopSoundSystem() = 0;

    //! Sets the output rate in hz. (Between 4000 and 65535).
    virtual void SetOutputRate(Int32 pOutputRate);

    //! Sets the minimum number of software channels. 
    virtual void SetMaxNbSoftwareChannels(Int32 pNbSoftwareChannels);

    /**
     *  Sets the sound card driver. It MUST be called before the sound system is
     *  started and/or after the sound system is stopped.
     *  @brief  Sets the sound card driver.
     *  @param  pDriverIndex   The index of the driver to use.
     *  @return True if it succeed, False if it fails.
     */
    virtual Bool SetDriver(Int32 pDriverIndex);

    /**
     *  Sets up the soundsystem output type. This means if it uses DirectSound(tm), or 
     *  Windows(tm) Multimedia waveout, etc.
     *  It MUST be called before the sound system is
     *  started and/or after the sound system is stopped.
     *  @brief  Sets up the soundsystem output type.
     *  @param  pOutputType   The output system to use.
     *  @return True if it succeed, False if it fails.
     */
    virtual Bool SetOutputType(eOutputType pOutputType);

    /**
     *  Sets the minimum allowable hardware channels before the sound system
     *  drops back 100% software.
     *  It MUST be called before the sound system is
     *  started and/or after the sound system is stopped.
     *  @brief  Sets the minimum allowable hardware channels.
     *  @param  pNbHardwareChannels   The minimum allowable hardware channels.
     *  @return True if it succeed, False if it fails.
     */
    virtual Bool SetMinNbHardwareChannels(Int32 pNbHardwareChannels);

    /**
     *  Sets a digital mixer type.
     *  This function does not nescessarily need to be called, 
     *  autodetection will select the fastest mixer for your machine. 
     *  It is here if you need to test all mixer types for debugging purposes, 
     *  or a mixer has a feature that the autodetected one doesnt.
     *  (i.e. low quality mixers or volume ramping)
     *  It MUST be called before the sound system is
     *  started and/or after the sound system is stopped.
     *  @brief  Sets a digital mixer type.
     *  @param  pMixerType   The mixer type to set.
     *  @return True if it succeed, False if it fails.
     */
    virtual Bool SetMixerType(eMixerType pMixerType);

    /**
     *  Sets the master volume for any sound effects played.
     *  The volume range is from 0 (silent) to 255 (full volume).
     *  It won't change the volume for music and CD output.
     *  @brief  Sets the master volume for any sound effects played.
     *  @param  pVolume The volume to set.
     */
    virtual void SetSFXMasterVolume(Int32 pVolume);

    /**
     *  Sets the mode for the users speaker setup.
     *  To get true 5.1 dolby digital or DTS output you will need a soundcard that can 
     *  encode it, and a receiver that can decode it. If not the results can be 
     *  unpredictable. Calling this will reset the pan separation setting. It sets it 
     *  to 0 if the kSpeakerModeMono is chosen and 1 otherwise. You will need 
     *  to reset the pan separation if required afterwards. 
     *  Note that soundcard drivers may ignore this if in hardware mode.
     *  @brief  Sets the mode for the users speaker setup.
     *  @param  pSpeakerMode    A speaker mode describing the users speaker setup.
     */
    virtual void SetSpeakerMode(eSpeakerMode pSpeakerMode);
    virtual eSpeakerMode GetSpeakerMode() const;

    /**
     *  Sets the master pan separation for 2d sound effects.
     *  It is set to 1.0 by default.
     *  @brief  Sets the master pan separation for 2d sound effects.
     *  @param  pPanSeparation  The pan scalar. 1.0 means full pan separation, 0 means mono.
     */
    virtual void SetPanSeparation(Float pPanSeparation);

    //! Returns the number of active channels or ones that are playing.
    virtual Int32 GetNbPlayingChannels();

    //! Returns the total number of channels allocated.
    virtual Int32 GetNbChannels();

    /**
     *  Returns the number of sound cards or devices enumerated for the current 
     *  output type. (DirectSound, WaveOut etc.).
     */
    virtual Int32 GetNbDrivers();

    //! Returns the current output type.
    virtual eOutputType GetOutputType();

    //! Returns the current mixer type.
    virtual eMixerType GetMixerType();

    //! Returns the current mixing rate in hz.
    virtual Int32 GetOutputRate();

    //! Returns the currently selected driver index.
    virtual Int32 GetDriver();

    //! Returns the name of the driver with the given index.
    virtual String GetDriverName(Int32 pDriverIndex);

    /**
     *  Retreives the position, velocity and orientation of the 3d sound listener.
     *  The position/velocity are in 'distance unit' which is set with SetDistanceFactor.
     *  By default, the 'distance unit' is set to meters which is a distance scale of 1.0.
     *  Remember to use units PER SECOND, NOT PER FRAME as this is a common mistake. 
     *  Do not just use (pos - lastpos) from the last frame's data for velocity, 
     *  as this is not correct. You need to time compensate it so it is given in units 
     *  per SECOND. You could alter your pos - lastpos calculation to something like 
     *  this.  vel = (pos-lastpos) / (time taken since last frame in seconds).
     *  ie at 60fps the formula would look like this vel = (pos-lastpos) / 0.0166667.
     *  @brief  Retreives the position, velocity and orientation of the 3d sound listener.
     *  @param  pPosition   Vector reference to store the listener's position in world
     *                      space, measured in 'distance units'
     *  @param  pVelocity   Vector reference to store the listener's velocity measured
     *                      in 'distance units' per SECOND.
     *  @param  pForward    Vector reference to store the listener's forward direction.
     *  @param  pUp         Vector reference to store the listener's up direction.
     */
    virtual void Get3DListenerAttributes(Vector3f& pPosition, Vector3f& pVelocity,
                                         Vector3f& pForward, Vector3f& pUp);

    /**
     *  Sets the position, velocity and orientation of the 3d sound listener.
     *  The position/velocity are in 'distance unit' which is set with SetDistanceFactor.
     *  By default, the 'distance unit' is set to meters which is a distance scale of 1.0.
     *  Remember to use units PER SECOND, NOT PER FRAME as this is a common mistake. 
     *  Do not just use (pos - lastpos) from the last frame's data for velocity, 
     *  as this is not correct. You need to time compensate it so it is given in units 
     *  per SECOND. You could alter your pos - lastpos calculation to something like 
     *  this.  vel = (pos-lastpos) / (time taken since last frame in seconds).
     *  ie at 60fps the formula would look like this vel = (pos-lastpos) / 0.0166667.
     *  @brief  Sets the position, velocity and orientation of the 3d sound listener.
     *  @param  pPosition   The listener's position to set, in world space, measured in 
     *                      'distance units'.
     *  @param  pVelocity   The listener's velocity to set measured in 'distance units'
     *                      per SECOND.
     *  @param  pForward    The listener's forward direction to set.
     *  @param  pUp         The listener's up direction to set.
     */
    virtual void Set3DListenerAttributes(const Vector3f& pPosition, const Vector3f& pVelocity,
                                         const Vector3f& pForward, const Vector3f& pUp);

    
    /**
     *  Sets the sound system's 3d engine relative distance factor, 
     *  compared to 1.0 meters. It equates to 'how many units per meter' does your 
     *  engine have. By default this value is set at 1.0, or meters.
     *  @brief  Sets the sound system's 3d engine relative distance factor.
     *  @param  pScale  The distance scale. Scale 1.0 = 1 meter units. 
     *                  If you are using feet then scale would equal 3.28.
     */
    virtual void SetDistanceFactor(Float pScale);
    virtual Float GetDistanceFactor() const;

    /**
     *  Sets the doppler shift scale factor.
     *  This is a general scaling factor for how much the pitch varies due to doppler 
     *  shifting.  Increasing the value above 1.0 exaggerates the effect, 
     *  whereas lowering it reduces the effect. 0 removes the effect all together.
     *  Default doppler scale is 1.0f (Giving a Doppler speed of 340 m/s)
     *  @brief  Sets the doppler shift scale factor.
     *  @param  pScale  The Doppler shift scale.
     */
    virtual void SetDopplerFactor(Float pScale);
    virtual Float GetDopplerFactor() const;

    /**
     *  Sets the global attenuation rolloff factor.
     *  Normally volume for a sample will scale at 1 / distance. This gives a 
     *  logarithmic attenuation of volume as the source gets further away (or closer).
     *  Setting this value makes the sound drop off faster or slower. The higher the 
     *  value, the faster volume will fall off. The lower the value, the slower it 
     *  will fall off.  For example a rolloff factor of 1 will simulate the real world, 
     *  where as a value of 2 will make sounds attenuate 2 times quicker.
     *  Default rolloff factor is 1.0.
     *  @brief  Sets the global attenuation rolloff factor.
     *  @param  pScale  The global rolloff factor to set. Valid ranges are 0 to 10.
     */
    virtual void SetRolloffFactor(Float pScale);
    virtual Float GetRolloffFactor() const;

    /**
     *  Updates the 3d sound engine and DMA engine (only on some platforms),
     *  and should be called once a game frame.
     *  @brief  Updates the 3d sound engine and DMA engine (only on some platforms).
     */
    virtual void Update(Double pElapsedTime,
                        const Vector3f& pListenerPosition,
                        const Vector3f& pListenerForward,
                        const Vector3f& pListenerUp);

    /**
     *  Returns the master volume for any sound effects played.
     *  Doesn't have anything to do with music/cd output volume.
     *  Valid ranges are from 0 (silent) to 255 (full volume).
     *  @brief  Returns the master volume for any sound effects played.
     */
    virtual Int32 GetSFXMasterVolume();

    virtual void SetSpeedRatio(Float pSpeedRatio);
    virtual Float GetSpeedRatio() const;



protected:
    SoundSubsystem();

    Vector3f mLastListenerPosition;
   
private:
    Float       mDistanceFactor;
    Float       mDopplerFactor;
    Float       mRolloffFactor;
    Float       mSFXMasterVolume;
    Vector3f    mListenerPosition;
    Float       mSpeedRatio;
};


} // namespace Gamedesk


#endif  //  _SOUND_SUBSYSTEM_H_
