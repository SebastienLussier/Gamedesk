/**
 *  @file       MainViewer.cpp
 *  @brief      The main viewer tool..
 *  @author     Marco Arsenault.
 *  @date       30/03/04.
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
#include "MainViewerDll.h"
#include "MainViewer.h"
#include "MainViewerUI.h"

#include "CameraManipulator.h"
#include "TrackballManipulator.h"
#include "DragManipulator.h"
#include "Viewer.h"
#include "EditorBase.h"

#include "Graphic/Renderer.h"
#include "Graphic/RenderWindow.h"
#include "Graphic/GraphicSubsystem.h"
#include "Resource/ResourceManager.h"
#include "Config/ConfigFile.h"
#include "Input/InputSubsystem.h"

#include "System/SystemInfo.h"
#include "WorldManager.h"
#include "World/Camera.h"
#include "Containers/Containers.h"
#include "Graphic/Font.h"
#include "WorldManager.h"


IMPLEMENT_CLASS(MainViewer);


MainViewer::MainViewer() : 
    mMainViewerUI(0),
    mCameraManipulator(0),
    mDragManipulator(0),
    mTrackballManipulator(0),
    mClickedEntity(0),
	mClickedPos(0, 0)
{
}

MainViewer::~MainViewer()
{
}

void MainViewer::Init()
{
    // Make sure the editor has been set.
    GD_ASSERT(GetEditor());

    mMainViewerUI->Init();
   
    Mouse& mouse = InputSubsystem::GetMouse();
    mouse.AddButtonListener(this, Mouse::Button_Left, Mouse::Button_Pressed);
    mouse.AddButtonListener(this, Mouse::Button_Left, Mouse::Button_Released);

    Keyboard& keyboard = InputSubsystem::GetKeyboard();
    keyboard.AddKeyListener(this, Keyboard::Key_L, Keyboard::Key_Pressed);
    keyboard.AddKeyListener(this, Keyboard::Key_Spacebar, Keyboard::Key_Pressed);
    keyboard.AddKeyListener(this, Keyboard::Key_LeftCtrl, Keyboard::Key_Pressed);
    keyboard.AddKeyListener(this, Keyboard::Key_Z, Keyboard::Key_Pressed);
    keyboard.AddKeyListener(this, Keyboard::Key_Y, Keyboard::Key_Pressed);

    if( GraphicSubsystem::Instance() == NULL )
        throw SubsystemNotLoadedException( MainViewer::StaticClass(), GraphicSubsystem::StaticClass(), Here );

    // Add the tools manipulator.
    mDragManipulator = GD_NEW(DragManipulator, this, "DragManipulator")(GetEditor());
    mMainViewerUI->GetViewer()->AddManipulator(mDragManipulator);
    mTrackballManipulator = GD_NEW(TrackballManipulator, this, "TrackballManipulator")(GetEditor());
    mMainViewerUI->GetViewer()->AddManipulator(mTrackballManipulator);
    mCameraManipulator = GD_NEW(CameraManipulator, this, "CameraManipulator");
    mMainViewerUI->GetViewer()->AddManipulator(mCameraManipulator);

    // Camera manipulator.
    // TODO: We might want to put that in another place...
    Camera* camera = GetEditor()->GetWorldManager().GetCurrentCamera();
    if( mCameraManipulator->CanManipulate( camera ) )
        mCameraManipulator->SetManipulatedEntity( camera );     

    // Start listening to the selection changes.
    GetEditor()->GetWorldManager().AddSelectionListener(this);

    // Create font
    mFont.GetFont( "Data/Fonts/tahoma.ttf", 14 );
}

void MainViewer::Kill()
{
    mMainViewerUI->Kill();

    Mouse& mouse = InputSubsystem::GetMouse();
    mouse.RemoveButtonListener(this, Mouse::Button_Left, Mouse::Button_Pressed);
    mouse.RemoveButtonListener(this, Mouse::Button_Left, Mouse::Button_Released);
    
    Keyboard& keyboard = InputSubsystem::GetKeyboard();
    keyboard.RemoveKeyListener(this, Keyboard::Key_L, Keyboard::Key_Pressed);
    keyboard.RemoveKeyListener(this, Keyboard::Key_Spacebar, Keyboard::Key_Pressed);
    keyboard.RemoveKeyListener(this, Keyboard::Key_LeftCtrl, Keyboard::Key_Pressed);
    keyboard.RemoveKeyListener(this, Keyboard::Key_Z, Keyboard::Key_Pressed);
    keyboard.RemoveKeyListener(this, Keyboard::Key_Y, Keyboard::Key_Pressed);

    // Remove the tools manipulator.
    mMainViewerUI->GetViewer()->RemoveManipulator(mTrackballManipulator);
    mMainViewerUI->GetViewer()->RemoveManipulator(mDragManipulator);
    mMainViewerUI->GetViewer()->RemoveManipulator(mCameraManipulator);

    GD_DELETE(mTrackballManipulator);
    GD_DELETE(mDragManipulator);
    GD_DELETE(mCameraManipulator);

    // Stop listening to the selection changes.
    GetEditor()->GetWorldManager().RemoveSelectionListener(this);
}

void MainViewer::LoadSettings( ConfigSection& pSection )
{
    Super::LoadSettings( pSection );
}

void MainViewer::SaveSettings( ConfigSection& pSection ) const
{
    Super::SaveSettings( pSection );
}

QWidget* MainViewer::CreateUI()
{
    if(!mMainViewerUI)
    {
        mMainViewerUI = new MainViewerUI(this);
        mMainViewerUI->setObjectName("Main Viewer");
        mMainViewerUI->show();
		SetWindow(mMainViewerUI);

		GetEditor()->SetMainView(mMainViewerUI->GetViewer());
    }

    return mMainViewerUI;
}

void MainViewer::DestroyUI()
{
    if(mMainViewerUI)
    {
        mMainViewerUI->hide();
        SetWindow(0);
    }    
}

void MainViewer::Render()
{
    GetEditor()->GetWorldManager().Render();

    static UInt32 iFrameCount = 0;
    iFrameCount++;

	String manipName = "Selection";

	if(mDragManipulator->IsActivated())
	{
        mDragManipulator->Render();
		manipName = "Translation";
	}
	else if(mTrackballManipulator->IsActivated())
	{
        mTrackballManipulator->Render();
		manipName = "Rotation";
	}    

    mFont->DrawString( 6, mMainViewerUI->GetViewer()->height() - 18,
					   "FPS: %.2f", mMainViewerUI->GetViewer()->GetFPS());

	mFont->DrawString( 6, mMainViewerUI->GetViewer()->height() - 32,
					   "%i rendered models", GetEditor()->GetWorldManager().GetNbRenderedEntities() );

    mFont->DrawString( 6, 8, "Manipulator Mode : %s", manipName.c_str());
}



void MainViewer::OnMouseButtonPressed( const Mouse::Button& pButton )
{
    if(pButton != Mouse::Button_Left)
        return;

    if( mDragManipulator->IsActivated() || mTrackballManipulator->IsActivated() )
        return;

    // Get the world manager.
    WorldManager& worldManager = GetEditor()->GetWorldManager();
    Keyboard& keyboard = InputSubsystem::GetKeyboard();

    // Map the click position to the viewer widget area.
    Int32 posX, posY;
    InputSubsystem::GetMouse().GetPos(posX, posY);
	mClickedOnTool = IsPointOnTool(posX, posY) && mMainViewerUI->hasFocus();
	if(!mClickedOnTool)
		return;

    QPoint widgetPoint = mMainViewerUI->GetViewer()->mapFromGlobal(QPoint(posX, posY));
    mClickedPos = Vector2f(widgetPoint.x(), widgetPoint.y());

    // Do the selection test.
    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    renderer->SetRenderTarget( mMainViewerUI->GetViewer()->GetRenderWindow() );

    mClickedEntity = worldManager.DoSelectionTest(mClickedPos);
	if(mClickedEntity)
	{
        // Check if the clicked object is in the current selection
        Bool selectionInList = false;
        std::list<Entity*>::const_iterator itSelection;
        for(itSelection = mSelection.begin(); itSelection != mSelection.end(); itSelection++)
        {
            if(*itSelection == mClickedEntity)
            {
                selectionInList = true;
                break;
            }
        }
        
        // If it wasn't it's a new selection, we select it.
        if(!selectionInList)
        {
            worldManager.SetSelection(mClickedEntity, keyboard.IsShiftDown());
            mClickedEntity = NULL;
        }
	}
	else
	{
        // Send an empty selection.
	    worldManager.SetSelection(NULL, keyboard.IsShiftDown());
        mClickedEntity = NULL;
	}
}

void MainViewer::OnMouseButtonReleased( const Mouse::Button& pButton )
{
    if(pButton != Mouse::Button_Left)
        return;

    // Get the world manager.
    WorldManager& worldManager = GetEditor()->GetWorldManager();
    Keyboard& keyboard = InputSubsystem::GetKeyboard();

    // Map the release position to the viewer widget area.
    Int32 posX, posY;
    InputSubsystem::GetMouse().GetPos(posX, posY);
	if(!mClickedOnTool)
		return;

    QPoint widgetPoint = mMainViewerUI->GetViewer()->mapFromGlobal(QPoint(posX, posY));
    Vector2f releasePos(widgetPoint.x(), widgetPoint.y());

    // If the mouse click and the mouse release position are the same, we set the new selection..
    if(mClickedPos == releasePos && mClickedEntity != NULL)
    {
        worldManager.SetSelection(mClickedEntity, keyboard.IsShiftDown());
        mClickedEntity = NULL;
    }
}

void MainViewer::OnKeyPressed( const Keyboard::Key& /*pKey*/ )
{    
    if(InputSubsystem::GetKeyboard().IsCtrlDown() && 
       InputSubsystem::GetKeyboard().IsDown(Keyboard::Key_Z))
    {
        GetEditor()->GetCommandManager().Undo();
    }
    else if(InputSubsystem::GetKeyboard().IsCtrlDown() && 
            InputSubsystem::GetKeyboard().IsDown(Keyboard::Key_Y))
    {
        GetEditor()->GetCommandManager().Redo();
    }
    else if(InputSubsystem::GetKeyboard().IsDown(Keyboard::Key_Spacebar))
    {
        GetEditor()->GetWorldManager().NextCamera();
    }
}

void MainViewer::OnSelectionChanged(const std::list<Entity*>& pSelection)
{
    mSelection = pSelection;

    // Set the manipulated object for the trackball manipulator.
    if(mTrackballManipulator->CanManipulate(pSelection))
        mTrackballManipulator->SetManipulatedEntity(pSelection);
    else
        mTrackballManipulator->SetManipulatedEntity(NULL);

    // Set the manipulated object for the trackball manipulator.
    if(mDragManipulator->CanManipulate(pSelection))
        mDragManipulator->SetManipulatedEntity(pSelection);
    else
        mDragManipulator->SetManipulatedEntity(NULL);
}
