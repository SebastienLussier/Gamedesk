/**
 *  @file       ParticleEmitterProperties.h
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       31/03/04.
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
#ifndef     _PARTICLE_EMITTER_PROPERTIES_H_
#define     _PARTICLE_EMITTER_PROPERTIES_H_


#include "EntityProperties.h"


class ParticleEmitter;


/**
 *  ParticleEmitterProperties class.  This object is used as a facade to a ParticleEmitter.
 *  As a PropertiesSender, it can be connected with
 *  Listeners to keep their data up to date.
 *  @brief  ParticleEmitterProperties class.
 *  @author Sébastien Lussier.
 *  @date   08/04/04.
 */
class EDITORLIB_API ParticleEmitterProperties : public EntityProperties
{
    DECLARE_CLASS(ParticleEmitterProperties, EntityProperties);

public:
    //! Constructor.
	ParticleEmitterProperties();

    //! Destructor.
    virtual ~ParticleEmitterProperties();

    //! Initialize all properties.
    virtual void InitProperties();

    void            SetBirthrate( UInt32 pBirthrate );
    UInt32          GetBirthrate() const;
    
    void            SetLife( Float pLife );
    Float           GetLife() const;
    void            SetLifeRand( Float pLifeRand );
    Float           GetLifeRand() const;

    void            SetSizeStart( Float pSizeStart );
    void            SetSizeStartRand( Float pSizeStartRand );
    Float           GetSizeStart() const;
    Float           GetSizeStartRand() const;

    void            SetSizeEnd( Float pSizeEnd );
    void            SetSizeEndRand( Float pSizeEndRand );
    Float           GetSizeEnd() const;
    Float           GetSizeEndRand() const;

    void            SetColorStart( const Color4f& pColorStart );
    const Color4f&  GetColorStart() const;

    void            SetColorEnd( const Color4f& pColorEnd );
    const Color4f&  GetColorEnd() const;

    void            SetInitialSpeed( Float pSpeed );
    Float           GetInitialSpeed() const;
    void            SetInitialSpeedRand( Float pSpeedRand );
    Float           GetInitialSpeedRand() const;

    void            SetAccel( Float pAccel );
    Float           GetAccel() const;

    void            SetGravity( const Vector3f& pGravity );
    const Vector3f& GetGravity() const;

    void            SetEmissionConeAngle( UInt32 pConeAngle );
    UInt32          GetEmissionConeAngle() const;

    void            SetMaxParticleCount( UInt32 pMaxParticleCount );
    UInt32          GetMaxParticleCount() const;
    
private:
    PropertyUInt32  mMaxParticleCount;

    PropertyUInt32  mBirthrate;

    PropertyFloat   mLife;
    PropertyFloat   mLifeRand;

    PropertyFloat   mSizeStart;
    PropertyFloat   mSizeStartRand;
    
    PropertyFloat   mSizeEnd;
    PropertyFloat   mSizeEndRand;

    PropertyColor4f mColorStart;
    PropertyColor4f mColorEnd;
            
    PropertyFloat   mInitialSpeed;
    PropertyFloat   mInitialSpeedRand;

    PropertyFloat       mAccel;
    PropertyVector3f    mGravity;

    PropertyUInt32  mEmissionConeAngle;
};


#endif  //  _PARTICLE_EMITTER_PROPERTIES_H_