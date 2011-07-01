/**
 *  @file       PSPRenderer.cpp
 *  @brief      Gamedesk renderer for PSP.
 *  @author     Sébastien Lussier.
 *  @date       09/09/2008.
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
#include "PSPGraphic.h"
#include "PSPRenderer.h"

#include "PSPTextureStage.h"

#include "Graphic/Buffer/SoftwareIndexBuffer.h"
#include "Graphic/Buffer/SoftwareVertexBuffer.h"
#include "Graphic/RenderTarget/RenderTarget.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(PSPRenderer);

// Global display list for immediate calls
static UInt32 __attribute__((aligned(16))) gGlobalDisplayList[256*1024];

// Screen size
#define PSP_SCREEN_WIDTH				480
#define PSP_SCREEN_HEIGHT				272
#define PSP_SCREEN_ASPECT				((Float32)PSP_SCREEN_WIDTH / (Float32)PSP_SCREEN_HEIGHT)
#define PSP_SCREEN_OFFSETX				((4096 - PSP_SCREEN_WIDTH) / 2)
#define PSP_SCREEN_OFFSETY				((4096 - PSP_SCREEN_HEIGHT) / 2)

// Frame buffer
#define PSP_VRAM_TOP					0x00000000
#define PSP_VRAM_WIDTH					512

// 16bit mode
#define PSP_VRAM_16BIT_BUFFER_SIZE		(PSP_VRAM_WIDTH*PSP_SCREEN_HEIGHT*2)
#define PSP_VRAM_16BIT_BUFFER_0			(void*)(PSP_VRAM_TOP)
#define PSP_VRAM_16BIT_BUFFER_1			(void*)(PSP_VRAM_TOP+PSP_VRAM_16BIT_BUFFER_SIZE)
#define PSP_VRAM_16BIT_BUFFER_2			(void*)(PSP_VRAM_TOP+(PSP_VRAM_16BIT_BUFFER_SIZE*2))

// 32bit mode
#define PSP_VRAM_32BIT_BUFFER_SIZE		(PSP_VRAM_WIDTH*PSP_SCREEN_HEIGHT*4)
#define PSP_VRAM_32BIT_BUFFER_0			(void*)(PSP_VRAM_TOP)
#define PSP_VRAM_32BIT_BUFFER_1			(void*)(PSP_VRAM_TOP+PSP_VRAM_32BIT_BUFFER_SIZE)
#define PSP_VRAM_32BIT_BUFFER_2			(void*)(PSP_VRAM_TOP+(PSP_VRAM_32BIT_BUFFER_SIZE*2))



#define INV     0xFFFFFFFF




const UInt32 GDToGURenderState[] = {
                    GU_ALPHA_TEST,                      // AlphaTest,
                    INV,								// AutoNormal,
                    GU_BLEND,                           // Blend,
                    GU_CLIP_PLANES,                     // ClipPlane_i,
                    INV,								// ColorMaterial,
                    GU_CULL_FACE,                       // CullFace,
                    GU_DEPTH_TEST,                      // DepthTest,
                    INV,								// DepthMask
                    GU_DITHER,                          // Dithering,
                    INV,								// Fog,
                    GU_LIGHT0,                          // Light_i,
                    GU_LIGHTING,                        // Lighting,
                    GU_LINE_SMOOTH,                     // LineSmooth,
                    INV,								// LineStipple,
                    INV,								// LocalViewerLights,
                    GU_COLOR_LOGIC_OP,                  // LogicOp,
                    INV,								// Map1Color4,
                    INV,								// Map1Index,
                    INV,								// Map1Normal,
                    INV,								// Map1TextureCoord1,
                    INV,								// Map1TextureCoord2,
                    INV,								// Map1TextureCoord3,
                    INV,								// Map1TextureCoord4,
                    INV,								// Map1Vertex3,
                    INV,								// Map1Vertex4,
                    INV,								// Map2Color4,
                    INV,								// Map2Index,
                    INV,								// Map2Normal,
                    INV,								// Map2TextureCoord1,
                    INV,								// Map2TextureCoord2,
                    INV,								// Map2TextureCoord3,
                    INV,								// Map2TextureCoord4,
                    INV,								// Map2Vertex3,
                    INV,								// Map2Vertex4,
                    INV,								// Normalize,
                    INV,								// PointSmooth,
                    INV,								// PointSprite
                    INV,								// PolygonSmooth,
                    INV,								// PolygonStipple,
                    GU_SCISSOR_TEST,                    // ScissorTest,
                    GU_STENCIL_TEST,                    // StencilTest,
                    INV,								// Texture1D,
                    GU_TEXTURE_2D,                      // Texture2D,
                    INV,								// TextureGenQ,
                    INV,								// TextureGenR,
                    INV,								// TextureGenS,
                    INV,								// TextureGenT,
                    INV									// TwoSidedLightning
                                        };

const UInt32 GDToGUPixelCompareFunc[] = {
                    GU_NEVER,                           // CmpNever,
                    GU_LESS,                            // CmpLess,
                    GU_EQUAL,                           // CmpEqual,
                    GU_LEQUAL,                          // CmpLessEqual,
                    GU_GREATER,                         // CmpGreater,
                    GU_NOTEQUAL,                        // CmpNotEqual,
                    GU_GEQUAL,                          // CmpGreaterEqual,
                    GU_ALWAYS                           // CmpAlways
                                        };

const UInt32 GDToGUPixelBlendingFactor[] =  {
                    GU_FIX,                             // BlendZero,
                    GU_FIX,                             // BlendOne,
                    GU_SRC_COLOR,                       // BlendSrcColor,
                    GU_SRC_ALPHA,                       // BlendSrcAlpha,
                    GU_ONE_MINUS_SRC_COLOR,             // BlendInvSrcColor,
                    GU_ONE_MINUS_SRC_ALPHA,             // BlendInvSrcAlpha,
                    INV,								// BlendSrcAlphaSaturate,
                    GU_DST_COLOR,                       // BlendDstColor,
                    GU_DST_ALPHA,                       // BlendDstAlpha,
                    GU_ONE_MINUS_DST_COLOR,             // BlendInvDstColor,
                    GU_ONE_MINUS_DST_ALPHA              // BlendInvDstAlpha
                                            };

const UInt32 GDToGUStencilOperation[] = {
                    GU_KEEP,                            // OpKeep,
                    GU_ZERO,                            // OpZero,
                    GU_REPLACE,                         // OpReplace,
                    GU_INCR,                            // OpIncrease,
                    GU_DECR,                            // OpDecrease,
                    GU_INVERT                           // OpInvert
                                        };

const UInt32 GDToGURenderMode[] = {
                    INV,								// RenderNormal,
                    INV,								// RenderSelection,
                    INV									// RenderFeedback
									};

const UInt32 GDToGUFillMode[] = {
                    INV,								// FillPoint,
                    INV,								// FillWireframe,
                    INV									// FillSolid
                                };

const UInt32 GDToGUPolygonFace[] =  {
                    INV,								// FrontFace
                    INV,								// BackFace
                    INV									// BothFace
                                    };

const UInt32 GDToGUFogMode[] =  {
                    GU_LINEAR,                          // FogLinear,
                    INV,								// FogExp,
                    INV									// FogExp2
                                };

const UInt32 GDToGUPrimitiveType[] =    {
                    GU_POINTS,                          // PointList,
                    GU_LINES,                           // LineList,
                    GU_LINE_STRIP,                      // LineStrip,
                    INV,								// LineLoop,
                    GU_TRIANGLES,                       // TriangleList,
                    GU_TRIANGLE_STRIP,                  // TriangleStrip,
                    GU_TRIANGLE_FAN,                    // TriangleFan,
                    INV,								// QuadList,
                    INV,								// QuadStrip,
                    INV									// PolygonList
                                        };

const UInt32 GDToGUShadeModel[] =   {
                    GU_FLAT,                            // Flat,
                    GU_SMOOTH                           // Smooth
                                    };

const UInt32 GDToGUMatrixMode[] =   {
                    INV,								// ModelViewMatrix,
                    GU_PROJECTION,                      // ProjectionMatrix,
                    GU_TEXTURE,                         // TextureMatrix,
                    INV									// ColorMatrix
                                    };

const UInt32 GDToGUCullingMode[] =  {
                    INV,								// NoCulling
                    GU_CW,								// FrontFace
                    GU_CCW,								// BackFace
                    INV									// CullBoth
                                    };





PSPRenderer::PSPRenderer() :
    mIndexBuffer(NULL),
    mVertexBuffer(NULL),
    mCurrentVertexFormat(VertexFormat::None),
    mFogFar(0.0f),
    mFogNear(0.0f),
    mFogColor(0)
{
}

PSPRenderer::~PSPRenderer()
{
    Kill();
}

void PSPRenderer::Init( RenderWindow* pRenderWindow )
{
    if( mWasInitialized )
        return;

    Super::Init( pRenderWindow );

	// Initalize the GU system.
    sceGuInit();

    // Send a few commands to initialize GU
    sceGuStart(GU_DIRECT, gGlobalDisplayList);

	// Set draw buffer parameters (and store in context for buffer-swap).
	sceGuDrawBuffer(GU_PSM_8888, PSP_VRAM_32BIT_BUFFER_0, PSP_VRAM_WIDTH);

	// Set display buffer parameters.
	sceGuDispBuffer(PSP_SCREEN_WIDTH, PSP_SCREEN_HEIGHT, PSP_VRAM_32BIT_BUFFER_1, PSP_VRAM_WIDTH);

	// Set depth buffer parameters.
	sceGuDepthBuffer(PSP_VRAM_32BIT_BUFFER_2, PSP_VRAM_WIDTH);

	sceGuOffset(2048 - (PSP_SCREEN_WIDTH/2),2048 - (PSP_SCREEN_HEIGHT/2));
	sceGuViewport(2048,2048,PSP_SCREEN_WIDTH,PSP_SCREEN_HEIGHT);
	sceGuDepthRange(65535,0);
	sceGuScissor(0,0,PSP_SCREEN_WIDTH,PSP_SCREEN_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);

	sceGuEnable(GU_TEXTURE_2D);
	sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGB);
	sceGuTexEnvColor(0xffff00);

	sceGuEnable(GU_CLIP_PLANES);

	sceGuClearColor(0xff000000);
	sceGuClearDepth(0);
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();

	SetMatrixMode(ModelViewMatrix);

	// Done, wait for sync
	sceGuFinish();
	sceGuSync(0,0);

	sceGuDisplay(GU_TRUE);

	// Initialize matrices


	// Texture stages - Only one on PSP!
    mTextureStages.push_back( GD_NEW(PSPTextureStage, this, "PSPGraphic::PSPRenderer")(0) );
}

void PSPRenderer::Kill()
{
	// Shutdown the GU system.
	sceGuTerm();

    Super::Kill();
}

void PSPRenderer::Clear( Bitfield pBufferFlags )
{
    int mask = 0;

    if( pBufferFlags & ColorBuffer )        mask |= GU_COLOR_BUFFER_BIT;
    if( pBufferFlags & DepthBuffer )        mask |= GU_DEPTH_BUFFER_BIT;
    if( pBufferFlags & StencilBuffer )      mask |= GU_STENCIL_BUFFER_BIT;

    sceGuClear( mask );
}

void PSPRenderer::BeginScene( PrimitiveType pPrimitiveType )
{
	sceGuStart(GU_DIRECT, gGlobalDisplayList);
}

void PSPRenderer::EndScene()
{
	sceGuFinish();
}

void PSPRenderer::SetVertexFormat( VertexFormat pVertexFormat )
{
    mCurrentVertexFormat  = pVertexFormat;
	mInternalVertexFormat = 0;

	GD_ASSERT( !mCurrentVertexFormat.HasComponent(VertexFormat::Color3) );

    // GU_VERTEX_32BITF
    if( mCurrentVertexFormat.HasComponent( VertexFormat::Position3 ) )
    	mInternalVertexFormat |= GU_VERTEX_32BITF | GU_TRANSFORM_3D;

    // GU_NORMAL_32BITF
    if( mCurrentVertexFormat.HasComponent( VertexFormat::Normal3 ) )
        mInternalVertexFormat |= GU_NORMAL_32BITF;

    // GU_COLOR_8888
    if( mCurrentVertexFormat.HasComponent( VertexFormat::Color4 ) )
        mInternalVertexFormat |= GU_COLOR_8888;

    // GU_TEXTURE_32BITF
    if( mCurrentVertexFormat.HasComponent( VertexFormat::TexCoord2 ) )
        mInternalVertexFormat |= GU_TEXTURE_32BITF;
}

void PSPRenderer::SetStreamSource( VertexFormat::Component pComponent, VertexBuffer* pVertexBuffer )
{
    GD_ASSERT(mCurrentVertexFormat == pComponent);
    mVertexBuffer = pVertexBuffer;
}

void PSPRenderer::SetIndices( IndexBuffer* pIndexBuffer )
{
    mIndexBuffer = pIndexBuffer;
}

void PSPRenderer::DrawPrimitive( PrimitiveType pType, UInt32 pStartVertex, UInt32 pVertexCount )
{
    GD_ASSERT(mVertexBuffer);
    GD_ASSERT(mCurrentVertexFormat != VertexFormat::None);

    void* vertices = mVertexBuffer->Lock(VertexBuffer::Lock_Read);
    sceGumDrawArray(GDToGUPrimitiveType[pType], mInternalVertexFormat, pVertexCount, 0, vertices);

    mVertexBuffer->Unlock();
}

void PSPRenderer::DrawIndexedPrimitive( PrimitiveType pType, UInt32 pStartIndex, UInt32 pIndexCount )
{
    GD_ASSERT(mVertexBuffer);
    GD_ASSERT(mIndexBuffer);
    GD_ASSERT(mCurrentVertexFormat != VertexFormat::None);

    void* vertices = mVertexBuffer->Lock(VertexBuffer::Lock_Read);
    void* indices = mIndexBuffer->Lock(IndexBuffer::Lock_Read);

    sceGumDrawArray(GDToGUPrimitiveType[pType], mInternalVertexFormat, pIndexCount, indices, vertices);

    mIndexBuffer->Unlock();
    mVertexBuffer->Unlock();
}

void PSPRenderer::SetRenderState( RenderState pState, Bool pEnable, UInt32 pIndex )
{
	// Those are currently not supported!
	//GU_COLOR_TEST
	//GU_COLOR_LOGIC_OP
	//GU_FACE_NORMAL_REVERSE
	//GU_PATCH_FACE
	//GU_FRAGMENT_2X

	switch(pState)
	{
	case AlphaTest:
		sceGuSetStatus(GU_ALPHA_TEST, pEnable);
		break;

	case DepthTest:
		sceGuSetStatus(GU_DEPTH_TEST, pEnable);
		break;

	case ScissorTest:
		sceGuSetStatus(GU_SCISSOR_TEST, pEnable);
		break;

	case Blend:
		sceGuSetStatus(GU_BLEND, pEnable);
		break;

	case CullFace:
		sceGuSetStatus(GU_CULL_FACE, pEnable);
		sceGuSetStatus(GU_PATCH_CULL_FACE, pEnable);
		break;

	case Dithering:
		sceGuSetStatus(GU_DITHER, pEnable);
		break;

	case ClipPlane_i:
		sceGuSetStatus(GU_CLIP_PLANES, pEnable);
		break;

	case Texture2D:
		sceGuSetStatus(GU_TEXTURE_2D, pEnable);
		break;

	case Lighting:
		sceGuSetStatus(GU_LIGHTING, pEnable);
		break;

	case Light_i:
		GD_ASSERT(pIndex < 4);
		sceGuSetStatus(GU_LIGHT0 + pIndex, pEnable);
		break;

	case LineSmooth:
		sceGuSetStatus(GU_LINE_SMOOTH, pEnable);
		break;

	default:
		GD_ASSERT(0);	// Unsupported state!
	}
}


void PSPRenderer::SetAlphaFunc( PixelCompareFunc pFunc, Float pRef )
{
	sceGuAlphaFunc(GDToGUPixelCompareFunc[pFunc], (int)(pRef * 255), 255);
}


void PSPRenderer::SetBlendFunc( PixelBlendingFactor pSrcFactor, PixelBlendingFactor pDstFactor )
{
	sceGuBlendFunc(GU_ADD, GDToGUPixelBlendingFactor[pSrcFactor], GDToGUPixelBlendingFactor[pDstFactor], 0, 0);
}


void PSPRenderer::SetDepthFunc( PixelCompareFunc pFunc )
{
    sceGuDepthFunc( GDToGUPixelCompareFunc[pFunc] );
}


void PSPRenderer::SetStencilFunc( PixelCompareFunc pFunc, UInt32 pRef, UInt32 pMask )
{
	sceGuStencilFunc(GDToGUPixelCompareFunc[pFunc], pRef, pMask);
}


void PSPRenderer::SetStencilOperation( StencilOperation pOpFail, StencilOperation pOpZFail, StencilOperation pOpZPass )
{
	sceGuStencilOp(GDToGUStencilOperation[pOpFail], GDToGUStencilOperation[pOpZFail], GDToGUStencilOperation[pOpZPass]);
}


void PSPRenderer::SetClipPlane( UInt32 pClipPlaneIndex, const Plane3d& pPlane )
{
    GD_ASSERT(0);	// Unsupported!
}


void PSPRenderer::SetCulling( CullingMode pMode )
{
	switch(pMode)
	{
	case NoCulling:
        SetRenderState( CullFace, false );
		break;

	case CullFrontFace:
	case CullBackFace:
		SetRenderState( CullFace, true );
        sceGuFrontFace(GDToGUCullingMode[pMode]);
		break;

	default:
		 GD_ASSERT(0);	// Unsupported!
	}
}

void PSPRenderer::SetFogMode( FogMode pMode )
{
    GD_ASSERT( 0 );	// Unsupported
}

void PSPRenderer::SetFogDensity( Float pDensity )
{
    GD_ASSERT( 0 );	// Unsupported
}

void PSPRenderer::SetFogRange( Float pStart, Float pEnd )
{
    mFogNear = pStart;
    mFogFar = pEnd;
    sceGuFog( mFogNear, mFogFar, mFogColor );
}

void PSPRenderer::SetFogColor( const Color4f& pColor )
{
    mFogColor = pColor.ToR8G8B8A8();
    sceGuFog( mFogNear, mFogFar, mFogColor );
}

void PSPRenderer::SetAmbiantColor( const Color4f& pColor )
{
    sceGuAmbientColor( pColor.ToR8G8B8A8() );
}


void PSPRenderer::SetMaterial( const Material& pMaterial )
{
    sceGuMaterial( GU_AMBIENT, pMaterial.mAmbient.ToR8G8B8A8() );
    sceGuMaterial( GU_DIFFUSE, pMaterial.mDiffuse.ToR8G8B8A8() );
    sceGuMaterial( GU_SPECULAR, pMaterial.mSpecular.ToR8G8B8A8() );
}

void PSPRenderer::SetPolygonMode( PolygonFace pFace, FillMode pFillMode )
{
    GD_ASSERT(0); // Unsupported
}

Int32 PSPRenderer::SetRenderMode(RenderMode pMode)
{
    GD_ASSERT(0); // Unsupported
    return 0;
}

void PSPRenderer::SetClearColor(const Color4f& pColor)
{
    sceGuClearColor(pColor.ToA8B8G8R8());
}

void PSPRenderer::SetShadeModel(ShadeModel pModel)
{
    sceGuShadeModel( GDToGUShadeModel[pModel] );
}

void PSPRenderer::SetColorMask( Bool pRed, Bool pGreen, Bool pBlue, Bool pAlpha )
{
    UInt32 mask = 0;
    mask |= 0xFF000000 * (UInt32)pRed;
    mask |= 0x00FF0000 * (UInt32)pGreen;
    mask |= 0x0000FF00 * (UInt32)pBlue;
    mask |= 0x000000FF * (UInt32)pAlpha;

    sceGuPixelMask( mask );
}

void PSPRenderer::SetMatrixMode(MatrixMode pMode)
{
	switch(pMode)
	{
	case ModelViewMatrix:
		sceGumMatrixMode(GU_MODEL);
		break;

	case ProjectionMatrix:
		sceGumMatrixMode(GU_PROJECTION);
		break;

	default:
		GD_ASSERT(0);	// Unsupported!
	}
}

void PSPRenderer::LoadIdentity()
{
    sceGumLoadIdentity();
}

void PSPRenderer::MultMatrix(const Matrix4f& pMatrix)
{
    sceGumMultMatrix((const ScePspFMatrix4*)&pMatrix);
}

void PSPRenderer::PushMatrix()
{
    sceGumPushMatrix();
}

void PSPRenderer::PopMatrix()
{
    sceGumPopMatrix();
}

void PSPRenderer::SetPolygonOffset(Float pFactor, Float pUnit)
{
	GD_ASSERT(0); // Unsupported
	//sceGuDepthOffset(unsigned int offset) ?????
}

void PSPRenderer::SetLineWidth(Float pWidth)
{
	GD_ASSERT(0); // Unsupported
}

void PSPRenderer::SetLineStipple(Byte pRepeat, UInt16 pPattern)
{
    GD_ASSERT(0); // Unsupported
}

Vector3f PSPRenderer::ScreenToWorld( const Vector3f& pScreen )
{
    Int32    viewport[4];
	Matrix4d modelview;
	Matrix4d projection;
    Double   posX, posY, posZ;

    GetModelViewMatrix(modelview);
    GetProjectionMatrix(projection);
	GetViewport(viewport);

    UnProject(pScreen.x, pScreen.y, pScreen.z, modelview, projection, viewport, posX, posY, posZ);

	return Vector3f(posX, posY, posZ);
}

Vector3f PSPRenderer::ScreenToWorld( const Vector2f& pScreen )
{
    Float depth;

    GetDepth(pScreen.x, pScreen.y, depth);

    return ScreenToWorld( Vector3f(pScreen.x, pScreen.y, depth) );
}

Vector3f PSPRenderer::WorldToScreen( const Vector3f& pWorld )
{
	Int32    viewport[4];
	Matrix4d modelview;
	Matrix4d projection;
	Double   objX, objY, objZ;

    GetModelViewMatrix(modelview);
    GetProjectionMatrix(projection);
	GetViewport(viewport);

    Project( pWorld.x, pWorld.y, pWorld.z, modelview, projection, viewport, objX, objY, objZ);

	return Vector3f(objX, objY, objZ);
}

void PSPRenderer::GetModelViewMatrix( Matrix4d& pMatrix )
{
	GD_ASSERT(0); // Unsupported
}

extern int gum_current_mode;
extern ScePspFMatrix4* gum_current_matrix;

void PSPRenderer::GetModelViewMatrix( Matrix4f& pMatrix )
{
	int currentMode = gum_current_mode;			// Get current matrix mode
	SetMatrixMode(ModelViewMatrix);				// Switch to model view
	pMatrix = *((Matrix4f*)gum_current_matrix);	// Copy matrix
	sceGumMatrixMode(currentMode);			    // Restore previous matrix mode
}

void PSPRenderer::GetProjectionMatrix( Matrix4d& pMatrix )
{
	GD_ASSERT(0); // Unsupported
}

void PSPRenderer::GetProjectionMatrix( Matrix4f& pMatrix )
{
    int currentMode = gum_current_mode;			// Get current matrix mode
	SetMatrixMode(ProjectionMatrix);			// Switch to projection
	pMatrix = *((Matrix4f*)gum_current_matrix);	// Copy matrix
	sceGumMatrixMode(currentMode);			    // Restore previous matrix mode
}

void PSPRenderer::GetDepth(Float pX, Float pY, Float& pZ)
{
    GD_ASSERT(0); // Unsupported
}

void PSPRenderer::UnProject(Double pWinX, Double pWinY, Double pWinZ,
                            Matrix4d& pModelViewMatrix, Matrix4d& pProjectionMatrix,
                            Int32 pViewport[4],
                            Double& pObjX, Double& pObjY, Double& pObjZ)
{
    GD_ASSERT(0); // Unsupported
}

void PSPRenderer::Project(Double pObjX, Double pObjY, Double pObjZ,
                          Matrix4d& pModelViewMatrix, Matrix4d& pProjectionMatrix,
                          Int32 pViewport[4],
                          Double& pWinX, Double& pWinY, Double& pWinZ)
{
    GD_ASSERT(0); // Unsupported;
}


void PSPRenderer::SetSelectionBuffer(Int32 pBufferSize, UInt32* pBuffer)
{
	GD_ASSERT(0); // Unsupported
}

void PSPRenderer::InitNames()
{
	GD_ASSERT(0); // Unsupported
}

void PSPRenderer::PushName(UInt32 pName)
{
	GD_ASSERT(0); // Unsupported
}

void PSPRenderer::PopName()
{
	GD_ASSERT(0); // Unsupported
}

void PSPRenderer::LoadName(UInt32 pName)
{
	GD_ASSERT(0); // Unsupported
}

void PSPRenderer::PickMatrix(Double pX, Double pY, Double pDelX, Double pDelY,
					        Int32 pViewport[4])
{
	GD_ASSERT(0); // Unsupported
}

void PSPRenderer::SetColor(const Color3f&)							{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetColor(Float , Float , Float )					{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetColor(const Color4f& )							{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetColor(Float , Float , Float , Float )			{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetNormal(const Vector3f& )						{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetNormal(Float , Float , Float )					{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetVertex(Int32 , Int32 )                         { GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetVertex(Float , Float )                         { GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetVertex(const Vector2i& )						{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetVertex(const Vector2f& )						{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetVertex(Int32 , Int32 , Int32 )				    { GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetVertex(Float , Float , Float )				    { GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetVertex(const Vector3i& )						{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetVertex(const Vector3f& )						{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetUV( Float , Float  )                           { GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetUV( Float , Float , Float  )					{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetUV( const Vector2f&  )                         { GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetUV( const Vector3f&  )                         { GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetUV( UInt32 , Float, Float )							{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetUV( UInt32 , Float , Float , Float  )			{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetUV( UInt32 , const Vector2f&  )				{ GD_ASSERT(0); } // Unsupported
void PSPRenderer::SetUV( UInt32 , const Vector3f&  )				{ GD_ASSERT(0); } // Unsupported

void PSPRenderer::SetLight( UInt32 pLightIndex, const Light& pLight )
{
    GD_ASSERT( pLightIndex < 4 );
    mLight[pLightIndex] = pLight;

    int guLightIndex = GU_LIGHT0 + pLightIndex;

    switch( pLight.mType )
    {
    case LightPoint:
	    sceGuLight(guLightIndex, GU_POINTLIGHT, GU_DIFFUSE_AND_SPECULAR, (const ScePspFVector3*)&pLight.mPosition);
        break;

    case LightDirectional:
        sceGuLight(guLightIndex, GU_DIRECTIONAL, GU_DIFFUSE_AND_SPECULAR, (const ScePspFVector3*)&pLight.mPosition);
        break;

    case LightSpot:
        sceGuLight(guLightIndex, GU_SPOTLIGHT, GU_DIFFUSE_AND_SPECULAR, (const ScePspFVector3*)&pLight.mPosition);
        sceGuLightSpot(guLightIndex, (const ScePspFVector3*)&pLight.mSpotDirection, pLight.mSpotExponent, pLight.mSpotCutoff);
        break;

    default:
        GD_ASSERT(0);   // Unsupported!
    }

    sceGuLightColor(guLightIndex, GU_AMBIENT, pLight.mAmbient.ToR8G8B8A8());
    sceGuLightColor(guLightIndex, GU_DIFFUSE, pLight.mDiffuse.ToR8G8B8A8());
    sceGuLightColor(guLightIndex, GU_SPECULAR, pLight.mSpecular.ToR8G8B8A8());
    sceGuLightAtt(guLightIndex, pLight.mAttenuationConstant, pLight.mAttenuationLinear, pLight.mAttenuationQuadratic);
}

Light PSPRenderer::GetLight( UInt32 pLightIndex ) const
{
    GD_ASSERT( pLightIndex < 4 );
    return mLight[pLightIndex];
}

void PSPRenderer::DrawLine( Float pStartX, Float pStartY, Float pEndX, Float pEndY )
{
    GD_ASSERT(0);	// Unsupported
}

void PSPRenderer::Draw2DTile( Float pX, Float pY, Float pW, Float pH, Float pUStart, Float pVStart, Float pUEnd, Float pVEnd )
{
    GD_ASSERT(0);	// Unsupported
}

void PSPRenderer::DrawQuad( const Vector3f& pV0, const Vector3f& pV1, const Vector3f& pV2, const Vector3f& pV3, Bool pTextured )
{
    GD_ASSERT(0);	// Unsupported
}

void PSPRenderer::DrawSphere( Float pRadius, UInt32 pPrecision )
{
    GD_ASSERT(0);	// Unsupported
}

void PSPRenderer::DrawBox( const Vector3f& pMin, const Vector3f& pMax )
{
	GD_ASSERT(0);	// Unsupported
}

void PSPRenderer::SetViewport(Int32 pX, Int32 pY, Int32 pWidth, Int32 pHeight)
{
    sceGuViewport(2048 + pX, 2048 + pY, pWidth, pHeight);

	mViewport[0] = pX;
	mViewport[1] = pY;
	mViewport[2] = pWidth;
	mViewport[3] = pHeight;
}

void PSPRenderer::GetViewport(Int32 pViewport[4])
{
    pViewport = mViewport;
}

void PSPRenderer::Perspective(Float pFovY, Float pAspectRatio,
                              Float pNearViewDistance, Float pFarViewDistance)
{
    sceGumPerspective(pFovY, pAspectRatio, pNearViewDistance, pFarViewDistance);
}

void PSPRenderer::SetView( const Vector3f& pPosition, const Vector3f& pDir, const Vector3f& pUp )
{
    sceGumLookAt( (ScePspFVector3*)&pDir,	(ScePspFVector3*)&pPosition, (ScePspFVector3*)&pUp );
}

void PSPRenderer::Begin2DProjection( Float pLeft, Float pRight, Float pBottom, Float pTop, Float pZNear, Float pZFar )
{
    // Disables Depth Testing
    SetRenderState( DepthTest, false );

    // Select the projection matrix, store it, and then reset it
    SetMatrixMode( ProjectionMatrix );
    PushMatrix();
    LoadIdentity();

    // Set up an ortho screen
    sceGumOrtho( pLeft, pRight, pBottom, pTop, pZNear, pZFar );

    // Select the model view matrix, store it, and then reset it
    SetMatrixMode( ModelViewMatrix );
    PushMatrix();
    LoadIdentity();
}

void PSPRenderer::End2DProjection()
{
    // Select the projection matrix, then restore it
    SetMatrixMode( ProjectionMatrix );
    PopMatrix();

    // Select the model view matrix, then restore it
    SetMatrixMode( ModelViewMatrix );
    PopMatrix();

    // #TODO: Check if depth testing was enabled/disabled before the 2D projection.
    SetRenderState( DepthTest, true );                  // Renables Depth Testing
}

void PSPRenderer::Translate( const Vector3f& pTranslation )
{
    sceGumTranslate( (const ScePspFVector3*)&pTranslation );
}

void PSPRenderer::Rotate( const Quaternionf& pRotation )
{
	Matrix4f mat;
    pRotation.ToMatrix( mat );
	MultMatrix( mat );
}

void PSPRenderer::Scale( const Vector3f& pScaling )
{
    sceGumScale( (const ScePspFVector3*)&pScaling );
}

void PSPRenderer::SetPointSize(Float pPointSize)
{
    GD_ASSERT(0);	// Unsupported
}

void PSPRenderer::SetPointMinSize(Float pMinPointSize)
{
    GD_ASSERT(0);	// Unsupported
}

void PSPRenderer::SetPointMaxSize(Float pMaxPointSize)
{
    GD_ASSERT(0);	// Unsupported
}

void PSPRenderer::SetPointAttenuation(Float pConstant, Float pLinear, Float pQuadratic)
{
    GD_ASSERT(0);	// Unsupported
}
