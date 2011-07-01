/**
 *  @file       OGLTables.h
 *  @brief	    Array used to convert between Gamedesk enums and OpenGL values.
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
#ifndef     _OGL_TABLES_H_
#define     _OGL_TABLES_H_


namespace Gamedesk {
	

const UInt32 GDToGLRenderState[] = {
                    GL_ALPHA_TEST,                      // AlphaTest,
                    GL_AUTO_NORMAL,                     // AutoNormal,
                    GL_BLEND,                           // Blend,
                    GL_CLIP_PLANE0,                     // ClipPlane_i,
                    GL_CULL_FACE,                       // CullFace,
                    GL_DEPTH_TEST,                      // DepthTest,
                    GL_DEPTH_TEST,                      // DepthMask
                    GL_DITHER,                          // Dithering,
                    GL_FOG,                             // Fog,
                    GL_LIGHT0,                          // Light_i,
                    GL_LIGHTING,                        // Lighting,
                    GL_LINE_SMOOTH,                     // LineSmooth,
                    GL_LINE_STIPPLE,                    // LineStipple,
                    GL_LIGHT_MODEL_LOCAL_VIEWER,        // LocalViewerLights,
                    GL_LOGIC_OP,                        // LogicOp,
                    GL_MAP1_COLOR_4,                    // Map1Color4,
                    GL_MAP1_INDEX,                      // Map1Index,
                    GL_MAP1_NORMAL,                     // Map1Normal,
                    GL_MAP1_TEXTURE_COORD_1,            // Map1TextureCoord1,
                    GL_MAP1_TEXTURE_COORD_2,            // Map1TextureCoord2,
                    GL_MAP1_TEXTURE_COORD_3,            // Map1TextureCoord3,
                    GL_MAP1_TEXTURE_COORD_4,            // Map1TextureCoord4,
                    GL_MAP1_VERTEX_3,                   // Map1Vertex3,
                    GL_MAP1_VERTEX_4,                   // Map1Vertex4,
                    GL_MAP2_COLOR_4,                    // Map2Color4,
                    GL_MAP2_INDEX,                      // Map2Index,
                    GL_MAP2_NORMAL,                     // Map2Normal,
                    GL_MAP2_TEXTURE_COORD_1,            // Map2TextureCoord1,
                    GL_MAP2_TEXTURE_COORD_2,            // Map2TextureCoord2,
                    GL_MAP2_TEXTURE_COORD_3,            // Map2TextureCoord3,
                    GL_MAP2_TEXTURE_COORD_4,            // Map2TextureCoord4,
                    GL_MAP2_VERTEX_3,                   // Map2Vertex3,
                    GL_MAP2_VERTEX_4,                   // Map2Vertex4,
                    GL_NORMALIZE,                       // Normalize,
                    GL_POINT_SMOOTH,                    // PointSmooth,
                    GL_POINT_SPRITE,                    // PointSprite
                    GL_POLYGON_SMOOTH,                  // PolygonSmooth,
                    GL_POLYGON_STIPPLE,                 // PolygonStipple,
                    GL_SCISSOR_TEST,                    // ScissorTest,
                    GL_STENCIL_TEST,                    // StencilTest,
                    GL_TEXTURE_1D,                      // Texture1D,
                    GL_TEXTURE_2D,                      // Texture2D,
                    GL_TEXTURE_GEN_Q,                   // TextureGenQ,
                    GL_TEXTURE_GEN_R,                   // TextureGenR,
                    GL_TEXTURE_GEN_S,                   // TextureGenS,
                    GL_TEXTURE_GEN_T,                   // TextureGenT,
                    GL_LIGHT_MODEL_TWO_SIDE             // TwoSidedLightning
                                        };
                   
const UInt32 GDToGLPixelCompareFunc[] = {
                    GL_NEVER,                           // CmpNever,
                    GL_LESS,                            // CmpLess,
                    GL_EQUAL,                           // CmpEqual,
                    GL_LEQUAL,                          // CmpLessEqual,
                    GL_GREATER,                         // CmpGreater,
                    GL_NOTEQUAL,                        // CmpNotEqual,
                    GL_GEQUAL,                          // CmpGreaterEqual,
                    GL_ALWAYS                           // CmpAlways
                                        };

const UInt32 GDToGLPixelBlendingFactor[] =  {
                    GL_ZERO,                            // BlendZero,
                    GL_ONE,                             // BlendOne,
                    GL_SRC_COLOR,                       // BlendSrcColor,
                    GL_SRC_ALPHA,                       // BlendSrcAlpha,
                    GL_ONE_MINUS_SRC_COLOR,             // BlendInvSrcColor,
                    GL_ONE_MINUS_SRC_ALPHA,             // BlendInvSrcAlpha,
                    GL_SRC_ALPHA_SATURATE,              // BlendSrcAlphaSaturate,
                    GL_DST_COLOR,                       // BlendDstColor,
                    GL_DST_ALPHA,                       // BlendDstAlpha,
                    GL_ONE_MINUS_DST_COLOR,             // BlendInvDstColor,
                    GL_ONE_MINUS_DST_ALPHA              // BlendInvDstAlpha    
                                            };

const UInt32 GDToGLStencilOperation[] = {
                    GL_KEEP,                            // OpKeep,
                    GL_ZERO,                            // OpZero,
                    GL_REPLACE,                         // OpReplace,
                    GL_INCR,                            // OpIncrease,
                    GL_DECR,                            // OpDecrease,
                    GL_INVERT                           // OpInvert
                                        };

const UInt32 GDToGLRenderMode[] = {
                    GL_RENDER,                          // RenderNormal,
                    GL_SELECT,                          // RenderSelection,
                    GL_FEEDBACK                         // RenderFeedback
									};

const UInt32 GDToGLFillMode[] = {
                    GL_POINT,                           // FillPoint,
                    GL_LINE,                            // FillWireframe,
                    GL_FILL                             // FillSolid
                                };

const UInt32 GDToGLPolygonFace[] =  {
                    GL_FRONT,                           // FrontFace
                    GL_BACK,                            // BackFace
                    GL_FRONT_AND_BACK                   // BothFace
                                    };

const UInt32 GDToGLFogMode[] =  {
                    GL_LINEAR,                          // FogLinear,
                    GL_EXP,                             // FogExp,
                    GL_EXP2                             // FogExp2
                                };

const UInt32 GDToGLPrimitiveType[] =    {
                    GL_POINTS,                          // PointList,
                    GL_LINES,                           // LineList,
                    GL_LINE_STRIP,                      // LineStrip,
                    GL_LINE_LOOP,                       // LineLoop,
                    GL_TRIANGLES,                       // TriangleList,
                    GL_TRIANGLE_STRIP,                  // TriangleStrip,
                    GL_TRIANGLE_FAN,                    // TriangleFan,
                    GL_QUADS,                           // QuadList,
                    GL_QUAD_STRIP,                      // QuadStrip,
                    GL_POLYGON                          // PolygonList
                                        };

const UInt32 GDToGLShadeModel[] =   {
                    GL_FLAT,                            // Flat,
                    GL_SMOOTH                           // Smooth
                                    };

const UInt32 GDToGLMatrixMode[] =   {
                    GL_MODELVIEW,                       // ModelViewMatrix,
                    GL_PROJECTION,                      // ProjectionMatrix,
                    GL_TEXTURE,                         // TextureMatrix,
                    GL_COLOR                            // ColorMatrix
                                    };

const UInt32 GDToGLCullingMode[] =  {
                    GL_FALSE,                           // NoCulling
                    GL_FRONT,                           // FrontFace
                    GL_BACK,                            // BackFace
                    GL_FRONT_AND_BACK                   // CullBoth
                                    };


} // namespace Gamedesk


#endif  //  _OGL_TABLES_H_
