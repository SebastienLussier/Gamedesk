/**
 *  @file       Camera.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       20/12/03.
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
#ifndef     _CAMERA_H_
#define     _CAMERA_H_


#include "Entity.h"


namespace Gamedesk {
	

/**
 *  Camera class used to represent a camera in a 3d world.
 *  The camera object keeps the perspective information, its position and 
 *  its orientation.
 *  @brief  Camera class for a camera in a 3d world.
 *  @author Marco Arsenault.
 *  @date   24/11/03.
 */
class ENGINE_API Camera : public Entity
{
    DECLARE_CLASS(Camera, Entity)

public:
    //! Default constructor.
	Camera();

    //! Destructor.
    virtual ~Camera();

    //! Render the camera.
    virtual void Render() const;

    //! Render the camera as a selection.
    virtual void RenderSelected() const;

    //! Apply the view matrix.
    void ApplyViewMatrix();
    
    /**
     *  Pitch the camera by the angle pAngle.
     *  @param  pAngle the angle in degree for the pitch.
     */
	void Pitch(Float pAngle);

    /**
     *  Yaw the camera by the angle pAngle.
     *  @param  pAngle the angle in degree for the yaw.
     */
    void Yaw(Float pAngle);

    /**
     *  Roll the camera by the angle pAngle.
     *  @param  pAngle the angle in degree for the roll.
     */
    void Roll(Float pAngle);

    /**
     *  Move the camera in the forward/backward direction by pMovement.
     *  @param  pMovement The length of the movement.
     */
	void Move(Float pMovement);

    /**
     *  Pan the camera in the left/right direction by pMovement.
     *  @param  pMovement The length of the pan.
     */
	void PanLeftRight(Float pMovement);

    /**
     *  Pan the camera in the up/down direction by pMovement.
     *  @param  pMovement The length of the pan.
     */
    void PanUpDown(Float pMovement);

    /**
     *  Set the field of view angle.
     *  @param  pAngle The field of view angle in degree.
     */
    void SetFovAngle(Float pAngle);

    //! Returns the field of view angle in degree.
	Float GetFovAngle() const;

    /**
     *  Set the far view clipping distance.
     *  @param  pFarView The distance of the far view clipping plane.
     */
    void SetFarView(Float pFarView);

    //! Returns the far view clpping distance.
	Float GetFarView() const;

    /**
     *  Set the near view clipping distance.
     *  @param  pNearView The distance of the near view clipping plane.
     */
    void SetNearView(Float pNearView);

    //! Returns the near view clipping distance.
	Float GetNearView() const;

    //! Returns the view vector.
    const Vector3f& GetView() const;

    //! Returns the up vector.
    const Vector3f& GetUp() const;

    //! Returns the right vector.
    const Vector3f& GetRight() const;
    
    //! Rotate the camera so that it point at the given position.
    virtual void LookAt( const Vector3f& pLookAtPos );

    virtual void Serialize( Stream& pStream );

protected:
	//! Render the camera model.
	void RenderCameraModel() const;

    //! Apply a spring damp.
    Vector3f SpringDamp(const Vector3f& currPos,    // Current Position
	                    const Vector3f& trgPos,     // Target Position
	                    const Vector3f& prevTrgPos, // Previous Target Position
	                    Double deltaTime,    // Change in Time
	                    Float springConst,  // Hooke's Constant
	                    Float dampConst,    // Damp Constant
	                    Float springLen) const;

properties:
    /**
     * @name    FOV Angle
     * @desc    Field of view angle in degree
     * @range   [5,179]
     */
    Float mFovAngle;

    /**
    * @name     Near Plane Distance
    * @desc     Distance to near clipping plane
    * @range    [0, 100]
    */
    Float mNearView;

    /**
     * @name    Far Plane Distance
     * @desc    Distance to far clipping plane
     * @min     100
     */
    Float mFarView;

protected:
    Vector3f    mView;
    Vector3f    mUp;
    Vector3f    mRight;
};


} // namespace Gamedesk


#endif  //  _CAMERA_H_
