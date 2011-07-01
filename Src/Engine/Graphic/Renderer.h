 /**
 *  @file       Renderer.h
 *  @brief      Base renderer class and utilities.
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
#ifndef     _RENDERER_H_
#define     _RENDERER_H_


#include "Graphic/Color3.h"
#include "Graphic/Color4.h"
#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Maths/Quaternion.h"
#include "Maths/Matrix4.h"
#include "Maths/Plane3.h"

#include "Material/Material.h"
#include "Buffer/VertexFormat.h"
#include "Texture/TextureStage.h"
#include "RenderTarget/RenderTarget.h"


namespace Gamedesk {


class Light;
class VertexBuffer;
class IndexBuffer;
class Texture;
class RenderWindow;


class ENGINE_API Renderer : public Object
{
    DECLARE_ABSTRACT_CLASS(Renderer,Object);

public:
    enum RenderState
    {
        AlphaTest,
        AutoNormal,
        Blend,
        ClipPlane_i,
        CullFace,
        DepthTest,
        DepthMask,
        Dithering,
        Fog,
        Light_i,
        Lighting,
        LineSmooth,
        LineStipple,
        LocalViewerLights,
        LogicOp,
        Map1Color4,
        Map1Index,
        Map1Normal,
        Map1TextureCoord1,
        Map1TextureCoord2,
        Map1TextureCoord3,
        Map1TextureCoord4,
        Map1Vertex3,
        Map1Vertex4,
        Map2Color4,
        Map2Index,
        Map2Normal,
        Map2TextureCoord1,
        Map2TextureCoord2,
        Map2TextureCoord3,
        Map2TextureCoord4,
        Map2Vertex3,
        Map2Vertex4,
        Normalize,
        PointSmooth,
        PointSprite,
        PolygonSmooth,
        PolygonStipple,
        ScissorTest,
        StencilTest,
        Texture1D,
        Texture2D,
        TextureGenQ,
        TextureGenR,
        TextureGenS,
        TextureGenT,
        TwoSidedLightning
    };

    //! Pixel comparison functions
    enum PixelCompareFunc
    {
        CmpNever,               //!< Never
        CmpLess,                //!< Less
        CmpEqual,               //!< Equal
        CmpLessEqual,           //!< Less Equal
        CmpGreater,             //!< Greater
        CmpNotEqual,            //!< Not Equal
        CmpGreaterEqual,        //!< Greater Equal
        CmpAlways               //!< Always
    };

    //! Pixel blending factor
    enum PixelBlendingFactor
    {
        BlendZero,              //!< Zero    
        BlendOne,               //!< One
        BlendSrcColor,          //!< Source color
        BlendSrcAlpha,          //!< Source alpha
        BlendInvSrcColor,       //!< Inverse source color
        BlendInvSrcAlpha,       //!< Inverse source alpha
        BlendSrcAlphaSaturate,  //!< Source alpha saturate
        BlendDstColor,          //!< Destination color
        BlendDstAlpha,          //!< Destination alpha
        BlendInvDstColor,       //!< Inverse destination color
        BlendInvDstAlpha        //!< Inverse destination alpha
    };

    enum StencilOperation
    {
        OpKeep,
        OpZero,
        OpReplace,
        OpIncrease,
        OpDecrease,
        OpInvert
    };

	enum RenderMode
	{
		RenderNormal,
		RenderSelection,
		RenderFeedback
	};

    enum CullingMode
    {
        NoCulling,
        CullFrontFace,
        CullBackFace,
        CullBoth
    };

    enum FillMode
    {
        FillPoint,
        FillWireframe,
        FillSolid
    };

    enum PolygonFace
    {
        FrontFace,
        BackFace,
        BothFace
    };

    enum FogMode
    {
        FogLinear,
        FogExp,
        FogExp2
    };

    enum PrimitiveType
    {
        PointList,
        LineList,
        LineStrip,
        LineLoop,
        TriangleList,
        TriangleStrip,
        TriangleFan,
        QuadList,
        QuadStrip,
        PolygonList
    };

    enum BufferType
    {
        ColorBuffer     = 0x00000001,
        DepthBuffer     = 0x00000002,
        StencilBuffer   = 0x00000004
    };

    //! Type of light
    enum LightType
    {
        LightPoint,         //!< Point light (positional)
        LightDirectional,   //!< Directional light
        LightSpot           //!< Spot light
    };

    enum ShadeModel
    {
        Flat,
        Smooth
    };

    enum MatrixMode
    {
        ModelViewMatrix,
        ProjectionMatrix,
        TextureMatrix,
        ColorMatrix
    };

public:
    Renderer();
    virtual ~Renderer();

    void            SetRenderTarget( RenderTarget* pRenderTarget );
    RenderTarget*   GetRenderTarget() const;
    
    virtual void Init( RenderWindow* pWindow );
    virtual void Kill();
    
    /**
     *  Clear the active render target buffers.
     *  @param  pBufferFlags    Specify which buffers should be cleared.
     */
    virtual void Clear( Bitfield pBufferFlags ) = 0;

    virtual void BeginScene( PrimitiveType pPrimitiveType ) = 0;
    virtual void EndScene() = 0;
    
    /**
     *  Set the vertex format currently used by the primitive drawing methods.
     *  @param  pVertexFormat   The vertex format you want to use.
     */
    virtual void SetVertexFormat( VertexFormat pVertexFormat ) = 0;

    /**
     *  Specify from which vertex buffer the given vertex component data must be retreived.
     *  @param  pComponent      The component that can be found in the vertex buffer.
     *  @param  pVertexBuffer   The vertex buffer containing the given vertex component data.
     *  @remarks The specified vertex component must be a valid component found in the current vertex format.
     *  @see SetVertexFormat.
     *  @see DrawPrimitive, DrawIndexedPrimitive.
     */
    virtual void SetStreamSource( VertexFormat::Component pComponent, VertexBuffer* pVertexBuffer ) = 0;

    /**
     *  Set the current index buffer used by the indexed primitive rendering methods.
     *  @param  pIndexBuffer    The index buffer containing valid indices for rendering indexed primitives.
     *  @see DrawIndexedPrimitive.
     */
    virtual void SetIndices( IndexBuffer* pIndexBuffer ) = 0;
    
    /**
     *  Renders a sequence of nonindexed, geometric primitives of the specified type from the current set of data input streams.
     *  @param  pType           Type of primitive to render.
     *  @param  pStartVertex    The index of the first vertex to use in the active vertex buffers (specified via SetStreamSource()).
     *  @param  pVertexCount    Number of vertex to use render (must fit with the primitive type).
     */
    virtual void DrawPrimitive( PrimitiveType pType, UInt32 pStartVertex, UInt32 pVertexCount ) = 0;

    /**
     *  Renders the specified geometric primitive, based on indexing into an array of vertices (specified with SetIndices()).
     *  @param  pType           Type of primitive to render.
     *  @param  pStartIndex     The index of the first index to use in the active index buffer (specified via SetIndices()).
     *  @param  pIndexCount     Number of index to use.
     */
    virtual void DrawIndexedPrimitive( PrimitiveType pType, UInt32 pStartIndex, UInt32 pIndexCount ) = 0;
    
    /**
     *  Enable or disable a given render state.
     */
    virtual void SetRenderState( RenderState pRenderState, Bool pEnable, UInt32 pIndex = 0 );

    //! Alphablending & Transparency
    virtual void SetAlphaFunc( PixelCompareFunc pFunc, Float pRef );    // Ref = [0.0f, 1.0f]
    virtual void SetBlendFunc( PixelBlendingFactor pSrcFactor, PixelBlendingFactor pDstFactor );
    
    //! Depth Buffer
    virtual void SetDepthFunc( PixelCompareFunc pFunc );
    
    //! Stencil Buffer
    virtual void SetStencilFunc( PixelCompareFunc pFunc, UInt32 pRef, UInt32 pMask );
    virtual void SetStencilOperation( StencilOperation pOpFail,  StencilOperation pOpZFail, StencilOperation pOpZPass );

    //! Fog Settings
    virtual void SetFogMode( FogMode pMode );
    virtual void SetFogDensity( Float pDensity );
    virtual void SetFogRange( Float pStart, Float pEnd );
    virtual void SetFogColor( const Color4f& pColor );

    //! Misc
    virtual void SetClipPlane( UInt32 pClipPlaneIndex, const Plane3d& pPlane );
    virtual void SetCulling( CullingMode pMode );
    virtual void SetAmbiantColor( const Color4f& pColor );
    virtual void SetMaterial( const Material& pMaterial );
	virtual Int32 SetRenderMode(RenderMode pMode);
    virtual void SetPolygonMode( PolygonFace pFace, FillMode pFillMode );
    virtual void SetClearColor(const Color4f& pColor);
    virtual void SetShadeModel(ShadeModel pModel);
    virtual void SetMatrixMode(MatrixMode pMode);

    virtual void SetColorMask( Bool pRed, Bool pGreen, Bool pBlue, Bool pAlpha );

    virtual void LoadIdentity();
    virtual void MultMatrix(const Matrix4f& pMatrix);
    virtual void PushMatrix();
    virtual void PopMatrix();
    virtual void SetPolygonOffset(Float pFactor, Float pUnit);
	virtual void SetLineWidth(Float pWidth);
    virtual void SetLineStipple(Byte pRepeat, UInt16 pPattern);
    
    virtual void GetModelViewMatrix(Matrix4d& pModelViewMatrix);
	virtual void GetModelViewMatrix(Matrix4f& pModelViewMatrix);
	virtual void GetProjectionMatrix(Matrix4d& pProjectionMatrix);
	virtual void GetProjectionMatrix(Matrix4f& pProjectionMatrix);
	virtual void GetDepth(Float pX, Float pY, Float& pZ);

    //! Projection
    virtual Vector3f ScreenToWorld( const Vector2f& pScreen );
    virtual Vector3f ScreenToWorld( const Vector3f& pScreen );
    virtual Vector3f WorldToScreen( const Vector3f& pWorld );
    virtual void UnProject( Double pWinX, Double pWinY, Double pWinZ,
                            Matrix4d& pModelViewMatrix, Matrix4d& pProjectionMatrix,
                            Int32 pViewport[4],
                            Double& pObjX, Double& pObjY, Double& pObjZ );
    virtual void Project( Double pObjX, Double pObjY, Double pObjZ,
                          Matrix4d& pModelViewMatrix, Matrix4d& pProjectionMatrix,
                          Int32 pViewport[4],
                          Double& pWinX, Double& pWinY, Double& pWinZ );

	// Selection
	virtual void SetSelectionBuffer(Int32 pBufferSize, UInt32* pBuffer);
	virtual void InitNames();
	virtual void PushName(UInt32 pName);
	virtual void PopName();
	virtual void LoadName(UInt32 pName);
	virtual void PickMatrix(Double pX, Double pY, Double pDelX, Double pDelY, 
					        Int32 pViewport[4]);
	
    //! View
    virtual void SetView( const Vector3f& pPosition, const Vector3f& pDir, const Vector3f& pUp );
    const Vector3f& GetViewPos() const;
    const Vector3f& GetViewDir() const;
    const Vector3f& GetViewUp() const;

    //! Viewport
    virtual void SetViewport(Int32 pX, Int32 pY, Int32 pWidth, Int32 pHeight);
    virtual void GetViewport(Int32 pViewport[4]);

    //! Projection
    virtual void Perspective(Float pFovY, Float pAspectRatio,
                             Float pNearViewDistance, Float pFarViewDistance);
    virtual void Begin2DProjection( Float pLeft, Float pRight, Float pBottom, Float pTop, Float pZNear, Float pZFar );
    virtual void End2DProjection();
    
    //! Transform
    virtual void Translate( const Vector3f& pTranslation );
    virtual void Rotate( const Quaternionf& pRotation );
    virtual void Scale( const Vector3f& pScaling );

    //! Simple primitive drawing
    virtual void DrawLine( Float pStartX, Float pStartY, Float pEndX, Float pEndY );
    virtual void Draw2DTile( Float pX, Float pY, Float pW, Float pH, Float pUStart = 0.0f, Float pVStart = 0.0f, Float pUEnd = 1.0f, Float pVEnd = 1.0f );
    virtual void DrawQuad( const Vector3f& pV0, const Vector3f& pV1, const Vector3f& pV2, const Vector3f& pV3, Bool pTextured = true );
    virtual void DrawSphere( Float pRadius, UInt32 pPrecision );
    virtual void DrawBox( const Vector3f& pMin, const Vector3f& pMax );

    //! Basic  rendering functions.
    virtual void SetColor(Float pR, Float pG, Float pB);
    virtual void SetColor(const Color3f& pColor);    

    virtual void SetColor(Float pR, Float pG, Float pB, Float pA);
    virtual void SetColor(const Color4f& pColor);

    virtual void SetNormal(Float pX, Float pY, Float pZ);
    virtual void SetNormal(const Vector3f& pVector);    

    virtual void SetVertex(Int32 pX, Int32 pY);
    virtual void SetVertex(Float pX, Float pY);
    virtual void SetVertex(const Vector2i& pVector);
    virtual void SetVertex(const Vector2f& pVector);

    virtual void SetVertex(Int32 pX, Int32 pY, Int32 pZ);
    virtual void SetVertex(Float pX, Float pY, Float pZ);
    virtual void SetVertex(const Vector3i& pVector);
    virtual void SetVertex(const Vector3f& pVector);   

    virtual void SetUV( Float pX, Float pY );
    virtual void SetUV( Float pX, Float pY, Float pZ );
    virtual void SetUV( const Vector2f& pUV );
    virtual void SetUV( const Vector3f& pUV );
    virtual void SetUV( UInt32 pStage, Float pX, Float pY );
    virtual void SetUV( UInt32 pStage, Float pX, Float pY, Float pZ );
    virtual void SetUV( UInt32 pStage, const Vector2f& pUV );
    virtual void SetUV( UInt32 pStage, const Vector3f& pUV );  
    
    //! Texturing
    TextureStage*   GetTextureStage( UInt32 pStage );
    UInt32          GetTextureStageCount() const;
    Float           GetMaxAnisotropy() const;

    //! Point sprites
    virtual void SetPointSize(Float pPointSize);
    virtual void SetPointMinSize(Float pMinPointSize);
    virtual void SetPointMaxSize(Float pMaxPointSize);
    virtual void SetPointAttenuation(Float pConstant, Float pLinear, Float pQuadratic);

    //! TODO !//
    virtual void    SetLight( UInt32 pLightIndex, const Light& pLight );
    virtual Light   GetLight( UInt32 pLightIndex ) const;

protected:
    Bool                    mWasInitialized;
    RenderTarget*           mRenderTarget;

    Vector<TextureStage*>   mTextureStages;

    Float                   mMaxAnisotropy;

    //! Current view (camera) info.
    Vector3f                mViewPos;
    Vector3f                mViewDir;
    Vector3f                mViewUp;
};


class Light
{
public:
    Light() :
        mAmbient(0.0f,0.0f,0.0f,1.0f),
        mDiffuse(0.0f,0.0f,0.0f,1.0f),
        mSpecular(0.0f,0.0f,0.0f,1.0f),
        mPosition(0,0,1),
        mSpotDirection(0,0,-1),
        mSpotExponent(0),
        mSpotCutoff(180),
        mAttenuationConstant(1),
        mAttenuationLinear(0),
        mAttenuationQuadratic(0),
        mType(Renderer::LightDirectional)
    {
    }
  
    Color4f     mAmbient;
    Color4f     mDiffuse;
    Color4f     mSpecular;
    Vector3f    mPosition;
    
    Vector3f    mSpotDirection;
    Float       mSpotExponent;
    Float       mSpotCutoff;
    
    Float       mAttenuationConstant;
    Float       mAttenuationLinear;
    Float       mAttenuationQuadratic;

    Renderer::LightType   mType;
};



} // namespace Gamedesk


#endif  //  _RENDERER_H_
