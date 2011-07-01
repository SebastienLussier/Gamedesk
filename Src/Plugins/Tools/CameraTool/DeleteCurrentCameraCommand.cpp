/**
 *  @file       DeleteCurrentCameraCommand.cpp
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       12/02/04.
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
#include "DeleteCurrentCameraCommand.h"
#include "EditorBase.h"
#include "WorldManager.h"
#include "Properties/Camera3DProperties.h"
#include "World/Entity.h"


DeleteCurrentCameraCommand::DeleteCurrentCameraCommand(EditorBase* pEditor) : 
    Command("Delete Current Camera"),
    mEditor(pEditor),
    mCameraRemoved(false)
{
    GD_ASSERT(pEditor);
    
    // Get the current camera to remove.
    mRemovedCurrentCamera = mEditor->GetWorldManager().GetCurrentCamera();
}

DeleteCurrentCameraCommand::~DeleteCurrentCameraCommand()
{
    // Delete the object if we have its responsibility.
    if(mCameraRemoved)
    {
        GD_DELETE(mRemovedCurrentCamera->GetObject());
        GD_DELETE(mRemovedCurrentCamera);
        mRemovedCurrentCamera = 0;
    }

    mEditor = 0;
}

void DeleteCurrentCameraCommand::Do()
{
    mEditor->GetWorldManager().RemoveObject(mRemovedCurrentCamera);
    mCameraRemoved = true;
}

void DeleteCurrentCameraCommand::Undo()
{
    mEditor->GetWorldManager().InsertObject(mRemovedCurrentCamera);

    // Get the cameras.
    WorldManager& worldManager = mEditor->GetWorldManager();
    const std::list<Camera3DProperties*>& camList = worldManager.GetCameras();


    // Find the camera index.
    std::list<Camera3DProperties*>::const_iterator itCamera = camList.begin();
    UInt32 cameraIndex;
    for(cameraIndex = 0; cameraIndex < camList.size(); cameraIndex++)
    {
        if(*itCamera == mRemovedCurrentCamera)
            break;
   
        itCamera++;
    }
    GD_ASSERT(itCamera != camList.end());

    mEditor->GetWorldManager().SetCurrentCamera(cameraIndex);
    mCameraRemoved = false;
}