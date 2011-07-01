/**
 *  @file       OGLRenderer.cpp
 *  @brief      Gamedesk renderer for OpenGL.
 *  @author     Sébastien Lussier.
 *  @date       16/08/02.
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
#include "OGLGraphic.h"
#include "OGLRenderer.h"

#include "OGLTextureStage.h"
#include "OGLVertexBuffer.h"
#include "OGLIndexBuffer.h"

#include "Graphic/RenderTarget/RenderTarget.h"

#include "OGLTables.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(OGLRenderer);


OGLRenderer::OGLRenderer() :
    mCaps(OGLCaps::Instance()),
    mIndexBuffer(NULL),
    mCurrentVertexFormat(VertexFormat::None)
{
}

OGLRenderer::~OGLRenderer()
{
    Kill();
}

void OGLRenderer::Init( RenderWindow* pRenderWindow )
{
    if( mWasInitialized )
        return;

    Super::Init( pRenderWindow );
    
    GLenum err = glewInit();
    if( err != GLEW_OK )
        Core::MsgBox( "GLEW Initialisation Error", (const char*)glewGetErrorString(err) );

    if( !GLEW_ARB_vertex_buffer_object )
        Core::MsgBox( "Extension unsupported!", "Extension \"ARB_vertex_buffer_object\" is unsupported:\n%s", GetCardDesc().c_str() );

    mCaps.Init();
    mStateManager.Init();

    if( WGLEW_EXT_swap_control )
        wglSwapIntervalEXT(1);

    if( GL_EXT_texture_filter_anisotropic )
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &mMaxAnisotropy);

    // Texture stages          
    GLint texStageCount;
    glGetIntegerv( GL_MAX_TEXTURE_UNITS, &texStageCount );
    for( Int32 i = 0; i < texStageCount; i++ )
        mTextureStages.push_back( GD_NEW(OGLTextureStage, this, "OGLGraphic::OGLRenderer")(i) );

    // Point Sprite
    glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, 0.0f );

    glEnable(GL_POINT_SMOOTH) ;
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST) ;

    mQuadric = gluNewQuadric();
}

String OGLRenderer::GetCardDesc()
{
    String desc;

    const GLubyte* vendor = glGetString( GL_VENDOR );
    const GLubyte* renderer = glGetString( GL_RENDERER );
    const GLubyte* version = glGetString( GL_VERSION );

    desc += "\n    Vendor: ";
    desc += (char*)vendor;
    desc += "\n    Renderer: ";
    desc += (char*)renderer;
    desc += "\n    OpenGL version: ";
    desc += (char*)version;

    return desc;
}

void OGLRenderer::Kill()
{
    mStateManager.Kill();

    if( mQuadric )
		gluDeleteQuadric(mQuadric);
    
    Super::Kill();
}

void OGLRenderer::Clear( Bitfield pBufferFlags )
{
    GLbitfield mask = 0;
    
    if( pBufferFlags & ColorBuffer )        mask |= GL_COLOR_BUFFER_BIT;
    if( pBufferFlags & DepthBuffer )        mask |= GL_DEPTH_BUFFER_BIT;
    if( pBufferFlags & StencilBuffer )      mask |= GL_STENCIL_BUFFER_BIT;

    glClear( mask );
}

void OGLRenderer::BeginScene( PrimitiveType pPrimitiveType )
{
    glBegin( GDToGLPrimitiveType[pPrimitiveType] );
}

void OGLRenderer::EndScene()
{
    glEnd();
}

void OGLRenderer::SetVertexFormat( VertexFormat pVertexFormat )
{
    mCurrentVertexFormat = pVertexFormat;
    
    // GL_VERTEX_ARRAY
    if( mCurrentVertexFormat.HasComponent( VertexFormat::Position3 ) )
        mStateManager.EnableClientState( GL_VERTEX_ARRAY );
    else
        mStateManager.DisableClientState( GL_VERTEX_ARRAY );

    // GL_NORMAL_ARRAY
    if( mCurrentVertexFormat.HasComponent( VertexFormat::Normal3 ) )
        mStateManager.EnableClientState( GL_NORMAL_ARRAY );
    else
        mStateManager.DisableClientState( GL_NORMAL_ARRAY );
    
    // GL_COLOR_ARRAY
    if( mCurrentVertexFormat.HasComponent( VertexFormat::Color3 ) ||
        mCurrentVertexFormat.HasComponent( VertexFormat::Color4 ) )
        mStateManager.EnableClientState( GL_COLOR_ARRAY );
    else
        mStateManager.DisableClientState( GL_COLOR_ARRAY );
    
    // GL_TEXTURE_COORD_ARRAY
    glClientActiveTextureARB(GL_TEXTURE0_ARB);
    if( mCurrentVertexFormat.HasComponent( VertexFormat::TexCoord2 ) )
        mStateManager.EnableClientState( GL_TEXTURE_COORD_ARRAY );
    else
        mStateManager.DisableClientState( GL_TEXTURE_COORD_ARRAY );

    // GL_TEXTURE_COORD_ARRAY
    glClientActiveTextureARB(GL_TEXTURE1_ARB);
    if( mCurrentVertexFormat.HasComponent( VertexFormat::TexCoord2_2 ) )
        mStateManager.EnableClientState( GL_TEXTURE_COORD_ARRAY );
    else
        mStateManager.DisableClientState( GL_TEXTURE_COORD_ARRAY );

    // GL_EDGE_FLAG_ARRAY
    /*if( mCurrentVertexFormat.HasComponent( VertexFormat::EdgeFlag ) )
        mStateManager.EnableClientState( GL_EDGE_FLAG_ARRAY );
    else
        mStateManager.DisableClientState( GL_EDGE_FLAG_ARRAY );*/
}

void OGLRenderer::SetStreamSource( VertexFormat::Component pComponent, VertexBuffer* pVertexBuffer )
{
    GD_ASSERT( mCurrentVertexFormat.HasComponent( pComponent ) );      // #TODO: Throw an exception!
    
    if( pVertexBuffer->IsA( OGLHWVertexBuffer::StaticClass() ) )
    {
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, reinterpret_cast<OGLHWVertexBuffer*>(pVertexBuffer)->GetBufferID() );
    
        switch( pComponent )
        {
        case VertexFormat::Position3:
            glVertexPointer( 3, GL_FLOAT, 0, (Char*)NULL );
            mPositionBuffer = pVertexBuffer;
            break;

        case VertexFormat::Normal3:
            glNormalPointer( GL_FLOAT, 0, (Char*)NULL );
            break;

        case VertexFormat::Color3:
            glColorPointer( 3, GL_FLOAT, 0, (Char*)NULL );
            break;

        case VertexFormat::Color4:
            glColorPointer( 4, GL_FLOAT, 0, (Char*)NULL );
            break;

        case VertexFormat::TexCoord2:
            glClientActiveTextureARB(GL_TEXTURE0_ARB);
            glTexCoordPointer( 2, GL_FLOAT, 0, (Char*)NULL );
            break;

        case VertexFormat::TexCoord2_2:
            glClientActiveTextureARB(GL_TEXTURE1_ARB);
            glTexCoordPointer( 2, GL_FLOAT, 0, (Char*)NULL );
            break;
        }
    }
}

void OGLRenderer::SetIndices( IndexBuffer* pIndexBuffer )
{
    mIndexBuffer = pIndexBuffer;
    if( mIndexBuffer )
    {
        if( mIndexBuffer->IsA( OGLHWIndexBuffer::StaticClass() ) )
        {
            mStateManager.EnableClientState( GL_INDEX_ARRAY );
            glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, reinterpret_cast<OGLHWIndexBuffer*>(mIndexBuffer)->GetBufferID() );
        }
    }
    else
    {
        mStateManager.DisableClientState( GL_INDEX_ARRAY );
    }
}
    
void OGLRenderer::DrawPrimitive( PrimitiveType pType, UInt32 pStartVertex, UInt32 pVertexCount )
{
    mStateManager.DisableClientState( GL_INDEX_ARRAY );
    
    if( mRenderMode != GL_SELECT )
        glDrawArrays( GDToGLPrimitiveType[pType], pStartVertex, pVertexCount );
    else
        DrawWorkAround(pType, pStartVertex, pVertexCount, false);
}

void OGLRenderer::DrawIndexedPrimitive( PrimitiveType pType, UInt32 pStartIndex, UInt32 pIndexCount )
{
    GD_ASSERT( mIndexBuffer );      // #TODO: Throw an exception!
 
    if( mIndexBuffer->IsA( OGLHWIndexBuffer::StaticClass() ) )
    {
        mStateManager.EnableClientState( GL_INDEX_ARRAY );

        if( mRenderMode != GL_SELECT )
            glDrawElements( GDToGLPrimitiveType[pType], 
                        pIndexCount, 
                        mIndexBuffer->GetItemSize() == sizeof(UInt16) ? GL_UNSIGNED_SHORT : 
                            (mIndexBuffer->GetItemSize() == sizeof(UInt32) ?  GL_UNSIGNED_INT :
                                GL_UNSIGNED_BYTE), 
                        (void*)(pStartIndex*mIndexBuffer->GetItemSize()) );
        else
            DrawWorkAround(pType, pStartIndex, pIndexCount, true);
    }
}

void OGLRenderer::DrawWorkAround( PrimitiveType pType, UInt32 pStart, UInt32 pCount, Bool pIndexed )
{
    Vector3f* points = reinterpret_cast<Vector3f*>( mPositionBuffer->Lock( VertexBuffer::Lock_Read ) );
    UInt16*   indices = NULL;
    
    if( pIndexed && mIndexBuffer )
        indices = reinterpret_cast<UInt16*>(mIndexBuffer->Lock( IndexBuffer::Lock_Read ));

    glBegin( GDToGLPrimitiveType[pType] );

    if( pIndexed && mIndexBuffer )
    {
        for( UInt32 i = pStart; i < pStart + pCount; i++ )
            glVertex3fv( points[ indices[i] ] );
    }
    else
    {
        for( UInt32 i = pStart; i < pStart + pCount; i++ )
            glVertex3fv( points[i] );
    }

    glEnd();

    mPositionBuffer->Unlock();
    if( pIndexed && mIndexBuffer )
        mIndexBuffer->Unlock();
}

//RenderTarget*   CreateRenderTarget( RenderTarget::Type targetType, PixelFormat pf, INT iWidth, INT iHeight );
//void            SetRenderTarget( RenderTarget* pRenderTarget = NULL );

//INT             GetTextureStageCount();
//TextureStage*   GetTextureStage( INT iIndex );


void OGLRenderer::SetRenderState( RenderState pState, Bool pEnable, UInt32 pIndex )
{
    #define GL_ENABLE_DISABLE( glState )    pEnable ? mStateManager.Enable((glState)) : mStateManager.Disable((glState))

    switch( pState )
    {
    case LocalViewerLights:     glLightModelf( GL_LIGHT_MODEL_LOCAL_VIEWER, (pEnable ? 1.0f : 0.0f) );  break;
    case TwoSidedLightning:     glLightModelf( GL_LIGHT_MODEL_TWO_SIDE, (pEnable ? 1.0 : 0.0f) );       break;

    case ClipPlane_i:           GL_ENABLE_DISABLE( GL_CLIP_PLANE0+pIndex );                 break;
    case Light_i:               GL_ENABLE_DISABLE( GL_LIGHT0+pIndex );                      break;
    case DepthMask:             mStateManager.DepthMask( pEnable ? GL_TRUE : GL_FALSE );    break;

    default:                    GL_ENABLE_DISABLE( GDToGLRenderState[pState] ); break;
    }

    #undef GL_ENABLE_DISABLE
}


void OGLRenderer::SetAlphaFunc( PixelCompareFunc pFunc, Float pRef )
{
    mStateManager.AlphaFunc( GDToGLPixelCompareFunc[pFunc], pRef );
}


void OGLRenderer::SetBlendFunc( PixelBlendingFactor pSrcFactor, PixelBlendingFactor pDstFactor )
{
    mStateManager.BlendFunc( GDToGLPixelBlendingFactor[pSrcFactor], GDToGLPixelBlendingFactor[pDstFactor] );
}


void OGLRenderer::SetDepthFunc( PixelCompareFunc pFunc )
{
    mStateManager.DepthFunc( GDToGLPixelCompareFunc[pFunc] );
}


void OGLRenderer::SetStencilFunc( PixelCompareFunc pFunc, UInt32 pRef, UInt32 pMask )
{
    mStateManager.StencilFunc( GDToGLPixelCompareFunc[pFunc], pRef, pMask );
}


void OGLRenderer::SetStencilOperation( StencilOperation pOpFail, StencilOperation pOpZFail, StencilOperation pOpZPass )
{
    mStateManager.StencilOp( GDToGLStencilOperation[pOpFail], GDToGLStencilOperation[pOpZFail], GDToGLStencilOperation[pOpZPass] );
}


void OGLRenderer::SetClipPlane( UInt32 pClipPlaneIndex, const Plane3d& pPlane )
{
    GD_ASSERT( pClipPlaneIndex < mCaps.GetMaxClipPlanes() );
    GLenum plane = GL_CLIP_PLANE0+pClipPlaneIndex;
    
    glClipPlane( plane, (double*)const_cast<Plane3d&>(pPlane) );
}


void OGLRenderer::SetCulling( CullingMode pMode )
{
    if( pMode == NoCulling )
    {
        SetRenderState( CullFace, false );
    }
    else
    {
        SetRenderState( CullFace, true );
        mStateManager.CullFace( GDToGLCullingMode[pMode] );
    }
}

void OGLRenderer::SetFogMode( FogMode pMode )
{
    glFogf( GL_FOG_MODE, GDToGLFogMode[pMode] );
}

void OGLRenderer::SetFogDensity( Float pDensity )
{
    GD_ASSERT( pDensity >= 0.0f );
    glFogf( GL_FOG_DENSITY, pDensity );
}

void OGLRenderer::SetFogRange( Float pStart, Float pEnd )
{
    glFogf( GL_FOG_START, pStart );
    glFogf( GL_FOG_END, pEnd );
}

void OGLRenderer::SetFogColor( const Color4f& pColor )
{
    glFogfv( GL_FOG_COLOR, (Float*)const_cast<Color4f&>(pColor) );
}

void OGLRenderer::SetAmbiantColor( const Color4f& pColor )
{
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, (Float*)const_cast<Color4f&>(pColor) );
}


void OGLRenderer::SetMaterial( const Material& pMaterial )
{
    glMaterialfv( GL_FRONT, GL_AMBIENT, pMaterial.mAmbient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, pMaterial.mDiffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, pMaterial.mSpecular );
    glMaterialfv( GL_FRONT, GL_EMISSION, pMaterial.mEmissive );
    glMaterialfv( GL_FRONT, GL_SHININESS, &pMaterial.mShininess );
}

void OGLRenderer::SetPolygonMode( PolygonFace pFace, FillMode pFillMode )
{
    mStateManager.PolygonMode( GDToGLPolygonFace[pFace], GDToGLFillMode[pFillMode] );
}

Int32 OGLRenderer::SetRenderMode(RenderMode pMode)
{
    mRenderMode = GDToGLRenderMode[pMode];
    return mStateManager.RenderMode( mRenderMode );
}

void OGLRenderer::SetClearColor(const Color4f& pColor)
{
    glClearColor(pColor.R, pColor.G, pColor.B, pColor.A);
}

void OGLRenderer::SetShadeModel(ShadeModel pModel)
{
    mStateManager.ShadeModel( GDToGLShadeModel[pModel] );
}

void OGLRenderer::SetColorMask( Bool pRed, Bool pGreen, Bool pBlue, Bool pAlpha )
{
    glColorMask( pRed, pGreen, pBlue, pAlpha );
}

void OGLRenderer::SetMatrixMode(MatrixMode pMode)
{
    mStateManager.MatrixMode( GDToGLMatrixMode[pMode] );
}

void OGLRenderer::LoadIdentity()
{
    glLoadIdentity();
}

void OGLRenderer::MultMatrix(const Matrix4f& pMatrix)
{
    glMultMatrixf(pMatrix);
}

void OGLRenderer::PushMatrix()
{
    glPushMatrix();
}

void OGLRenderer::PopMatrix()
{
    glPopMatrix();
}

void OGLRenderer::SetPolygonOffset(Float pFactor, Float pUnit)
{
	glPolygonOffset(pFactor, pUnit);
}

void OGLRenderer::SetLineWidth(Float pWidth)
{
	glLineWidth(pWidth);
}

void OGLRenderer::SetLineStipple(Byte pRepeat, UInt16 pPattern)
{
    glLineStipple(pRepeat, pPattern);
}

Vector3f OGLRenderer::ScreenToWorld( const Vector3f& pScreen )
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

Vector3f OGLRenderer::ScreenToWorld( const Vector2f& pScreen )
{
    Float depth;

    GetDepth(pScreen.x, pScreen.y, depth);
    
    return ScreenToWorld( Vector3f(pScreen.x, pScreen.y, depth) );
}

Vector3f OGLRenderer::WorldToScreen( const Vector3f& pWorld )
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

void OGLRenderer::GetModelViewMatrix( Matrix4d& pMatrix )
{
    glGetDoublev(GL_MODELVIEW_MATRIX, pMatrix);
}

void OGLRenderer::GetModelViewMatrix( Matrix4f& pMatrix )
{
    glGetFloatv(GL_MODELVIEW_MATRIX, pMatrix);
}

void OGLRenderer::GetProjectionMatrix( Matrix4d& pMatrix )
{
    glGetDoublev(GL_PROJECTION_MATRIX, pMatrix);
}

void OGLRenderer::GetProjectionMatrix( Matrix4f& pMatrix )
{
    glGetFloatv(GL_PROJECTION_MATRIX, pMatrix);
}

void OGLRenderer::GetDepth(Float pX, Float pY, Float& pZ)
{
    glReadPixels(pX, pY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pZ);
}

void OGLRenderer::UnProject(Double pWinX, Double pWinY, Double pWinZ,
                            Matrix4d& pModelViewMatrix, Matrix4d& pProjectionMatrix,
                            Int32 pViewport[4],
                            Double& pObjX, Double& pObjY, Double& pObjZ)
{
    gluUnProject(pWinX, pWinY, pWinZ, 
                 pModelViewMatrix, pProjectionMatrix, (GLint*)pViewport,
                 &pObjX, &pObjY, &pObjZ);
}

void OGLRenderer::Project(Double pObjX, Double pObjY, Double pObjZ,
                          Matrix4d& pModelViewMatrix, Matrix4d& pProjectionMatrix,
                          Int32 pViewport[4],
                          Double& pWinX, Double& pWinY, Double& pWinZ)
{
    gluProject(pObjX, pObjY, pObjZ, 
               pModelViewMatrix, pProjectionMatrix, (GLint*)pViewport,
               &pWinX, &pWinY, &pWinZ);
}


void OGLRenderer::SetSelectionBuffer(Int32 pBufferSize, UInt32* pBuffer)
{
	glSelectBuffer(pBufferSize, (GLuint*)pBuffer);
}

void OGLRenderer::InitNames()
{
	glInitNames();
}

void OGLRenderer::PushName(UInt32 pName)
{
	glPushName(pName);
}

void OGLRenderer::PopName()
{
	glPopName();
}

void OGLRenderer::LoadName(UInt32 pName)
{
	glLoadName(pName);
}

void OGLRenderer::PickMatrix(Double pX, Double pY, Double pDelX, Double pDelY, 
					        Int32 pViewport[4])
{
	gluPickMatrix(pX, pY, pDelX, pDelY, (GLint*)pViewport);
}

void OGLRenderer::SetColor(const Color3f& pColor)                       { glColor3fv(pColor.RGB); }
void OGLRenderer::SetColor(Float pR, Float pG, Float pB)                { glColor3f(pR, pG, pB); }
void OGLRenderer::SetColor(const Color4f& pColor)                       { glColor4fv(pColor.RGBA); }
void OGLRenderer::SetColor(Float pR, Float pG, Float pB, Float pA)      { glColor4f(pR, pG, pB, pA); }
void OGLRenderer::SetNormal(const Vector3f& pVector)                    { glNormal3fv(pVector.elem); }
void OGLRenderer::SetNormal(Float pX, Float pY, Float pZ)               { glNormal3f(pX, pY, pZ); }
void OGLRenderer::SetVertex(Int32 pX, Int32 pY)                         { glVertex2i(pX, pY); }
void OGLRenderer::SetVertex(Float pX, Float pY)                         { glVertex2f(pX, pY); }
void OGLRenderer::SetVertex(const Vector2i& pVector)                    { glVertex2iv((GLint*)pVector.elem); }
void OGLRenderer::SetVertex(const Vector2f& pVector)                    { glVertex2fv(pVector.elem); }
void OGLRenderer::SetVertex(Int32 pX, Int32 pY, Int32 pZ)               { glVertex3i(pX, pY, pZ); }
void OGLRenderer::SetVertex(Float pX, Float pY, Float pZ)               { glVertex3f(pX, pY, pZ); }
void OGLRenderer::SetVertex(const Vector3i& pVector)                    { glVertex3iv((GLint*)pVector.elem); }
void OGLRenderer::SetVertex(const Vector3f& pVector)                    { glVertex3fv(pVector.elem); }
void OGLRenderer::SetUV( Float pX, Float pY )                           { glTexCoord2f( pX, pY ); }
void OGLRenderer::SetUV( Float pX, Float pY, Float pZ )                 { glTexCoord3f( pX, pY, pZ ); }
void OGLRenderer::SetUV( const Vector2f& pUV )                          { glTexCoord2fv( pUV.elem ); }
void OGLRenderer::SetUV( const Vector3f& pUV )                          { glTexCoord3fv( pUV.elem ); }
void OGLRenderer::SetUV( UInt32 pStage, Float pX, Float pY )            { glMultiTexCoord2fARB( GL_TEXTURE0_ARB+pStage, pX, pY ); } 
void OGLRenderer::SetUV( UInt32 pStage, Float pX, Float pY, Float pZ )  { glMultiTexCoord3fARB( GL_TEXTURE0_ARB+pStage, pX, pY, pZ ); }
void OGLRenderer::SetUV( UInt32 pStage, const Vector2f& pUV )           { glMultiTexCoord2fvARB( GL_TEXTURE0_ARB+pStage, pUV.elem ); }
void OGLRenderer::SetUV( UInt32 pStage, const Vector3f& pUV )           { glMultiTexCoord3fvARB( GL_TEXTURE0_ARB+pStage, pUV.elem ); }

void OGLRenderer::SetLight( UInt32 pLightIndex, const Light& pLight )
{
    Float pos[4];

    GD_ASSERT( pLightIndex < mCaps.GetMaxLights() );
    GLenum lightIndex = GL_LIGHT0+pLightIndex;

    glLightfv( lightIndex, GL_AMBIENT, pLight.mAmbient );
    glLightfv( lightIndex, GL_DIFFUSE, pLight.mDiffuse );
    glLightfv( lightIndex, GL_SPECULAR, pLight.mSpecular );

    memcpy( pos, &(pLight.mPosition), sizeof(Vector3f) );
    if( pLight.mType == LightPoint )
        pos[3] = 1.0f;
    else
        pos[3] = 0.0f;
    glLightfv( lightIndex, GL_POSITION, pos );

    if( pLight.mType == LightSpot )
    {
        glLightfv( lightIndex, GL_SPOT_DIRECTION, pLight.mSpotDirection );
        glLightf( lightIndex, GL_SPOT_EXPONENT, pLight.mSpotExponent );
        glLightf( lightIndex, GL_SPOT_CUTOFF, pLight.mSpotCutoff );
    }
    else
    {
        glLighti( lightIndex, GL_SPOT_CUTOFF, 180 );
    }

    glLightf( lightIndex, GL_CONSTANT_ATTENUATION, pLight.mAttenuationConstant );
    glLightf( lightIndex, GL_LINEAR_ATTENUATION, pLight.mAttenuationLinear );
    glLightf( lightIndex, GL_QUADRATIC_ATTENUATION, pLight.mAttenuationQuadratic );
}

Light OGLRenderer::GetLight( UInt32 pLightIndex ) const
{
    Light light;
    Float pos[4];

    GD_ASSERT( pLightIndex < mCaps.GetMaxLights() );
    GLenum lightIndex = GL_LIGHT0+pLightIndex;

    glGetLightfv( lightIndex, GL_AMBIENT, light.mAmbient );
    glGetLightfv( lightIndex, GL_DIFFUSE, light.mDiffuse );
    glGetLightfv( lightIndex, GL_SPECULAR, light.mSpecular );
    glGetLightfv( lightIndex, GL_POSITION, pos );
    glGetLightfv( lightIndex, GL_SPOT_DIRECTION, light.mSpotDirection );
    glGetLightfv( lightIndex, GL_SPOT_EXPONENT, &light.mSpotExponent );
    glGetLightfv( lightIndex, GL_SPOT_CUTOFF, &light.mSpotCutoff );
    glGetLightfv( lightIndex, GL_CONSTANT_ATTENUATION, &light.mAttenuationConstant );
    glGetLightfv( lightIndex, GL_LINEAR_ATTENUATION, &light.mAttenuationLinear );
    glGetLightfv( lightIndex, GL_QUADRATIC_ATTENUATION, &light.mAttenuationQuadratic );

    memcpy( light.mPosition, pos, sizeof(Vector3f) );

    if( light.mSpotCutoff == 180 )
        light.mType = LightSpot;
    else if( pos[3] == 1.0f )
        light.mType = LightPoint;
    else
        light.mType = LightDirectional;

    return light;
}

void OGLRenderer::DrawLine( Float pStartX, Float pStartY, Float pEndX, Float pEndY )
{
    glBegin(GL_LINES);
        glVertex2f( pStartX, pStartY );
        glVertex2f( pEndX, pEndY );
    glEnd();
}

void OGLRenderer::Draw2DTile( Float pX, Float pY, Float pW, Float pH, Float pUStart, Float pVStart, Float pUEnd, Float pVEnd )
{
    glBegin(GL_QUADS);
        glTexCoord2f( pUEnd, pVStart );
        glVertex2f(pX,pY);                      // Vertex Coord (Bottom Left)

        glTexCoord2f( pUStart, pVStart );
        glVertex2f(pX+pW,pY);                   // Vertex Coord (Bottom Right)

        glTexCoord2f( pUStart, pVEnd );
        glVertex2f(pX+pW,pY+pH);                // Vertex Coord (Top Right)

        glTexCoord2f( pUEnd, pVEnd );
        glVertex2f(pX,pY+pH);                   // Vertex Coord (Top Left)
    glEnd();
}

void OGLRenderer::DrawQuad( const Vector3f& pV0, const Vector3f& pV1, const Vector3f& pV2, const Vector3f& pV3, Bool pTextured )
{
    if( pTextured )
    {
        glBegin(GL_QUADS);
            glTexCoord2i( 1, 0 );
            glVertex3fv(pV0);
            glTexCoord2i( 0, 0 );
            glVertex3fv(pV1);
            glTexCoord2i( 0, 1 );
            glVertex3fv(pV2);
            glTexCoord2i( 1, 1 );
            glVertex3fv(pV3);
        glEnd();
    }
    else
    {
        glBegin(GL_QUADS);
            glVertex3fv(pV0);
            glVertex3fv(pV1);
            glVertex3fv(pV2);
            glVertex3fv(pV3);
        glEnd();
    }
}

void OGLRenderer::DrawSphere( Float pRadius, UInt32 pPrecision )
{
    gluSphere( mQuadric, pRadius, pPrecision, pPrecision );
}

struct Vertex_UV_T3_N3_V3f
{
    Vector2f    uv;
    Vector3f    tangent;
    Vector3f    normal;
    Vector3f    pos;
};

void OGLRenderer::DrawBox( const Vector3f& pMin, const Vector3f& pMax )
{

    #define SetVertices( i, i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10 ) \
            cubeVertices[i].uv      = Vector2f(i0, i1);          \
            cubeVertices[i].tangent = Vector3f(i2, i3, i4);      \
            cubeVertices[i].normal  = Vector3f(i5, i6, i7);      \
            cubeVertices[i].pos     = Vector3f(i8, i9, i10);     
            
    Vertex_UV_T3_N3_V3f cubeVertices[6*4];
        
                   //   UV        Tangent     Normal      Position

    // Top
    SetVertices( 0,     0, 1,     1, 0, 0,    0, 1, 0,    pMin.x, pMax.y, pMin.z );
	SetVertices( 1,     0, 0,     1, 0, 0,    0, 1, 0,    pMin.x, pMax.y, pMax.z );
    SetVertices( 2,     1, 0,     1, 0, 0,    0, 1, 0,    pMax.x, pMax.y, pMax.z );   
    SetVertices( 3,     1, 1,     1, 0, 0,    0, 1, 0,    pMax.x, pMax.y, pMin.z );

    // Bottom
	SetVertices( 4,     0, 1,    -1, 0, 0,    0,-1, 0,    pMax.x, pMin.y, pMin.z );
    SetVertices( 5,     0, 0,    -1, 0, 0,    0,-1, 0,    pMax.x, pMin.y, pMax.z );
    SetVertices( 6,     1, 0,    -1, 0, 0,    0,-1, 0,    pMin.x, pMin.y, pMax.z );
    SetVertices( 7,     1, 1,    -1, 0, 0,    0,-1, 0,    pMin.x, pMin.y, pMin.z );	

    // Left
	SetVertices( 8,     0, 0,     0, 0, 1,   -1, 0, 0,    pMin.x, pMin.y, pMin.z );
    SetVertices( 9,     1, 0,     0, 0, 1,   -1, 0, 0,    pMin.x, pMin.y, pMax.z );
    SetVertices(10,     1, 1,     0, 0, 1,   -1, 0, 0,    pMin.x, pMax.y, pMax.z );
    SetVertices(11,     0, 1,     0, 0, 1,   -1, 0, 0,    pMin.x, pMax.y, pMin.z );
    
    // Right
	SetVertices(12,     1, 1,     0, 0,-1,    1, 0, 0,    pMax.x, pMax.y, pMin.z );
    SetVertices(13,     0, 1,     0, 0,-1,    1, 0, 0,    pMax.x, pMax.y, pMax.z );
    SetVertices(14,     0, 0,     0, 0,-1,    1, 0, 0,    pMax.x, pMin.y, pMax.z );
    SetVertices(15,     1, 0,     0, 0,-1,    1, 0, 0,    pMax.x, pMin.y, pMin.z );

    // Front
	SetVertices(16,     1, 0,    -1, 0, 0,    0, 0,-1,    pMin.x, pMin.y, pMin.z );
	SetVertices(17,     1, 1,    -1, 0, 0,    0, 0,-1,    pMin.x, pMax.y, pMin.z );
    SetVertices(18,     0, 1,    -1, 0, 0,    0, 0,-1,    pMax.x, pMax.y, pMin.z );
    SetVertices(19,     0, 0,    -1, 0, 0,    0, 0,-1,    pMax.x, pMin.y, pMin.z );

    // Back
	SetVertices(20,     1, 0,     1, 0, 0,    0, 0, 1,    pMax.x, pMin.y, pMax.z );
	SetVertices(21,     1, 1,     1, 0, 0,    0, 0, 1,    pMax.x, pMax.y, pMax.z );
    SetVertices(22,     0, 1,     1, 0, 0,    0, 0, 1,    pMin.x, pMax.y, pMax.z );
    SetVertices(23,     0, 0,     1, 0, 0,    0, 0, 1,    pMin.x, pMin.y, pMax.z );

    glBegin(GL_QUADS);
    for( UInt32 i = 0; i < 24; i++ )
    {
        glMultiTexCoord2fvARB( GL_TEXTURE0_ARB+0, cubeVertices[i].uv );
        glMultiTexCoord4fvARB( GL_TEXTURE0_ARB+1, cubeVertices[i].tangent );
        glNormal3fv( cubeVertices[i].normal );
        glVertex3fv( cubeVertices[i].pos );
    }
    glEnd();
}

void OGLRenderer::SetViewport(Int32 pX, Int32 pY, Int32 pWidth, Int32 pHeight)
{
    glViewport(pX, pY, pWidth, pHeight);
}

void OGLRenderer::GetViewport(Int32 pViewport[4])
{
    glGetIntegerv(GL_VIEWPORT, (GLint*)pViewport);
}

void OGLRenderer::Perspective(Float pFovY, Float pAspectRatio,
                              Float pNearViewDistance, Float pFarViewDistance)
{
    gluPerspective(pFovY, pAspectRatio, pNearViewDistance, pFarViewDistance);
}

void OGLRenderer::SetView( const Vector3f& pPosition, const Vector3f& pDir, const Vector3f& pUp )
{
    Super::SetView( pPosition, pDir, pUp );

    gluLookAt( pPosition.x,          pPosition.y,          pPosition.z,
               pPosition.x + pDir.x, pPosition.y + pDir.y, pPosition.z + pDir.z, 
               pUp.x,                pUp.y,                pUp.z );
}

void OGLRenderer::Begin2DProjection( Float pLeft, Float pRight, Float pBottom, Float pTop, Float pZNear, Float pZFar )
{
    // Disables Depth Testing
    SetRenderState( DepthTest, false );

    // Select the projection matrix, store it, and then reset it
    SetMatrixMode( ProjectionMatrix );
    glPushMatrix();
    glLoadIdentity();

    // Set up an ortho screen
    glOrtho( pLeft, pRight, pBottom, pTop, pZNear, pZFar );

    // Select the model view matrix, store it, and then reset it
    SetMatrixMode( ModelViewMatrix );
    glPushMatrix();
    glLoadIdentity();
}

void OGLRenderer::End2DProjection()
{
    // Select the projection matrix, then restore it
    SetMatrixMode( ProjectionMatrix );
    glPopMatrix();

    // Select the model view matrix, then restore it
    SetMatrixMode( ModelViewMatrix );
    glPopMatrix();

    // #TODO: Check if depth testing was enabled/disabled before the 2D projection.
    SetRenderState( DepthTest, true );                  // Renables Depth Testing
}

void OGLRenderer::Translate( const Vector3f& pTranslation )
{
    glTranslatef( pTranslation.x, pTranslation.y, pTranslation.z );
}

void OGLRenderer::Rotate( const Quaternionf& pRotation )
{
    Matrix4f mat;
    pRotation.ToMatrix( mat );
    glMultMatrixf( mat );
}

void OGLRenderer::Scale( const Vector3f& pScaling ) 
{
    glScalef( pScaling.x, pScaling.y, pScaling.z );
}

void OGLRenderer::SetPointSize(Float pPointSize) 
{
    glPointSize( pPointSize );
}

void OGLRenderer::SetPointMinSize(Float pMinPointSize)
{
    glPointParameterf( GL_POINT_SIZE_MIN, pMinPointSize );
}

void OGLRenderer::SetPointMaxSize(Float pMaxPointSize)
{
    glPointParameterf( GL_POINT_SIZE_MAX, pMaxPointSize );
}

void OGLRenderer::SetPointAttenuation(Float pConstant, Float pLinear, Float pQuadratic)
{
    Float quadratic[] =  { pConstant, pLinear, pQuadratic };
    glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );
}


} // namespace Gamedesk
