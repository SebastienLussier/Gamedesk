/**
 *  @file       ODECarProperties.cpp
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       12/04/04.
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
#include "ODECarProperties.h"
#include "World/ODECar.h"


ImplementProperty( ODECarProperties, mChassisMass, Float, GetChassisMass, SetChassisMass );
ImplementProperty( ODECarProperties, mWheelMass, Float, GetWheelsMass, SetWheelsMass );
ImplementProperty( ODECarProperties, mWheelRad, Float, GetWheelsRadius, SetWheelsRadius );
ImplementProperty( ODECarProperties, mWheelThick, Float, GetWheelsThickness, SetWheelsThickness );
ImplementProperty( ODECarProperties, mFMax, Float, GetFMax, SetFMax );
ImplementProperty( ODECarProperties, mFMax2, Float, GetFMax2, SetFMax2 );
ImplementProperty( ODECarProperties, mSteeringLimit, Float, GetSteeringLimit, SetSteeringLimit );
ImplementProperty( ODECarProperties, mCfm, Float, GetCFM, SetCFM );
ImplementProperty( ODECarProperties, mErp, Float, GetERP, SetERP );
ImplementProperty( ODECarProperties, mChassisSize, Vector3f, GetChassisSize, SetChassisSize );
ImplementProperty( ODECarProperties, mFrontWheelPos, Vector3f, GetFrontWheelPos, SetFrontWheelPos );
ImplementProperty( ODECarProperties, mBackWheelPos, Vector3f, GetBackWheelPos, SetBackWheelPos );


ODECarProperties::ODECarProperties()
{
}

ODECarProperties::~ODECarProperties()
{
}

void ODECarProperties::InitProperties()
{
    Super::InitProperties();

    RegisterProperty( ODECarProperties, "Chassis Mass", mChassisMass );
    RegisterProperty( ODECarProperties, "Wheel Mass", mWheelMass );
    RegisterProperty( ODECarProperties, "Wheel Radius", mWheelRad );
    RegisterProperty( ODECarProperties, "Wheel Thickness", mWheelThick );
    RegisterProperty( ODECarProperties, "FMax", mFMax );
    RegisterProperty( ODECarProperties, "FMax2", mFMax2 );
    RegisterProperty( ODECarProperties, "Steering Max Angle", mSteeringLimit );
    RegisterProperty( ODECarProperties, "Constraint Force Mixing", mCfm );
    RegisterProperty( ODECarProperties, "Error Reduction Param", mErp );
    RegisterProperty( ODECarProperties, "Chassis Size", mChassisSize );
    RegisterProperty( ODECarProperties, "Front Wheel Pos", mFrontWheelPos );
    RegisterProperty( ODECarProperties, "Back Wheel Pos", mBackWheelPos );   
    
    mChassisMass.SetMinimum( 1 );
    mChassisMass.SetMaximum( 200 );
    mChassisMass.UseRangeValidation( true );
    mChassisMass.UseSlider( true );

    mWheelMass.SetMinimum( 1 );
    mWheelMass.SetMaximum( 200 );
    mWheelMass.UseRangeValidation( true );
    mWheelMass.UseSlider( true );

    mWheelRad.SetMinimum( 0.1f );
    mWheelRad.SetMaximum( 10.0f );
    mWheelRad.UseRangeValidation( true );
    mWheelRad.UseSlider( true, 10 );

    mWheelThick.SetMinimum( 0.1f );
    mWheelThick.SetMaximum( 10.0f );
    mWheelThick.UseRangeValidation( true );
    mWheelThick.UseSlider( true, 10 );

    mFMax.SetMinimum( 0.1f );
    mFMax.SetMaximum( 200.0f );
    mFMax.UseRangeValidation( true );
    mFMax.UseSlider( true );

    mFMax2.SetMinimum( 0.1f );
    mFMax2.SetMaximum( 200.0f );
    mFMax2.UseRangeValidation( true );
    mFMax2.UseSlider( true );

    mSteeringLimit.SetMinimum( 0.1f );
    mSteeringLimit.SetMaximum( Maths::PI );
    mSteeringLimit.UseRangeValidation( true );
    mSteeringLimit.UseSlider( true, 100 );

    mCfm.SetMinimum( 0.0f );
    mCfm.SetMaximum( 1.0f );
    mCfm.UseRangeValidation( true );
    mCfm.UseSlider( true, 100 );

    mErp.SetMinimum( 0.0f );
    mErp.SetMaximum( 1.0f );
    mErp.UseRangeValidation( true );
    mErp.UseSlider( true, 100 );    
}

Float ODECarProperties::GetChassisMass() const
{
    return Cast<ODECar>(GetObject())->GetChassisMass();
}
void ODECarProperties::SetChassisMass( Float pChassisMass )
{
    Cast<ODECar>(GetObject())->SetChassisMass(pChassisMass);
    FirePropertyChanged(&mChassisMass);
}

Float ODECarProperties::GetWheelsMass() const
{
    return Cast<ODECar>(GetObject())->GetWheelsMass();
}
void ODECarProperties::SetWheelsMass( Float pWheelsMass )
{
    Cast<ODECar>(GetObject())->SetWheelsMass(pWheelsMass);
    FirePropertyChanged(&mWheelMass);
}

Float ODECarProperties::GetWheelsRadius() const
{
    return Cast<ODECar>(GetObject())->GetWheelsRadius();
}
void ODECarProperties::SetWheelsRadius( Float pWheelsRadius )
{
    Cast<ODECar>(GetObject())->SetWheelsRadius(pWheelsRadius);
    FirePropertyChanged(&mWheelRad);
}

Float ODECarProperties::GetWheelsThickness() const
{
    return Cast<ODECar>(GetObject())->GetWheelsThickness();
}
void ODECarProperties::SetWheelsThickness( Float pWheelsThickness )
{
    Cast<ODECar>(GetObject())->SetWheelsThickness(pWheelsThickness);
    FirePropertyChanged(&mWheelThick);
}

const Vector3f& ODECarProperties::GetChassisSize() const
{
    return Cast<ODECar>(GetObject())->GetChassisSize();
}
void ODECarProperties::SetChassisSize( const Vector3f& pChassisSize )
{
    Cast<ODECar>(GetObject())->SetChassisSize(pChassisSize);
    FirePropertyChanged(&mChassisSize);
}

const Vector3f& ODECarProperties::GetFrontWheelPos() const
{
    return Cast<ODECar>(GetObject())->GetFrontWheelPos();
}
void ODECarProperties::SetFrontWheelPos( const Vector3f& pFrontWheelPos )
{
    Cast<ODECar>(GetObject())->SetFrontWheelPos(pFrontWheelPos);
    FirePropertyChanged(&mFrontWheelPos);
}

const Vector3f& ODECarProperties::GetBackWheelPos() const
{
    return Cast<ODECar>(GetObject())->GetBackWheelPos();
}
void ODECarProperties::SetBackWheelPos( const Vector3f& pBackWheelPos )
{
    Cast<ODECar>(GetObject())->SetBackWheelPos(pBackWheelPos);
    FirePropertyChanged(&mBackWheelPos);
}

Float ODECarProperties::GetSteeringLimit() const
{
    return Cast<ODECar>(GetObject())->GetSteeringLimit();
}
void ODECarProperties::SetSteeringLimit( Float pSteeringLimit )
{
    Cast<ODECar>(GetObject())->SetSteeringLimit(pSteeringLimit);
    FirePropertyChanged(&mSteeringLimit);
}

Float ODECarProperties::GetCFM() const
{
    return Cast<ODECar>(GetObject())->GetCFM();
}
void ODECarProperties::SetCFM( Float pCFM )
{
    Cast<ODECar>(GetObject())->SetCFM(pCFM);
    FirePropertyChanged(&mCfm);
}

Float ODECarProperties::GetERP() const
{
    return Cast<ODECar>(GetObject())->GetERP();
}
void ODECarProperties::SetERP( Float pERP )
{
    Cast<ODECar>(GetObject())->SetERP(pERP);
    FirePropertyChanged(&mErp);
}

Float ODECarProperties::GetFMax() const
{
    return Cast<ODECar>(GetObject())->GetFMax();
}
void ODECarProperties::SetFMax( Float pFMax )
{
    Cast<ODECar>(GetObject())->SetFMax(pFMax);
    FirePropertyChanged(&mFMax);
}

Float ODECarProperties::GetFMax2() const
{
    return Cast<ODECar>(GetObject())->GetFMax2();    
}
void ODECarProperties::SetFMax2( Float pFMax2 )
{
    Cast<ODECar>(GetObject())->SetFMax2(pFMax2);
    FirePropertyChanged(&mFMax2);
}

IMPLEMENT_CLASS(ODECarProperties);
