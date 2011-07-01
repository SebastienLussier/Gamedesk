/**
 *  @file       LookCameraProperties.cpp
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
#include "EditorLib.h"
#include "FollowCameraProperties.h"
#include "World/FollowCamera.h"


IMPLEMENT_CLASS(FollowCameraProperties);


ImplementProperty(FollowCameraProperties, mSpringForce, Float, GetSpringForce, SetSpringForce);
ImplementProperty(FollowCameraProperties, mFollowDistance, Float, GetFollowDistance, SetFollowDistance);
ImplementProperty(FollowCameraProperties, mUpOffset, Float, GetUpOffset, SetUpOffset);


FollowCameraProperties::FollowCameraProperties()
{
}

FollowCameraProperties::~FollowCameraProperties()
{
}

void FollowCameraProperties::InitProperties()
{
    Super::InitProperties();

    RegisterProperty(FollowCameraProperties, "Spring Force",  mSpringForce);
    mSpringForce.SetMinimum(0.1f);
    mSpringForce.SetMaximum(5.0f);
    mSpringForce.UseRangeValidation(true);
    mSpringForce.UseSlider(true, 10);

    RegisterProperty(FollowCameraProperties, "Follow Distance",  mFollowDistance);
    mFollowDistance.](1);
    mFollowDistance.SetMaximum(100);
    mFollowDistance.UseRangeValidation(true);
    mFollowDistance.UseSlider(true);

    RegisterProperty(FollowCameraProperties, "Up Offset",  mUpOffset);
    mUpOffset.SetMinimum(0);
    mUpOffset.SetMaximum(10);
    mUpOffset.UseRangeValidation(true);
    mUpOffset.UseSlider(true, 10);
}

void FollowCameraProperties::SetSpringForce(Float pSpringForce)
{
    Cast<FollowCamera>(GetObject())->SetSpringForce(pSpringForce);
    FirePropertyChanged(&mSpringForce);
}

Float FollowCameraProperties::GetSpringForce() const
{
    return Cast<FollowCamera>(GetObject())->GetSpringForce();
}

void FollowCameraProperties::SetFollowDistance(Float pFollowDistance)
{
    Cast<FollowCamera>(GetObject())->SetFollowDistance(pFollowDistance);
    FirePropertyChanged(&mFollowDistance);
}

Float FollowCameraProperties::GetFollowDistance() const
{
    return Cast<FollowCamera>(GetObject())->GetFollowDistance();
}

void FollowCameraProperties::SetUpOffset(Float pUpOffset)
{
    Cast<FollowCamera>(GetObject())->SetUpOffset(pUpOffset);
    FirePropertyChanged(&mUpOffset);
}

Float FollowCameraProperties::GetUpOffset() const
{
    return Cast<FollowCamera>(GetObject())->GetUpOffset();
}
