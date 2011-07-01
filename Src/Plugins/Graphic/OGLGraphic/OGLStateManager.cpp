/**
 *  @file       OGLStateManager.cpp
 *  @brief	    Manage OpenGL state and minimize call to the OpenGL API.
 *  @author     Sébastien Lussier.
 *  @date       16/01/04.
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
#include "OGLStateManager.h"

#include "OGLCaps.h"


//#define DISABLE_STATE_CHECK


#ifdef GD_DEBUG
    #define KEEP_STATS  \
                else \
                { \
                    mRedundantCount++; \
                } \
                mCallCount++;
#else
    #define KEEP_STATS
#endif


namespace Gamedesk {
	
	
INLINE void OGLStateManager::Init()
{
#ifdef GD_DEBUG
    mCallCount = 0;
    mRedundantCount = 0;
#endif
    SyncWithGL();
}

INLINE void OGLStateManager::Kill()
{
#ifdef GD_DEBUG
    if( mCallCount != 0 )
        Core::DebugOut( "OGLStateManager: # of calls: %d, # of redundant calls: %d (%f%%)\n", mCallCount, mRedundantCount, 100*((float)mRedundantCount / (float)mCallCount) );
#endif
}

INLINE void OGLStateManager::SyncWithGL()
{    
    SyncEnableStates();

    GLboolean   booleanVal;
    GLint       intVal;
    GLfloat     floatVal;
    
    #define GetGLBoolean( x, y ) \
                glGetBooleanv( (x), &booleanVal ); \
                (y) = booleanVal;

    #define GetGLInteger( x, y ) \
                glGetIntegerv( (x), &intVal ); \
                (y) = intVal;

    #define GetGLFloat( x, y ) \
                glGetFloatv( (x), &floatVal ); \
                (y) = floatVal;

    // glAlphaFunc
    GetGLInteger    ( GL_ALPHA_TEST_FUNC, mAlphaFunc );
    GetGLFloat      ( GL_ALPHA_TEST_REF, mAlphaRef );
    
    // glBlendFunc
    GetGLInteger    ( GL_BLEND_SRC, mBlendSrcFactor );
    GetGLInteger    ( GL_BLEND_DST, mBlendDstFactor );
    
    // glDepthFunc
    GetGLInteger    ( GL_DEPTH_FUNC, mDepthFunc );

    // glStencilFunc
    GetGLInteger    ( GL_STENCIL_FUNC, mStencilFunc );
    GetGLInteger    ( GL_STENCIL_REF,  mStencilFuncRef );
    GetGLInteger    ( GL_STENCIL_VALUE_MASK, mStencilValueMask );

    // glStencilMask
    GetGLInteger    ( GL_STENCIL_WRITEMASK, mStencilWriteMask );

    // glStencilOp
    GetGLInteger    ( GL_STENCIL_FAIL, mStencilFailOp );
    GetGLInteger    ( GL_STENCIL_PASS_DEPTH_FAIL, mStencilZFailOp );
    GetGLInteger    ( GL_STENCIL_PASS_DEPTH_PASS, mStencilZPassOp );
    
    // glDepthMask
    GetGLBoolean    ( GL_DEPTH_WRITEMASK, mDepthMask );
    
    // glEdgeFlag
    GetGLBoolean    ( GL_EDGE_FLAG, mEdgeFlag );
    
    // glFrontFace
    GetGLInteger    ( GL_FRONT_FACE, mFrontFaceMode );
    
    // glCullFace
    GetGLInteger    ( GL_CULL_FACE, mCullFaceMode );
    
    // glLogicOp
    GetGLInteger    ( GL_LOGIC_OP_MODE, mLogicOpMode );
    
    // glMatrixMode
    GetGLInteger    ( GL_MATRIX_MODE, mMatrixMode );

    // glRenderMode
    GetGLInteger    ( GL_RENDER_MODE, mRenderMode );

    // glShadeMode
    GetGLInteger    ( GL_SHADE_MODEL, mShadeMode );

    // glColorMask
    GLboolean   colorMaskVals[4];
    glGetBooleanv( GL_COLOR_WRITEMASK, colorMaskVals );
    mColorMaskRed   = colorMaskVals[0];
    mColorMaskGreen = colorMaskVals[1];
    mColorMaskBlue  = colorMaskVals[2];
    mColorMaskAlpha = colorMaskVals[3];

    // glPolygonMode
    GLint       faceModeVals[2];
    glGetIntegerv( GL_POLYGON_MODE, faceModeVals );
    mPolygonFrontFaceMode = faceModeVals[0];
    mPolygonBackFaceMode  = faceModeVals[1];

    // glEnableClientState, glDisableClientState
    for( UInt32 i = 0; i < GL_EDGE_FLAG_ARRAY-GL_VERTEX_ARRAY+1; i++ )
        mClientStateEnable[i] = GL_FALSE;

    #undef GetGLBoolean
    #undef GetGLInteger
    #undef GetGLFloat
}

void OGLStateManager::SyncEnableStates()
{
    #define SetEnableMap( x ) \
        mEnableMap[(x)] = glIsEnabled( (x) );

    SetEnableMap( GL_ALPHA_TEST );
    SetEnableMap( GL_AUTO_NORMAL );
    SetEnableMap( GL_BLEND );
    
    for( UInt32 i = 0; i < OGLCaps::Instance().GetMaxClipPlanes(); i++ )
        SetEnableMap( GL_CLIP_PLANE0+i );

    SetEnableMap( GL_COLOR_LOGIC_OP );
    SetEnableMap( GL_COLOR_MATERIAL );
    SetEnableMap( GL_CULL_FACE );
    SetEnableMap( GL_DEPTH_TEST );
    SetEnableMap( GL_DITHER );
    SetEnableMap( GL_FOG );    
    SetEnableMap( GL_INDEX_LOGIC_OP );
    
    for( UInt32 i = 0; i < OGLCaps::Instance().GetMaxLights(); i++ )
        SetEnableMap( GL_LIGHT0+i );
    
    SetEnableMap( GL_LIGHTING );
    SetEnableMap( GL_LINE_SMOOTH );
    SetEnableMap( GL_LINE_STIPPLE );
    SetEnableMap( GL_MAP1_COLOR_4 );
    SetEnableMap( GL_MAP1_INDEX );
    SetEnableMap( GL_MAP1_NORMAL );
    SetEnableMap( GL_MAP1_TEXTURE_COORD_1 );
    SetEnableMap( GL_MAP1_TEXTURE_COORD_2 );
    SetEnableMap( GL_MAP1_TEXTURE_COORD_3 );
    SetEnableMap( GL_MAP1_TEXTURE_COORD_4 );
    SetEnableMap( GL_MAP1_VERTEX_3 );
    SetEnableMap( GL_MAP1_VERTEX_4 );
    SetEnableMap( GL_MAP2_COLOR_4 );
    SetEnableMap( GL_MAP2_INDEX );
    SetEnableMap( GL_MAP2_NORMAL );
    SetEnableMap( GL_MAP2_TEXTURE_COORD_1 );
    SetEnableMap( GL_MAP2_TEXTURE_COORD_2 );
    SetEnableMap( GL_MAP2_TEXTURE_COORD_3 );
    SetEnableMap( GL_MAP2_TEXTURE_COORD_4 );
    SetEnableMap( GL_MAP2_VERTEX_3 );
    SetEnableMap( GL_MAP2_VERTEX_4 );
    SetEnableMap( GL_NORMALIZE );
    SetEnableMap( GL_POINT_SMOOTH );
    SetEnableMap( GL_POLYGON_OFFSET_FILL );
    SetEnableMap( GL_POLYGON_OFFSET_LINE );
    SetEnableMap( GL_POLYGON_OFFSET_POINT );
    SetEnableMap( GL_POLYGON_SMOOTH );
    SetEnableMap( GL_POLYGON_STIPPLE );
    SetEnableMap( GL_SCISSOR_TEST );
    SetEnableMap( GL_STENCIL_TEST );
    SetEnableMap( GL_TEXTURE_1D );
    SetEnableMap( GL_TEXTURE_2D );
    SetEnableMap( GL_TEXTURE_GEN_Q );
    SetEnableMap( GL_TEXTURE_GEN_R );
    SetEnableMap( GL_TEXTURE_GEN_S );
    SetEnableMap( GL_TEXTURE_GEN_T );

    // OpenGL 1.1
    SetEnableMap( GL_COLOR_LOGIC_OP );
    SetEnableMap( GL_COLOR_ARRAY );
    SetEnableMap( GL_EDGE_FLAG_ARRAY );
    SetEnableMap( GL_INDEX_ARRAY );
    SetEnableMap( GL_INDEX_LOGIC_OP );
    SetEnableMap( GL_NORMAL_ARRAY );
    SetEnableMap( GL_POLYGON_OFFSET_FILL );
    SetEnableMap( GL_POLYGON_OFFSET_LINE );
    SetEnableMap( GL_POLYGON_OFFSET_POINT );
    SetEnableMap( GL_TEXTURE_COORD_ARRAY );
    SetEnableMap( GL_VERTEX_ARRAY );

    // Extensions
    //SetEnableMap( GL_COLOR_TABLE_EXT );
    //SetEnableMap( GL_CONVOLUTION_1D_EXT );
    //SetEnableMap( GL_CONVOLUTION_2D_EXT );
    //SetEnableMap( GL_FOG_OFFSET_EXT );
    //SetEnableMap( GL_INTERLACE_EXT );
    //SetEnableMap( GL_INTERLACE_READ_I3D );
    //SetEnableMap( GL_MULTISAMPLE_EXT );
    //SetEnableMap( GL_OCCLUSION_TEST_EXT );
    //SetEnableMap( GL_PIXEL_TEX_GEN_EXT );
    //SetEnableMap( GL_POST_COLOR_MATRIX_COLOR_TABLE_EXT );
    //SetEnableMap( GL_POST_CONVOLUTION_COLOR_TABLE_EXT );
    //SetEnableMap( GL_RESCALE_NORMAL_EXT );
    //SetEnableMap( GL_SAMPLE_ALPHA_TO_MASK_EXT );
    //SetEnableMap( GL_SAMPLE_ALPHA_TO_ONE_EXT );
    //SetEnableMap( GL_SAMPLE_MASK_EXT );
    //SetEnableMap( GL_SEPARABLE_2D_EX );
    //SetEnableMap( GL_TEXTURE_3D_EXT );
    //SetEnableMap( GL_TEXTURE_COLOR_TABLE_EXT );

    #undef SetEnableMap    
}

INLINE void OGLStateManager::Enable( GLenum cap )
{
#ifndef DISABLE_STATE_CHECK
    if( !mEnableMap[cap] )
    {
#endif
        glEnable( cap );
        mEnableMap[cap] = GL_TRUE;
#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::Disable( GLenum cap )
{
#ifndef DISABLE_STATE_CHECK
    if( mEnableMap[cap] )
    {
#endif
        glDisable( cap );
        mEnableMap[cap] = GL_FALSE;
#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE GLboolean OGLStateManager::IsEnabled(GLenum cap)
{
    return mEnableMap[cap];
}

INLINE void OGLStateManager::EnableClientState( GLenum cap )
{
    glEnableClientState( cap );
}

INLINE void OGLStateManager::DisableClientState( GLenum cap )
{
    glDisableClientState( cap );
}

INLINE void OGLStateManager::AlphaFunc( GLenum func, GLclampf ref )
{
#ifndef DISABLE_STATE_CHECK
    if( func != mAlphaFunc || ref != mAlphaRef )
    {
#endif

        glAlphaFunc( func, ref );
        mAlphaFunc = func;
        mAlphaRef  = ref;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::BlendFunc( GLenum sfactor, GLenum dfactor )
{
#ifndef DISABLE_STATE_CHECK
    if( sfactor != mBlendSrcFactor || dfactor != mBlendDstFactor )
    {
#endif

        glBlendFunc( sfactor, dfactor );
        mBlendSrcFactor = sfactor;
        mBlendDstFactor = dfactor;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::DepthFunc( GLenum func )
{
#ifndef DISABLE_STATE_CHECK
    if( func != mDepthFunc )
    {
#endif
        
        glDepthFunc( func );
        mDepthFunc = func;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::StencilFunc( GLenum func, GLint ref, GLuint mask )
{
#ifndef DISABLE_STATE_CHECK
    if( func != mStencilFunc || ref != mStencilFuncRef || mask != mStencilValueMask )
    {
#endif

        glStencilFunc( func, ref, mask );
        mStencilFunc      = func;
        mStencilFuncRef   = ref;
        mStencilValueMask = mask;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::StencilMask( GLuint mask )
{
#ifndef DISABLE_STATE_CHECK
    if( mStencilWriteMask != mask )
    {
#endif

        glStencilMask( mask );
        mStencilWriteMask = mask;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::StencilOp( GLenum fail, GLenum zfail, GLenum zpass )
{
#ifndef DISABLE_STATE_CHECK
    if( fail != mStencilFailOp || zfail != mStencilZFailOp || zpass != mStencilZPassOp )
    {
#endif

        glStencilOp( fail, zfail, zpass );
        mStencilFailOp = fail;
        mStencilZFailOp = zfail;
        mStencilZPassOp = zpass;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::ColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha )
{
#ifndef DISABLE_STATE_CHECK
    if( red != mColorMaskRed || green != mColorMaskGreen || blue != mColorMaskBlue || alpha != mColorMaskAlpha )
    {
#endif

        glColorMask( red, green, blue, alpha );
        mColorMaskRed = red;
        mColorMaskGreen = green;
        mColorMaskBlue = blue;
        mColorMaskAlpha = alpha;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::DepthMask( GLboolean flag )
{
#ifndef DISABLE_STATE_CHECK
    if( mDepthMask != flag )
    {
#endif
        
        glDepthMask( flag );
        mDepthMask = flag;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::EdgeFlag( GLboolean flag )
{
#ifndef DISABLE_STATE_CHECK
    if( mEdgeFlag != flag )
    {
#endif

        glEdgeFlag( flag );
        mEdgeFlag = flag;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}



INLINE void OGLStateManager::FrontFace( GLenum mode )
{
#ifndef DISABLE_STATE_CHECK
    if( mode != mFrontFaceMode )
    {
#endif

        glFrontFace( mode );
        mFrontFaceMode = mode;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::CullFace( GLenum mode )
{
#ifndef DISABLE_STATE_CHECK
    if( mode != mCullFaceMode )
    {
#endif

        glCullFace( mode );
        mCullFaceMode = mode;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::LogicOp( GLenum opcode )
{
#ifndef DISABLE_STATE_CHECK
    if( opcode != mLogicOpMode )
    {
#endif

        glLogicOp( opcode );
        mLogicOpMode = opcode;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::PolygonMode( GLenum face, GLenum mode )
{
#ifndef DISABLE_STATE_CHECK
    if( face == GL_FRONT && mode != mPolygonFrontFaceMode )
    {
#endif

        glPolygonMode( face, mode );
        mPolygonFrontFaceMode = mode;

#ifndef DISABLE_STATE_CHECK
    }
    else if( face == GL_BACK && mode != mPolygonBackFaceMode )
    {
#endif

        glPolygonMode( face, mode );
        mPolygonBackFaceMode = mode;

#ifndef DISABLE_STATE_CHECK
    }
    else if( face == GL_FRONT_AND_BACK && (mode != mPolygonFrontFaceMode || mode != mPolygonBackFaceMode) )
    {
#endif
        glPolygonMode( face, mode );
        mPolygonFrontFaceMode = mode;
        mPolygonBackFaceMode = mode;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE void OGLStateManager::MatrixMode( GLenum mode )
{
#ifndef DISABLE_STATE_CHECK
    if( mode != mMatrixMode )
    {
#endif
        glMatrixMode( mode );
        mMatrixMode = mode;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}

INLINE Int32 OGLStateManager::RenderMode( GLenum mode )
{
	Int32 returnValue = mode;

#ifndef DISABLE_STATE_CHECK
    if( mode != mRenderMode )
    {
#endif

        returnValue = glRenderMode( mode );
        mRenderMode = mode;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif

	return returnValue;
}

INLINE void OGLStateManager::ShadeModel( GLenum mode )
{
#ifndef DISABLE_STATE_CHECK
    if( mode != mShadeMode )
    {
#endif

        glShadeModel( mode );
        mShadeMode = mode;

#ifndef DISABLE_STATE_CHECK
    }
    KEEP_STATS;
#endif
}


} // namespace Gamedesk
