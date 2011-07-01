#include "OGLGraphic.h"
#include "OGLTextureStage.h"

#include "OGLTexture.h"
#include "Graphic/RenderTarget/RenderTexture.h"


namespace Gamedesk {
	

const GLenum GDToGLTextureMode[] = {
                        GL_MODULATE,
                        GL_DECAL,
                        GL_BLEND,
                        GL_REPLACE,
                        GL_ADD
                        };

OGLTextureStage::OGLTextureStage( UInt32 pStage ) 
    : TextureStage( pStage )
    , mTextureType(-1)
    , mTexture(NULL)
    , mRenderTexture(NULL)
{
}

OGLTextureStage::~OGLTextureStage()
{
}

void OGLTextureStage::MakeCurrent()
{
    if( mStage != mActiveStage )
    {
        glActiveTexture( GL_TEXTURE0+mStage );
        mActiveStage = mStage;
    }
}

void OGLTextureStage::ResetTexture()
{
    MakeCurrent();

    if( mTextureType != -1 )
    {
        if( mRenderTexture )
            mRenderTexture->UnBind();

        glBindTexture( mTextureType, 0 );
        glDisable( mTextureType );

        mTexture       = NULL;
        mRenderTexture = NULL;
        mTextureType   = -1;
    }
}

void OGLTextureStage::SetTexture( Texture& pTexture )
{    
    MakeCurrent();

    // Texture already binded.
    if( &pTexture == mTexture )
        return;
    else
        mTexture = &pTexture;
    
    // Unbind any render texture if binded.
    if( mRenderTexture && (&mRenderTexture->GetTexture() != mTexture) )
    {
        mRenderTexture->UnBind();
        mRenderTexture = NULL;
    }

    // Get texture ID and texture type.
    UInt32  textureID   = 0;
    Int32   textureType = -1;

    if( mTexture->GetClass() == OGLTexture2D::StaticClass() )
    {
        textureType = GL_TEXTURE_2D;
        textureID   = ((OGLTexture2D*)mTexture)->GetTextureID();
    }
    else if( mTexture->GetClass() == OGLTexture1D::StaticClass() )
    {
        textureType = GL_TEXTURE_1D;
        textureID   = ((OGLTexture1D*)mTexture)->GetTextureID();
    }
    else if( mTexture->GetClass() == OGLTexture3D::StaticClass() )
    {
        textureType = GL_TEXTURE_3D;
        textureID   = ((OGLTexture3D*)mTexture)->GetTextureID();
    }
    else if( mTexture->GetClass() == OGLCubemap::StaticClass() )
    {
        textureType = GL_TEXTURE_CUBE_MAP;
        textureID   = ((OGLCubemap*)mTexture)->GetTextureID();
    }
    else
    {
        // Whoa! Shouldn't get there!
        GD_ASSERT(0);
    }

    // If new texture type differ from current, enable it.
    if( textureType != mTextureType )
    {
        if( mTextureType != -1 )
            glDisable( mTextureType );

        mTextureType = textureType;
        glEnable( mTextureType );
    }

    // Finally, bind texture.
    glBindTexture( mTextureType, textureID );
}

void OGLTextureStage::SetTexture( RenderTexture& pRenderTexture )
{
    // Bind the render texture texture's ;)
    SetTexture( pRenderTexture.GetTexture() );
    
    // Keep a pointer to the render texture, to be able to unbind when
    // we'll remove it from this stage.
    mRenderTexture = &pRenderTexture;
    mRenderTexture->Bind();
}

void OGLTextureStage::SetTextureMode( TextureMode pMode )
{
    GD_ASSERT(pMode != Tex_Add);    // Unsupported!

    MakeCurrent();
    
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GDToGLTextureMode[pMode] );
}

void OGLTextureStage::SetTextureCoordGenMode( TextureCoordGenMode pGenMode )
{
    MakeCurrent();

    if( pGenMode == TextureStage::Gen_SphereMap )
    {
        glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
        glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
    }
    else if( pGenMode == TextureStage::Gen_NormalMap )
    {
        glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP );
        glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP );
        glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP );
    }
    else if( pGenMode == TextureStage::Gen_ReflectionMap )
    {
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP ); 
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP ); 
        glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP ); 
    }
    else
    {
        glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
        glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
        glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
    }
}

void OGLTextureStage::SetPointSpriteCoordGen( Bool pEnable )
{
    MakeCurrent();

    glTexEnvi( GL_POINT_SPRITE, GL_COORD_REPLACE, pEnable );
}


} // namespace Gamedesk


// GL_ARB_texture_env_combine
    /**
     *	REPLACE                 Arg0
     *  MODULATE                Arg0 * Arg1
     *  ADD                     Arg0 + Arg1
     *  ADD_SIGNED_ARB          Arg0 + Arg1 - 0.5
     *  SUBTRACT_ARB            Arg0 - Arg1
     *  INTERPOLATE_ARB         Arg0 * (Arg2) + Arg1 * (1-Arg2)
     *   
     *  where Arg0, Arg1 and Arg2 are derived from
     *
     *  PRIMARY_COLOR_ARB       primary color of incoming fragment
     *  TEXTURE                 texture color of corresponding texture unit
     *  CONSTANT_ARB            texture environment constant color
     *  PREVIOUS_ARB            result of previous texture environment; on
     *                          texture unit 0, this maps to PRIMARY_COLOR_ARB
     */
/*
glTexEnv( TARGET,           PNAME,                      PARAM(S) )

                            // Defines how the source (to be applied) and destination (current in the framebuffer) colors are blended. The mode may be one of the following:
          GL_TEXTURE_ENV,   GL_TEXTURE_ENV_MODE,        GL_MODULATE,        // Modulates the dest color with the src color.
                                                        GL_DECAL,           // Applies the src color to the dest color as a decal.
                                                        GL_BLEND,           // Blends the src color with the dest color.
                                                        GL_REPLACE,         // Replaces the dest color with the src color.
                                                        GL_COMBINE_ARB,     // Combines the dest color with the src color or texture blend color according to the combine operation as specified in the combine mode. 

                            
                            // Define the combine operation to use when GL_COMBINE_ARB env. mode is used.
                            GL_COMBINE_RGB_ARB,         
                            GL_COMBINE_ALPHA_ARB,
                                                        GL_REPLACE,             // C' = C0 
                                                        GL_MODULATE,            // C' = C0 * C1 
                                                        GL_ADD,                 // C' = C0 + C1
                                                        GL_ADD_SIGNED_ARB,      // C' = C0 + C1 - 0.5
                                                        GL_INTERPOLATE_ARB,     // C' = C0 C2 + C1 (1 - C2) 
                                                        GL_SUBTRACT_ARB,        // C' = C0 - C1
                                                        GL_DOT3_RGB_ARB,        // C' = 4*((C0.r - 0.5)*(C1.r - 0.5) + (C0.g - 0.5)*(C1.g - 0.5) + (C0.b - 0.5)*(C1.b - 0.5))
                                                        GL_DOT3_RGBA_ARB,       // C' = 4*((C0.r - 0.5)*(C1.r - 0.5) + (C0.g - 0.5)*(C1.g - 0.5) + (C0.b - 0.5)*(C1.b - 0.5))
                             
                            // Define the source for an operand in the combine operation.
                            GL_SOURCE0_RGB_ARB,  
                            GL_SOURCE1_RGB_ARB,  
                            GL_SOURCE2_RGB_ARB,
                            GL_SOURCE0_ALPHA_ARB,
                            GL_SOURCE1_ALPHA_ARB,                        
                            GL_SOURCE2_ALPHA_ARB,   
                                                        GL_TEXTURE,             // Use the texture color.
                                                        GL_CONSTANT_ARB,        // Use a constant color
                                                        GL_PRIMARY_COLOR_ARB,   // Use the color of the fragment.
                                                        GL_PREVIOUS_ARB,        // Use the previous texture unit color. (For unit 0, equal to GL_PRIMARY_COLOR_ARB).

                            // Specifies the function for a color operand in the combine operation.
                            GL_OPERAND0_RGB_ARB,  
                            GL_OPERAND1_RGB_ARB, 
                            GL_OPERAND2_RGB_ARB,
                                                        GL_SRC_COLOR,           // Cn = rgb
                                                        GL_ONE_MINUS_SRC_COLOR, // Cn = (1 - rgb)
                                                        GL_SRC_ALPHA,           // Cn = a
                                                        GL_ONE_MINUS_SRC_ALPHA, // Cn = (1 - a)

                            // Specifies the function for an alpha operand in the combine operation.
                            GL_OPERAND0_ALPHA_ARB,      
                            GL_OPERAND1_ALPHA_ARB,
                            GL_OPERAND2_ALPHA_ARB,
                                                        GL_SRC_ALPHA,           // Cn = a
                                                        GL_ONE_MINUS_SRC_ALPHA, // Cn = (1 - a)
                            
                            // The final result will be multiply by these factors (and clamped in the range [0,1]                            
                            GL_RGB_SCALE_ARB,
                            GL_ALPHA_SCALE,
                                                        1.0, 2.0, 4.0           // Must be one of these.
*/