/**
 *  @file       WorldManager.h
 *  @brief      A WorldManager class to manage the world's data with the editor.
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
#ifndef     _WORLD_MANAGER_H_
#define     _WORLD_MANAGER_H_


#include "EntityCreationSender.h"
#include "SelectionSender.h"
#include "CameraChangeSender.h"
#include "Containers/Containers.h"
#include "World/World.h"
#include "Maths/Vector2.h"
#include "Input/Keyboard.h"


class Class;
class Entity;
class EditorBase;


/**
 *  WorldManager.  This class is used as a facade to the world object.
 *  It can insert, remove, delete and create object for the editor.
 *  It manages the cameras and everything needed to manipulate the world
 *  in the editor.
 *  @brief  WorldManager class.
 *  @author Marco Arsenault.
 *  @date   31/01/04.
 */
class EDITORLIB_API WorldManager : public World,
                                   public EntityCreationSender,
                                   public SelectionSender,
                                   public CameraChangeSender,
                                   public Keyboard::Listener
                                  
{
    DECLARE_CLASS(WorldManager,World);

public:
    //! Constructor.
    WorldManager();
    
    //! Destructor.
    virtual ~WorldManager();

    //! Initialize the world.
    void Init();

    void InsertEntity( Entity* pEntity );
    void RemoveEntity( Entity* pEntity );

    //! Do the selection test and return the selected entity.
	Entity* DoSelectionTest( const Vector2f& pSelectionPoint );

    //! Directly set the selection with the given parameter.
    void SetSelection( Entity* pEntities, Bool pAdd = false );

    //! Get the current selection
    void GetSelection( std::list<Entity*>& pSelection ) const;

    //! Delete the selection.
    void DeleteSelection();

    //! Set the current camera with the given index.
    void SetCurrentCamera( UInt32 pCameraIndex );


    //virtual void OnKeyReleased( const Keyboard::Key& pKey );
       
private:
 	std::list<Entity*> mSelection;
};


#endif  //  _WORLD_MANAGER_H_
