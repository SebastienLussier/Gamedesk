/**
 *  @file       World.h
 *  @brief      A World class to manage the world's data with the editor.
 *  @author     Marco Arsenault.
 *  @date       31/01/04.
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
#pragma once

#include "ObjectLink.h"

namespace GameEdit
{

/**
 *  World.  This class is used as a facade to the world object.
 *  It can insert, remove, delete and create object for the editor.
 *  It manages the cameras and everything needed to manipulate the world
 *  in the editor.
 *  @brief  World class.
 *  @author Marco Arsenault.
 *  @date   31/01/04.
 */
public ref class World
{
public:
    virtual event EventHandler^ SelectionChanged
	{
		void add(EventHandler^ handler);
		void remove(EventHandler^ handler);
		void raise(Object^ sender, EventArgs^ e);
	}

    virtual event EventHandler^ CameraChanged
	{
		void add(EventHandler^ handler);
		void remove(EventHandler^ handler);
		void raise(Object^ sender, EventArgs^ e);
	}
	
public:
    //! Constructor.
    World();
    
    //! Destructor.
    virtual ~World();
    
    void InsertEntity( ObjectLink^ pEntity );
    void RemoveEntity( ObjectLink^ pEntity );

    //! Do the selection test and return the selected entity.
	ObjectLink^ DoSelectionTest( const Gamedesk::Vector2f& pSelectionPoint );

    //! Directly set the selection with the given parameter.
    void SetSelection( ObjectLink^ pEntity );

    //! Get the current selection
    void GetSelection( List<ObjectLink^>^ pSelection );

    //! Delete the selection.
    void DeleteSelection();

    //! Set the current camera with the given index.
    void SetCurrentCamera( UInt32 pCameraIndex );
       
private:
	Gamedesk::World*		mWorld;
 	List<ObjectLink^>^      mSelection;

    void FireSelectionChanged();
    void FireCameraChanged();

    EventHandler^		mSelectionChanged;
	EventHandler^		mCameraChanged;
};


} // namespace GameEdit
