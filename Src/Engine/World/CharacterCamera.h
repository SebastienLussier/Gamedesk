/**
 *  @file       CharacterCamera.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       27/11/05.
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
#ifndef     _CHARACTER_CAMERA_H_
#define     _CHARACTER_CAMERA_H_


#include "Camera.h"


namespace Gamedesk {


class Character;

/**
 *  CharacterCamera class used to represent a camera in a 3d world that follows the character.
 *  @brief  CharacterCamera class for a camera in a 3d world that follows a character.
 *  @author Marco Arsenault.
 *  @date   27/11/05.
 */
class ENGINE_API CharacterCamera : public Camera
{
    DECLARE_CLASS(CharacterCamera, Camera)

public:
    //! Default constructor.
	CharacterCamera();

    //! Destructor.
    virtual ~CharacterCamera();

    //! Update the camera.
    virtual void Update(Double pElapsedTime);

    //! Set the character to follow.
    void SetCharacter(Character* pCharacter);

    //| Get the position of the camera anchor.
    Vector3f GetAnchorPosition() const;

    //| Get the position of the camera focus.
    Vector3f GetFocusPosition() const;

    //! Set the spring force.
    void SetSpringForce(Float pSpringForce);

    //! Get the spring force.
    Float GetSpringForce() const;

    //! Set the follow distance.
    void SetFollowDistance(Float pFollowDistance);

    //! Get the follow distance
    Float GetFollowDistance() const;

private:
	Character*  mCharacter;
    Vector3f    mCurrentAnchorPos;
    Vector3f    mPreviousAnchorPos;
    Float       mSpringForce;
    Float       mFollowDistance;
};


} // namespace Gamedesk


#endif  //  _CHARACTER_CAMERA_H_
