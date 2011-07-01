/**
 *  @file       SoundHdl.cpp
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
#include "Engine.h"
#include "SoundHdl.h"
#include "SoundManager.h"
#include "SoundData.h"


namespace Gamedesk {
	
	
SoundHdl::SoundHdl() : 
    mSound(0)
{
}  

SoundHdl::SoundHdl(const String& pSoundFileName) : 
    mSound(0)
{
    SetSound(pSoundFileName);
}    

SoundHdl::SoundHdl( const SoundHdl& pOther ) : 
    mSound(0)
{
    if(pOther)
        SetSound(pOther->GetSoundData().GetFileName());
}

SoundHdl::~SoundHdl()
{
    if(mSound)
    {
        SoundManager::Instance().Release(mSound);
        mSound = 0;
    }
}

SoundHdl& SoundHdl::operator=(const SoundHdl& pOther)
{
    if(*this == pOther || mSound == pOther.mSound)
        return *this;

    SetSound(pOther->GetSoundData().GetFileName());

    return *this;
}

Bool SoundHdl::operator==(const SoundHdl& pOther)
{
    return mSound == pOther.mSound;
}

Bool SoundHdl::operator!=(const SoundHdl& pOther)
{
    return mSound != pOther.mSound;
}

Sound* SoundHdl::operator->() const
{
    GD_ASSERT(mSound);
    return mSound;
}

Sound* SoundHdl::operator*() const
{
    GD_ASSERT(mSound);
    return mSound;
}

SoundHdl::operator Bool () const
{
    return mSound != 0;
}

void SoundHdl::SetSound(const String& pSoundFileName, Bool pIs3DSound)
{
    try
    {
        if(mSound)
        {
            SoundManager::Instance().Release(mSound);
            mSound = 0;
        }

        mSound = SoundManager::Instance().Create(pSoundFileName, pIs3DSound);
    }
    catch(Exception& /*e*/)
    {
        mSound = 0;
    }        
}


} // namespace Gamedesk
