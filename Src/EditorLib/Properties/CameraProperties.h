/**
 *  @file       CameraProperties.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       31/01/04.
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
#ifndef     _CAMERA_PROPERTIES_H_
#define     _CAMERA_PROPERTIES_H_


#include "EntityProperties.h"


class Camera3D;


/**
 *  CameraProperties class.  This object is used as a facade to a Camera.
 *  As a PropertiesSender and PropertiesListener, it can be connected with
 *  other Senders/Listeners to keep the data up to date.
 *  @brief  CameraProperties class.
 *  @author Marco Arsenault.
 *  @date   31/01/04.
 */
class EDITORLIB_API CameraProperties : public EntityProperties
{
    DECLARE_CLASS(CameraProperties, EntityProperties);

public:
    //! Constructor.
	CameraProperties();

    //! Destructor.
    virtual ~CameraProperties();

    //! Initialize all properties.
    virtual void InitProperties();

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
    void SetFovAngle( Float pAngle );

    /**
     *  Get the field of view angle.
     *  @return The field of view angle in degree.
     */
    Float GetFovAngle() const;

    /**
     *  Set the far view clipping distance.
     *  @param  pFarView The distance of the far view clipping plane.
     */
    void SetFarView(Float pFarView);
    Float GetFarView() const;

    /**
     *  Set the near view clipping distance.
     *  @param  pNearView The distance of the near view clipping plane.
     */
    void SetNearView(Float pNearView);
    Float GetNearView() const;
    
    /**
     *  Rotate the camera so that it point at the given position.
     */    
    void LookAt( const Vector3f& pLookAtPos );
    
private:
    PropertyFloat   mFovAngle;
    PropertyFloat   mFarView;
    PropertyFloat   mNearView;
};


#endif  //  _CAMERA_PROPERTIES_H_
