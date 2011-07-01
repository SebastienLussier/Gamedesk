/**
 *  @file       LookCamera.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       04/05/04.
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
#ifndef     _LOOK_CAMERA_H_
#define     _LOOK_CAMERA_H_


#include "Camera.h"


namespace Gamedesk {


/**
 *  LookCamera class used to represent a camera in a 3d world that looks at an
 *  entity (set with SetLookedEntity).
 *  @brief  LookCamera class for a camera in a 3d world that looks at an entity.
 *  @author Marco Arsenault.
 *  @date   04/05/04.
 */
class ENGINE_API LookCamera : public Camera
{
    DECLARE_CLASS(LookCamera, Camera)

public:
    //! Default constructor.
	LookCamera();

    //! Destructor.
    virtual ~LookCamera();

    //! Update the camera.
    virtual void Update(Double pElapsedTime);

    //! Set the object to look.
    void SetLookedEntity(Entity* pEntity);

private:
	Entity*   mLookedEntity;
};


} // namespace Gamedesk


#endif  //  _LOOK_CAMERA_H_