/**
 *  @file       FMODTables.h
 *  @brief	    Array used to convert between Gamedesk enums and FMOD values.
 *  @author     Marco Arsenault.
 *  @date       22/02/04.
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
#ifndef     _FMOD_TABLES_H_
#define     _FMOD_TABLES_H_


#include "FMODSoundDll.h"


namespace Gamedesk {


const Int32 GDToFMODSpeakerMode[] = {   FSOUND_SPEAKERMODE_DOLBYDIGITAL,
                                        FSOUND_SPEAKERMODE_HEADPHONES,
                                        FSOUND_SPEAKERMODE_MONO,
                                        FSOUND_SPEAKERMODE_QUAD,
                                        FSOUND_SPEAKERMODE_STEREO,
                                        FSOUND_SPEAKERMODE_SURROUND };

const Int32 GDToFMODOutputType[] = {    -1, /*AUTODETECT*/
                                        FSOUND_OUTPUT_NOSOUND,
                                        FSOUND_OUTPUT_WINMM,
                                        FSOUND_OUTPUT_DSOUND,
                                        FSOUND_OUTPUT_A3D,
                                        FSOUND_OUTPUT_OSS,
                                        FSOUND_OUTPUT_ESD,
                                        FSOUND_OUTPUT_ALSA  };

const Int32 GDToFMODMixerType[] = {     FSOUND_MIXER_AUTODETECT,
                                        FSOUND_MIXER_BLENDMODE,
                                        FSOUND_MIXER_MMXP5,
                                        FSOUND_MIXER_MMXP6,
                                        FSOUND_MIXER_QUALITY_AUTODETECT,
                                        FSOUND_MIXER_QUALITY_FPU,
                                        FSOUND_MIXER_QUALITY_MMXP5,
                                        FSOUND_MIXER_QUALITY_MMXP6  };

const Int32 GDToFMODSoundMode[] = {     FSOUND_LOOP_OFF,
                                        FSOUND_LOOP_NORMAL  };

const Int32 GDToFMODConstant[] = {      FSOUND_STEREOPAN    };

  
} // namespace Gamedesk
  
  
#endif  //  _FMOD_TABLES_H_
