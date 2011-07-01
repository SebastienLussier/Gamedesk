/**
 *  @file       MainViewer.h
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
#ifndef     _MAINVIEWER_H_
#define     _MAINVIEWER_H_


#include "EditorTool.h"
#include "Viewer.h"
#include "Graphic/RenderWindow.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "SelectionListener.h"

#include "Graphic/Font.h"
#include "Graphic/FontHdl.h"


class CameraManipulator;
class DragManipulator;
class TrackballManipulator;
class ConfigSection;
class MainViewerUI;


/**
 *  
 *  @author Marco Arsenault.
 *  @date   30/03/04
 */
class MainViewer : public EditorTool, public RenderListener,
                   public Mouse::Listener, public Keyboard::Listener,
                   public SelectionListener
{
    DECLARE_CLASS(MainViewer,EditorTool);

public:
    MainViewer();
    virtual ~MainViewer();

	virtual void Init();
    virtual void Kill();
    
    virtual void LoadSettings( ConfigSection& pSection );
    virtual void SaveSettings( ConfigSection& pSection ) const;

	virtual void Render();

	//! Create the ui window for the tool.
    virtual QWidget* CreateUI();

    //! Destroy the ui windnow for the tool.
    virtual void DestroyUI();

protected:
    // Mouse Listener
    virtual void OnMouseButtonPressed( const Mouse::Button& pButton );
    virtual void OnMouseButtonReleased( const Mouse::Button& pButton );

    // Keyboard Listener
    virtual void OnKeyPressed( const Keyboard::Key& pKey );

    //! Called when a selection is changed.
    virtual void OnSelectionChanged(const std::list<Entity*>& pSelection);

private:
    FontHdl                 mFont;

	MainViewerUI*           mMainViewerUI;

    std::list<Entity*>      mSelection;
	Vector2f                mClickedPos;
    Entity*                 mClickedEntity;

    CameraManipulator*      mCameraManipulator;
    TrackballManipulator*   mTrackballManipulator;
    DragManipulator*        mDragManipulator;

	Bool                    mClickedOnTool;
};


#endif  //  _MAINVIEWER_H_
