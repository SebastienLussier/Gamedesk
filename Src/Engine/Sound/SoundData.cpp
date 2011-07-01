/**
 *  @file       SoundData.cpp
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
#include "Engine.h"
#include "SoundData.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(SoundData);


SoundData::SoundData() : 
    mFileName(""),
    mFileData(0)
{
}

SoundData::~SoundData()
{
    GD_DELETE(mFileData);
    mFileData = 0;
}

void SoundData::SetFileName(const String& pFileName)
{
    mFileName = pFileName;
}

const String& SoundData::GetFileName() const
{
    return mFileName;
}

void SoundData::SetFileDataSize(Int32 pFileDataSize)
{
    mFileDataSize = pFileDataSize;
}

Int32 SoundData::GetFileDataSize() const
{
    return mFileDataSize;
}

void SoundData::SetFileData(Char* pData)
{
    mFileData= pData;
}

Char* SoundData::GetFileData()
{
    return mFileData;
}

void SoundData::SetNbChannels(Int16 pNbChannels)
{
    mNbChannels = pNbChannels;
}

Int16 SoundData::GetNbChannels() const
{
    return mNbChannels;
}

void SoundData::SetSampleRate(Int32 pSampleRate)
{
    mSampleRate = pSampleRate;
}

Int32 SoundData::GetSampleRate() const
{
    return mSampleRate;
}

void SoundData::SetBytesPerSecond(Int32 pBytesPerSecond)
{
    mBytesPerSecond = pBytesPerSecond;
}

Int32 SoundData::GetBytesPerSecond() const
{
    return mBytesPerSecond;
}

void SoundData::SetBytesPerSample(Int16 pBytesPerSample)
{
    mBytesPerSample = pBytesPerSample;
}

Int16 SoundData::GetBytesPerSample() const
{
    return mBytesPerSample;
}

void SoundData::SetBitsPerSample(Int16 pBitsPerSample)
{
    mBitsPerSample = pBitsPerSample;
}

Int16 SoundData::GetBitsPerSample() const
{
    return mBitsPerSample;
}

void SoundData::SetSoundDataSize(Int32 pSoundDataSize)
{
    mSoundDataSize = pSoundDataSize;
}

Int32 SoundData::GetSoundDataSize() const
{
    return mSoundDataSize;
}

Char* SoundData::GetSoundData()
{
    return mFileData + mSoundDataOffset;
}

void SoundData::SetSoundDataOffset(Int32 pOffset)
{
    mSoundDataOffset= pOffset;
}


} // namespace Gamedesk
