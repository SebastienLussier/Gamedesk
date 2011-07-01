/**
 *  @file       SoundBrowserUI.cpp
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
#include "SoundBrowserDll.h"
#include "SoundBrowserUI.h"
#include "SoundBrowserTool.h"

#include "EditorBase.h"
#include "WorldManager.h"

#include "QEditorDragObject.h"
//Added by qt3to4:
#include <QGridLayout>
#include "World/Entity.h"
#include "World/Camera.h"
#include "World/Sound3D.h"
#include "Properties/CameraProperties.h"
#include "Properties/SoundSubsystemProperties.h"
#include "PropertyList/PropertyList.h"


SoundBrowserUI::SoundBrowserUI(QWidget* pParent, SoundBrowserTool* pTool) :
    QSoundBrowser(pParent, 0, Qt::WStyle_Tool),
    mTool(pTool)
{
    QDir dir("Data/Sounds/");
    dir.setNameFilter("*.wav");
    QStringList strList(dir.entryList(QDir::Files));

    mSoundList->clear();
    mSoundList->insertStringList(strList);

    QGridLayout* layout = new QGridLayout( mSoundSubsystemFrame );
    mPropertyList = new PropertyList( mSoundSubsystemFrame );
    layout->addWidget( mPropertyList, 0, 0 );
}

SoundBrowserUI::~SoundBrowserUI()
{
    GD_DELETE(mSoundSubsystemProperties);
}

void SoundBrowserUI::Init()
{
    mSoundSubsystemProperties = new SoundSubsystemProperties();
    mPropertyList->SetEditedObject( mSoundSubsystemProperties );
}

void SoundBrowserUI::SetFileNameLabel(const String& pFileName)
{
    mFileNameLbl->setText(pFileName.c_str());
}

void SoundBrowserUI::SetAttributesLabel(const String& pAttributes)
{
    mAttributesLbl->setText(pAttributes.c_str());
}

void SoundBrowserUI::SetFileSizeLabel(const String& pFileSize)
{
    mFileSizeLbl->setText(pFileSize.c_str());
}

void SoundBrowserUI::SetSoundLengthLabel(const String& pSoundLength)
{
    mSoundLengthLbl->setText(pSoundLength.c_str());
}

void SoundBrowserUI::OnListItemSelected()
{
    if(!mSoundList->currentText().isEmpty())
    {
        mTool->SetSelectedSound(String("Data/Sounds/") + String(mSoundList->currentText().ascii()));
    }
}

void SoundBrowserUI::OnPlayStopPressed()
{
    mTool->PlayStopPressed();
}

void SoundBrowserUI::OnListItemDoubleClicked(Q3ListBoxItem* pItem)
{
    Camera* camera = mTool->GetEditor()->GetWorldManager().GetCurrentCamera();
    Vector3f  pos = camera->GetPosition() + (camera->GetView() * 2.0f);
    
    Sound3D* newSound = Cast<Sound3D>( mTool->GetEditor()->GetWorldManager().SpawnEntity( Sound3D::StaticClass(), pos ) );
    newSound->SetSound( String("Data/Sounds/") + String(pItem->text().ascii()) );
}
