/**
 *  @file       SoundBrowserTool.cpp
 *  @brief      A tool to check the sounds..
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
#include "SoundBrowserDll.h"
#include "SoundBrowserTool.h"
#include "SoundBrowserUI.h"
#include "EditorBase.h"
#include "Sound/SoundData.h"


IMPLEMENT_CLASS(SoundBrowserTool);


SoundBrowserTool::SoundBrowserTool() : 
    mSoundBrowserUI(0),
    mSoundPlaying(false)
{
}

SoundBrowserTool::~SoundBrowserTool()
{
}

void SoundBrowserTool::LoadSettings( ConfigSection& pSection )
{
    Super::LoadSettings( pSection );

    // #TODO: Load your tool's settings here.
}

void SoundBrowserTool::SaveSettings( ConfigSection& pSection ) const
{
    Super::SaveSettings( pSection );

    // #TODO: Save your tool's settings here.
}

void SoundBrowserTool::SetSelectedSound(const String& pSoundFileName)
{
    if(mSoundPlaying)
        StopSound();
    
    mSound.SetSound(pSoundFileName, false);
    mSound->SetMode(Sound::kSoundModeLoop);

    if(mSoundBrowserUI->mAutoPlayCheckBox->isChecked())
        PlaySound();

    // Set the file name label.
    Int32 fileNamePos = pSoundFileName.find_last_of("/") + 1;
    String fileName = pSoundFileName.substr(fileNamePos, pSoundFileName.length() - fileNamePos);
    mSoundBrowserUI->SetFileNameLabel(fileName);

    // Set the attributes label.
    String attributes = "";
    attributes += ToString(mSound->GetSoundData().GetSampleRate()) + String(" Hz; ");
    attributes += ToString(mSound->GetSoundData().GetBitsPerSample()) + String("-bit; ");
    attributes += String(mSound->GetSoundData().GetNbChannels() == 1 ? "Mono" : "Stereo");
    //44 100Hz; 16-bit; Stereo
    mSoundBrowserUI->SetAttributesLabel(attributes);

    // Set the file size label.
    String fileSize = "";
    String fileSizeUnit(" KB");
    Int32 totalNbBytes = mSound->GetSoundData().GetFileDataSize();
    Float fileDataSize = totalNbBytes / 1024.0f;
    if(fileDataSize >= 1024.0f)
    {
        fileDataSize /= 1024.0f;
        fileSizeUnit = " MB";
    }
    
    fileSize += ToString(fileDataSize, 4) + fileSizeUnit;
    fileSize += String(" (") + ToString(totalNbBytes) + String(" bytes)");
    mSoundBrowserUI->SetFileSizeLabel(fileSize);

    // Set the sound length label.
    Int32 soundDataSize = mSound->GetSoundData().GetSoundDataSize();
    Int32 nbSamples = soundDataSize / mSound->GetSoundData().GetBytesPerSample();
    Float nbSeconds = soundDataSize / static_cast<Float>(mSound->GetSoundData().GetBytesPerSecond());
    Int32 nbMinutes = nbSeconds / 60;
    nbSeconds -= nbMinutes * 60;

    String minutes = String(nbMinutes < 10 ? "0" : "") + ToString(nbMinutes) + String(":");
    String seconds = String(nbSeconds < 10 ? "0" : "") + ToString(nbSeconds, 2) + String(" ");
    String samples = String(" (") + ToString(nbSamples) + String(" samples)");
    String soundLength = minutes + seconds + samples;
    mSoundBrowserUI->SetSoundLengthLabel(soundLength);
}

void SoundBrowserTool::PlayStopPressed()
{
    if(mSoundPlaying)
        StopSound();
    else
        PlaySound();
}

void SoundBrowserTool::CreateUI()
{
    if(!mSoundBrowserUI)
    {
        mSoundBrowserUI = new SoundBrowserUI(GetEditor()->GetWorkspace(), this);
        mSoundBrowserUI->setName("Sound Browser");
        mSoundBrowserUI->show();

        SetWindow(mSoundBrowserUI);
    }
}

void SoundBrowserTool::DestroyUI()
{
    if(mSoundBrowserUI)
    {
        mSoundBrowserUI->hide();
        SetWindow(0);
    }  
}

void SoundBrowserTool::Init()
{
    if(mSoundBrowserUI)
    {
        mSoundBrowserUI->Init();
    }
}

void SoundBrowserTool::PlaySound()
{
    mSound->Play();
    mSoundPlaying = true;
    mSoundBrowserUI->mPlayStopBtn->setText("Stop");
}

void SoundBrowserTool::StopSound()
{
    mSound->Stop();
    mSoundPlaying = false;
    mSoundBrowserUI->mPlayStopBtn->setText("Play");
}
