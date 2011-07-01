/**
 *  @file       CameraTool.h
 *  @brief      CameraTool for the editor..
 *  @author     Marco Arsenault
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
#ifndef     _CAMERA_TOOL_H_
#define     _CAMERA_TOOL_H_


#include "EditorTool.h"
#include "CameraManipulator.h"
#include "EntityCreationListener.h"
#include "CameraChangeListener.h"
#include "Input/Keyboard.h"


class CameraToolUI;
class ConfigSection;


/**
 *  
 *  @author Marco Arsenault.
 *  @date   03/02/04
 */
class CAMERATOOL_API CameraTool : public EditorTool, 
                                  public EntityCreationListener,
                                  public CameraChangeListener,
                                  public Keyboard::Listener
                        
{
    DECLARE_CLASS(CameraTool, EditorTool);

public:
    //! Constructor
    CameraTool();
    //! Destructor
    virtual ~CameraTool();

public:
    //! Init the camera tool.
    virtual void Init();
    //! Kill the camera tool.
    virtual void Kill();

    //! Load the tool's settings.
    virtual void LoadSettings( ConfigSection& pSection );

    //! Save the tool's settings.
    virtual void SaveSettings( ConfigSection& pSection ) const;

    //! Create the ui window for the tool.
    virtual void CreateUI();

    //! Destroy the ui window for the tool.
    virtual void DestroyUI();

    //! Called when an entity is created.
    virtual void OnEntityCreated(EntityProperties* pEntityProperties);

    //! Called when an entity is deleted.
    virtual void OnEntityDeleted(EntityProperties* pEntityProperties);

    //! Called when the current camera is changed.
    virtual void OnCameraChanged(CameraProperties* pCamera);
    
    //! Called when the given key is pressed.
    virtual void OnKeyPressed(const Keyboard::Key& pKey);
    
protected:
    friend CameraToolUI;

    //! Select a new camera.
    void SelectCamera(Int32 pCameraIndex);

private:
    CameraManipulator   mCameraManipulator; //!< The Camera manipulator for the main view.
    CameraToolUI*       mCameraToolUI;
};


#endif  //  _CAMERA_TOOL_H_
