/**
 *  @file       CameraToolUI.h
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
#ifndef     _CAMERA_TOOL_UI_H_
#define     _CAMERA_TOOL_UI_H_


#include "Qt/QCameraTool.h"
#include "CameraToolDLL.h"


class CameraTool;
class PropertyList;


class CAMERATOOL_API CameraToolUI : public QCameraTool
{
    CLASS_DISABLE_COPY(CameraToolUI);

public:
    //! Constructor.
    CameraToolUI(CameraTool* pCameraTool, QWidget* pParent);
    //! Destructor.
    virtual ~CameraToolUI();

    //! Update the UI.
    void UpdateUI();

protected:
    //! Called when a new camera is activated.
    virtual void OnCameraActivated(int pIndex);
    //! Called when the "New Normal Camera" button is clicked.
    virtual void OnNewNormalCamera();
    //! Called when the "New Look Camera" button is clicked.
    virtual void OnNewLookCamera();
    //! Called when the "New Follow Camera" button is clicked.
    virtual void OnNewFollowCamera();
    //! Called when the "Delete current camera" button is clicked.
    virtual void OnDeleteCurrentCamera();

private:
    CameraTool*     mCameraTool;
    PropertyList*   mPropertyList;
};


#endif  //  _CAMERA_TOOL_UI_H_
