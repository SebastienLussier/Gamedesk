/**
 *  @file       Manipulator.h
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
#ifndef     _MANIPULATOR_H_
#define     _MANIPULATOR_H_


#include "Input/InputState.h"


namespace GameEdit
{
    /**
     *  Manipulator base class.  Manipulators are used to manipulate entities in the 
     *  3d scene.  Other manipulator class should derive from this class.
     *  Each manipulator is responsible to keep their manipulated object.
     *  @brief  Manipulator base class.
     *  @author Marco Arsenault.
     *  @date   29/01/04.
     */
    public ref class Manipulator
    {
    public:
        //! Default constructor.
	    Manipulator();

        //! Destructor.
        virtual ~Manipulator();

        //! Activate.the manipulator.
        virtual void Activate();

        //! Deactivate.the manipulator.
        virtual void Deactivate();

        //! Returns a bool indicating whether the manipulator is activated or not.
        Bool IsActivated() const;

        virtual void Render() const {}
        
        //! Returns the activation input state.
        virtual const InputState& GetActivationInputState() const = 0;

        //! Returns the deactivation input state.
        virtual const InputState& GetDeactivationInputState() const = 0;

        //! Returns true if the manipulator supports the given entities. false otherwise.
        virtual Bool CanManipulate(Entity* pEntity) const = 0;

	    //! Returns true if the manipulator supports the given entities. false otherwise.
        virtual Bool CanManipulate(const std::list<Entity*>& pEntities) const;

        /**
         *  Sets the manipulated entity.  It is the concrete manipulator's
         *  responsability to keep the manipulated entity.
         *  @brief Sets the manipulated entity.
         */
        virtual void SetManipulatedEntity(Entity* pEntity) = 0;

	    /**
         *  Sets the manipulated entities.  It is the concrete manipulator's
         *  responsability to keep the manipulated entities.
         *  @brief Sets the manipulated entities.
         */
        virtual void SetManipulatedEntity(const std::list<Entity*>& pEntities);

	    //! Tells the manipulator if he has the focus or not.
	    void SetFocus(Bool pHasFocus);

	    //! Returns whether the manipulator has the focus or not.
	    Bool HasFocus() const;

    private:
        Bool    mActivated;
	    Bool	mHasFocus;
    };
}

#endif  //  _MANIPULATOR_H_
