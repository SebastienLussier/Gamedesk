 /**
 *  @file       TrackballManipulator.cpp
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       03/02/04.
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
#include "MainViewerDll.h"
#include "TrackballManipulator.h"
#include "RotateCommand.h"
#include "World/Camera.h"
#include "World/Terrain.h"
#include "World/SkyDome.h"
#include "Input/InputSubsystem.h"
#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "EditorBase.h"
#include "Viewer.h"
#include "World/Camera.h"
#include "WorldManager.h"


const Float sgk_rotationSpeed(0.3f);


TrackballManipulator::TrackballManipulator(EditorBase* pEditor) : 
    mActivationInputState(Keyboard::Key_R, Keyboard::Key_Pressed,
                          Mouse::Button_None, Mouse::Button_Pressed),
    mDeactivationInputState(Keyboard::Key_R, Keyboard::Key_Pressed,
                            Mouse::Button_None, Mouse::Button_Pressed),
    mEditor(pEditor)
{
    GD_ASSERT(pEditor);
}

TrackballManipulator::~TrackballManipulator()
{
}

void TrackballManipulator::Activate()
{
    Manipulator::Activate();
        
    Mouse& mouse = InputSubsystem::GetMouse();
    mouse.AddMoveListener(this);
    mouse.AddButtonListener(this, Mouse::Button_Left, Mouse::Button_Pressed);
    mouse.AddButtonListener(this, Mouse::Button_Left, Mouse::Button_Released);
}

void TrackballManipulator::Deactivate()
{
    Manipulator::Deactivate();
        
    Mouse& mouse = InputSubsystem::GetMouse();
    mouse.RemoveMoveListener(this);
    mouse.RemoveButtonListener(this, Mouse::Button_Left, Mouse::Button_Pressed);
    mouse.RemoveButtonListener(this, Mouse::Button_Left, Mouse::Button_Released);
}

void TrackballManipulator::Render() const
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetRenderState( Renderer::Lighting, false );
    renderer->SetPolygonMode( Renderer::FrontFace, Renderer::FillWireframe );
    renderer->SetShadeModel( Renderer::Flat );

    renderer->PushMatrix();
    renderer->Translate( mBallPos );
    renderer->Rotate( mNewRotation );
    renderer->SetColor( Color4f(0.3f, 0.3f, 0.3f, 1.0f) );
    renderer->DrawSphere( 1, 7 );
    renderer->PopMatrix();

/*
    renderer->SetColor( Color4f(1.0f, 0.3f, 0.3f, 1.0f) );

    Vector3f screen;
    Vector3f world;
    
    screen   = renderer->WorldToScreen( mBallPos );

    Int32 viewport[4];
    renderer->GetViewport( viewport );
    renderer->Begin2DProjection( viewport[0], viewport[2], viewport[1], viewport[3], -1, 1 );
    renderer->DrawLine( screen.x - 5, screen.y, screen.x + 5, screen.y );
    renderer->DrawLine( screen.x, screen.y - 5, screen.x, screen.y + 5 );
    renderer->End2DProjection();
    
    Camera3DProperties* camProp = mEditor->GetWorldManager().GetCurrentCamera();
    Camera3D* camera = Cast<Camera3D>(camProp->GetObject());

    Vector3f worldRay   = renderer->ScreenToWorld( screen ) - camera->GetPosition();
    worldRay.Normalize();
    
    renderer->PushMatrix();
    renderer->Translate( camera->GetPosition() + worldRay * 5 );
    renderer->Rotate( mNewRotation - camera->GetOrientation() ); 

    //renderer->DrawCircle( normal, radius, subdiv );
    Vector3f normal( 0, 1, 0 );
    Float    radius = 1;
    Float    subdiv = 50;
    Float    angle = Maths::PI_2 / subdiv;

     
    renderer->SetColor( Color4f(1.0f, 0.0f, 0.0f, 1.0f) );
    renderer->BeginScene( Renderer::LineStrip );
    for( Int32 i = 0; i < subdiv; i++ )
    {
        renderer->SetVertex( Vector3f( Maths::Cos( angle * i ) * radius, 0, Maths::Sin( angle * i ) * radius ) );
    }
    renderer->EndScene();

    renderer->SetColor( Color4f(0.0f, 1.0f, 0.0f, 1.0f) );
    renderer->BeginScene( Renderer::LineStrip );
    for( Int32 i = 0; i < subdiv; i++ )
    {
        renderer->SetVertex( Vector3f( 0, Maths::Cos( angle * i ) * radius, Maths::Sin( angle * i ) * radius ) );
    }
    renderer->EndScene();

    renderer->SetColor( Color4f(0.0f, 0.0f, 1.0f, 1.0f) );
    renderer->BeginScene( Renderer::LineStrip );
    for( Int32 i = 0; i < subdiv; i++ )
    {
        renderer->SetVertex( Vector3f( Maths::Sin( angle * i ) * radius, Maths::Cos( angle * i ) * radius, 0 ) );
    }
    renderer->EndScene();

    renderer->PopMatrix();
*/
    renderer->SetShadeModel( Renderer::Smooth );
    renderer->SetPolygonMode( Renderer::FrontFace, Renderer::FillSolid );

    renderer->SetColor( Color4f(0.0f, 1.0f, 0.0f, 1.0f) );
    
    renderer->BeginScene( Renderer::LineList );
    renderer->SetVertex( mBallPos );
    renderer->SetVertex( mDragStart );
    renderer->EndScene();

    renderer->SetColor( Color4f(0.0f, 0.0f, 1.0f, 1.0f) );
    
    renderer->BeginScene( Renderer::LineList );
    renderer->SetVertex( mBallPos );
    renderer->SetVertex( mDragPos );    
    renderer->EndScene();

    renderer->SetRenderState( Renderer::Lighting, true );
    renderer->SetColor( Color4f(1.0f, 1.0f, 1.0f, 1.0f) );
}

const InputState& TrackballManipulator::GetActivationInputState() const
{
    return mActivationInputState;
}

const InputState& TrackballManipulator::GetDeactivationInputState() const
{
    return mDeactivationInputState;
}

Bool TrackballManipulator::CanManipulate(const std::list<Entity*>& pEntities) const
{
	Bool canManipulateAll = true;

	// Check if all objects are supported,
	std::list<Entity*>::const_iterator itEntity;
	for(itEntity = pEntities.begin(); itEntity != pEntities.end(); itEntity++)
	{
	    if(!(*itEntity)->IsA(Entity::StaticClass()) ||
            (*itEntity)->IsA(Terrain::StaticClass()) ||
            (*itEntity)->IsA(SkyDome::StaticClass()))
        {
            canManipulateAll = false;
            break;
        }
	}    

	return canManipulateAll;
}

Bool TrackballManipulator::CanManipulate(Entity* pEntities) const
{
    return pEntities->IsA(Entity::StaticClass());
}

void TrackballManipulator::SetManipulatedEntity(Entity* pEntities)
{
	mEntities.clear();
    if(pEntities)
        mEntities.push_back(pEntities);
}

void TrackballManipulator::SetManipulatedEntity(const std::list<Entity*>& pEntities)
{
    mEntities = pEntities;
}

void TrackballManipulator::OnMouseMove(Int32 /*pRelX*/, Int32 /*pRelY*/)
{
    if(!IsActivated() || mEntities.size() == 0 || !HasFocus() || 
        InputSubsystem::GetMouse().IsUp(Mouse::Button_Left))
    {
        return;
    }
       
    mDragPos = CursorToSpherePos();

    Quaternionf quat;
    Matrix4f    matRotation;
    
    matRotation = mBallRot.GetInverted().ToMatrix( matRotation );
    
    Vector3f localStart = (mDragStart-mBallPos) * matRotation;
    Vector3f localPos   = (mDragPos-mBallPos) * matRotation;
   
    quat = quat.GetRotationArc( localPos, localStart ).GetNormalized();
    mNewRotation = quat*mBallRot;
    mNewRotation.Normalize();
    
    // #TODO: Rotate all objects around the center of the arcball
    for( std::list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); ++it )
	    (*it)->SetOrientation( mNewRotation );
}

void TrackballManipulator::BeginDrag()
{
    BoundingBox bb;

    // Get the ball position.
    mBallPos = Vector3f(0,0,0);
    
    for( std::list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); ++it )
        mBallPos += (*it)->GetBoundingBox().GetCenter() + (*it)->GetPosition();

    mBallPos /= mEntities.size();
    
    // #TODO: Fix for multiple objects.
    for( std::list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); ++it )
        mBallRot = (*it)->GetOrientation();
    
    mDragStart = CursorToSpherePos();
    mDragPos   = mDragStart;
}

void TrackballManipulator::EndDrag()
{
}

Vector3f TrackballManipulator::CursorToSpherePos()
{
    // Get the renderer.
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    mEditor->GetMainView()->MakeCurrent();

     // Get the camera.
    Camera* camera = mEditor->GetWorldManager().GetCurrentCamera();
    Float farViewDistance = camera->GetFarView();

    // Get the mouse position.
    Int32 x, y;
    InputSubsystem::GetMouse().GetPos(x, y);
    QPoint widgetPos = mEditor->GetMainView()->mapFromGlobal(QPoint(x, y));

    Int32    viewport[4];
    renderer->GetViewport(viewport);
    
    Vector2f screenRay( widgetPos.x(), viewport[3] - widgetPos.y() );

    // Get the vector from the mouse positions.
    Vector3f worldRayStart = camera->GetPosition();
    Vector3f worldRayEnd   = renderer->ScreenToWorld( screenRay ) - camera->GetPosition();

    worldRayEnd.Normalize();
    worldRayEnd *= farViewDistance;

    Vector3f onSphere = SphereLineIntersection( worldRayStart, worldRayEnd, mBallPos, 1 );

    return (onSphere-mBallPos).GetNormalized() + mBallPos;
}

void TrackballManipulator::OnMouseButtonPressed(const Mouse::Button& pButton)
{
    if(pButton != Mouse::Button_Left || !HasFocus())
        return;

    Int32 x, y;
    InputSubsystem::GetMouse().GetPos(x, y);
    QPoint widgetPos = mEditor->GetMainView()->mapFromGlobal(QPoint(x, y));

    BeginDrag();

    // #TODO: Fix for no command
    /*
    mOldObjectOrientation = mEntityProperties->GetOrientation();
    */
}


void TrackballManipulator::OnMouseButtonReleased(const Mouse::Button& pButton)
{
    if(pButton != Mouse::Button_Left || !HasFocus())
        return;

    EndDrag();

    // #TODO: Fix for no command
    /*
    if(mEntityProperties->GetOrientation() != mOldObjectOrientation)
    {
        RotateCommand* rotateCommand = new RotateCommand(mEntityProperties, 
                                                         mOldObjectOrientation);
        mEditor->GetCommandManager().PerformCommand(rotateCommand);
    }*/
}

Vector3f TrackballManipulator::SphereLineIntersection(const Vector3f& pStart, const Vector3f& pEnd, 
                                const Vector3f& pSpherePos, Float pRadius)
{
    // pStart  P1 coordinates (point of line)
    // pEnd  P2 coordinates (point of line)
    // pSpherePos, pRadius  P3 coordinates and radius (sphere)
    Vector3f interVector;
    Float a, b, c, mu, i ;

    a = (pEnd - pStart).GetLengthSqr();
    
    b = 2 * ( (pEnd.x - pStart.x)*(pStart.x - pSpherePos.x) +
              (pEnd.y - pStart.y)*(pStart.y - pSpherePos.y) + 
              (pEnd.z - pStart.z)*(pStart.z - pSpherePos.z) );
    
    c = pSpherePos.GetLengthSqr() + 
        pStart.GetLengthSqr() -
        2 * ( pSpherePos | pStart ) - 
        pRadius * pRadius;
    
    i = b * b - 4 * a * c ;

    if( i < 0.0 )
    {
        // no intersection
        interVector = Closest(pStart, pEnd, pSpherePos);
    }
    if( i == 0.0 )
    {
        // one intersection
        mu = -b/(2*a) ;
        interVector = pStart + ((pEnd - pStart) * mu);
    }
    if ( i > 0.0 )
    {
        // two intersections

        // first intersection
        mu = (-b + Maths::Sqrt( b*b - 4*a*c )) / (2*a);
        Vector3f inter1 = pStart + ((pEnd - pStart) * mu);
        // second intersection
        mu = (-b - Maths::Sqrt( b*b - 4*a*c )) / (2*a);
        Vector3f inter2 = pStart + ((pEnd - pStart) * mu);

        // Get the camera.
        Camera* camera = mEditor->GetWorldManager().GetCurrentCamera();
        if((inter1 - camera->GetPosition()).GetLength() <
           (inter2 - camera->GetPosition()).GetLength())
        {
            interVector = inter1;
        }
        else
        {
            interVector = inter2;
        }
    }

    return interVector;
}


// Return the closest point on the line segment start_end to point.
Vector3f TrackballManipulator::Closest(const Vector3f& pStart, 
                                       const Vector3f& pEnd, 
                                       const Vector3f& pPoint)
{
    // Acquire a vector from the start point to the point in question.	
    Vector3f A;
    A.x = pPoint.x - pStart.x;	
    A.y = pPoint.y - pStart.y;	
    A.z = pPoint.z - pStart.z;		
    
    // Acquire a vector from the start point to the end point.	
    Vector3f B;
    B.x = pEnd.x - pStart.x;	
    B.y = pEnd.y - pStart.y;	
    B.z = pEnd.z - pStart.z;		
    
    // Determine the length of the segment start_end.	
    Float d = B.GetLengthSqr();		
    
    // Normalize the vector.	
    B.Normalize();		
    
    // Calculate the dot product between the two vectors.	
    Float t = A.x * B.x + A.y * B.y + A.z * B.z;		
    // Rule out some special cases.		
    // t <= 0, meaning behind the start point.		
    if(t*t <= 0)			
        return pStart;		
    // t >= the length of start_end, meaning past the end point.		
    else if(t*t >= d)	// t squared, to avoid an unnecessary square root operation.
        return pEnd;				
    
    // If the closest point on start_end to point is somewhere along the segment, determine exactly where.	
    Vector3f C;
    C.x = B.x * t + pStart.x;	
    C.y = B.y * t + pStart.y;	
    C.z = B.z * t + pStart.z;		
    
    // Return the final value.	
    return C;
}




