/**
 *  @file       World.cpp
 *  @brief
 *  @author     Marco Arsenault.
 *  @date       01/02/04.
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
#include "World.h"

#include "Package/Package.h"

#include "Maths/Intersection.h"
#include "Maths/Frustum.h"

#include "World/SpacePartition/SpacePartition.h"

#include "World/SkyDome.h"
#include "World/Terrain.h"

#include "World/Character.h"
#include "World/ParticleEmitter.h"
#include "World/LensFlare.h"
#include "World/Level.h"

#include "World/Model3D.h"
#include "World/Camera.h"
#include "World/LookCamera.h"
#include "World/FollowCamera.h"
#include "World/CharacterCamera.h"

#include "Sound/SoundSubsystem.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/RenderTarget/RenderTarget.h"
#include "Debug/PerformanceMonitor.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(World);


World::World() :
    mCurrentCamera(NULL),
    mNbRenderedEntities(0),
    mWorldInitialized(false),
    mSpacePartition(0)
{
}

World::~World()
{
}

void World::Init(SpacePartition* pSpacePartition)
{
	Profile("World Init");

	mSpacePartition = pSpacePartition;

    if( SoundSubsystem::Instance() )
    {
    	SoundSubsystem::Instance()->SetSFXMasterVolume( 0 );
    }

    // Inserts a camera.
    SpawnEntity( Camera::StaticClass() );
    SetCurrentCamera(0);

    SpawnEntity( SkyDome::StaticClass() );
    //SpawnEntity( Terrain::StaticClass() );

    //SpawnEntity( Class::GetClassByName("TestTexture2D"),     Vector3f(-20, 20,-30) );
    //SpawnEntity( Class::GetClassByName("TestCubemap"),       Vector3f( 0,  20,-30) );
    //SpawnEntity( Class::GetClassByName("TestRenderTexture"), Vector3f( 20, 20,-30) );
    //SpawnEntity( Class::GetClassByName("TestShaderProgram"),   Vector3f(  0,  0, -50) );
    //SpawnEntity( Class::GetClassByName("TestUI"),            Vector3f(  0,  0,  0) );

    //SpawnEntity( Level::StaticClass() );

    //SpawnEntity( Class::GetClassByName("ODETestObject"), Vector3f(-20,10,-20) );
	
    if( SoundSubsystem::Instance() )
    {
    	SoundSubsystem::Instance()->SetSFXMasterVolume( 255 );
    }

    mWorldInitialized = true;
}

void World::Kill()
{
    List<Entity*>::const_iterator itEntity;
    for(itEntity = mEntities.begin(); itEntity != mEntities.end(); itEntity = mEntities.begin())
    {
        Entity* entity = (*itEntity);
        RemoveEntity(entity);

        if( entity->GetOwner() == this )
        {
            entity->Kill();
            GD_DELETE(entity);
        }
    }

    mWorldInitialized = false;
}

void World::AddCamera(Camera* pCamera)
{
    mCameras.push_back( pCamera );
}

void World::RemoveCamera(Camera* pCamera)
{
    mCameras.remove( pCamera );
    if( pCamera == mCurrentCamera )
    {
        mCurrentCamera = 0;
    }
}

void World::SetCurrentCamera( UInt32 pCameraIndex )
{
    GD_ASSERT( pCameraIndex >= 0 && pCameraIndex < mCameras.size() );

    List<Camera*>::iterator itCamera = mCameras.begin();
    for( UInt32 i = 0; i < pCameraIndex; i++)
        itCamera++;

    mCurrentCamera = *itCamera;
}

void World::NextCamera()
{
    // Find the next cam.
    UInt32 iCamIndex = 0;
    List<Camera*>::iterator itCamera = mCameras.begin();
    while(*itCamera != mCurrentCamera)
    {
        itCamera++;
        iCamIndex++;
    }

    SetCurrentCamera((iCamIndex + 1) % mCameras.size());
}

Camera* World::GetCurrentCamera()
{
    return mCurrentCamera;
}

const List<Camera*>& World::GetCameras() const
{
    return mCameras;
}

Entity* World::SpawnEntity( Class* pEntityClass, const Vector3f& pPosition, const Quaternionf& pOrientation, const String& pName )
{
    GD_ASSERT( pEntityClass && pEntityClass->IsDerivedFrom(Entity::StaticClass()) );

    {Profile("Spawn");
    {Profile(pEntityClass->GetName());
    Entity* spawnedObject = Cast<Entity>( pEntityClass->AllocateNew() );

    spawnedObject->SetOwner(this);
    spawnedObject->SetName( pName );
    spawnedObject->SetWorld(this);

    spawnedObject->Init();
    spawnedObject->SetPosition( pPosition );
    spawnedObject->SetOrientation( pOrientation );

    InsertEntity( spawnedObject );

    return spawnedObject;
    }
    }
}

void World::DestroyEntity( Entity* pEntity )
{
    pEntity->SetWorld(NULL);
    RemoveEntity( pEntity );

    pEntity->Kill();
    GD_DELETE(pEntity);
}

void World::InsertEntity( Entity* pEntity )
{
    if(mSpacePartition)
        mSpacePartition->Insert( pEntity );
	mEntities.push_back(pEntity);

    if( pEntity->IsA( Camera::StaticClass() ) )
        AddCamera( Cast<Camera>( pEntity ) );
}

void World::RemoveEntity( Entity* pEntity )
{
    if(mSpacePartition)
        mSpacePartition->Remove( pEntity );
	mEntities.remove(pEntity);

    if( pEntity->IsA( Camera::StaticClass() ) )
        RemoveCamera( Cast<Camera>( pEntity ) );
}

const List<Entity*>& World::GetEntities() const
{
    return mEntities;
}

void World::Update( Double pElapsedTime )
{
    // Update all the objects in the world.
    List<Entity*>::const_iterator itEntity;
    for(itEntity = mEntities.begin(); itEntity != mEntities.end(); ++itEntity)
        (*itEntity)->Update(pElapsedTime);
}

void World::Render()
{
    if( !mWorldInitialized )
        return;

    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    UInt32 viewWidth = renderer->GetRenderTarget()->GetWidth();
    UInt32 viewHeight = renderer->GetRenderTarget()->GetHeight();

    // Init.
    //renderer->BeginScene( Renderer::PointList );

    renderer->SetViewport( 0, 0, viewWidth, viewHeight );
    renderer->SetClearColor(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
    renderer->SetRenderState( Renderer::Texture2D, false );
    renderer->SetRenderState( Renderer::DepthTest, true );
    //renderer->SetRenderState( Renderer::Lighting, true );
    //renderer->SetCulling( Renderer::CullBackFace );

    // Render.
    renderer->Clear( Renderer::ColorBuffer | Renderer::DepthBuffer );

    renderer->SetMatrixMode(Renderer::ProjectionMatrix);
    renderer->LoadIdentity();

    Camera* currentCamera = GetCurrentCamera();
    renderer->Perspective(currentCamera->GetFovAngle(),
                          viewWidth / static_cast<Float>(viewHeight),
                          currentCamera->GetNearView(), currentCamera->GetFarView());

    renderer->SetMatrixMode(Renderer::ModelViewMatrix);
    renderer->LoadIdentity();
    currentCamera->ApplyViewMatrix();
    /*
    Light light;
    light.mPosition = Vector3f( 4.32f, -3.0f, -3.4f ).Normalize();
    light.mAmbient  = Color4f(1.0f,1.0f, 1.0f,1.0f);
    light.mDiffuse  = Color4f(1.0f,1.0f, 1.0f,1.0f);
    light.mSpecular = Color4f(1.0f,1.0f, 1.0f,1.0f);
    light.mType = Renderer::LightDirectional;
    renderer->SetRenderState( Renderer::Light_i, true, 0 );
    renderer->SetLight( 0, light );*/

#if 0
    Matrix4d modelView;
    renderer->GetModelViewMatrix( modelView );

    Vector3f vRight( modelView[0], modelView[4], modelView[8] );
    Vector3f vUp( modelView[1], modelView[5], modelView[9] );

    Vector3f vBottomLeft  = light.mPosition + (-vRight - vUp).Normalize() * 2;
    Vector3f vBottomRight = light.mPosition + ( vRight - vUp).Normalize() * 2;
    Vector3f vTopRight    = light.mPosition + ( vRight + vUp).Normalize() * 2;
    Vector3f vTopLeft     = light.mPosition + (-vRight + vUp).Normalize() * 2;

    /*renderer->SetRenderState( Renderer::DepthMask, false );
    renderer->SetRenderState( Renderer::Lighting, false );
    renderer->SetRenderState( Renderer::Blend, true );
    renderer->SetBlendFunc( Renderer::BlendSrcAlpha, Renderer::BlendOne );*/
    renderer->SetRenderState( Renderer::Lighting, false );
    renderer->DrawQuad( vBottomLeft, vBottomRight, vTopRight, vTopLeft, false );
    renderer->SetRenderState( Renderer::Lighting, true );
#endif


    renderer->SetAmbiantColor( Color4f(1.0f, 1.0f, 1.0f, 1.0f) );

      /*Light lightPoint;
    lightPoint.mPosition = currentCamera->GetPosition();
    lightPoint.mAmbient  = Color4f(0.00f,0.00f, 0.00f,1.0f);
    lightPoint.mDiffuse  = Color4f(1.0f,1.0f, 1.0f,1.0f);
    lightPoint.mSpecular = Color4f(1.0f,1.0f, 1.0f,1.0f);
    lightPoint.mType = Renderer::LightPoint;
    lightPoint.mAttenuationConstant  = 0;
    lightPoint.mAttenuationLinear    = 0;
    lightPoint.mAttenuationQuadratic = 0.01f;
    renderer->SetRenderState( Renderer::Light_i, true, 0 );
    renderer->SetLight( 0, lightPoint );*/

	// Get the frustum.
	Matrix4f modelViewMatrix;
    Matrix4f projectionMatrix;
    Frustum  frustum;

	renderer->GetModelViewMatrix(modelViewMatrix);
	renderer->GetProjectionMatrix(projectionMatrix);
	frustum.CalculateFrustum(projectionMatrix, modelViewMatrix);

	mNbRenderedEntities = 0;

    // Render the objects in the world.
    /*
    List<Entity*> visibleEntities;
	mSpacePartition->Query(frustum, visibleEntities);
    */
    List<Entity*>::const_iterator itEntity;
    for( itEntity = mEntities.begin(); itEntity != mEntities.end(); ++itEntity )
    {
        if( *itEntity != currentCamera )
        {
			// Do frustum culling.
			if((*itEntity)->GetClass() != Terrain::StaticClass() &&
               (*itEntity)->GetClass() != SkyDome::StaticClass())
			{
				BoundingBox boundingBox = (*itEntity)->GetBoundingBox();

                if( (*itEntity)->GetClass() != ParticleEmitter::StaticClass() )
                {
                    Matrix4f translation = Matrix4f::Translation((*itEntity)->GetPosition());
				    Matrix4f rotation;
				    (*itEntity)->GetOrientation().ToMatrix(rotation);

				    Matrix4f trsMatrix = rotation * translation;
				    boundingBox.SetMin( boundingBox.Min() * trsMatrix );
				    boundingBox.SetMax( boundingBox.Max() * trsMatrix );
                }

				if( frustum.BoxInFrustum(boundingBox) )
				{
					mNbRenderedEntities++;
				}
				else
				{
					continue;
				}
			}

            renderer->PushMatrix();

            if( String((*itEntity)->GetClass()->GetName()) == "SkyDome" )
                renderer->Translate(currentCamera->GetPosition()-Vector3f(0,100,0) );
            else
                renderer->Translate((*itEntity)->GetPosition());

            renderer->Rotate((*itEntity)->GetOrientation());

            if( (*itEntity)->IsSelected() )
                (*itEntity)->RenderSelected();

            (*itEntity)->Render();

            renderer->PopMatrix();
        }
    }

    renderer->SetMatrixMode(Renderer::ModelViewMatrix);
    renderer->LoadIdentity();
    currentCamera->ApplyViewMatrix();

    //renderer->EndScene();
}


Entity* World::LineTrace( const Vector3f& pOrigin, const Vector3f& pDir )
{
    /*const List<Entity*>& entities = mOctree.GetEntities();
    List<Entity*>::const_iterator itEntity;

    for( itEntity = entities.begin(); itEntity != entities.end(); ++itEntity )
    {
        if( !(*itEntity)->IsA( Model3D::StaticClass() ) )
            continue;

        BoundingBox boundingBox = (*itEntity)->GetBoundingBox();

        Matrix4f translation = Matrix4f::Translation((*itEntity)->GetPosition());
		Matrix4f rotation;

		(*itEntity)->GetOrientation().ToMatrix(rotation);
		Matrix4f trsMatrix = (rotation * translation).GetAffineInverse();

        // Transform global ray to local
        Ray3f ray( pOrigin * trsMatrix, trsMatrix.Transform( pDir, 0 ) );

        if( Intersect( ray, boundingBox ) )
        {
            Model3D* model3d = Cast<Model3D>( (*itEntity) );
            if( model3d->LineCheck( ray ) )
                return model3d;
        }
    }*/

    return NULL;
}

void World::SaveWorld( const String& pFilename )
{
    Package* package = PackageManager::Instance()->CreatePackage( pFilename );

    SetOwner( package );
    package->Save();
    SetOwner( NULL );

    GD_DELETE(package);
}

void World::LoadWorld( const String& pFilename )
{
    Package* package = PackageManager::Instance()->CreatePackage( pFilename );

    SetOwner( package );
    package->Load();
    SetOwner( NULL );
}

void World::Serialize( Stream& pStream )
{
    Super::Serialize( pStream );

    if(mSpacePartition)
        mSpacePartition->Serialize(pStream);

    pStream << mEntities;
    pStream << mCameras;
    pStream << mCurrentCamera;
}


} // namespace Gamedesk
