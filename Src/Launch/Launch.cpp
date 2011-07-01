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
#include "Engine.h"

#include "Application/Application.h"
#include "Application/Win32/Win32Window.h"

#include "World/SpacePartition/Octree.h"
#include "World/World.h"
#include "Config/ConfigFile.h"
#include "Module/ModuleManager.h"
#include "Object/ObjectIterator.h"
#include "FileManager/FileManager.h"
#include "SystemInfo/SystemInfo.h"
#include "World/Camera.h"
#include "World/Model3D.h"

#include "Input/InputSubsystem.h"
#include "Sound/SoundSubsystem.h"
#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"
#include "Graphic/RenderTarget/RenderWindow.h"

#include "Graphic/Font/Font.h"
#include "Graphic/Font/FontHdl.h"

#include "Graphic/Mesh/MeshHdl.h"

#include "Debug/PerformanceMonitor.h"


class Gamedesk : public Application, public Keyboard::Listener, public Mouse::Listener, public Window::Listener
{
    DECLARE_CLASS(Gamedesk,Application);

public:
    Gamedesk() 
        : mWorld(NULL)
        , mTime(0)
        , mLastTime(0)
        , mRun(true)
    {
        for( Class::Iterator it(Object::StaticClass()); it; ++it )
        {
            printf( "%s\n", (*it)->GetName().c_str() );
        }
    }

    ~Gamedesk()
    {
        GD_DELETE(mWorld);
        GD_DELETE(mRenderWindow);
        GD_DELETE(mMainWindow);
    }

    void SetupWindows()
    {
        mMainWindow = GD_NEW(Win32Window, this, "Launch::Gamedesk");
        mMainWindow->Init( "Gamedesk", 800, 600, true, false );
        
        RenderWindow::WindowInfo windowInfo;
        windowInfo.mWindowInternal = (Handle)mMainWindow;
        windowInfo.mWindowHandle   = (Handle)mMainWindow->GetHandle();
        windowInfo.mFullscreen     = mMainWindow->IsFullScreen();
        windowInfo.mSize           = mMainWindow->GetClientSize();
        
        RenderTarget::Format targetFormat;
        targetFormat.mAlpha        = true;
        targetFormat.mDepth        = true;
        targetFormat.mDoubleBuffer = true;
        targetFormat.mStencil      = true;
        targetFormat.mStereo       = false;

        Class* renderWindowClass = ModuleManager::Instance()->LoadClass( "Win32OGLRenderWindow.Win32OGLRenderWindow" );
        mRenderWindow = Cast<RenderWindow>( renderWindowClass->AllocateNew() );
        mRenderWindow->Create( windowInfo, targetFormat );
        mLastTime = SystemInfo::Instance()->GetSeconds();
    }

    void Init()
    {
        Super::Init();

        mWorld = GD_NEW(World, this, "Launch::Gamedesk");
        mWorld->Init(&mOctree);

        Model3D* pModel = Cast<Model3D>(mWorld->SpawnEntity(Model3D::StaticClass()));
        pModel->SetMesh("link's house.ase");
        pModel->Select(true);

		Keyboard& keyboard = InputSubsystem::GetKeyboard();
		keyboard.AddKeyListener(this, Keyboard::Key_W, Keyboard::Key_Down);
		keyboard.AddKeyListener(this, Keyboard::Key_S, Keyboard::Key_Down);
		keyboard.AddKeyListener(this, Keyboard::Key_A, Keyboard::Key_Down);
		keyboard.AddKeyListener(this, Keyboard::Key_D, Keyboard::Key_Down);
        keyboard.AddKeyListener(this, Keyboard::Key_Escape, Keyboard::Key_Down);
	        
		Mouse& mouse = InputSubsystem::GetMouse();
		mouse.AddMoveListener(this);

        mMainWindow->AddListener(this);

        mFont.GetFont( "Data/Fonts/tahoma.ttf", 14 );
    }

    virtual void Kill()
    {
		Keyboard& keyboard = InputSubsystem::GetKeyboard();
		keyboard.RemoveKeyListener(this, Keyboard::Key_W, Keyboard::Key_Down);
		keyboard.RemoveKeyListener(this, Keyboard::Key_S, Keyboard::Key_Down);
		keyboard.RemoveKeyListener(this, Keyboard::Key_A, Keyboard::Key_Down);
		keyboard.RemoveKeyListener(this, Keyboard::Key_D, Keyboard::Key_Down);
        keyboard.RemoveKeyListener(this, Keyboard::Key_Escape, Keyboard::Key_Down);
	        
		Mouse& mouse = InputSubsystem::GetMouse();
		mouse.RemoveMoveListener(this);

        mMainWindow->RemoveListener(this);

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

            if(SoundSubsystem::Instance())
                mDelta *= SoundSubsystem::Instance()->GetSpeedRatio();

            {
				Profile("Tick");

				// Update the input system.
				{
					Profile("Input");
					InputSubsystem::Instance()->Update();
				}

				// Update the physic subsystem.
				//{
				//Profile("Physic");
				//PhysicSubsystem::Instance()->Update(mDelta);
				//}

				// Update the world.
				{
					Profile("World Update");
					mWorld->Update(mDelta);
				}

				// Render the world.
				{
					Profile("World Render");
					mWorld->Render();
					RenderWindow* renderWindow = Cast<RenderWindow>( GraphicSubsystem::Instance()->GetRenderer()->GetRenderTarget() );
					mFont->DrawString( 6, 20, "FPS: %.2f", mFPS );
					renderWindow->SwapBuffers();
				}

				// Update the sound subsystem.
                if(SoundSubsystem::Instance())
				{
					Profile("Sound");
					Camera* camera = mWorld->GetCurrentCamera();
					SoundSubsystem::Instance()->Update( mDelta, 
														camera->GetPosition(),
														camera->GetView(),
														camera->GetUp() );
				}
            }
        }
    }

	void OnMouseMove( Int32 pRelX, Int32 pRelY )
	{
		const Float sgk_rotationSpeed(30.0f);
        Float rotSpeed = sgk_rotationSpeed * mDelta;
		
		mWorld->GetCurrentCamera()->Pitch(-pRelY * rotSpeed);
		mWorld->GetCurrentCamera()->Yaw(-pRelX * rotSpeed);
	}

    void OnKeyDown( const Keyboard::Key& pKey )
	{
		const Float sgk_movementSpeed(200.0f);
        Float movSpeed = sgk_movementSpeed * mDelta;

		if(pKey == Keyboard::Key_W)
			mWorld->GetCurrentCamera()->Move(movSpeed);
		else if(pKey == Keyboard::Key_S)
			mWorld->GetCurrentCamera()->Move(-movSpeed);
	    
		if(pKey == Keyboard::Key_D)
			mWorld->GetCurrentCamera()->PanLeftRight(movSpeed);
		else if(pKey == Keyboard::Key_A)
			mWorld->GetCurrentCamera()->PanLeftRight(-movSpeed);

        if( pKey == Keyboard::Key_Escape )
            mRun = false;
	}

    void OnResizeWindow( const Window& pWindow )
    {
        mRenderWindow->Resize( pWindow.GetClientSize().x, pWindow.GetClientSize().y );
    }

protected:
	Octree		mOctree;
    World*      mWorld;

    // Update time.
    Double      mDelta;
    Double		mTime;
	Double		mLastTime;
    FontHdl		mFont;

    // FPS
    Float       mFPS;
 	Int32       mFrameCount;	

    Bool        mRun;
};


IMPLEMENT_CLASS(Gamedesk);



int main( int argc, char* argv[] )
{
    Gamedesk gamedesk;
    gamedesk.Launch( argc, argv );

    return EXIT_SUCCESS;
}
