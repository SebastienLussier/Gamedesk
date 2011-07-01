/**
 *  @file       ParticleEmitter.h
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       08/04/04.
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
#ifndef     _PARTICLE_EMITTER_H_
#define     _PARTICLE_EMITTER_H_


#include "Entity.h"

#include "Maths/Maths.h"
#include "Maths/Vector3.h"
#include "Graphic/Color4.h"

#include "Graphic/Texture/TextureHdl.h"


namespace Gamedesk {


class Particle
{
public:
    Vector3f    mPosition;
    Vector3f    mVelocity;

    Color4f     mColor;
    Float       mInitialSize;
    Float       mFinalSize;
    Float       mSize;
    Float       mInitialLife;
    Float       mLife;
    Bool        mDead;
};



class ENGINE_API ParticleEmitter : public Entity
{
    DECLARE_CLASS(ParticleEmitter,Entity);

public:
    //! Constructor
    ParticleEmitter();
    virtual ~ParticleEmitter();

    void EmitParticle();

    virtual void Update( Double pElapsedTime );
    virtual void Render() const;
    virtual void RenderSelected() const;

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

    virtual void    Serialize( Stream& pStream );

properties:
    /**
     * @name    Max Particle Count
     * @desc    Maximum number of particles living at once
     * @max     10000
     */
    UInt32 mMaxParticleCount;

    /**
     * @name    Emission Cone Cutoff
     * @desc    Angle at which the particles will be emitted.
     * @range   [1, 180]
     */
    UInt32 mEmissionConeAngle;

    /**
     * @name    Birthrate
     * @desc    Number of particles spawned per seconds
     * @max     10000
     */
    UInt32 mBirthrate;

    /**
     * @name    Life
     * @desc    Length of the life of a particle in seconds.
     * @range   [0.1, 10]
     */
    Float mLife;

    /**
     * @name    Life Random
     * @desc    Random value applied to the life of each particle, in seconds.
     * @range   [0.0, 10]
     */
    Float mLifeRand;

    /**
     * @name    Size Start
     * @desc    Size of the particles at their birth.
     * @range   [0.1, 10]
     */
    Float mSizeStart;

    /**
     * @name    Size Start Random
     * @desc    Random value applied to the size of each particles at their birth.
     * @range   [0.0, 10]
     */
    Float mSizeStartRand;

    /**
     * @name    Size End
     * @desc    Size of the particles at their death.
     * @range   [0.1, 10]
     */
    Float mSizeEnd;

    /**
     * @name    Size End Random
     * @desc    Random value applied to the size of each particles at their death.
     * @range   [0.0, 10]
     */
    Float mSizeEndRand;

    /**
     * @name    Color Start
     * @desc    Color of each particles at their birth.
     */
    Color4f mColorStart;

    /**
     * @name    Color End
     * @desc    Color of each particles at their death.
     */
    Color4f mColorEnd;

    /**
     * @name    Initial Speed
     * @desc    Speed of each particles at their birth.
     * @range   [0.0, 10]
     */
    Float mInitialSpeed;

    /**
     * @name    Initial Speed Random
     * @desc    Random value applied to the speed of each particles at their birth.
     * @range   [0.0, 10]
     */
    Float mInitialSpeedRand;

    /**
     * @name    Acceleration
     * @desc    Acceleration of the particle during its lifetime.
     * @range   [0.1, 10]
     */
    Float mAccel;

    /**
     * @name    Gravity
     * @desc    Force of the gravity.
     */
    Vector3f mGravity;

private:
    Vector3f GetRandomVectorFromDir( const Vector3f& dir, Float angle );

private:
    List<Particle*>    mParticles;
    List<Particle*>    mDeadParticles;
    
    HTexture2D      mTexture;

    UInt32          mParticleCount;
};


} // namespace Gamedesk


#endif  //  _PARTICLE_EMITTER_H_
