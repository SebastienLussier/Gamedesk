/**
 *  @file       SoundBrowserTool.h
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
#ifndef     _SOUNDBROWSERTOOL_H_
#define     _SOUNDBROWSERTOOL_H_


#include "EditorTool.h"
#include "Sound/SoundHdl.h"


class ConfigSection;
class SoundBrowserUI;


/**
 *  
 *  @author Marco Arsenault.
 *  @date   21/02/04
 */
class SoundBrowserTool : public EditorTool
{
    DECLARE_CLASS(SoundBrowserTool, EditorTool);

public:
    SoundBrowserTool();
    virtual ~SoundBrowserTool();

    void Init();
    
    virtual void LoadSettings( ConfigSection& pSection );
    virtual void SaveSettings( ConfigSection& pSection ) const;

    //! Called when a sound is selected in the UI list.
    void SetSelectedSound(const String& pSoundFileName);
    //! Called when the Play/Stop button is pressed.
    void PlayStopPressed();

protected:
    //! Create the ui window for the tool.
    virtual void CreateUI();
    //! Destroy the ui window for the tool.
    virtual void DestroyUI();

    //! Play the current sound.
    void PlaySound();
    //! Stop the current sound.
    void StopSound();

private:
    SoundBrowserUI* mSoundBrowserUI;

    SoundHdl        mSound;
    Bool            mSoundPlaying;
};


#endif  //  _SOUNDBROWSERTOOL_H_
