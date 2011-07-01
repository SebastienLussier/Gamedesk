/**
 *  @file       SoundManager.cpp
 *  @brief	    Manage access to sounds.
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
#include "SoundManager.h"
#include "SoundSubsystem.h"
#include "Sound.h"
#include "SoundData.h"
#include "Resource/ResourceManager.h"


namespace Gamedesk {
	
	
SoundManager SoundManager::mInstance;
SoundManager& SoundManager::Instance()
{
    return mInstance;
}


SoundManager::SoundManager()
{
}


Sound* SoundManager::Create(const String& pSoundFileName, Bool pIs3DSound)
{
    // Ask the sound subsystem to create a sound for us.
    Sound* sound = Cast<Sound>(SoundSubsystem::Instance()->Create(Sound::StaticClass()));

    std::map<String, SoundData*>::iterator itFind = mLoadedSounds.find(pSoundFileName);
    if(itFind != mLoadedSounds.end())
    {
        itFind->second->AddRef();

        sound->Create(itFind->second, pIs3DSound);
    }
    else
    {
        ResourceImporter* importer = ResourceManager::Instance()->GetImporterForFile(pSoundFileName, SoundData::StaticClass());
        GD_ASSERT(importer);
        SoundData* soundData = Cast<SoundData>(importer->Import(pSoundFileName));
        mLoadedSounds[pSoundFileName] = soundData;

        sound->Create(soundData, pIs3DSound);
        sound->AddRef();
    }        

    sound->Init();
    return sound;
}

void SoundManager::Release(Sound* pSound)
{
    String soundFileName = pSound->GetSoundData().GetFileName();
    std::map<String, SoundData*>::iterator itFind = mLoadedSounds.find(soundFileName);
    GD_ASSERT(itFind != mLoadedSounds.end());
            
    if(itFind->second->RemoveRef())
    {
        itFind->second->Kill();
        GD_DELETE(itFind->second);

        mLoadedSounds.erase(itFind);
    }

    GD_DELETE(pSound);
}


} // namespace Gamedesk
