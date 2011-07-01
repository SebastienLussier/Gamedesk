/**
 *  @file       Character.h
 *  @brief	    
 *  @author     Marco Arsenault.
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
#ifndef     _CHARACTER_H_
#define     _CHARACTER_H_


#include "World/Entity.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Sound/SoundHdl.h"
#include "Graphic/Mesh/MeshHdl.h"
#include "World/Sound3D.h"


namespace Gamedesk {


class ENGINE_API Character : public Entity,
                             public Keyboard::Listener,
                             public Mouse::Listener
{
    DECLARE_CLASS(Character, Entity);

public:
    Character();
    virtual ~Character();

    virtual void Init();
    
    virtual void Update( Double pElapsedTime );
    virtual void Render() const;

    Vector3f GetViewDirection() const;

    virtual void OnKeyPressed( const Keyboard::Key& pKey );
    virtual void OnKeyReleased( const Keyboard::Key& pKey );
    virtual void OnMouseMove( Int32 pRelX, Int32 pRelY );

    virtual void SetPosition(const Vector3f& pPosition);
    Vector3f     GetPreStepPosition() const;

private:
    Vector3f    mViewDirection;

    //Sound3D*    mStepSounds;
    MeshHdl     mMesh;

    Bool        mUpPressed;
    Bool        mDownPressed;
    Bool        mLeftPressed;
    Bool        mRightPressed;

    Float       mWalkSpeed;
    Float       mStepSpeed;
    Vector3f    mPreStepPosition;
    Float       mRotationSpeed;
    Float       mRotationAngle;

    // Stepping.
    Float       mStepAngle;
    Float       mStepHeight;
};


} // namespace Gamedesk


#endif	//	_CHARACTER_H_
