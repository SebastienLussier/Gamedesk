/**
 *  @file       WavReader.h
 *  @brief      Read a wav file into it's corresponding data structures
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
#ifndef     _WAV_READER_H_
#define     _WAV_READER_H_


#include "Sound/SoundData.h"


/**
 *  File reader used to parse the Wav file and fill a SoundData object with its content.
 */
class WavReader
{
public:
    WavReader();

    SoundData* Read(const String& pFileName);

private:
    WavReader(const WavReader& pOther);
    const WavReader& operator=(const WavReader& pOther);
};


#endif  //  _WAV_READER_H_
