#include "PSPGraphic.h"
#include "PSPTextureStage.h"

#include "PSPTexture.h"
#include "Graphic/RenderTarget/RenderTexture.h"


namespace Gamedesk {
	
	
const UInt32 GDToGUTextureMode[] =  {
                                        GU_TFX_MODULATE,
                                        GU_TFX_DECAL,
                                        GU_TFX_BLEND,
                                        GU_TFX_REPLACE,
                                        GU_TFX_ADD
                                    };

const UInt32  GDToGUWrapMode[] = {
                                     GU_CLAMP,                  // Wrap_Clamp
                                     GU_REPEAT                  // Wrap_Repeat
                                 };

const UInt32 GDToGUTexFilter[] = {
                                    GU_NEAREST,                 // Min/MagFilter_Nearest,
                                    GU_LINEAR,                  // Min/MagFilter_Linear,
                                    GU_NEAREST_MIPMAP_NEAREST,  // MinFilter_NearestMipmapNearest,
                                    GU_LINEAR_MIPMAP_NEAREST,   // MinFilter_LinearMipmapNearest,
                                    GU_NEAREST_MIPMAP_LINEAR,   // MinFilter_NearestMipmapLinear,
                                    GU_LINEAR_MIPMAP_LINEAR,    // MinFilter_LinearMipmapLinear,
                                };
/*
#define GU_PSM_5650		// 16-bit 5:6:5:0 color format
#define GU_PSM_5551		// 16-bit 5:5:5:1 color format
#define GU_PSM_4444		// 16-bit 4:4:4:4 color format
#define GU_PSM_8888		// 32-bit 8:8:8:8 color format
#define GU_PSM_T4		// 4-bit index color format
#define GU_PSM_T8		// 8-bit index color format
#define GU_PSM_T16		// 16-bit index color format
#define GU_PSM_T32		// 32-bit index color format
#define GU_PSM_DXT1		// DXT1 compression format
#define GU_PSM_DXT3		// DXT3 compression format
#define GU_PSM_DXT5		// DXT5 compression format
#define GU_PSM_DXT1_EXT // DXT1 compression format (high-order bit extension)
#define GU_PSM_DXT3_EXT // DXT3 compression format (high-order bit extension)
#define GU_PSM_DXT5_EXT // DXT5 compression format (high-order bit extension)
*/

#define INV 0xFFFFFFFF  // Invalid texture format

const UInt32 GDToGUTextureFormat[] =
    {
        INV,            // Format_L8
        INV,            // Format_A8
        INV,            // Format_L8A8
        INV,            // Format_R8G8B8
        GU_PSM_8888,    // Format_R8G8B8A8
        INV,            // Format_B8G8R8
        INV,            // Format_B8G8R8A8
        GU_PSM_DXT1,    // Format_DXT1
        GU_PSM_DXT3,    // Format_DXT3
        GU_PSM_DXT5,    // Format_DXT5
    };

PSPTextureStage::PSPTextureStage( UInt32 pStage )
    : TextureStage( pStage )
    , mTextureType(-1)
    , mTexture(NULL)
    , mRenderTexture(NULL)
{
}

PSPTextureStage::~PSPTextureStage()
{
}

void PSPTextureStage::MakeCurrent()
{
    return;     // There is only one texture stage on PSP!!
}

void PSPTextureStage::ResetTexture()
{
    if( mTextureType != -1 )
    {
        if( mRenderTexture )
            mRenderTexture->UnBind();

        sceGuTexImage(0,0,0,0,0);
        sceGuDisable(GU_TEXTURE_2D);

        mTexture       = NULL;
        mRenderTexture = NULL;
        mTextureType   = -1;
    }
}

void PSPTextureStage::SetTexture( Texture& pTexture )
{
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

    // Check texture type.
    PSPTexture2D* pspTexture = Cast<PSPTexture2D>(mTexture);

    // If new texture type differ from current, enable it.
    if( mTextureType == -1 )
    {
        mTextureType = GU_TEXTURE_2D;
        sceGuEnable(GU_TEXTURE_2D);
    }

    UInt32 texFormat = GDToGUTextureFormat[pspTexture->GetFormat()];
    GD_ASSERT(texFormat != INV);

    UInt32 mipsCount = pspTexture->GetImage().GetNumMipmaps();
    GD_ASSERT(mipsCount > 0 && mipsCount <= 8);

    sceGuTexMode(texFormat, mipsCount - 1, 0, 0);

    sceGuTexImage(0, pspTexture->GetWidth(), pspTexture->GetHeight(), pspTexture->GetWidth(), pspTexture->GetImage().GetData() );
    sceGuTexWrap(GDToGUWrapMode[pspTexture->GetWrapMode(Texture::Wrap_S)], GDToGUWrapMode[pspTexture->GetWrapMode(Texture::Wrap_T)]);
    sceGuTexFilter(GDToGUTexFilter[pspTexture->GetMinFilter()], GDToGUTexFilter[pspTexture->GetMagFilter()]);

    // #TODO-PSP: No need to do this for every texture ?
    sceGuTexScale(1.0f,1.0f);
	sceGuTexOffset(0.0f,0.0f);
}

void PSPTextureStage::SetTexture( RenderTexture& pRenderTexture )
{
    // Bind the render texture texture's ;)
    SetTexture( pRenderTexture.GetTexture() );

    // Keep a pointer to the render texture, to be able to unbind when
    // we'll remove it from this stage.
    mRenderTexture = &pRenderTexture;
    mRenderTexture->Bind();
}

void PSPTextureStage::SetTextureMode( TextureMode pMode )
{
    sceGuTexFunc(GDToGUTextureMode[pMode], GU_TCC_RGBA);
}

void PSPTextureStage::SetTextureCoordGenMode( TextureCoordGenMode pGenMode )
{
    GD_ASSERT(0);   // Unsupported!
}

void PSPTextureStage::SetPointSpriteCoordGen( Bool pEnable )
{
    GD_ASSERT(0);   // Unsupported!
}
