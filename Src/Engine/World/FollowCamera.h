/**
 *  @file       FollowCamera.h
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
#ifndef     _FOLLOW_CAMERA_H_
#define     _FOLLOW_CAMERA_H_


#include "Camera.h"


namespace Gamedesk {


/**
 *  FollowCamera class used to represent a camera in a 3d world that follows an
 *  entity (set with SetFollowedEntity).
 *  @brief  FollowCamera3D class for a camera in a 3d world that follows an entity.
 *  @author Marco Arsenault.
 *  @date   04/05/04.
 */
class ENGINE_API FollowCamera : public Camera
{
    DECLARE_CLASS(FollowCamera, Camera)

public:
    //! Default constructor.
	FollowCamera();

    //! Destructor.
    virtual ~FollowCamera();

    //! Update the camera.
    virtual void Update(Double pElapsedTime);

    //! Set the entity to follow.
    void SetFollowedEntity(Entity* pEntity);

    //! Set the spring force.
    void SetSpringForce(Float pSpringForce);

    //! Get the spring force.
    Float GetSpringForce() const;

    //! Set the follow distance.
    void SetFollowDistance(Float pFollowDistance);

    //! Get the follow distance
    Float GetFollowDistance() const;

    //! Set the up offset.
    void SetUpOffset(Float pUpOffset);

    //! Get the up offset.
    Float GetUpOffset() const;

properties:
    /**
    * @name    Spring Force
    * @desc    Force of the spring which will try to bring the camera to it's ideal position.
    * @range   [0.1f, 5.0f]
    */
    Float mSpringForce;

    /**
    * @name    Distance
    * @desc    Distance from the followed entity.
    * @range   [1, 100]
    */
    Float mFollowDistance;

    /**
    * @name    Height
    * @desc    Height relative to the followed entity.
    * @range   [0, 10]
    */
    Float mUpOffset;

private:
	Entity*     mFollowedEntity;
    Vector3f    mCurrentObjectPosition;
    Vector3f    mPreviousObjectPosition;
};


} // namespace Gamedesk


#endif  //  _FOLLOW_CAMERA_H_
