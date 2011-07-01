/**
 *  @file       CameraTool.cpp
 *  @brief      Camera tool for the editor..
 *  @author     Marco Arsenault.
 *  @date       03/02/04.
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
#include "CameraToolDLL.h"
#include "CameraTool.h"
#include "CameraToolUI.h"
#include "EditorBase.h"
#include "Resource/ResourceManager.h"
#include "Config/ConfigFile.h"
#include "System/SystemInfo.h"
#include "Viewer.h"
#include "World/Camera.h"
#include "Properties/CameraProperties.h"
#include "WorldManager.h"
#include "Input/InputSubsystem.h"


IMPLEMENT_CLASS(CameraTool);


CameraTool::CameraTool() : 
    mCameraToolUI(NULL)
{
}

CameraTool::~CameraTool()
{
}

void CameraTool::Init()
{
    // Make sure the editor has been set.
    GD_ASSERT(GetEditor());

    Camera* camera = GetEditor()->GetWorldManager().GetCurrentCamera();
    if( camera )
    {
        CameraProperties* prop = Cast<CameraProperties>( camera->GetProperties() );
        if( mCameraManipulator.CanManipulate( prop ) )
            mCameraManipulator.SetManipulatedEntity( prop );        
    }
    GetEditor()->GetMainView()->AddManipulator( &mCameraManipulator );

    GetEditor()->GetWorldManager().AddEntityCreationListener(this);
    GetEditor()->GetWorldManager().AddCameraChangeListener(this);

    InputSubsystem::GetKeyboard().AddKeyListener(this, Keyboard::Key_NumPad0, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().AddKeyListener(this, Keyboard::Key_NumPad1, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().AddKeyListener(this, Keyboard::Key_NumPad2, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().AddKeyListener(this, Keyboard::Key_NumPad3, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().AddKeyListener(this, Keyboard::Key_NumPad4, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().AddKeyListener(this, Keyboard::Key_NumPad5, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().AddKeyListener(this, Keyboard::Key_NumPad6, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().AddKeyListener(this, Keyboard::Key_NumPad7, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().AddKeyListener(this, Keyboard::Key_NumPad8, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().AddKeyListener(this, Keyboard::Key_NumPad9, Keyboard::Key_Pressed);

    mCameraToolUI->UpdateUI();
}

void CameraTool::Kill()
{
    InputSubsystem::GetKeyboard().RemoveKeyListener(this, Keyboard::Key_NumPad0, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().RemoveKeyListener(this, Keyboard::Key_NumPad1, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().RemoveKeyListener(this, Keyboard::Key_NumPad2, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().RemoveKeyListener(this, Keyboard::Key_NumPad3, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().RemoveKeyListener(this, Keyboard::Key_NumPad4, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().RemoveKeyListener(this, Keyboard::Key_NumPad5, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().RemoveKeyListener(this, Keyboard::Key_NumPad6, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().RemoveKeyListener(this, Keyboard::Key_NumPad7, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().RemoveKeyListener(this, Keyboard::Key_NumPad8, Keyboard::Key_Pressed);
    InputSubsystem::GetKeyboard().RemoveKeyListener(this, Keyboard::Key_NumPad9, Keyboard::Key_Pressed);

    GetEditor()->GetMainView()->RemoveManipulator(&mCameraManipulator);

    GetEditor()->GetWorldManager().RemoveEntityCreationListener(this);
    GetEditor()->GetWorldManager().RemoveCameraChangeListener(this);
}

void CameraTool::LoadSettings( ConfigSection& pSection )
{
    Super::LoadSettings( pSection );
}

void CameraTool::SaveSettings( ConfigSection& pSection ) const
{
    Super::SaveSettings( pSection );
}

void CameraTool::CreateUI()
{
    if(!mCameraToolUI)
    {
        mCameraToolUI = new CameraToolUI(this, GetEditor()->GetWorkspace());
        mCameraToolUI->setName("Camera Tool");
        SetWindow(mCameraToolUI);
        mCameraToolUI->show();
    }
}

void CameraTool::DestroyUI()
{
    if(mCameraToolUI)
    {
        mCameraToolUI->hide();
        SetWindow(0);
    } 
}

void CameraTool::OnEntityCreated(EntityProperties* pEntityProperties)
{
    if(pEntityProperties->IsA(CameraProperties::StaticClass()))
    {
        mCameraToolUI->UpdateUI();
    }
}

void CameraTool::OnEntityDeleted(EntityProperties* pEntityProperties)
{
    if(pEntityProperties->IsA(CameraProperties::StaticClass()))
    {
        mCameraToolUI->UpdateUI();
    }
}

void CameraTool::OnCameraChanged(CameraProperties* pCamera)
{
    mCameraManipulator.SetManipulatedEntity(pCamera);
    mCameraToolUI->UpdateUI();
}

void CameraTool::SelectCamera(Int32 pCameraIndex)
{
    GetEditor()->GetWorldManager().SetCurrentCamera(pCameraIndex);
}

void CameraTool::OnKeyPressed(const Keyboard::Key& pKey)
{
    SelectCamera(pKey - Keyboard::Key_NumPad0);
}
