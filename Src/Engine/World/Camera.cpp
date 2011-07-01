/**
 *  @file       Camera.cpp
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       20/12/03.
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
#include "Camera.h"
#include "Maths/Maths.h"
#include "Maths/Quaternion.h"
#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Camera)


static const Float sgk_camSize = 0.5f;


Camera::Camera() : 
    mFovAngle(75.0f),
    mNearView(0.1f),
	mFarView(30000.0f),
    mView(0, 0, -1),
    mUp(0, 1, 0),
    mRight(1, 0, 0)
{
    Float halfSize = sgk_camSize / 2.0f;
    mBoundingBox.SetMin( Vector3f(-halfSize, -halfSize, -halfSize) );
    mBoundingBox.SetMax( Vector3f(halfSize, halfSize, halfSize) );
}

Camera::~Camera()
{
}

void Camera::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetRenderState( Renderer::Lighting, false );
    renderer->SetPolygonMode(Renderer::BothFace, Renderer::FillWireframe);
    renderer->SetCulling( Renderer::NoCulling );
    
    renderer->SetColor(Color4f(1.0f, 0.5f, 0.0f, 1.0f));

	RenderCameraModel();

	renderer->SetColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    
    renderer->SetRenderState( Renderer::Lighting, true );
    renderer->SetPolygonMode(Renderer::BothFace, Renderer::FillSolid);
    renderer->SetCulling( Renderer::CullBackFace );
}

void Camera::RenderSelected() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetRenderState( Renderer::Lighting, false );
    renderer->SetPolygonMode(Renderer::BothFace, Renderer::FillWireframe);
    renderer->SetCulling( Renderer::NoCulling );
    
    renderer->SetColor(Color4f(1.0f, 0.0f, 0.0f, 1.0f));

	renderer->SetPolygonOffset(1.0f, 2.0f);
	renderer->SetLineWidth(2.0f);
			
	RenderCameraModel();

	renderer->SetLineWidth(1.0f);
	renderer->SetPolygonOffset(0, 0);

	renderer->SetColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    
    renderer->SetRenderState( Renderer::Lighting, true );
    renderer->SetPolygonMode(Renderer::BothFace, Renderer::FillSolid);
    renderer->SetCulling( Renderer::CullBackFace );
}

void Camera::ApplyViewMatrix()
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetView( GetPosition(), GetView(), GetUp() );
}

void Camera::Pitch(Float pAngle)
{
	Quaternionf rotationQuat( mRight, Maths::ToRadians(pAngle) );

    mView = rotationQuat * mView;
    mView.Normalize();
   
    mRight = mView cross mUp;
    mRight.Normalize();
}

void Camera::Yaw(Float pAngle)
{
    Quaternionf rotationQuat( mUp, Maths::ToRadians(pAngle) );
  
    mView  = rotationQuat * mView;
    mRight = rotationQuat * mRight;
        
    mView.Normalize();
    mRight.Normalize();
}

void Camera::Roll(Float /*pAngle*/)
{
    // Force mUp = 0,1,0 for now...
}

void Camera::Move(Float pMovement)
{
    mPosition += mView * pMovement;
}

void Camera::PanLeftRight(Float pMovement)
{
	mPosition += mRight * pMovement;
}

void Camera::PanUpDown(Float pMovement)
{
	mPosition += mUp * pMovement;
}

void Camera::SetFovAngle(Float pAngle)
{
    mFovAngle = pAngle;
}

Float Camera::GetFovAngle() const 
{
    return mFovAngle;
}

void Camera::SetFarView(Float pFarView)
{
	mFarView = pFarView;
}

Float Camera::GetFarView() const
{	
	return mFarView; 
}

void Camera::SetNearView(Float pNearView)
{
	mNearView = pNearView;
}

Float Camera::GetNearView() const
{	
	return mNearView;
}

const Vector3f& Camera::GetView() const
{
    return mView;
}

const Vector3f& Camera::GetUp() const
{
    return mUp;
}

const Vector3f& Camera::GetRight() const
{
    return mRight;
}

void Camera::RenderCameraModel() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);
    
	Vector3f origin(0, 0, 0);
	Vector3f front = GetView() * sgk_camSize;
	Vector3f halfFront = GetView() * sgk_camSize / 2.0f;
    Vector3f halfRight = GetRight() * sgk_camSize / 2.0f;
	Vector3f halfUp = GetUp() * sgk_camSize / 2.0f;
	Vector3f quarterFront = halfFront / 2.0f;
	Vector3f quarterRight = halfRight / 2.0f;
	Vector3f quarterUp = halfUp / 2.0f;
	
	
	Float halfFov = Maths::ToRadians(mFovAngle / 2.0f);
	Float sinFov = Maths::Sin(halfFov);
	Float cosFov = Maths::Cos(halfFov);
    
	// Render the camera box.
    renderer->BeginScene(Renderer::QuadList);
    renderer->SetVertex(-halfRight - halfUp);
    renderer->SetVertex(halfRight - halfUp);
    renderer->SetVertex(halfRight + halfUp);
    renderer->SetVertex(-halfRight + halfUp);

	renderer->SetVertex(-halfRight - halfUp - front);
    renderer->SetVertex(halfRight - halfUp - front);
    renderer->SetVertex(halfRight + halfUp - front);
    renderer->SetVertex(-halfRight + halfUp - front);

	renderer->SetVertex(-halfRight + halfUp);
    renderer->SetVertex(halfRight + halfUp);
    renderer->SetVertex(halfRight + halfUp - front);
    renderer->SetVertex(-halfRight + halfUp - front);

	renderer->SetVertex(halfRight - halfUp);
    renderer->SetVertex(-halfRight - halfUp);
    renderer->SetVertex(-halfRight - halfUp - front);
    renderer->SetVertex(halfRight - halfUp - front);

	renderer->SetVertex(halfRight - halfUp - front);
    renderer->SetVertex(halfRight - halfUp);
    renderer->SetVertex(halfRight + halfUp);
    renderer->SetVertex(halfRight + halfUp - front);

	renderer->SetVertex(-halfRight - halfUp);
    renderer->SetVertex(-halfRight - halfUp - front);
    renderer->SetVertex(-halfRight + halfUp - front);
    renderer->SetVertex(-halfRight + halfUp);
    renderer->EndScene();

	// Render the up indicator.
	renderer->BeginScene(Renderer::TriangleList);
    renderer->SetVertex(-halfRight + halfUp);
    renderer->SetVertex(halfUp + quarterUp);
    renderer->SetVertex(halfRight + halfUp);
    renderer->EndScene();

	// Render the FOV.
    renderer->BeginScene(Renderer::TriangleList);
    renderer->SetVertex(origin);
    renderer->SetVertex(halfFront * cosFov + halfRight * sinFov + halfUp * sinFov);
    renderer->SetVertex(halfFront * cosFov + halfRight * -sinFov + halfUp * sinFov);

	renderer->SetVertex(origin);
    renderer->SetVertex(halfFront * cosFov + halfRight * -sinFov - halfUp * sinFov);
    renderer->SetVertex(halfFront * cosFov + halfRight * sinFov - halfUp * sinFov);

	renderer->SetVertex(origin);
    renderer->SetVertex(halfFront * cosFov + halfRight * sinFov + halfUp * -sinFov);
    renderer->SetVertex(halfFront * cosFov + halfRight * sinFov + halfUp * sinFov);

	renderer->SetVertex(origin);
    renderer->SetVertex(halfFront * cosFov - halfRight * sinFov + halfUp * sinFov);
    renderer->SetVertex(halfFront * cosFov - halfRight * sinFov + halfUp * -sinFov);
    renderer->EndScene();
}

void Camera::LookAt( const Vector3f& pLookAtPos )
{
     mView = pLookAtPos - mPosition;
     mView.Normalize();

     mRight = mUp cross mView;
     mRight.Normalize();
}

void Camera::Serialize( Stream& pStream )
{
    Super::Serialize( pStream );

    pStream << mFovAngle;
    pStream << mFarView;
    pStream << mNearView;

    pStream << mView;
    pStream << mUp;
    pStream << mRight;
}

Vector3f Camera::SpringDamp(const Vector3f& currPos,    // Current Position
	                        const Vector3f& trgPos,     // Target Position
	                        const Vector3f& prevTrgPos, // Previous Target Position
	                        Double deltaTime,    // Change in Time
	                        Float springConst,  // Hooke's Constant
	                        Float dampConst,    // Damp Constant
	                        Float springLen) const
{
	Vector3f disp;           // Displacement
	Vector3f velocity;       // Velocity   
	Float forceMag;         // Force Magnitude

	// Calculate Spring Force
	disp = currPos - trgPos;
    if(disp == Vector3f::ZERO)
        return currPos;

	velocity = (prevTrgPos - trgPos) * deltaTime;
	forceMag = springConst * (springLen - disp.GetLength()) + 
		dampConst * ((disp | velocity) / disp.GetLength());

	// Apply Spring Force
	disp.Normalize();
	disp *= forceMag * deltaTime;
	return currPos + disp;
}


} // namespace Gamedesk
