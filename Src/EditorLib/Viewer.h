/**
 *  @file       Viewer.h
 *  @brief      A viewer base class for the editor.
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
#ifndef     _VIEWER_H_
#define     _VIEWER_H_


#include "EditorLib.h"
#include "QRenderWindow.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Containers/Containers.h"


class Manipulator;


/**
 *  Viewer class.  This class is used to manage manipulator activation/deactivation.
 *  @brief  Viewer class.
 *  @author Marco Arsenault.
 *  @date   31/01/04.
 */
class EDITORLIB_API Viewer : public QRenderWindow, 
                             public Keyboard::Listener, public Mouse::Listener
{
    CLASS_DISABLE_COPY(Viewer);

public:
    //! Constructor.
    Viewer( QWidget* pParent, RenderListener* pRenderListener );

    //! Destructor.
    virtual ~Viewer();

    virtual void Init();
    virtual void Kill();

    //! Add a manipulator.
    void AddManipulator(Manipulator* pManipulator);

    //! Remove a manipulator.
    void RemoveManipulator(Manipulator* pManipulator);

    //! Returns the active manipulator, NULL if there are none.
    Manipulator* GetActiveManipulator();

    //! Called when the given key is pressed.
    virtual void OnKeyPressed( const Keyboard::Key& pKey );

    //! Called when the given key is released.
    virtual void OnKeyReleased( const Keyboard::Key& pKey );

     //! Called when the given mouse button is pressed.
    virtual void OnMouseButtonPressed( const Mouse::Button& pButton );

    //! Called when the given mouse button is released.
    virtual void OnMouseButtonReleased( const Mouse::Button& pButton );
   
private:
    //! Activate a manipulator.
    void ActivateManipulator(Manipulator* pManipulator);
    //! Deactivate a manipulator.
    void DeactivateManipulator(Manipulator* pManipulator);

	//! Returns whether the given point is inside the viewer.
	Bool IsPointOnViewer(Int32 pX, Int32 pY) const;

    std::list<Manipulator*> mManipulators;
    Manipulator*            mActiveManipulator;

	Bool					mKeyPressedOnViewer;
	Bool					mButtonPressedOnViewer;
};


#endif  //  _VIEWER_H_
