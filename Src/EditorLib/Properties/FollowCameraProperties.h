/**
 *  @file       FollowCameraProperties.h
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
#ifndef     _FOLLOW_CAMERA_PROPERTIES_H_
#define     _FOLLOW_CAMERA_PROPERTIES_H_


#include "CameraProperties.h"


/**
 *  FollowCameraProperties class.  This object is used as a facade to a FollowCamera.
 *  As a PropertiesSender and PropertiesListener, it can be connected with
 *  other Senders/Listeners to keep the data up to date.
 *  @brief  FollowCamera3DProperties class.
 *  @author Marco Arsenault.
 *  @date   04/05/04.
 */
class EDITORLIB_API FollowCameraProperties : public CameraProperties
{
    DECLARE_CLASS(FollowCameraProperties, CameraProperties);

public:
    //! Constructor.
	FollowCameraProperties();

    //! Destructor.
    virtual ~FollowCameraProperties();

    //! Initialize all properties.
    virtual void InitProperties();

    //! Set the spring force.
    void SetSpringForce(Float pSpringForce);

    //! Get the spring force.
    Float GetSpringForce() const;

    //! Set the follow distance.
    void SetFollowDistance(Float pFollowDistance);

    //! Get the follow distance.
    Float GetFollowDistance() const;

    //! Set the up offset.
    void SetUpOffset(Float pUpOffset);

    //! Get the up offset.
    Float GetUpOffset() const;
 
private:
    PropertyFloat   mSpringForce;
    PropertyFloat   mFollowDistance;
    PropertyFloat   mUpOffset;
};


#endif  //  _FOLLOW_CAMERA_PROPERTIES_H_
