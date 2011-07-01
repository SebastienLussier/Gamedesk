/**
 *  @file       OGLRenderer.h
 *  @brief      Gamedesk renderer for OpenGL.
 *  @author     Sébastien Lussier.
 *  @date       03/11/03.
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
#ifndef     _OGL_RENDERER_H_
#define     _OGL_RENDERER_H_


#include "Graphic/Renderer.h"


#include "OGLCaps.h"
#include "OGLStateManager.h"

#include "Maths/Matrix4.h"


namespace Gamedesk {


class OGLGRAPHIC_API OGLRenderer : public Renderer
{
    DECLARE_CLASS(OGLRenderer, Renderer);

public:
    OGLRenderer();
    virtual ~OGLRenderer();

    virtual void Init( RenderWindow* pRenderWindow );
    virtual void Kill();

    virtual void Clear( Bitfield pBufferFlags );
    virtual void BeginScene( PrimitiveType pPrimitiveType );
    virtual void EndScene();

    virtual void SetVertexFormat( VertexFormat pVertexFormat );
    virtual void SetStreamSource( VertexFormat::Component pComponent, VertexBuffer* pVertexBuffer );
    virtual void SetIndices( IndexBuffer* pIndexBuffer );
    
    virtual void DrawPrimitive( PrimitiveType pType, UInt32 pStartVertex, UInt32 pVertexCount );
    virtual void DrawIndexedPrimitive( PrimitiveType pType, UInt32 pStartIndex, UInt32 pIndexCount );

    void SetRenderState( RenderState pRenderState, Bool pEnable, UInt32 pIndex=0 );

    //! Alphablending & Transparency
    void SetAlphaFunc( PixelCompareFunc pFunc, Float pRef );
    void SetBlendFunc( PixelBlendingFactor pSrcFactor, PixelBlendingFactor pDstFactor );

    //! Depth Buffer
    void SetDepthFunc( PixelCompareFunc pFunc );

    //! Stencil Buffer
    void SetStencilFunc( PixelCompareFunc pFunc, UInt32 pRef, UInt32 pMask );
    void SetStencilOperation( StencilOperation pOpFail,  StencilOperation pOpZFail, StencilOperation pOpZPass );

    //! Fog Settings
    void SetFogMode( FogMode pMode );
    void SetFogDensity( Float pDensity );
    void SetFogRange( Float pStart, Float pEnd );
    void SetFogColor( const Color4f& pColor );

    void SetClipPlane( UInt32 pClipPlaneIndex, const Plane3d& pPlane );
    void SetCulling( CullingMode pMode );
    void SetAmbiantColor( const Color4f& pColor );
    void SetMaterial( const Material& pMaterial );
    void SetPolygonMode( PolygonFace pFace, FillMode pFillMode );
	Int32 SetRenderMode( RenderMode pMode );
    void SetClearColor(const Color4f& pColor);
    void SetClearDepth(Double pDepth);
    void SetShadeModel(ShadeModel pModel);
    
    void SetColorMask( Bool pRed, Bool pGreen, Bool pBlue, Bool pAlpha );
    
    void SetMatrixMode(MatrixMode pMode);
    void LoadIdentity();
    void MultMatrix(const Matrix4f& pMatrix);
    void PushMatrix();
    void PopMatrix();    

    void SetPolygonOffset(Float pFactor, Float pUnit);
	void SetLineWidth(Float pWidth);
    void SetLineStipple(Byte pRepeat, UInt16 pPattern);

    Vector3f ScreenToWorld( const Vector2f& pScreen );
    Vector3f ScreenToWorld( const Vector3f& pScreen );
    Vector3f WorldToScreen( const Vector3f& pWorld );

    void GetModelViewMatrix(Matrix4d& pModelViewMatrix);
	void GetModelViewMatrix(Matrix4f& pModelViewMatrix);
	void GetProjectionMatrix(Matrix4d& pProjectionMatrix);
	void GetProjectionMatrix(Matrix4f& pProjectionMatrix);
	void GetDepth(Float pX, Float pY, Float& pZ);
    void UnProject(Double pWinX, Double pWinY, Double pWinZ,
                   Matrix4d& pModelViewMatrix, Matrix4d& pProjectionMatrix,
                   Int32 pViewport[4],
                   Double& pObjX, Double& pObjY, Double& pObjZ);
    void Project(Double pObjX, Double pObjY, Double pObjZ,
                 Matrix4d& pModelViewMatrix, Matrix4d& pProjectionMatrix,
                 Int32 pViewport[4],
                 Double& pWinX, Double& pWinY, Double& pWinZ);

	// Selection.
	void SetSelectionBuffer(Int32 pBufferSize, UInt32* pBuffer);
	virtual void InitNames();
	virtual void PushName(UInt32 pName);
	virtual void PopName();
	virtual void LoadName(UInt32 pName);
	virtual void PickMatrix(Double pX, Double pY, Double pDelX, Double pDelY, 
					        Int32 pViewport[4]);

    //! View
    void SetView( const Vector3f& pPosition, const Vector3f& pDir, const Vector3f& pUp );

    //! Projection
    void SetViewport(Int32 pX, Int32 pY, Int32 pWidth, Int32 pHeight);
    void GetViewport(Int32 pViewport[4]);
    void Perspective(Float pFovY, Float pAspectRatio,
                     Float pNearViewDistance, Float pFarViewDistance);

    

    void Begin2DProjection( Float pLeft, Float pRight, Float pBottom, Float pTop, Float pZNear, Float pZFar );
    void End2DProjection();
    
    void Translate( const Vector3f& pTranslation );
    void Rotate( const Quaternionf& pRotation );
    void Scale( const Vector3f& pScaling );

    //! Simple primitive drawing
    void DrawLine( Float pStartX, Float pStartY, Float pEndX, Float pEndY );
    void Draw2DTile( Float pX, Float pY, Float pW, Float pH, Float pUStart = 0.0f, Float pVStart = 0.0f, Float pUEnd = 1.0f, Float pVEnd = 1.0f );
    void DrawQuad( const Vector3f& pV0, const Vector3f& pV1, const Vector3f& pV2, const Vector3f& pV3, Bool pTextured = true );
    void DrawSphere( Float pRadius, UInt32 pPrecision );
    void DrawBox( const Vector3f& pMin, const Vector3f& pMax );
    
    //! Basic  rendering functions.
    void SetColor(Float pR, Float pG, Float pB);
    void SetColor(const Color3f& pColor);    
    
    void SetColor(Float pR, Float pG, Float pB, Float pA);
    void SetColor(const Color4f& pColor);
    
    void SetNormal(Float pX, Float pY, Float pZ);
    void SetNormal(const Vector3f& pVector);    

    void SetVertex(Int32 pX, Int32 pY);
    void SetVertex(Float pX, Float pY);
    void SetVertex(const Vector2i& pVector);
    void SetVertex(const Vector2f& pVector);

    void SetVertex(Int32 pX, Int32 pY, Int32 pZ);
    void SetVertex(Float pX, Float pY, Float pZ);
    void SetVertex(const Vector3i& pVector);
    void SetVertex(const Vector3f& pVector);    
    
    void SetUV( Float pX, Float pY );
    void SetUV( Float pX, Float pY, Float pZ );
    void SetUV( const Vector2f& pUV );
    void SetUV( const Vector3f& pUV );
    void SetUV( UInt32 pStage, Float pX, Float pY );
    void SetUV( UInt32 pStage, Float pX, Float pY, Float pZ );
    void SetUV( UInt32 pStage, const Vector2f& pUV );
    void SetUV( UInt32 pStage, const Vector3f& pUV );    

    //! Point sprites
    void SetPointSize(Float pPointSize);
    void SetPointMinSize(Float pMinPointSize);
    void SetPointMaxSize(Float pMaxPointSize);
    void SetPointAttenuation(Float pConstant, Float pLinear, Float pQuadratic);
    
    //! Lighting
    void    SetLight( UInt32 pLightIndex, const Light& pLight );
    Light   GetLight( UInt32 pLightIndex ) const;

protected:
    void DrawWorkAround( PrimitiveType pType, UInt32 pStart, UInt32 pIndex, Bool pIndexed );

protected:
    String GetCardDesc();

    OGLCaps&            mCaps;
    OGLStateManager     mStateManager;

    GLenum              mRenderMode;
    IndexBuffer*        mIndexBuffer;
    VertexBuffer*       mPositionBuffer;
    VertexFormat        mCurrentVertexFormat;

    GLUquadricObj*      mQuadric;
};


} // namespace Gamedesk


#endif  //  _OGL_RENDERER_H_
