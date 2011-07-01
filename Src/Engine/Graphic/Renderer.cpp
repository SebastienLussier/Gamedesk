/**
 *  @file       Renderer.cpp
 *  @brief	    Base renderer class and utilities.
 *  @author     Sébastien Lussier.
 *  @date       14/12/02.
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
#include "Renderer.h"

#include "Graphic/RenderTarget/RenderTarget.h"
#include "Graphic/RenderTarget/RenderWindow.h"
#include "Graphic/Texture/Texture.h"


namespace Gamedesk {
	
	
IMPLEMENT_ABSTRACT_CLASS(Renderer);


Renderer::Renderer()
    : mWasInitialized(false)
    , mRenderTarget(NULL)
    , mMaxAnisotropy(0)
{
}

Renderer::~Renderer()
{
    for( Vector<TextureStage*>::iterator itStage = mTextureStages.begin(); itStage != mTextureStages.end(); ++itStage )
        GD_DELETE(*itStage);
}

void Renderer::Init( RenderWindow* pRenderWindow )
{
    if( mWasInitialized )
        return;

    Super::Init();

    SetRenderTarget( pRenderWindow );

    mWasInitialized = true;
}
 
void Renderer::Kill()
{
    Super::Kill();
    mWasInitialized = false;
}

void Renderer::SetRenderTarget( RenderTarget* pRenderTarget )
{
    mRenderTarget = pRenderTarget;
    mRenderTarget->MakeCurrent();
}

RenderTarget* Renderer::GetRenderTarget() const
{
    return mRenderTarget;
}
  
void Renderer::SetRenderState( RenderState /*pRenderState*/, Bool /*pEnable*/, 
                               UInt32 /*pIndex*/ ) {}

//! Alphablending & Transparency
void Renderer::SetAlphaFunc( PixelCompareFunc /*pFunc*/, Float /*pRef*/ ){}
void Renderer::SetBlendFunc( PixelBlendingFactor /*pSrcFactor*/, 
                             PixelBlendingFactor /*pDstFactor*/ ){}

//! Depth Buffer
void Renderer::SetDepthFunc( PixelCompareFunc /*pFunc*/ ){}

//! Stencil Buffer
void Renderer::SetStencilFunc( PixelCompareFunc /*pFunc*/, UInt32 /*pRef*/, 
                               UInt32 /*pMask*/ ){}
void Renderer::SetStencilOperation( StencilOperation /*pOpFail*/,  
                                    StencilOperation /*pOpZFail*/, 
                                    StencilOperation /*pOpZPass*/ ){}

//! Fog Settings
void Renderer::SetFogMode( FogMode /*pMode*/ ){}
void Renderer::SetFogDensity( Float /*pDensity*/ ){}
void Renderer::SetFogRange( Float /*pStart*/, Float /*pEnd*/ ){}
void Renderer::SetFogColor( const Color4f& /*pColor*/ ){}

//! Misc
void Renderer::SetClipPlane( UInt32 /*pClipPlaneIndex*/, const Plane3d& /*pPlane*/ ){}
void Renderer::SetCulling( CullingMode /*pMode*/ ){}
void Renderer::SetAmbiantColor( const Color4f& /*pColor*/ ){}
void Renderer::SetMaterial( const Material& /*pMaterial*/ ){}
void Renderer::SetPolygonMode( PolygonFace /*pFace*/, FillMode /*pFillMode*/ ){}
Int32 Renderer::SetRenderMode( RenderMode /*pMode*/){ return -1; }
void Renderer::SetClearColor(const Color4f& /*pColor*/){}
void Renderer::SetShadeModel(ShadeModel /*pModel*/){}
void Renderer::SetMatrixMode(MatrixMode /*pMode*/){}
void Renderer::SetColorMask( Bool /*pRed*/, Bool /*pGreen*/, Bool /*pBlue*/, Bool /*pAlpha*/ ) {}

void Renderer::LoadIdentity(){}
void Renderer::MultMatrix(const Matrix4f& /*pMatrix*/){}
void Renderer::PushMatrix(){}
void Renderer::PopMatrix(){}

void Renderer::SetPolygonOffset(Float /*pFactor*/, Float /*pUnit*/){}
void Renderer::SetLineWidth(Float /*pWidth*/){}
void Renderer::SetLineStipple(Byte /*pRepeat*/, UInt16 /*pPattern*/){}

Vector3f Renderer::ScreenToWorld( const Vector2f& /*pScreen*/ ) { return Vector3f(); }
Vector3f Renderer::ScreenToWorld( const Vector3f& /*pScreen*/ ) { return Vector3f(); }
Vector3f Renderer::WorldToScreen( const Vector3f& /*pWorld*/ )  { return Vector3f(); }

void Renderer::GetModelViewMatrix( Matrix4d& /*pMatrix*/ ){}
void Renderer::GetModelViewMatrix( Matrix4f& /*pMatrix*/ ){}
void Renderer::GetProjectionMatrix( Matrix4d& /*pMatrix*/ ){}
void Renderer::GetProjectionMatrix( Matrix4f& /*pMatrix*/ ){}

void Renderer::GetDepth(Float /*pX*/, Float /*pY*/, Float& /*pZ*/){}
void Renderer::UnProject(Double /*pWinX*/, Double /*pWinY*/, Double /*pWinZ*/,
                         Matrix4d& /*pModelViewMatrix*/, Matrix4d& /*pProjectionMatrix*/,
                         Int32 /*pViewport*/[4],
                         Double& /*pObjX*/, Double& /*pObjY*/, Double& /*pObjZ*/){}
void Renderer::Project(Double /*pObjX*/, Double /*pObjY*/, Double /*pObjZ*/,
                       Matrix4d& /*pModelViewMatrix*/, Matrix4d& /*pProjectionMatrix*/,
                       Int32 /*pViewport*/[4],
                       Double& /*pWinX*/, Double& /*pWinY*/, Double& /*pWinZ*/){}

// Selection
void Renderer::SetSelectionBuffer(Int32 /*pBufferSize*/, UInt32* /*pBuffer*/){}
void Renderer::InitNames(){}
void Renderer::PushName(UInt32 /*pName*/){}
void Renderer::PopName(){}
void Renderer::LoadName(UInt32 /*pName*/){}
void Renderer::PickMatrix(Double /*pX*/, Double /*pY*/, Double /*pDelX*/, Double /*pDelY*/, 
					        Int32 /*pViewport*/[4]){}

//! Projection
void Renderer::SetViewport(Int32 /*pX*/, Int32 /*pY*/, 
                           Int32 /*pWidth*/, Int32 /*pHeight*/){}
void Renderer::GetViewport(Int32 /*pViewport*/[4]){}
void Renderer::Perspective(Float /*pFovY*/, Float /*pAspectRatio*/,
                           Float /*pNearViewDistance*/, Float /*pFarViewDistance*/){}

void Renderer::SetView( const Vector3f& pPosition, const Vector3f& pDir, const Vector3f& pUp )
{
    mViewPos = pPosition;
    mViewDir = pDir;
    mViewUp  = pUp;
}

const Vector3f& Renderer::GetViewPos() const
{
    return mViewPos;
}

const Vector3f& Renderer::GetViewDir() const
{
    return mViewDir;
}

const Vector3f& Renderer::GetViewUp() const
{
    return mViewUp;
}

void Renderer::Begin2DProjection( Float /*pLeft*/, Float /*pRight*/, Float /*pBottom*/, Float /*pTop*/, Float /*pZNear*/, Float /*pZFar*/ ) {}
void Renderer::End2DProjection(){}

void Renderer::Translate( const Vector3f& /*pTranslation*/ ) {}
void Renderer::Rotate( const Quaternionf& /*pRotation*/ ) {}
void Renderer::Scale( const Vector3f& /*pScaling*/ ) {}

//! Simple primitive drawing
void Renderer::DrawLine( Float /*pStartX*/, Float /*pStartY*/, Float /*pEndX*/, Float /*pEndY*/ ) {}
void Renderer::Draw2DTile( Float /*pX*/, Float /*pY*/, 
                           Float /*pW*/, Float /*pH*/,
                           Float /*pUStart*/, Float /*pVStart*/, Float /*pUEnd*/, Float /*pVEnd*/ ){}
void Renderer::DrawQuad( const Vector3f& /*pV0*/, const Vector3f& /*pV1*/, const Vector3f& /*pV1*/, const Vector3f& /*pV3*/, Bool /*pTextured*/ ) {}
void Renderer::DrawSphere( Float /*pRadius*/, UInt32 /*pPrecision*/ ) {}
void Renderer::DrawBox( const Vector3f& /*pMin*/, const Vector3f& /*pMax*/ ) {}

//! Basic rendering.
void Renderer::SetColor(const Color3f& /*pColor*/){}
void Renderer::SetColor(Float /*pR*/, Float /*pG*/, Float /*pB*/){}
void Renderer::SetColor(const Color4f& /*pColor*/){}
void Renderer::SetColor(Float /*pR*/, Float /*pG*/, Float /*pB*/, Float /*pA*/){}
void Renderer::SetNormal(const Vector3f& /*pVector*/){}
void Renderer::SetNormal(Float /*pX*/, Float /*pY*/, Float /*pZ*/){}
void Renderer::SetVertex(Int32 /*pX*/, Int32 /*pY*/){}
void Renderer::SetVertex(Float /*pX*/, Float /*pY*/){}
void Renderer::SetVertex(const Vector2i& /*pVector*/){}
void Renderer::SetVertex(const Vector2f& /*pVector*/){}
void Renderer::SetVertex(Int32 /*pX*/, Int32 /*pY*/, Int32 /*pZ*/){}
void Renderer::SetVertex(Float /*pX*/, Float /*pY*/, Float /*pZ*/){}
void Renderer::SetVertex(const Vector3i& /*pVector*/){}
void Renderer::SetVertex(const Vector3f& /*pVector*/){} 
void Renderer::SetUV( Float /*pX*/, Float /*pY*/ ){}
void Renderer::SetUV( Float /*pX*/, Float /*pY*/, Float /*pZ*/ ){}
void Renderer::SetUV( const Vector2f& /*pUV*/ ){}
void Renderer::SetUV( const Vector3f& /*pUV*/ ){}
void Renderer::SetUV( UInt32 /*pStage*/, Float /*pX*/, Float /*pY*/ ){}
void Renderer::SetUV( UInt32 /*pStage*/, Float /*pX*/, Float /*pY*/, Float /*pZ*/ ){}
void Renderer::SetUV( UInt32 /*pStage*/, const Vector2f& /*pUV*/ ){}
void Renderer::SetUV( UInt32 /*pStage*/, const Vector3f& /*pUV*/ ){} 

//! Texturing
TextureStage* Renderer::GetTextureStage( UInt32 pStage )
{
    GD_ASSERT( pStage < mTextureStages.size() );
    return mTextureStages[pStage];
}

UInt32 Renderer::GetTextureStageCount() const
{
    return mTextureStages.size();
}

Float Renderer::GetMaxAnisotropy() const
{
    return mMaxAnisotropy;
}

void Renderer::SetPointSize(Float /*pPointSize*/) 
{
}

void Renderer::SetPointMinSize(Float /*pMinPointSize*/)
{
}

void Renderer::SetPointMaxSize(Float /*pMaxPointSize*/)
{
}

void Renderer::SetPointAttenuation(Float /*pConstant*/, Float /*pLinear*/, Float /*pQuadratic*/)
{
}

//! TODO !//
void Renderer::SetLight( UInt32 /*pLightIndex*/, const Light& /*pLight*/ ){}
Light Renderer::GetLight( UInt32 /*pLightIndex*/ ) const{ return Light(); }


} // namespace Gamedesk
