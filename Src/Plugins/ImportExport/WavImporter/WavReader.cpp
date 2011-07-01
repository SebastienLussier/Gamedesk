/**
 *  @file       WavReader.cpp
 *  @brief      Read a Wav file into it's corresponding data structures.
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
#include "WavImporterDll.h"
#include "WavReader.h"

#include "Resource/ResourceManager.h"


WavReader::WavReader()
{
}

SoundData* WavReader::Read(const String& pFileName)
{
    SoundData* newSoundData = GD_NEW(SoundData, this, "SoundData");
    newSoundData->SetFileName(pFileName);

    Char identifier[5];
    identifier[4] = '\0';

    // Open the file stream.
    std::ifstream fileStream(pFileName.c_str(), std::ios::in | std::ios::binary);

    // Read "RIFF"
    fileStream.read(identifier, 4);
    if(strcmp(identifier, "RIFF") != 0)
        throw ResourceImportException( String("The wav file should start with RIFF"), Here );

    // Read the total length (filesize - 8).
    Int32 fileDataSize;
    fileStream.read((Char*)(&fileDataSize), 4);
    fileDataSize += 8;
    newSoundData->SetFileDataSize(fileDataSize);

    // Read "WAVE".
    fileStream.read(identifier, 4);
    if(strcmp(identifier, "WAVE") != 0)
        throw ResourceImportException( String("The wav file should contain the WAVE identifier."), Here );
    
    // Read "fmt_"
    fileStream.read(identifier, 4);
    if(strcmp(identifier, "fmt ") != 0)
        throw ResourceImportException( String("The wav file should contain the fmt_ identifier."), Here );
    
    // Read the Length of the format chunk.
    Int32 chunkFileSize;
    fileStream.read((Char*)(&chunkFileSize), 4);
 
    // Read a drummy short.
    Int16 dummyShort;
    fileStream.read((Char*)(&dummyShort), 2);

    // Read the number of channels.
    Int16 nbChannels;
    fileStream.read((Char*)(&nbChannels), 2);
    newSoundData->SetNbChannels(nbChannels);

    // Read the sample rate.
    Int32 sampleRate;
    fileStream.read((Char*)(&sampleRate), 4);
    newSoundData->SetSampleRate(sampleRate);

    // Read the bytes per second.
    Int32 bytesPerSecond;
    fileStream.read((Char*)(&bytesPerSecond), 4);
    newSoundData->SetBytesPerSecond(bytesPerSecond);

    // Read the bytes per sample.
    Int16 bytesPerSample;
    fileStream.read((Char*)(&bytesPerSample), 2);
    newSoundData->SetBytesPerSample(bytesPerSample);

    // Read the bits per sample.
    Int16 bitsPerSample;
    fileStream.read((Char*)(&bitsPerSample), 2);
    newSoundData->SetBitsPerSample(bitsPerSample);

    // Read "data"
    fileStream.read(identifier, 4);
    if(strcmp(identifier, "data") != 0)
        throw ResourceImportException( String("The wav file should contain the data identifier."), Here );

    // Read the size of sound data.
    Int32 soundDataSize;
    fileStream.read((Char*)(&soundDataSize), 4);
    newSoundData->SetSoundDataSize(soundDataSize);
    newSoundData->SetSoundDataOffset(44);
    
    // Read the data itself.
    fileStream.seekg(0, std::ios::beg);
    Char* data = GD_NEW_ARRAY(Char, fileDataSize + 1, this, "Data");
    data[fileDataSize] = '\0';
    fileStream.read(data, fileDataSize);
    newSoundData->SetFileData(data);

    fileStream.close();
 
    return newSoundData;
}