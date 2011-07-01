/**
 *  @file       CameraToolUI.cpp
 *  @brief      The user interface for the camera tool.
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
#include "CameraToolUI.h"
#include "CameraTool.h"
#include "EditorBase.h"
#include "PropertyList/PropertyList.h"
#include "WorldManager.h"
#include "World/Camera.h"
#include "World/LookCamera.h"
#include "World/FollowCamera.h"
#include "Properties/CameraProperties.h"
#include "Command/CommandManager.h"
#include "Containers/Containers.h"
#include "CreateEntityCommand.h"
#include "DeleteEntityCommand.h"
//Added by qt3to4:
#include <QGridLayout>


CameraToolUI::CameraToolUI(CameraTool* pCameraTool, QWidget* pParent) : 
    QCameraTool(pParent, 0, Qt::WStyle_Tool),
    mCameraTool(pCameraTool)
{
    GD_ASSERT(pCameraTool);

    QGridLayout* layout = new QGridLayout( mPropertyEditorFrame );
    mPropertyList = new PropertyList( mPropertyEditorFrame );

    layout->addWidget( mPropertyList, 0, 0 );
        
    UpdateUI();
}

CameraToolUI::~CameraToolUI()
{
}

void CameraToolUI::UpdateUI()
{
    mCamerasCB->clear();

    WorldManager& worldManager = mCameraTool->GetEditor()->GetWorldManager();
    Camera* currentCamera = worldManager.GetCurrentCamera();
    const std::list<Camera*>& camList = worldManager.GetCameras();

    // Fill the camera list and select the current one.
    Int32 currentCameraIndex = -1;
    std::list<Camera*>::const_iterator itCamera = camList.begin();
    for(UInt32 i = 0; i < camList.size(); i++)
    {
        // Get the index if it's the current camera.
        if( *itCamera == currentCamera )
            currentCameraIndex = i;
   
        // Insert the item in the combobox.
        String cameraName = "Camera " + ToString(i);
        mCamerasCB->insertItem(cameraName.c_str());

        itCamera++;
    }
    
    if( currentCameraIndex != -1 )
    {
        // Set the current camera.
        mCamerasCB->setCurrentItem(currentCameraIndex);
        mPropertyList->SetEditedObject( currentCamera->GetProperties() );
    }
}

void CameraToolUI::OnCameraActivated(int pIndex)
{
    mCameraTool->SelectCamera(pIndex);
}

void CameraToolUI::OnNewNormalCamera()
{
    // Get the command manager.
    CommandManager& commandManager = mCameraTool->GetEditor()->GetCommandManager();

    // Create the command to create the object.
    CreateEntityCommand* cmd = new CreateEntityCommand(mCameraTool->GetEditor(), Camera::StaticClass());
    commandManager.PerformCommand(cmd);
}

void CameraToolUI::OnNewLookCamera()
{
    // Get the command manager.
    CommandManager& commandManager = mCameraTool->GetEditor()->GetCommandManager();

    // Create the command to create the object.
    CreateEntityCommand* cmd = new CreateEntityCommand(mCameraTool->GetEditor(), LookCamera::StaticClass());
    commandManager.PerformCommand(cmd);
}

void CameraToolUI::OnNewFollowCamera()
{
    // Get the command manager.
    CommandManager& commandManager = mCameraTool->GetEditor()->GetCommandManager();

    // Create the command to create the object.
    CreateEntityCommand* cmd = new CreateEntityCommand(mCameraTool->GetEditor(), FollowCamera::StaticClass());
    commandManager.PerformCommand(cmd);
}

void CameraToolUI::OnDeleteCurrentCamera()
{
    if( mCameraTool->GetEditor()->GetWorldManager().GetCameras().size() > 1 )
    {
        // Get the command manager.
        CommandManager& commandManager = mCameraTool->GetEditor()->GetCommandManager();

        // Create the DeleteCurrentCamera command.
        EditorBase* editor = mCameraTool->GetEditor();
        DeleteEntityCommand* cmd = new DeleteEntityCommand( editor, editor->GetWorldManager().GetCurrentCamera() );
        commandManager.PerformCommand(cmd);
    }
    else
    {
        QMessageBox::information( this, "Gamedesk Editor", "Can't delete the only camera in the scene." );
    }
}
