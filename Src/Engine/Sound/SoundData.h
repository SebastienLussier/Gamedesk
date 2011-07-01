/**
 *  @file       SoundData.h
 *  @brief	    Resource data for a sound.
 *  @author     Marco Arsenault.
 *  @date       02/03/04.
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
#ifndef     _SOUND_DATA_H_
#define     _SOUND_DATA_H_


#include "Resource/Resource.h"


namespace Gamedesk {


class ENGINE_API SoundData : public Resource
{
    DECLARE_CLASS(SoundData, Resource);

public:
    //! Constructor.
    SoundData();
    //! Destructor.
    virtual ~SoundData();

    //! Set the file name.
    void SetFileName(const String& pFileName);
    //! Get the file name.
    const String& GetFileName() const;

    //! Set the file data size.
    void SetFileDataSize(Int32 pFileDataSize);
    //! Get the file data size.
    Int32 GetFileDataSize() const;

    //! Set the file data.
    void SetFileData(Char* pData);
    //! Get the file data.
    Char* GetFileData();

    //! Set the number of channels.
    void SetNbChannels(Int16 pNbChannels);
    //! Get the number of channels.
    Int16 GetNbChannels() const;
    
    //! Set the sample rate in Hz.
    void SetSampleRate(Int32 pSampleRate);
    //! Get the sample rate in Hz.
    Int32 GetSampleRate() const;

    //! Set the number of bytes per second.
    void SetBytesPerSecond(Int32 pBytesPerSecond);
    //! Get the number of bytes per second.
    Int32 GetBytesPerSecond() const;

    /**
     *  Set the number of bytes per sample.
     *  1=8bit Mono, 2=8bit Stereo or 16bit Mono, 4=16bit Stereo.
     *  @brief  Set the number of bytes per sample.
     */
    void SetBytesPerSample(Int16 pBytesPerSample);
    /**
     *  Get the number of bytes per sample.
     *  1=8bit Mono, 2=8bit Stereo or 16bit Mono, 4=16bit Stereo.
     *  @brief  Get the number of bytes per sample.
     */
    Int16 GetBytesPerSample() const;

    //! Set the number of bits per sample.
    void SetBitsPerSample(Int16 pBitsPerSample);
    //! Get the number of bits per sample.
    Int16 GetBitsPerSample() const;

    //! Set the sound data size.
    void SetSoundDataSize(Int32 pSoundDataSize);
    //! Get the sound data size.
    Int32 GetSoundDataSize() const;
    
    //! Get the sound data.
    Char* GetSoundData();
    //! Set the sound data offset.
    void SetSoundDataOffset(Int32 pOffset);

private:
    String  mFileName;
    Int32   mFileDataSize;
    Char*   mFileData;
    Int16   mNbChannels;
    Int32   mSampleRate;
    Int32   mBytesPerSecond;
    Int16   mBytesPerSample;    //!< 1=8bit Mono, 2=8bit Stereo or 16bit Mono, 4=16bit Stereo.
    Int16   mBitsPerSample;
    Int32   mSoundDataSize;
    Int32   mSoundDataOffset;
};


} // namespace Gamedesk


#endif  //  _SOUND_DATA_H_
