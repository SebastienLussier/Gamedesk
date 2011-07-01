/**
 *  @file       CameraChangeSender.h
 *  @brief	    A sender class to notify the listeners about camera changes.
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
#ifndef     _CAMERA_CHANGE_SENDER_H_
#define     _CAMERA_CHANGE_SENDER_H_


#include "Containers/Containers.h"


class CameraChangeListener;
class Camera;


/**
 *  CameraChangeSender base class.  This class can notify the listeners about
 *  camera changes.
 *  @brief  CameraChangeSender base class.
 *  @author Marco Arsenault.
 *  @date   12/02/04.
 */
class EDITORLIB_API CameraChangeSender
{
    CLASS_DISABLE_COPY(CameraChangeSender);

public:
    //! Default constructor.
	CameraChangeSender();
    //! Destructor.
    virtual ~CameraChangeSender();

    //! Add a camera change listener.
    void AddCameraChangeListener(CameraChangeListener* pListener);

    //! Remove a camera change listener.
    void RemoveCameraChangeListener(CameraChangeListener* pListener);
    
protected:
    //! Notify the listeners that the camera has changed.
    void FireCameraChanged(Camera* pCamera);

private:
    List<CameraChangeListener*>  mCameraChangeListeners;
};


#endif  //  _CAMERA_CHANGE_SENDER_H_