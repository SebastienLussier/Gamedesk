/**
 *  @file       Mouse.h
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       05/01/04.
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
#ifndef     _MOUSE_H_
#define     _MOUSE_H_


#include "InputDevice.h"
#include "Maths/Vector2.h"



namespace Gamedesk {


/**
 *  Access information about the mouse state.
 */
class ENGINE_API Mouse : public InputDevice
{
    DECLARE_CLASS(Mouse,InputDevice);
    friend class InputSubsystem;
    
public:
    //! Buttons of the mouse.
    enum Button
    {
        Button_0            = 0,
        Button_1            = 1,
        Button_2            = 2,
        Button_3            = 3,
        Button_4            = 4,
        Button_5            = 5,
        Button_6            = 6,
        Button_7            = 7,
        Button_NumButtons   = 8,

        Button_Left         = Button_0,     //!< Left mouse button.
        Button_Right        = Button_1,     //!< Right mouse button.
        Button_Middle       = Button_2,     //!< Middle mouse button.
        
        Button_MaxValue     = 0xFFFFFFFE,
        Button_None
    };

    //! Axis of the mouse.
    enum Axis
    {
        Axis_X,                             //!< X axis.      
        Axis_Y,                             //!< Y axis.
        Axis_Z,                             //!< Z axis (usually the mouse wheel axis).
        Axis_NumAxis,
        Axis_MaxValue = 0xFFFFFFFF
    };
        
    //! State of a mouse button.
    enum ButtonState
    {
        Button_Up,                          //!< Button is up.
        Button_Pressed,                     //!< Button was up and is now down.
        Button_Down,                        //!< Button is down.
        Button_Released,                    //!< Button was down ans is now up.
        
        Button_NumStates,
        ButtonState_MaxValue = 0xFFFFFFFF
    };   
    
    /**
     *  Listener class used by other classes that wish to be notified of changes
     *  in the mouse status.
     */
    class ENGINE_API Listener
    {
    public:
        /**
         *  Called when the mouse is moved.
         *  @param  pRelX   Mouvement on the X axis, relative to the last position of the mouse.
         *  @param  pRelY   Mouvement on the Y axis, relative to the last position of the mouse.
         */
        virtual void OnMouseMove( Int32 pRelX, Int32 pRelY );

        //!  Called when the given mouse button is up.
        virtual void OnMouseButtonUp( const Button& pButton );

        //!  Called when the given mouse button is pressed.
        virtual void OnMouseButtonPressed( const Button& pButton );

        //!  Called when the given mouse button is down.
        virtual void OnMouseButtonDown( const Button& pButton );

        //!  Called when the given mouse button is released.
        virtual void OnMouseButtonReleased( const Button& pButton );
    };

public:
    //! Return the state of the given button.
    ButtonState GetState( const Button& pButton ) const;
    
    //! Return \btrue if the given button is down (or was pressed), \bfalse otherwise.
    Bool IsDown( const Button& pButton ) const;

    //! Return \btrue if the given button is up (or was released), \bfalse otherwise.
    Bool IsUp( const Button& pButton ) const;

    //! Return \btrue if the given button was pressed, \bfalse otherwise.
    Bool WasPressed( const Button& pButton ) const;

    //! Return \btrue if the given button was released, \bfalse otherwise.
    Bool WasReleased( const Button& pButton ) const;
        
    //! Get the relative position of the mouse on the X axis.
    Int32 GetRelX() const;

    //! Get the relative position of the mouse on the Y axis.
    Int32 GetRelY() const;

    //! Get the relative position of the mouse on the Z axis (mouse wheel).
    Int32 GetRelZ() const;

    //! Get the relative position of the mouse on the given axis.
    Int32 GetRelAxis( const Axis& pAxis ) const;

    //! Get the position of the mouse in screen coordinate.
    const Vector2i& GetPos() const;

    //! Get the position of the mouse in screen coordinate.
    void GetPos( Int32& pX, Int32& pY ) const;

    //! Add a mouse listener that will be notified of mouse movement.
    void AddMoveListener( Listener* pListener );

    //! Remove a mouse listener from the mouse move listeners.
    void RemoveMoveListener( Listener* pListener );

    //! Add a mouse listener that will be notified of a given button state.
    void AddButtonListener( Listener* pListener, const Button& pButton );

    //! Add a mouse listener that will be notified when a given button is in a given state.
    void AddButtonListener( Listener* pListener, const Button& pButton, const ButtonState& pState );

    //! Remove a mouse listener from a given mouse button listener list.
    void RemoveButtonListener( Listener* pListener, const Button& pButton );

    //! Remove a mouse listener from a given mouse button listener list.
    void RemoveButtonListener( Listener* pListener, const Button& pButton, const ButtonState& pState );

    //! Get the name of a button as a string.
    String GetButtonName( const Button& pButton ) const;

    //! Get the name of a state as a string.
    String GetStateName( const ButtonState& pState ) const;

    //! Get the name of an axis as a string.
    String GetAxisName( const Axis& pAxis ) const;
    
private:
    //! Private constructor.  Only the InputSubsystem is allowed to create a Mouse object.
    Mouse();

    //! Fire events (notify listeners of the changes in the mouse state).
    void FireEvents();

private:
    ButtonState             mButtonsStates[Button_NumButtons];          //!< State of each button.
    Int32                   mAxis[Axis_NumAxis];                        //!< Relative changes in position of each axis.
    Vector2i                mPosition;                                  //!< Current mouse position.

    std::list<Listener*>    mMoveListeners;                             //! Mouse movement listeners list.

    //! Button listeners list for each button possible state.
    std::list<Listener*>    mButtonsListeners[Button_NumButtons][Button_NumStates];     
};


} // namespace Gamedesk


#endif  //  _MOUSE_H_
