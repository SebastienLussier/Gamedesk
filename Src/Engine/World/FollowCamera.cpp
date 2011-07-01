/**
 *  @file       FollowCamera.cpp
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
#include "Engine.h"
#include "FollowCamera.h"
#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(FollowCamera)


FollowCamera::FollowCamera() : 
    mSpringForce(2.5f),
	mFollowDistance(5.0f),
	mUpOffset(0.5f),
	mFollowedEntity(0)
{
}

FollowCamera::~FollowCamera()
{
    mFollowedEntity = 0;
}

void FollowCamera::Update(Double pElapsedTime)
{
    // Follow the object if any.
    if(mFollowedEntity)
    {
	    mPreviousObjectPosition = mCurrentObjectPosition;

        Vector3f viewToObject = (mFollowedEntity->GetPosition() - mPosition).GetNormalized();
        Vector3f followPos = mFollowedEntity->GetPosition() - viewToObject * mFollowDistance;
        
        followPos += Vector3f(0, mUpOffset, 0);
        mCurrentObjectPosition = followPos;
	    
        mPosition = SpringDamp(mPosition, mCurrentObjectPosition, mPreviousObjectPosition, 
			                    pElapsedTime, mSpringForce, 0.25f, 1.0f);

        // Modify the view, up, right to look at the object.
        LookAt( mFollowedEntity->GetPosition() );
    }
}

void FollowCamera::SetFollowedEntity(Entity* pEntity)
{
    mFollowedEntity = pEntity;

    Vector3f viewToObject = (mFollowedEntity->GetPosition() - mPosition).GetNormalized();
    Vector3f followPos = mFollowedEntity->GetPosition() - viewToObject * mFollowDistance;

    followPos += Vector3f(0, mUpOffset, 0);

    mCurrentObjectPosition = followPos;
    mPreviousObjectPosition = followPos;
}

void FollowCamera::SetSpringForce(Float pSpringForce)
{
    mSpringForce = pSpringForce;
}

Float FollowCamera::GetSpringForce() const
{
    return mSpringForce;
}

void FollowCamera::SetFollowDistance(Float pFollowDistance)
{
    mFollowDistance = pFollowDistance;
}

Float FollowCamera::GetFollowDistance() const
{
    return mFollowDistance;
}

void FollowCamera::SetUpOffset(Float pUpOffset)
{
    mUpOffset = pUpOffset;
}

Float FollowCamera::GetUpOffset() const
{
    return mUpOffset;
}


} // namespace Gamedesk
