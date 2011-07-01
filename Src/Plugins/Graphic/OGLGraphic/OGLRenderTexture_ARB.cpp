#include "OGLGraphic.h"
#include "OGLRenderTexture_ARB.h"

#include "Graphic/Texture/Texture.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {


IMPLEMENT_CLASS(OGLRenderTexture1D_ARB);
IMPLEMENT_CLASS(OGLRenderTexture2D_ARB);
IMPLEMENT_CLASS(OGLRenderCubemap_ARB);


///////////////////////////////////////////////////////////////////////////////
// RenderTexture1D
///////////////////////////////////////////////////////////////////////////////
OGLRenderTexture1D_ARB::OGLRenderTexture1D_ARB()
{
}

OGLRenderTexture1D_ARB::~OGLRenderTexture1D_ARB()
{
}

void OGLRenderTexture1D_ARB::Create( UInt32 /*pWidth*/, Image::Format /*pFormat*/ )
{
}

void OGLRenderTexture1D_ARB::MakeCurrent()
{
}

void OGLRenderTexture1D_ARB::Bind()
{
}

void OGLRenderTexture1D_ARB::UnBind()
{
}


///////////////////////////////////////////////////////////////////////////////
// RenderTexture2D
///////////////////////////////////////////////////////////////////////////////
OGLRenderTexture2D_ARB::OGLRenderTexture2D_ARB()
{
}

OGLRenderTexture2D_ARB::~OGLRenderTexture2D_ARB()
{
    wglDeleteContext( mPixelBuffer.mRC );
	wglReleasePbufferDCARB( mPixelBuffer.mPBuffer, mPixelBuffer.mDC );
	wglDestroyPbufferARB( mPixelBuffer.mPBuffer );
}

void OGLRenderTexture2D_ARB::Create( UInt32 pWidth, UInt32 pHeight, Image::Format pFormat )
{
    mWidth  = pWidth;
    mHeight = pHeight;

    mTexture = mTexture2D = Cast<Texture2D>( GraphicSubsystem::Instance()->Create(Texture2D::StaticClass()) );
    mTexture2D->Create( mWidth, mHeight, pFormat );
    mTexture2D->Init();

    mTexture2D->SetMinFilter( Texture::MinFilter_Linear );
    mTexture2D->SetMagFilter( Texture::MagFilter_Linear );
    mTexture2D->SetWrapMode( Texture::Wrap_S, Texture::Wrap_Repeat );
    mTexture2D->SetWrapMode( Texture::Wrap_T, Texture::Wrap_Repeat );

    HDC hdc = wglGetCurrentDC();

    //
	// Define the minimum pixel format requirements we will need for our 
	// p-buffer. A p-buffer is just like a frame buffer, it can have a depth 
	// buffer associated with it and it can be double buffered.
	//
    UInt32  count = 0;
	Int32   pixelFormat;
    
	Int32 pfAttr[] =
	{
		WGL_SUPPORT_OPENGL_ARB,         TRUE,   // P-buffer will be used with OpenGL
		WGL_DRAW_TO_PBUFFER_ARB,        TRUE,   // Enable render to p-buffer
		WGL_BIND_TO_TEXTURE_RGBA_ARB,   TRUE,   // P-buffer will be used as a texture
		WGL_RED_BITS_ARB,               8,      // At least 8 bits for RED channel
		WGL_GREEN_BITS_ARB,             8,      // At least 8 bits for GREEN channel
		WGL_BLUE_BITS_ARB,              8,      // At least 8 bits for BLUE channel
		WGL_ALPHA_BITS_ARB,             8,      // At least 8 bits for ALPHA channel
		WGL_DEPTH_BITS_ARB,             16,     // At least 16 bits for depth buffer
		WGL_DOUBLE_BUFFER_ARB,          FALSE,  // We don't require double buffering
		0                                       // Zero terminates the list
	};
    wglChoosePixelFormatARB( hdc, (const int*)pfAttr, NULL, 1, (int*)&pixelFormat, (UINT*)&count );
    if( count == 0 )
	{
		MessageBox( NULL, "Could not find an acceptable pixel format!", "ERROR", MB_OK|MB_ICONEXCLAMATION );
		exit(-1);
	}

    //
	// Set some p-buffer attributes so that we can use this p-buffer as a
	// 2D RGBA texture target.
	//
	Int32 pb_attr[] =
	{
		WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB, // Our p-buffer will have a texture format of RGBA
		WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,   // Of texture target will be GL_TEXTURE_2D
		0                                             // Zero terminates the list
	};

    //
	// Create the p-buffer...
	//
	mPixelBuffer.mPBuffer = wglCreatePbufferARB( (HDC)hdc, pixelFormat, pWidth, pHeight, (int*)pb_attr );
	mPixelBuffer.mDC      = wglGetPbufferDCARB( mPixelBuffer.mPBuffer );
	mPixelBuffer.mRC      = wglCreateContext( mPixelBuffer.mDC );
	if( !mPixelBuffer.mPBuffer )
	{
		MessageBox( NULL, "Could not create the p-buffer", "ERROR", MB_OK|MB_ICONEXCLAMATION );
		exit(-1);
	}

    // 
    // Validate it's size
    //
    Int32 h, w;
    wglQueryPbufferARB( (HPBUFFERARB)mPixelBuffer.mPBuffer, WGL_PBUFFER_HEIGHT_ARB, (int*)&h );
	wglQueryPbufferARB( (HPBUFFERARB)mPixelBuffer.mPBuffer, WGL_PBUFFER_WIDTH_ARB, (int*)&w );
	if( h != (Int32)pWidth || w != (Int32)pHeight )
	{
		MessageBox( NULL, "The width and height of the created p-buffer don't match the requirements!", "ERROR", MB_OK|MB_ICONEXCLAMATION );
		exit(-1);
	}

    //
    // Make sure we share the contexts 
    //
    if( !wglShareLists( wglGetCurrentContext(), mPixelBuffer.mRC ) )
    {
        MessageBox( NULL, "wglShareLists failed !", "ERROR", MB_OK|MB_ICONEXCLAMATION );
		exit(-1);
    }
}

void OGLRenderTexture2D_ARB::MakeCurrent()
{
    if( IsBinded() )
        UnBind();

    if( !wglMakeCurrent( mPixelBuffer.mDC, mPixelBuffer.mRC ) )
    {
        MessageBox(NULL,"Could not make the window's context current!",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
        exit(-1);
    }
}

void OGLRenderTexture2D_ARB::Bind()
{
    Super::Bind();
    
    if( !wglBindTexImageARB( mPixelBuffer.mPBuffer, WGL_FRONT_LEFT_ARB ) )
    {
        MessageBox(NULL,"Could not bind p-buffer to render texture!",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
		exit(-1);
    }
}

void OGLRenderTexture2D_ARB::UnBind()
{
    Super::UnBind();
    
    if( !wglReleaseTexImageARB( mPixelBuffer.mPBuffer, WGL_FRONT_LEFT_ARB ) )
    {
        MessageBox(NULL,"Could not release p-buffer from render texture!",
            "ERROR",MB_OK|MB_ICONEXCLAMATION);
        exit(-1);
    }
}


///////////////////////////////////////////////////////////////////////////////
// RenderCubemap
///////////////////////////////////////////////////////////////////////////////
OGLRenderCubemap_ARB::OGLRenderCubemap_ARB()
{
}

OGLRenderCubemap_ARB::~OGLRenderCubemap_ARB()
{
}

void OGLRenderCubemap_ARB::Create( UInt32 /*pWidth*/, UInt32 /*pHeight*/, Image::Format /*pFormat*/ )
{
}

void OGLRenderCubemap_ARB::MakeCurrent()
{
}

void OGLRenderCubemap_ARB::Bind()
{
}

void OGLRenderCubemap_ARB::UnBind()
{
}


} // namespace Gamedesk
