/**
 *  @file       ODECarProperties.h
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
#ifndef     _ODE_CAR_PROPERTIES_H_
#define     _ODE_CAR_PROPERTIES_H_


#include "EntityProperties.h"


class ODECar;


class EDITORLIB_API ODECarProperties : public EntityProperties
{
    DECLARE_CLASS(ODECarProperties, EntityProperties);

public:
    //! Constructor.
	ODECarProperties();

    //! Destructor.
    virtual ~ODECarProperties();

    //! Initialize all properties.
    virtual void InitProperties();


    Float   GetChassisMass() const;
    void    SetChassisMass( Float pChassisMass );

    Float   GetWheelsMass() const;
    void    SetWheelsMass( Float pWheelsMass );

    Float   GetWheelsRadius() const;
    void    SetWheelsRadius( Float pWheelsRadius );

    Float   GetWheelsThickness() const;
    void    SetWheelsThickness( Float pWheelsThickness );

    const Vector3f& GetChassisSize() const;
    void    SetChassisSize( const Vector3f& pChassisSize );

    const Vector3f& GetFrontWheelPos() const;
    void    SetFrontWheelPos( const Vector3f& pFrontWheelPos );

    const Vector3f& GetBackWheelPos() const;
    void    SetBackWheelPos( const Vector3f& pBackWheelPos );

    Float   GetSteeringLimit() const;
    void    SetSteeringLimit( Float pSteeringLimit );

    Float   GetCFM() const;
    void    SetCFM( Float pCFM );

    Float   GetERP() const;
    void    SetERP( Float pERP );

    Float   GetFMax() const;
    void    SetFMax( Float pFMax );

    Float   GetFMax2() const;
    void    SetFMax2( Float pFMax2 );

private:
    PropertyFloat	    mChassisMass;
    PropertyFloat       mWheelMass;
	PropertyFloat	    mWheelRad;
    PropertyFloat       mWheelThick;

    PropertyFloat       mFMax;
    PropertyFloat       mFMax2;
    PropertyFloat       mSteeringLimit;
	PropertyFloat       mCfm; 
    PropertyFloat       mErp;

    PropertyVector3f    mChassisSize;
    PropertyVector3f    mFrontWheelPos;
    PropertyVector3f    mBackWheelPos;
};


#endif  //  _ODE_CAR_PROPERTIES_H_
