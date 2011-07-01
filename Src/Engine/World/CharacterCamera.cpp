/**
 *  @file       CharacterCamera.cpp
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
#include "Engine.h"
#include "CharacterCamera.h"
#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"
#include "Character.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(CharacterCamera)


CharacterCamera::CharacterCamera() : 
    mCharacter(0),
    mSpringForce(10),
    mFollowDistance(1.5f)
{
}

CharacterCamera::~CharacterCamera()
{
    mCharacter = 0;
}

void CharacterCamera::Update(Double pElapsedTime)
{
    // Follow the object if any.
    if(mCharacter)
    {
	    mPreviousAnchorPos = mCurrentAnchorPos;
        mCurrentAnchorPos = GetAnchorPosition();
        mPosition = SpringDamp(mPosition, mCurrentAnchorPos, mPreviousAnchorPos, 
			                   pElapsedTime, mSpringForce, 0.25f, 1.0f);

        LookAt( GetFocusPosition() );
    }
}

void CharacterCamera::SetCharacter(Character* pCharacter)
{
    mCharacter = pCharacter;

    Vector3f anchorPos = GetAnchorPosition();
    mCurrentAnchorPos = anchorPos;
    mPreviousAnchorPos = anchorPos;
    mPosition = anchorPos;
    
    LookAt( GetFocusPosition() );
}

Vector3f CharacterCamera::GetAnchorPosition() const
{
    return GetFocusPosition() - mCharacter->GetViewDirection() * mFollowDistance;
}

Vector3f CharacterCamera::GetFocusPosition() const
{
    return mCharacter->GetPreStepPosition() + Vector3f(0, 3.5f, 0);
}

void CharacterCamera::SetSpringForce(Float pSpringForce)
{
    mSpringForce = pSpringForce;
}

Float CharacterCamera::GetSpringForce() const
{
    return mSpringForce;
}

void CharacterCamera::SetFollowDistance(Float pFollowDistance)
{
    mFollowDistance = pFollowDistance;
}

Float CharacterCamera::GetFollowDistance() const
{
    return mFollowDistance;
}


} // namespace Gamedesk
