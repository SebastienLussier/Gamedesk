using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GDEdit.Manipulators
{
    class Manipulator
    {
        //! Returns a bool indicating whether the manipulator is activated or not.
        public bool Activated
        {
            get
            {
                return mActivated;
            }
        }

        //! Returns whether the manipulator has the focus or not.
	    public bool HasFocus
        {
            get
            {
                return mHasFocus;
            }
        }

        //! Destructor.
        virtual ~Manipulator();

        //! Activate.the manipulator.
        public virtual void Activate()
        {
            mActivated = true;
        }

        //! Deactivate.the manipulator.
        public virtual void Deactivate()
        {
            mActivated = false;
        }

        virtual void Render()
        {
        }
        
        //! Returns the activation input state.
        virtual const InputState& GetActivationInputState() const = 0;

        //! Returns the deactivation input state.
        virtual const InputState& GetDeactivationInputState() const = 0;

        //! Returns true if the manipulator supports the given entities. false otherwise.
        virtual bool CanManipulate(Entity* pEntity) const = 0;

	    //! Returns true if the manipulator supports the given entities. false otherwise.
        virtual bool CanManipulate(const std::list<Entity*>& pEntities) const;

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

	    

        private bool mActivated;
	    private bool mHasFocus;
    }
}
