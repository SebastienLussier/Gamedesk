/**
 *  @file       ModelBrowserTool.cpp
 *  @brief      Tool used to display a list of models and load them..
 *  @author     Sébastien Lussier.
 *  @date       10/02/04.
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
#include "ModelBrowserDll.h"
#include "ModelBrowserTool.h"

#include "Maths/BoundingBox.h"

#include "EditorBase.h"
#include "QRenderWindow.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/Font.h"

#include "ModelBrowserUI.h"
#include "World/Model3D.h"


IMPLEMENT_CLASS(ModelBrowserTool);


ModelBrowserTool::ModelBrowserTool() : 
    mModelBrowserWindow(NULL),
	mModel(NULL)
{
	mModel = GD_NEW(Model3D, this, "Model3D");
}

ModelBrowserTool::~ModelBrowserTool()
{
	GD_DELETE(mModel);
}

void ModelBrowserTool::LoadSettings( ConfigSection& pSection )
{
    Super::LoadSettings( pSection );

    // #TODO: Load your tool's settings here.
}

void ModelBrowserTool::SaveSettings( ConfigSection& pSection ) const
{
    Super::SaveSettings( pSection );

    // #TODO: Save your tool's settings here.
}

void ModelBrowserTool::Init()
{
    mCamera.SetPosition( Vector3f(0.0f, 1.0f, 3.0f) );
    mCamera.SetFovAngle( 60 );
    mCamera.SetNearView( 0.1f );
    mCamera.SetFarView( 10000.0f );
}


void ModelBrowserTool::CreateUI()
{
    if(!mModelBrowserWindow)
    {
        mModelBrowserWindow = new ModelBrowserUI(GetEditor()->GetWorkspace(), this);
        mModelBrowserWindow->setName("Model Browser");
        mModelBrowserWindow->show();
                
        SetWindow(mModelBrowserWindow);
    }
}

void ModelBrowserTool::DestroyUI()
{
    if( mModelBrowserWindow )
    {
        mModelBrowserWindow->hide();
        SetWindow( NULL );
    }    
}

void ModelBrowserTool::Render()
{
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    renderer->SetViewport( 0, 0, mModelBrowserWindow->mViewerFrame->width(), mModelBrowserWindow->mViewerFrame->height() );
    renderer->SetClearColor( Color4f( 0.1f, 0.2f, 0.4f, 1.0f) );

    renderer->SetCulling( Renderer::CullBackFace );
    
    // Render.
	renderer->Clear( Renderer::ColorBuffer | Renderer::DepthBuffer );
    
    renderer->SetRenderState( Renderer::DepthTest, true );

    // Camera.
    renderer->SetMatrixMode(Renderer::ProjectionMatrix);
    renderer->LoadIdentity();
    renderer->Perspective(mCamera.GetFovAngle(), 
                          (float)mModelBrowserWindow->mViewerFrame->width() / (float)mModelBrowserWindow->mViewerFrame->height(),
                          mCamera.GetNearView(), mCamera.GetFarView());

    renderer->SetMatrixMode(Renderer::ModelViewMatrix);
    renderer->LoadIdentity();
    renderer->SetView( mCamera.GetPosition(), mObjectCenter - mCamera.GetPosition(), mCamera.GetUp() );

    Light light;
    light.mPosition = mCamera.GetPosition();
    light.mAmbient = Color4f(0.1f,0.1f, 0.1f,1.0f);
    light.mDiffuse = Color4f(0.9f,0.9f, 0.9f,1.0f);
    light.mType = Renderer::LightPoint;
    renderer->SetRenderState( Renderer::Light_i, true, 0 );
    renderer->SetLight( 0, light );   
    renderer->SetRenderState( Renderer::Lighting, true );

    renderer->SetColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    
	// Change the orientation.
    static Float pAngle = 0;
    pAngle += 0.05f;
    mModel->SetOrientation(Quaternionf( Vector3f(0,1,0), pAngle ));

	renderer->Translate(mModel->GetPosition());
	renderer->Rotate(mModel->GetOrientation());
    mModel->Render();
}

void ModelBrowserTool::SetViewedModel( const String& pModelFilename )
{
    mModel->SetMesh( pModelFilename );
	SetCameraFocus();
}

void ModelBrowserTool::SetCameraFocus()
{
	BoundingBox boundingBox = mModel->GetBoundingBox();
    Vector3f boxCenter = boundingBox.GetCenter();
	
	// Cubify the bounding box.
	BoundingBox boundingCube = boundingBox;
	Float maxValue = Maths::Max(boundingCube.Max().x, Maths::Max(boundingCube.Max().y, boundingCube.Max().z));
	Float minValue = Maths::Min(boundingCube.Min().x, Maths::Min(boundingCube.Min().y, boundingCube.Min().z));
	boundingCube.SetMax( Vector3f(maxValue, maxValue, maxValue) );
    boundingCube.SetMin( Vector3f(minValue, minValue, minValue) );
	
	// Recenter
	Vector3f cubeCenter = boundingCube.GetCenter();
	Vector3f offset = cubeCenter - boxCenter;
	cubeCenter -= offset;
	boundingCube(0) -= offset;
	boundingCube(1) -= offset;

	Float fovAngle = Maths::ToRadians(mCamera.GetFovAngle());
	Float halfFov = fovAngle / 2.0f;
	Float sin = Maths::Sin(halfFov);
	Float cos = Maths::Cos(halfFov);

	// Compute the camera position and viewing position.
	Float halfPos = cubeCenter.y - boundingCube.Min().y;
	Float zPos = boundingBox.Max().z + (cos * halfPos / sin);

	mCamera.SetPosition(Vector3f(boxCenter.x, boxCenter.y, zPos));
	mObjectCenter = Vector3f(boxCenter.x, boxCenter.y, boxCenter.z);
}
