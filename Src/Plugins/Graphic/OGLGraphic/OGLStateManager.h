/**
 *  @file       OGLStateManager.h
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
#ifndef     _OGL_STATE_MANAGER_H_
#define     _OGL_STATE_MANAGER_H_


namespace Gamedesk {


/**
 *  Manage OpenGL state changes to reduce call to GL driver to a minimum.
 */
class OGLGRAPHIC_API OGLStateManager
{
public:
    INLINE void Init();
    INLINE void Kill();

    //! Set the cached state to their current values in the OpenGL driver.
    INLINE void SyncWithGL();
    
    INLINE void Enable( GLenum cap );
    INLINE void Disable( GLenum cap );
    INLINE GLboolean IsEnabled( GLenum cap );

    INLINE void EnableClientState( GLenum cap );
    INLINE void DisableClientState( GLenum cap );

    INLINE void AlphaFunc( GLenum func, GLclampf ref );
    INLINE void BlendFunc( GLenum sfactor, GLenum dfactor );
    INLINE void DepthFunc( GLenum func );    

    INLINE void ColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
    INLINE void DepthMask( GLboolean flag );

    INLINE void EdgeFlag( GLboolean flag );

    INLINE void StencilMask( GLuint mask );
    INLINE void StencilFunc( GLenum func, GLint ref, GLuint mask );
    INLINE void StencilOp( GLenum fail, GLenum zfail, GLenum zpass );

    INLINE void FrontFace( GLenum mode );

    INLINE void CullFace( GLenum mode );
    INLINE void LogicOp( GLenum opcode );
    
    INLINE void PolygonMode( GLenum face, GLenum mode );

    INLINE void MatrixMode( GLenum mode );

    INLINE Int32 RenderMode( GLenum mode );

    INLINE void ShadeModel( GLenum mode );

private:
    INLINE void SyncEnableStates();
    
    // glEnable, glDisable, glIsEnabled
    std::map<GLenum, GLboolean>         mEnableMap;

    // glAlphaFunc
    GLenum      mAlphaFunc;
    GLclampf    mAlphaRef;

    // glBlendFunc
    GLenum      mBlendSrcFactor;
    GLenum      mBlendDstFactor;
    
    // glDepthFunc
    GLenum      mDepthFunc;

    // glStencilFunc
    GLenum      mStencilFunc;
    GLint       mStencilFuncRef;
    GLuint      mStencilValueMask;

    // glStencilMask
    GLuint      mStencilWriteMask;

    // glStencilOp
    GLenum      mStencilFailOp;
    GLenum      mStencilZFailOp;
    GLenum      mStencilZPassOp;
    
    // glColorMask
    GLboolean   mColorMaskRed;
    GLboolean   mColorMaskGreen;
    GLboolean   mColorMaskBlue;
    GLboolean   mColorMaskAlpha;

    // glDepthMask
    GLboolean   mDepthMask;
    
    // glEdgeFlag
    GLboolean   mEdgeFlag;
    
    // glFrontFace
    GLenum      mFrontFaceMode;
    
    // glCullFace
    GLenum      mCullFaceMode;
    
    // glLogicOp
    GLenum      mLogicOpMode;
    
    // glPolygonMode
    GLenum      mPolygonFrontFaceMode;
    GLenum      mPolygonBackFaceMode;

    // glMatrixMode
    GLenum      mMatrixMode;

    // glRenderMode
    GLenum      mRenderMode;

    // glShadeMode
    GLenum      mShadeMode;

    // glEnableClientState, glDisableClientState
    GLboolean   mClientStateEnable[GL_EDGE_FLAG_ARRAY-GL_VERTEX_ARRAY+1];
        
#ifdef GD_DEBUG
    UInt32                              mCallCount;
    UInt32                              mRedundantCount;
#endif // GD_DEBUG
};


} // namespace Gamedesk


#endif  //  _OGL_STATE_MANAGER_H_
