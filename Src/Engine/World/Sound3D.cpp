/**
 *  @file       Sound3D.cpp
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       21/02/04.
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
#include "Sound3D.h"
#include "Maths/Maths.h"
#include "Maths/Vector3.h"
#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
static const Float sgk_soundSize = 0.1f;


Sound3D::Sound3D() : 
    mLastSoundPosition(0, 0, 0)
{
    Float halfSize = sgk_soundSize / 2.0f;
    mBoundingBox.SetMin( Vector3f(-halfSize, 0, -halfSize) );
    mBoundingBox.SetMax( Vector3f(halfSize, sgk_soundSize, halfSize) );
}

void Sound3D::SetSound( const String& pSoundFileName )
{
    mSound.SetSound(pSoundFileName);
    
    mSound->SetMode(Sound::kSoundModeLoop);
    mSound->Play();
}

Sound3D::~Sound3D()
{
}

const Sound3D& Sound3D::operator=(const Sound3D& pOther)
{
    mSound = pOther.mSound;
    
    return *this;
}

void Sound3D::Update(Double pElapsedTime)
{
    // Update the sound's position and velocity if it moved.
	if(mLastSoundPosition != mPosition)
	{
		Vector3f velocity = (mPosition - mLastSoundPosition) / pElapsedTime;
        mSound->SetAttributes(mPosition, velocity);
		mLastSoundPosition = mPosition;
	}
    // If it didn't move we must set the velocity back to 0.
    else
    {
        Vector3f velocity;
        Vector3f position;
        mSound->GetAttributes(position, velocity);
        if(velocity.GetLength() > 0 || mSound->IsPlaying())
        {
            mSound->SetAttributes(mSound->GetPosition(), Vector3f(0, 0, 0));
        }
    }

    mSound->SetFrequency(mSound->GetFrequency());
}

void Sound3D::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetRenderState( Renderer::Lighting, false );
    renderer->SetPolygonMode(Renderer::BothFace, Renderer::FillWireframe);
    renderer->SetCulling( Renderer::NoCulling );
    renderer->PushMatrix();

    renderer->SetColor(Color4f(0.0f, 1.0f, 0.0f, 1.0f));

    Float halfSize = sgk_soundSize / 2.0f;
    
    renderer->BeginScene(Renderer::TriangleList);
    renderer->SetVertex(Vector3f(-halfSize, 0.0f, 0.0f));
    renderer->SetVertex(Vector3f(0.0f, sgk_soundSize, 0.0f));
    renderer->SetVertex(Vector3f(halfSize, 0.0f, 0.0f));
    renderer->SetVertex(Vector3f(0.0f, 0.0f, -halfSize));
    renderer->SetVertex(Vector3f(0.0f, sgk_soundSize, 0.0f));
    renderer->SetVertex(Vector3f(0.0f, 0.0f, halfSize));
    renderer->EndScene();
   
    renderer->PopMatrix();
    renderer->SetRenderState( Renderer::Lighting, true );
    renderer->SetPolygonMode(Renderer::BothFace, Renderer::FillSolid);
    renderer->SetCulling( Renderer::CullBackFace );

    renderer->SetColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
}

SoundHdl& Sound3D::GetSoundHandle()
{
    return mSound;
}


IMPLEMENT_CLASS(Sound3D)


} // namespace Gamedesk
