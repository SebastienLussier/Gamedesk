/**
 *  @file       SoundBrowserUI.h
 *  @brief      UI for the Sound Browser.
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
#ifndef     _SOUNDBROWSER_UI_H_
#define     _SOUNDBROWSER_UI_H_


#include "Qt/QSoundBrowser.h"


class SoundBrowserTool;
class PropertyList;
class SoundSubsystemProperties;


class SoundBrowserUI : public QSoundBrowser
{
public:
    //! Constructor.
    SoundBrowserUI(QWidget* pParent, SoundBrowserTool* pTool);
    //! Destructor.
    ~SoundBrowserUI();

    void Init();

    //! Sets the file name label.
    void SetFileNameLabel(const String& pFileName);
    //! Sets the attributes label.
    void SetAttributesLabel(const String& pAttributes);
    //! Sets the file size label.
    void SetFileSizeLabel(const String& pFileSize);
    //! Sets the sound length label.
    void SetSoundLengthLabel(const String& pSoundLength);

protected:
    //! Called when a list item is double clicked.
    void OnListItemDoubleClicked(Q3ListBoxItem* pItem);
    //! Called when a list item is selected.
    void OnListItemSelected();
    //! Called when the Play/Stop button is pressed.
    void OnPlayStopPressed();
    
private:
    SoundBrowserTool*           mTool;
    PropertyList*               mPropertyList;
    SoundSubsystemProperties*   mSoundSubsystemProperties;
};


#endif  //  _SOUNDBROWSER_UI_H_
