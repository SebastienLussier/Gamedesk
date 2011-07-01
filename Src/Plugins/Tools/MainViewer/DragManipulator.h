/**
 *  @file       DragManipulator.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       04/02/04.
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
#ifndef     _DRAG_MANIPULATOR_H_
#define     _DRAG_MANIPULATOR_H_


#include "Manipulator/Manipulator.h"
#include "Input/InputState.h"
#include "Maths/Vector3.h"


class EditorBase;
class Entity;


/**
 *  DragManipulator class to drag a 3d object.
 *  @brief  DragManipulator class.
 *  @author Marco Arsenault.
 *  @date   04/02/04.
 */
class MAINVIEWER_API DragManipulator : public Manipulator
{
    CLASS_DISABLE_COPY(DragManipulator);

public:
    //! Default constructor.
	DragManipulator(EditorBase* pEditor);
    //! Destructor.
    virtual ~DragManipulator();

    //! Activate/Deactivate the manipulator.
    virtual void Activate();
    virtual void Deactivate();

    //! Returns the activation or deactivation input state.
    virtual const InputState& GetActivationInputState() const;
    virtual const InputState& GetDeactivationInputState() const;

    //! Returns true if the manipulator supports the given entity. false otherwise.
    virtual Bool CanManipulate(Entity* pEntity) const;
    virtual Bool CanManipulate(const std::list<Entity*>& pEntities) const;

    //! Sets the manipulated object(s).
    virtual void SetManipulatedEntity(Entity* pEntity);
    virtual void SetManipulatedEntity(const std::list<Entity*>& pEntities);

    /**
     *  Called when the mouse is moved.
     *  @param  pRelX   Mouvement on the X axis, relative to the last position of the mouse.
     *  @param  pRelY   Mouvement on the Y axis, relative to the last position of the mouse.
     */
    virtual void OnMouseMove( Int32 pRelX, Int32 pRelY );

    //! Called when the given button is pressed.
    virtual void OnMouseButtonPressed(const Mouse::Button& pButton);

    //! Called when the given button is released.
    virtual void OnMouseButtonReleased(const Mouse::Button& pButton);

private:
	//! Calculte the center position of all the manipulated object.
	Vector3f GetCenterPosition() const;

	//! Keep the origin position of each manipulated objects.
	void UpdateOriginPositions();

	std::list<Entity*>  mEntities;
	std::list<Vector3f> mOriginPositions;
    InputState          mActivationInputState;
    InputState          mDeactivationInputState;
    EditorBase*         mEditor;

    Vector2f            mClickPos;
	Vector2f            mScreenClickPos;
    Vector3f            mWorldPosition;
};


#endif  //  _DRAG_MANIPULATOR_H_
