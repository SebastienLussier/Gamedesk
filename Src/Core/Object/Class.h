/**
 *  @file       Class.h
 *  @brief      Class description associated to each classes (reflective factory).
 *  @author     Sébastien Lussier.
 *  @date       19/01/02.
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
#ifndef     _CLASS_H_
#define     _CLASS_H_


#include "Exception/Exception.h"
#include "Property.h"


namespace Gamedesk {


/**
 *  This class is an important part of the reflective factory design pattern.
 *  Every class deriving from Object has a static Class object defined for it,
 *  by the use of the <b>DECLARE_CLASS( Class, ParentClass )</b> and
 *  <b>IMPLEMENT_CLASS( Object )</b> macros.  You can look at the code in
 *  ObjectBasic.h to see how these macro are expended at compile time.
 *  @brief  Describe a class derived from Object.
 *  @author Sébastien Lussier.
 *  @date   19/01/02.
 */
class CORE_API Class : public Object
{
    DECLARE_CLASS(Class, Object);
    
public:
    /**
     *  Iterator through all classes derived from a given base class.	
     */
    class CORE_API Iterator
    {
    public:
        /**
         *  Constructor.
         *  @param  pBaseClass  All classes derived from this class will be listed.
         */
        Iterator( Class* pBaseClass ) 
            : mBaseClass( pBaseClass )
            , mCurrentClass( NULL )
        {
            ++(*this);
        }

        /**
         *  Look for the next class object derived from the base class.
         *  @return A reference to this iterator.
         */
        Iterator& operator ++ ()
        {
            mCurrentClass = NULL;

            while( mClassIterator.IsValid() && mCurrentClass == NULL )
            {
                if( (*mClassIterator)->IsDerivedFrom( mBaseClass ) )
                    mCurrentClass = (*mClassIterator);

                ++mClassIterator;
            }

            return *this;
        }

        /**
         *  Test if this iterator is still valid.
         *  @return \b True if the iterator still point at a valid Class.
         */
        Bool IsValid() const
        {
            return mCurrentClass != NULL;
        }

        /**
         *  Test if this iterator is still valid.
         *  @return \b True if the iterator still point at a valid Class.
         */
        operator Bool() const
        {
            return IsValid();
        }

        /**
         *  Get the Class pointed by the iterator.
         *  @return A reference to the Class pointed by the iterator.
         */
        Class* operator* () const
        {
            return mCurrentClass;
        }

    protected:
        ObjectIterator<Class>   mClassIterator;     //!< Used to iterate through all Class objects.
        Class*                  mBaseClass;         //!< Base class of classes pointed by this iterator.
        Class*                  mCurrentClass;      //!< Class currently pointed by the iterator.
    };

public:
    /**
     *  Default constructor, should not be used.
     *  @brief  Default constructor.
     */
    Class();

    /**
     *  Constructor used by the IMPLEMENT_CLASS macro.
     *  @brief  Constructor.
     *  @param  pStrName        a string, the name of the class.
     *  @param  pClsSuper       a Class pointer, the parent Class object.
     *  @param  pClassSize      the size of the class in bytes.
     *  @param  pFuncClassNew   a pointer to the static method used to instantiate object of this class.
     *  @param  pFuncRegisterProperties     a pointer to the static method used to register all properties of this class.
     *  @param  pAbstract       a boolean, true if class is abstract (containing pure virtual methods).
     */
    Class( const Char* pStrName, Class* pClsSuper, UInt32 pClassSize, Object* (*pFuncClassNew)(), void (*pFuncRegisterProperties)(Class*) = NULL, Bool pAbstract = false );

    /**
     *  Test if this class is derived from the class passed in parameter.
     *  For example, Class::StaticClass()->IsDerivedFrom( Object::StaticClass() )
     *  would return \b true.
     *  @brief  Test if this class derive from inClass.
     *  @param  pClass  a Class pointer, the possible parent class.
     *  @return \bTrue if this class derive from \binClass, \bfalse otherwise.
     */
    Bool IsDerivedFrom( Class* pClass ) const;

    /**
     *  Return true if this class is abstract (i.e. if this class containg pure virtual methods).
     *  @brief  Is this class abstract ?
     *  @return \b True if this class is abstract, \b false otherwise.
     */
    INLINE Bool IsAbstract() const  { return mAbstract; }

    /**
     *  Get the parent class Class object.  If used on the Object class, will
     *  return \b NULL.
     *  @brief  Get the parent Class object.
     *  @return A Class pointer, the parent class.
     */
    INLINE Class* GetSuper() const    { return mSuperClass; }

    /**
     *  Allocate a new object of the type of this class, on the heap.
     *  @brief  Allocate an object of this class.
     *  @return An Object pointer, the newly allocated object casted to an Object*.
     */
    INLINE Object* AllocateNew( const String& pName = "" ) const
    {
        Object* newObj = mFuncClassNew();
        
        if( newObj )
            newObj->SetName( pName );

        return newObj;
    }
    
    /**
     *  Look in the list of classes to find a Class matching the name given in
     *  parameter.  If no matching class is found, return \b NULL.
     *  @brief  Find a class object by it's name.
     *  @param  pStrName    a string, the name of the class to look for.
     *  @return A Class pointer, the Class matching the name, or \b NULL if no class was found.
     */
    static Class* GetClassByName( const Char* pStrName );

#if GD_CFG_USE_PROPERTIES == GD_ENABLED
    class PropertyIterator
    {
    public:
        PropertyIterator( Class* pClass )
            : mClass(pClass)
            , mCurrentClass(pClass)
        {
            if( mClass )
            {
                while(mCurrentClass->mSuperClass != NULL) 
                    mCurrentClass = mCurrentClass->mSuperClass;

                mIt = mCurrentClass->mProperties.begin();
                SwitchClass();
            }
        }

        Bool IsValid() const
        {
            return mCurrentClass && mIt != mCurrentClass->mProperties.end();
        }

        operator Bool () const
        {
            return IsValid();
        }

        PropertyIterator& operator ++ ()
        {
            GD_ASSERT_EX(mCurrentClass);

            ++mIt;
            SwitchClass();

            return *this;
        }

        Property* operator* () const     
        { 
            GD_ASSERT_EX(IsValid());
            return *mIt;
        }

    private:
        void SwitchClass()
        {
            // If we've reached the end of this class properties... go to the next class in the hierarchy
            // (starting from the base class up to the actual class)
            while( mIt == mCurrentClass->mProperties.end() && mCurrentClass != mClass )
            {    
                Class* currentClass = mCurrentClass;
                mCurrentClass = mClass;

                while(mCurrentClass->mSuperClass != currentClass) 
                    mCurrentClass = mCurrentClass->mSuperClass;

                mIt = mCurrentClass->mProperties.begin();
            }
        }

    private:
        Class*                      mClass;
        Class*                      mCurrentClass;
        Vector<Property*>::iterator mIt;
    };

    UInt32 GetPropertyCount() const
    {
        UInt32 count = (UInt32)mProperties.size();
        if( mSuperClass )
            count += mSuperClass->GetPropertyCount();

        return count;
    }

    const Property* GetProperty( const Char* pPropertyName ) const
    {
        const Property* prop = mSuperClass ? mSuperClass->GetProperty(pPropertyName) : NULL;

        if( !prop )
        {
            for( Vector<Property*>::const_iterator it = mProperties.begin(); it != mProperties.end(); ++it )
            {
                if( strcmp((*it)->GetName(), pPropertyName) == 0 )
                    return (*it);
            }
        }

        return NULL;
    }

    Property* GetProperty( const Char* pPropertyName )
    {
        Property* prop = mSuperClass ? mSuperClass->GetProperty(pPropertyName) : NULL;

        if( !prop )
        {
            for( Vector<Property*>::iterator it = mProperties.begin(); it != mProperties.end(); ++it )
            {
                if( strcmp((*it)->GetName(), pPropertyName) == 0 )
                    return (*it);
            }
        }

        return NULL;
    }

    void AddProperty( Property* pProperty )
    {
        GD_ASSERT_EX(pProperty != NULL);
        mProperties.push_back(pProperty);
    }
#endif

protected:
    Bool                mAbstract;              //!< Is this class abstract ?
    Class*              mSuperClass;            //!< The super class Class object.
    UInt32              mClassSize;             //!< The class size, in bytes.
    Object*             (*mFuncClassNew)();     //!< Function pointer used for instantiation of new objects.

#if GD_CFG_USE_PROPERTIES == GD_ENABLED
    Vector<Property*>   mProperties;
#endif
};


class CORE_API ClassNotFoundException : public Exception
{
    DECLARE_EXCEPTION(ClassNotFoundException);
    
public:
    ClassNotFoundException( const Char* pStrClassName, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage  = "Class not found : \"";
        mMessage += pStrClassName;
        mMessage += "\".";
        DebugOut();
    }
};


class CORE_API InvalidClassException : public Exception
{
    DECLARE_EXCEPTION(InvalidClassException);
    
public:
    InvalidClassException( Class* pGiven, Class* pWanted, CodeLocation pLoc ) : Exception( pLoc )
    {
        if( pWanted )
        {
            mMessage  = "Invalid class : excepted ";
            mMessage += pWanted->GetName();
            mMessage += ", given ";
            mMessage += pGiven->GetName();
            mMessage += ".";
        }
        else
        {
            mMessage  = "Invalid class \"";
            mMessage += pGiven->GetName();
            mMessage += "\".";
        }
        DebugOut();
    }
};


class CORE_API InvalidCastException : public Exception
{
    DECLARE_EXCEPTION(InvalidCastException);
    
public:
    InvalidCastException( Object* pObject, Class* pWantedClass, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage  = "Invalid object cast!.\nObject class : ";
        mMessage += pObject->GetClass()->GetName();
        mMessage += "\nTrying to cast into : ";
        mMessage += pWantedClass->GetName();
        mMessage += ".";
        DebugOut();
    }
};


/**
 *  Try to cast an Object pointer into another class.
 *  This cast is completely safe due to the use of the StaticClass() test.
 *  @brief  Safely cast an Object into a class derived from Object.
 *  @param  inObj   the object to cast.
 *  @return NULL if the cast was invalid, or the casted object.
 */
template <class T> INLINE T* Cast( Object* pObject )
{
    GD_ASSERT_EX( pObject != NULL );

    if( pObject->IsA(T::StaticClass()) )
        return reinterpret_cast<T*>(pObject);
    else
        throw InvalidCastException(pObject, T::StaticClass(), Here);
}


} // namespace Gamedesk


#endif  //  _CLASS_H_
