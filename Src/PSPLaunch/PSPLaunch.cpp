/**
 *  @file       Launch.cpp
 *  @brief
 *  @author     Sébastien Lussier.
 *  @date       21/10/03.
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
#include "Core.h"
#include "Engine.h"
#include "PSPGraphic.h"


#include "Application/Application.h"
#include "Application/Window.h"

#include "Config/ConfigFile.h"
#include "Module/ModuleManager.h"
#include "Object/ObjectIterator.h"
#include "FileManager/FileManager.h"
#include "SystemInfo/SystemInfo.h"

#include "Input/InputSubsystem.h"
#include "Sound/SoundSubsystem.h"
#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Image/Image.h"
#include "Graphic/Texture/Texture.h"
#include "Graphic/Renderer.h"
#include "Graphic/RenderTarget/RenderWindow.h"
#include "Graphic/Buffer/VertexBuffer.h"

#include "Graphic/Font/Font.h"
#include "Graphic/Font/FontHdl.h"

#include "Debug/PerformanceMonitor.h"

#include <pspsdk.h>

/* Define the module info section */
PSP_MODULE_INFO("PSPLaunch", 0, 1, 1);
PSP_HEAP_SIZE_KB(-64);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);


struct MyVertex
{
	Float	TexU, TexV;			// 2x4 bytes
	UInt32	Color;				// 1x4 bytes
	Float	PosX, PosY, PosZ;	// 3x4 bytes
								// 6x4 bytes = 24 bytes
};

struct MyVertex __attribute__((aligned(16))) MyVertexData[12*3] =
{
	{0, 0, 0xff7f0000,-1,-1, 1}, // 0
	{1, 0, 0xff7f0000,-1, 1, 1}, // 4
	{1, 1, 0xff7f0000, 1, 1, 1}, // 5

	{0, 0, 0xff7f0000,-1,-1, 1}, // 0
	{1, 1, 0xff7f0000, 1, 1, 1}, // 5
	{0, 1, 0xff7f0000, 1,-1, 1}, // 1

	{0, 0, 0xff7f0000,-1,-1,-1}, // 3
	{1, 0, 0xff7f0000, 1,-1,-1}, // 2
	{1, 1, 0xff7f0000, 1, 1,-1}, // 6

	{0, 0, 0xff7f0000,-1,-1,-1}, // 3
	{1, 1, 0xff7f0000, 1, 1,-1}, // 6
	{0, 1, 0xff7f0000,-1, 1,-1}, // 7

	{0, 0, 0xff007f00, 1,-1,-1}, // 0
	{1, 0, 0xff007f00, 1,-1, 1}, // 3
	{1, 1, 0xff007f00, 1, 1, 1}, // 7

	{0, 0, 0xff007f00, 1,-1,-1}, // 0
	{1, 1, 0xff007f00, 1, 1, 1}, // 7
	{0, 1, 0xff007f00, 1, 1,-1}, // 4

	{0, 0, 0xff007f00,-1,-1,-1}, // 0
	{1, 0, 0xff007f00,-1, 1,-1}, // 3
	{1, 1, 0xff007f00,-1, 1, 1}, // 7

	{0, 0, 0xff007f00,-1,-1,-1}, // 0
	{1, 1, 0xff007f00,-1, 1, 1}, // 7
	{0, 1, 0xff007f00,-1,-1, 1}, // 4

	{0, 0, 0xff00007f,-1, 1,-1}, // 0
	{1, 0, 0xff00007f, 1, 1,-1}, // 1
	{1, 1, 0xff00007f, 1, 1, 1}, // 2

	{0, 0, 0xff00007f,-1, 1,-1}, // 0
	{1, 1, 0xff00007f, 1, 1, 1}, // 2
	{0, 1, 0xff00007f,-1, 1, 1}, // 3

	{0, 0, 0xff00007f,-1,-1,-1}, // 4
	{1, 0, 0xff00007f,-1,-1, 1}, // 7
	{1, 1, 0xff00007f, 1,-1, 1}, // 6

	{0, 0, 0xff00007f,-1,-1,-1}, // 4
	{1, 1, 0xff00007f, 1,-1, 1}, // 6
	{0, 1, 0xff00007f, 1,-1,-1}, // 5
};


class PSPWindow : public Window
{
    Handle GetHandle() const { return 0; }
    void SetPosition( const Vector2i&  ) {}
    void SetSize( const Vector2i& ) {}
    void SetFocus() {}
    void ShowMinimized() {}
    void ShowMaximized() {}
    void ShowNormal() {}
    void ShowFullscreen() {}
    Bool SupportFullScreen() { return true; }
    Vector2i GetClientSize() const { return Vector2i(480, 272); }
    Vector2i GetClientPos() const { return Vector2i(0, 0); }
};

class Gamedesk : public Application
{
    DECLARE_CLASS(Gamedesk, Application);

public:
    Gamedesk()
        : mTime(0)
        , mLastTime(0)
        , mRun(true)
        , mVertexBuffer(NULL)
        , mTexture2D(NULL)
    {
    }

    ~Gamedesk()
    {
        GD_DELETE(mRenderWindow);
        GD_DELETE(mMainWindow);

        GD_DELETE(mVertexBuffer);
        GD_DELETE(mTexture2D);
    }

    void SetupWindows()
    {
        mMainWindow = GD_NEW(PSPWindow, this, "PSPLaunch::GamedeskPSP");
        mMainWindow->Init( "GamedeskPSP", 480, 272, true, false );

        RenderWindow::WindowInfo windowInfo;
        windowInfo.mFullscreen     = true;
        windowInfo.mSize           = mMainWindow->GetClientSize();

        RenderTarget::Format targetFormat;
        targetFormat.mAlpha        = true;
        targetFormat.mDepth        = true;
        targetFormat.mDoubleBuffer = true;
        targetFormat.mStencil      = true;
        targetFormat.mStereo       = false;

        Class* renderWindowClass = ModuleManager::Instance()->LoadClass( "PSPGraphic.PSPRenderWindow" );
        mRenderWindow = Cast<RenderWindow>( renderWindowClass->AllocateNew() );
        mRenderWindow->Create( windowInfo, targetFormat );
        mLastTime = SystemInfo::Instance()->GetSeconds();
    }

    void Init()
    {
        Super::Init();

        //mFont.GetFont( "Data/Fonts/tahoma.ttf", 14 );

		mVertexBuffer = Cast<VertexBuffer>(GraphicSubsystem::Instance()->Create(VertexBuffer::StaticClass()));
		GD_ASSERT(mVertexBuffer);
		mVertexBuffer->Create( 12*3, sizeof(MyVertex), VertexBuffer::Usage_Static );

		MyVertex* vertexBuf = reinterpret_cast<MyVertex*>(mVertexBuffer->Lock( VertexBuffer::Lock_Write ));

		// Fill it
		for(UInt32 i = 0; i < mVertexBuffer->GetItemCount(); ++i)
		{
			vertexBuf[i].TexU  = MyVertexData[i].TexU;
			vertexBuf[i].TexV  = MyVertexData[i].TexV;
			vertexBuf[i].Color = MyVertexData[i].Color;
			vertexBuf[i].PosX  = MyVertexData[i].PosX;
			vertexBuf[i].PosY  = MyVertexData[i].PosY;
			vertexBuf[i].PosZ  = MyVertexData[i].PosZ;
		}

		mVertexBuffer->Unlock();


		mTexture2D = Cast<Texture2D>(GraphicSubsystem::Instance()->Create(Texture2D::StaticClass()));
		GD_ASSERT(mTexture2D);

		// Load an image
		Image img;
		img.Create(256, 256, Image::Format_R8G8B8A8);
		Byte* data = img.GetData();

		FILE* pFile = fopen("earth.raw", "r");
		Byte* imgRaw = new Byte[256*256*3];
		fread(imgRaw, 256*256*3, 1, pFile);
		fclose(pFile);

		for(int y = 0; y < 256; y++)
		{
			for(int x = 0; x < 256; x++)
			{
				Byte R = *(imgRaw++);
				Byte G = *(imgRaw++);
				Byte B = *(imgRaw++);
				*(data++) = R;
				*(data++) = G;
				*(data++) = B;
				*(data++) = 0xFF;
			}
		}

		mTexture2D->Create(img, false);
		mTexture2D->Init();
    }

    virtual void Kill()
    {
        Super::Kill();
    }

    virtual void SetupSubsystems( ConfigFile& pConfigFile )
    {
        Super::SetupSubsystems( pConfigFile );
    }

    virtual void Run()
    {
        mFPS = 0;
        mLastTime = mTime = SystemInfo::Instance()->GetSeconds();
	    mFrameCount = 0;

        while( mRun )
        {
            mMainWindow->Tick();

            Double newTime = SystemInfo::Instance()->GetSeconds();

            mDelta = newTime - mTime;
            mTime = newTime;
	        mFrameCount++;

            if( mTime - mLastTime > 0.5 )
	        {
		        mFPS        = mFrameCount / (mTime-mLastTime);
		        mLastTime   = mTime;
		        mFrameCount = 0;
	        }

            {
				Profile("Tick");

				// Update the input system.
				if( InputSubsystem::Instance() )
				{
					Profile("Input");
					InputSubsystem::Instance()->Update();
				}

				static int val = 0;

				// Render
				{
					Profile("Render");


					Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();


					renderer->BeginScene(Renderer::TriangleList);
                        renderer->SetClearColor(Color4f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX));
						renderer->Clear( Renderer::ColorBuffer | Renderer::DepthBuffer );

						renderer->GetTextureStage(0)->SetTexture(*mTexture2D);
						renderer->SetMatrixMode(Renderer::ProjectionMatrix);
						renderer->LoadIdentity();
						renderer->Perspective(75.0f, 16.0f/9.0f, 0.5f, 1000.0f);

						renderer->SetMatrixMode(Renderer::ModelViewMatrix);
						renderer->LoadIdentity();

						Vector3f pos(0, 0, -4.0f);
						Quaternionf rot;
						rot.FromEuler(val * 0.79f * Maths::PI_ON_180, val * 0.98f * Maths::PI_ON_180, val * 1.32f * Maths::PI_ON_180);
						renderer->Translate(pos);
						renderer->Rotate(rot);

						renderer->SetVertexFormat(VertexFormat::Component(VertexFormat::TexCoord2 | VertexFormat::Color4 | VertexFormat::Position3));
						renderer->SetStreamSource(VertexFormat::Component(VertexFormat::TexCoord2 | VertexFormat::Color4 | VertexFormat::Position3), mVertexBuffer);
						renderer->DrawPrimitive(Renderer::TriangleList, 0, mVertexBuffer->GetItemCount() );
					renderer->EndScene();

					RenderWindow* renderWindow = Cast<RenderWindow>( renderer->GetRenderTarget() );
					//mFont->DrawString( 6, 20, "FPS: %.2f", mFPS );
					renderWindow->SwapBuffers();
				}

				val++;
            }
        }
    }

protected:
    // Update time.
    Double      mDelta;
    Double		mTime;
	Double		mLastTime;
    FontHdl		mFont;

    // FPS
    Float       mFPS;
 	Int32       mFrameCount;

    Bool        mRun;

    VertexBuffer* mVertexBuffer;
    Texture2D*		mTexture2D;
};


IMPLEMENT_CLASS(Gamedesk);
IMPLEMENT_MODULE(PSPLaunch);


int main( int argc, char* argv[] )
{
	StaticInitCoreClasses();
	StaticInitEngineClasses();
	StaticInitPSPGraphicClasses();

    Gamedesk gamedesk;
    gamedesk.Launch( argc, argv );

    return EXIT_SUCCESS;
}
