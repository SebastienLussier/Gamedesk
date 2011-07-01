/**
 *  @file       TrackballManipulator.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       03/02/04.
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
#ifndef     _TRACKBALL_MANIPULATOR_H_
#define     _TRACKBALL_MANIPULATOR_H_


#include "Manipulator/Manipulator.h"
#include "Input/InputState.h"
#include "Maths/Quaternion.h"
#include "Maths/Vector3.h"


class EditorBase;


/**
 *  TrackballManipulator class to rotate a 3d object.
 *  @brief  TrackballManipulator class.
 *  @author Marco Arsenault.
 *  @date   03/02/04.
 */
class MAINVIEWER_API TrackballManipulator : public Manipulator
{
    CLASS_DISABLE_COPY(TrackballManipulator);

public:
    //! Default constructor.
	TrackballManipulator(EditorBase* pEditor);
    //! Destructor.
    virtual ~TrackballManipulator();

    //! Activate.the manipulator.
    virtual void Activate();

    //! Deactivate.the manipulator.
    virtual void Deactivate();

    virtual void Render() const;

    //! Returns the activation input state.
    virtual const InputState& GetActivationInputState() const;

    //! Returns the deactivation input state.
    virtual const InputState& GetDeactivationInputState() const;

    //! Returns true if the manipulator supports the given entity. false otherwise.
    virtual Bool CanManipulate(Entity* pEntityProperties) const;
    virtual Bool CanManipulate(const std::list<Entity*>& pEntityProperties) const;

    //! Sets the manipulated object(s) that will be converted to a Camera3DProperties.
    virtual void SetManipulatedEntity(Entity* pEntityProperties);
    virtual void SetManipulatedEntity(const std::list<Entity*>& pEntityProperties);

    /**
     *  Called when the mouse is moved.
     *  @param  pRelX   Mouvement on the X axis, relative to the last position of the mouse.
     *  @param  pRelY   Mouvement on the Y axis, relative to the last position of the mouse.
     */
    virtual void OnMouseMove(Int32 pRelX, Int32 pRelY);

    //!  Called when the given mouse button is pressed.
    virtual void OnMouseButtonPressed(const Mouse::Button& pButton);

    //!  Called when the given mouse button is released.
    virtual void OnMouseButtonReleased(const Mouse::Button& pButton);

private:
    Vector3f Closest(const Vector3f& pStart, const Vector3f& pEnd, const Vector3f& pPoint);
    Vector3f SphereLineIntersection(const Vector3f& pStart, const Vector3f& pEnd, const Vector3f& pSpherePos, Float pRadius);
    Vector3f MapToSphere(const Vector2f& pWinPos) const;
    Quaternionf GetRotation(const Vector3f& pStartVector, const Vector3f& pEndVector) const;
    Quaternionf VirtualTrackBall(const Vector3f& pCop, const Vector3f& pCor, const Vector3f& pDir1, const Vector3f& pDir2);
    Quaternionf RotationArc(const Vector3f& pV0, const Vector3f& pV1);
    Vector3f PlaneLineIntersection(const Vector3f& pN, Float pD, const Vector3f& pV1, const Vector3f& pV2);
    Vector3f MouseDir(Int32 x, Int32 y);

    std::list<Entity*>  mEntities;
    InputState          mActivationInputState;
    InputState          mDeactivationInputState;
    EditorBase*         mEditor;
    Quaternionf         mOldObjectOrientation;
    
    void     BeginDrag();
    void     EndDrag();
    Vector3f CursorToSpherePos();
    
public:
    Vector3f            mBallPos;
    Quaternionf         mBallRot;
    Vector3f            mDragStart;
    Vector3f            mDragPos;
    Quaternionf         mNewRotation;
};


#endif  //  _TRACKBALL_MANIPULATOR_H_