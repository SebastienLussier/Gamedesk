/**
 *  @file       Character.cpp
 *  @brief	    Implementation for the character class.
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
#include "Character.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"

#include "Input/InputSubsystem.h"

#include "Graphic/Mesh/Mesh.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Character);


Character::Character():
    mViewDirection(1, 0, 0),
	mUpPressed(false),
    mDownPressed(false),
    mLeftPressed(false),
    mRightPressed(false),
    mWalkSpeed(10),
    mStepSpeed(12),
	mPreStepPosition(0, 0, 0),
    mRotationSpeed(3),
    mRotationAngle(0),
	mStepAngle(0),
	mStepHeight(0.05f)
{
    InputSubsystem::GetKeyboard().AddKeyListener( this, Keyboard::Key_W,    Keyboard::Key_Pressed );
    InputSubsystem::GetKeyboard().AddKeyListener( this, Keyboard::Key_W,    Keyboard::Key_Released );
    InputSubsystem::GetKeyboard().AddKeyListener( this, Keyboard::Key_S,  Keyboard::Key_Pressed );
    InputSubsystem::GetKeyboard().AddKeyListener( this, Keyboard::Key_S,  Keyboard::Key_Released );
    InputSubsystem::GetKeyboard().AddKeyListener( this, Keyboard::Key_A,  Keyboard::Key_Pressed );
    InputSubsystem::GetKeyboard().AddKeyListener( this, Keyboard::Key_A,  Keyboard::Key_Released );
    InputSubsystem::GetKeyboard().AddKeyListener( this, Keyboard::Key_D, Keyboard::Key_Pressed );
    InputSubsystem::GetKeyboard().AddKeyListener( this, Keyboard::Key_D, Keyboard::Key_Released );
}

Character::~Character()
{
    InputSubsystem::GetKeyboard().RemoveKeyListener( this, Keyboard::Key_W,    Keyboard::Key_Pressed );
    InputSubsystem::GetKeyboard().RemoveKeyListener( this, Keyboard::Key_W,    Keyboard::Key_Released );
    InputSubsystem::GetKeyboard().RemoveKeyListener( this, Keyboard::Key_S,  Keyboard::Key_Pressed );
    InputSubsystem::GetKeyboard().RemoveKeyListener( this, Keyboard::Key_S,  Keyboard::Key_Released );
    InputSubsystem::GetKeyboard().RemoveKeyListener( this, Keyboard::Key_A,  Keyboard::Key_Pressed );
    InputSubsystem::GetKeyboard().RemoveKeyListener( this, Keyboard::Key_A,  Keyboard::Key_Released );
    InputSubsystem::GetKeyboard().RemoveKeyListener( this, Keyboard::Key_D, Keyboard::Key_Pressed );
    InputSubsystem::GetKeyboard().RemoveKeyListener( this, Keyboard::Key_D, Keyboard::Key_Released );
}

void Character::Init()
{
    mMesh.GetMesh( "Data/Meshes/Iron Golem.mdl" );
//    mMesh->SetOrientation(Quaternionf(Vector3f(0,1,0), -Maths::PI / 2));
}

void Character::Update( Double pElapsedTime )
{   
    mPosition = mPreStepPosition;

    if(mUpPressed)
        mPosition += Vector3f(Maths::Cos(mRotationAngle), 0, Maths::Sin(mRotationAngle)) * mWalkSpeed * pElapsedTime;
    if(mDownPressed)
        mPosition -= Vector3f(Maths::Cos(mRotationAngle), 0, Maths::Sin(mRotationAngle)) * mWalkSpeed * pElapsedTime;
    if(mLeftPressed)
        mRotationAngle -= mRotationSpeed * pElapsedTime;
    if(mRightPressed)
        mRotationAngle += mRotationSpeed * pElapsedTime;

    // Do the stepping.
    mPreStepPosition = mPosition;
    if(mUpPressed || mDownPressed)
        mStepAngle += (mUpPressed ? 1 : -1) * mStepSpeed * pElapsedTime;
    else
        mStepAngle = 0;
    mPosition += Vector3f(0, Maths::Sin(mStepAngle) * mStepHeight, 0);

    mViewDirection.x = Maths::Cos(mRotationAngle);
    mViewDirection.z = Maths::Sin(mRotationAngle);
    mOrientation = Quaternionf(Vector3f(0,1,0), mRotationAngle);
}

void Character::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);
        
    renderer->SetColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    renderer->SetRenderState( Renderer::LocalViewerLights, true );
    mMesh->Render();
    renderer->SetRenderState( Renderer::LocalViewerLights, false );
    renderer->SetColor( Color4f(1.0f, 1.0f, 1.0f, 1.0f) );
}

Vector3f Character::GetViewDirection() const
{
    return mViewDirection;
}

void Character::OnKeyPressed( const Keyboard::Key& pKey )
{
    if( pKey == Keyboard::Key_W )
        mUpPressed = true;
    else if( pKey == Keyboard::Key_S )
        mDownPressed = true;
    else if( pKey == Keyboard::Key_A )
        mLeftPressed = true;
    else if( pKey == Keyboard::Key_D )
        mRightPressed = true;
}

void Character::OnKeyReleased( const Keyboard::Key& pKey )
{
    if( pKey == Keyboard::Key_W )
        mUpPressed = false;
    else if( pKey == Keyboard::Key_S )
        mDownPressed = false;
    else if( pKey == Keyboard::Key_A )
        mLeftPressed = false;
    else if( pKey == Keyboard::Key_D )
        mRightPressed = false;
}

void Character::OnMouseMove( Int32 /*pRelX*/, Int32 /*pRelY*/ )
{

}

void Character::SetPosition(const Vector3f& pPosition)
{
    Super::SetPosition(pPosition);
    mPreStepPosition = pPosition;
}

Vector3f Character::GetPreStepPosition() const
{
    return mPreStepPosition;
}


} // namespace Gamedesk
