#include "Engine.h"
#include "Graphic/RenderTarget/RenderTexture.h"
#include "Graphic/Texture/Texture.h"


namespace Gamedesk {
	
	
IMPLEMENT_ABSTRACT_CLASS(RenderTexture);
IMPLEMENT_ABSTRACT_CLASS(RenderTexture1D);
IMPLEMENT_ABSTRACT_CLASS(RenderTexture2D);
IMPLEMENT_ABSTRACT_CLASS(RenderCubemap);



RenderTexture::RenderTexture()
    : mBinded(false)
    , mTexture(NULL)
{
}

RenderTexture::~RenderTexture()
{
}

//! Make sure the render texture is ready for use as a normal texture.
void RenderTexture::Bind()
{
    GD_ASSERT(!mBinded);
    mBinded = true;
}

//! Make sure the render texture can be used as a render target.
void RenderTexture::UnBind()
{
    GD_ASSERT(mBinded);
    mBinded = false;
}

Bool RenderTexture::IsBinded() const
{
    return mBinded;
}

Texture& RenderTexture::GetTexture()
{
    GD_ASSERT(mTexture);
    return *mTexture;
}


} // namespace Gamedesk
