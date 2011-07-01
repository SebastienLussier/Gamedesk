/**
 *  @file       CameraProperties.cpp
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
#include "EditorLib.h"
#include "CameraProperties.h"
#include "World/Camera.h"


IMPLEMENT_CLASS(CameraProperties);


ImplementProperty( CameraProperties, mFovAngle, Float, GetFovAngle, SetFovAngle );
ImplementProperty( CameraProperties, mNearView, Float, GetNearView, SetNearView );
ImplementProperty( CameraProperties, mFarView,  Float, GetFarView, SetFarView );  


CameraProperties::CameraProperties()
{
}

CameraProperties::~CameraProperties()
{
}

void CameraProperties::InitProperties()
{
    Super::InitProperties();

    RemoveProperty( &mOrientation );
    RegisterProperty( CameraProperties, "FOV Angle",  mFovAngle );
    RegisterProperty( CameraProperties, "Near Plane",  mNearView );
    RegisterProperty( CameraProperties, "Far Plane",  mFarView );    

    mFovAngle.SetMinimum( 5 );
    mFovAngle.SetMaximum( 179 );
    mFovAngle.UseRangeValidation( true );
    mFovAngle.UseSlider( true );

    mNearView.SetMinimum( 0 );
    mNearView.SetMaximum( 100 );
    
    mFarView.SetMinimum( 100 );
}

void CameraProperties::Pitch(Float pAngle)
{
	Cast<Camera>(GetObject())->Pitch(pAngle);
}

void CameraProperties::Yaw(Float pAngle)
{
    Cast<Camera>(GetObject())->Yaw(pAngle);
}

void CameraProperties::Roll(Float pAngle)
{
    Cast<Camera>(GetObject())->Roll(pAngle);
}

void CameraProperties::Move(Float pMovement)
{
    Cast<Camera>(GetObject())->Move(pMovement);
    FirePropertyChanged(&mPosition);
}

void CameraProperties::PanLeftRight(Float pMovement)
{
	Cast<Camera>(GetObject())->PanLeftRight(pMovement);
    FirePropertyChanged(&mPosition);
}

void CameraProperties::PanUpDown(Float pMovement)
{
	Cast<Camera>(GetObject())->PanUpDown(pMovement);
    FirePropertyChanged(&mPosition);
}

void CameraProperties::SetFovAngle( Float pAngle)
{
    Cast<Camera>(GetObject())->SetFovAngle(pAngle);
    FirePropertyChanged(&mFovAngle);
}

Float CameraProperties::GetFovAngle() const
{
    return Cast<Camera>(GetObject())->GetFovAngle();
}

void CameraProperties::SetFarView(Float pFarView)
{
	Cast<Camera>(GetObject())->SetFarView(pFarView);
    FirePropertyChanged(&mFarView);
}

Float CameraProperties::GetFarView() const
{
	return Cast<Camera>(GetObject())->GetFarView();
}

void CameraProperties::SetNearView(Float pNearView)
{
	Cast<Camera>(GetObject())->SetNearView(pNearView);
    FirePropertyChanged(&mNearView);
}

Float CameraProperties::GetNearView() const
{
	return Cast<Camera>(GetObject())->GetNearView();
}

void CameraProperties::LookAt( const Vector3f& pLookAtPos )
{
    Cast<Camera>(GetObject())->LookAt( pLookAtPos );
}
