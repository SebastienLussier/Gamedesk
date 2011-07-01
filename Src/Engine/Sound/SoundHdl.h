/**
 *  @file       SoundHdl.h
 *  @brief	    Managed handle to a sound.
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
#ifndef     _SOUND_HDL_H_
#define     _SOUND_HDL_H_


#include "Sound.h"


 namespace Gamedesk {


class ENGINE_API SoundHdl
{
public:
    SoundHdl();
    SoundHdl(const String& pSoundFileName);
    SoundHdl(const SoundHdl& pOther);
    
    ~SoundHdl();
    
    SoundHdl& operator=(const SoundHdl& pOther);
    Bool operator==(const SoundHdl& pOther);
    Bool operator!=(const SoundHdl& pOther);
    Sound* operator->() const;
    Sound* operator*() const;
    operator Bool() const;

    //! Load a sound.
    void SetSound(const String& pSoundFileName, Bool pIs3DSound = true);

private:
    Sound*  mSound;
};


} // namespace Gamedesk


#endif  //  _SOUND_HDL_H_
