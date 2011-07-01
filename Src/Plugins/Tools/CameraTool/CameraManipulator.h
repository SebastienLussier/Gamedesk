/**
 *  @file       CameraManipulator.h
 *  @brief	    
 *  @author     Marco Arsenault.
 *  @date       09/01/04.
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
#ifndef     _CAMERA_MANIPULATOR_H_
#define     _CAMERA_MANIPULATOR_H_


#include "Manipulator/Manipulator.h"
#include "Input/InputState.h"


class CameraProperties;


/**
 *  CameraManipulator class to manipulate a camera entity
 *  from the keyboard and mouse input received.
 *  @brief  CameraManipulator base class.
 *  @author Marco Arsenault.
 *  @date   09/01/04.
 */
class CAMERATOOL_API CameraManipulator : public Manipulator
{
    CLASS_DISABLE_COPY(CameraManipulator);

public:
    //! Default constructor.
	CameraManipulator();

    //! Destructor.
    virtual ~CameraManipulator();

    //! Activate.the manipulator.
    virtual void Activate();

    //! Deactivate.the manipulator.
    virtual void Deactivate();

    //! Returns the activation input state.
    virtual const InputState& GetActivationInputState() const;
    
    //! Returns the deactivation input state.
    virtual const InputState& GetDeactivationInputState() const;

    //! Returns true if the manipulator supports the given object. false otherwise.
    virtual Bool CanManipulate(EntityProperties* pEntityProperties) const;
    
    //! Sets the manipulated object that will be converted to a Camera3DProperties.
    virtual void SetManipulatedEntity(EntityProperties* pEntityProperties);

    /**
     *  Called when the mouse is moved.
     *  @param  pRelX   Mouvement on the X axis, relative to the last position of the mouse.
     *  @param  pRelY   Mouvement on the Y axis, relative to the last position of the mouse.
     */
    virtual void OnMove( Int32 pRelX, Int32 pRelY );
    
    //! Called when the given key is down.
    virtual void OnKeyDown( const Keyboard::Key& pKey );

private:
    CameraProperties*   mCameraProperties;
    InputState          mActivationInputState;
    InputState          mDeactivationInputState;
};


#endif  //  _CAMERA_MANIPULATOR_H_