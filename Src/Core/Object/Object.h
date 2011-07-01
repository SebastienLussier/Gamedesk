/**
 *  @file       Object.h
 *  @brief      Abstract base class for the object hierarchy.
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
#ifndef     _OBJECT_H_
#define     _OBJECT_H_


#include "ObjectBasic.h"


namespace Gamedesk {


class Class;
class ObjectProperties;


/**
 *  Core of our object hierarchy, the Object class is an abstract class that defines
 *  a lot of very useful methods.
 *  @brief  Base class of GameDesk class hierarchy.
 *  @author Sebastien Lussier.
 *  @date   19/01/02.
 *  @todo   Write documentation on how to use the Object class.
 */
class CORE_API Object
{
    DECLARE_OBJECT_ABSTRACT_CLASS( Object );

public:
    enum ObjectFlags
    {
        OBJ_Internal = 0x00000001,
        OBJ_External = 0x00000002
    };

public:
    //! Destructor.
    virtual ~Object();

public:
    /**
     *  Do a proper initialization of an object so that any
     *  Object can be reused for memory optimizations.
     *  @brief  Do a proper initialization of an existing object.
     */
    virtual void Init();

    /**
     *  Clean the object without deleting it so that it can be
     *  reused for memory optimizations.
     *  @brief  Clean object without deleting it.
     */
    virtual void Kill();

    /**
     *  Used to determinate if this object is of a given class.
     *  @brief  Is this an object of a given class ?
     *  @param  inClass     a Class pointer, is this object an object of this class ?
     *  @return A boolean, \b true if this is an object of class \a inClass, \b false otherwise.
     */
    Bool IsA( Class* inClass ) const;

    /**
     *  Used by the serialization system to know which class to serialize.
     *  Used as a trick to serialize resource objects independently of the subsystem.
     *  (no subsystem specific serialization should occur).
     */
    virtual Class* SerializeAs()
    {
        return GetClass();
    }

    /**
     *  Serialize this object to/from a stream (ex. file, network, etc.)
     *  @brief  Save/Load this object from a stream.
     *  @param  pStream    a Stream derived class, from/to which the object will be serialized.
     *  @return A reference to the stream.
     */
    virtual void Serialize( Stream& /*pStream*/ ){}

    /**
     *  Name
     */
    void            SetName( const String& pName );
    const String&   GetName() const;

    /**
     *  Owner
     */
    void        SetOwner( Object* pOwner );
    Object*     GetOwner() const;
    Bool        IsOwnedBy( Object* pOwner );

    static Object* FindObject( const String& pName, const String& pOwnerName );
    
    class Package* GetPackage();

    /**
     *  Flags
     */
    void SetFlags( Bitfield pFlags )
    {
        mFlags |= pFlags;
    }

    void ClearFlags( Bitfield pFlags )
    {
        mFlags &= ~pFlags;
    }

    Bool HasFlags( Bitfield pFlags ) const
    {
        return (mFlags & pFlags) == pFlags;
    }

    // Object List Methods

    /**
     *  Get the first object in the list of allocated Object.
     *  @brief  Get the first object in the list of allocated Object.
     *  @return A pointer to the first Object in the list.
     */
    static  Object*             GetFirstObject() { return mFirstObject; }

    /**
     *  Get the last object in the list of allocated Object.
     *  @brief  Get the last object in the list of allocated Object.
     *  @return A pointer to the last Object in the list.
     */
    static  Object*             GetLastObject()  { return mLastObject; }

    /**
     *  Get the next object in the list of allocated Object.
     *  @brief  Get the next object in the list of allocated Object.
     *  @return A pointer to the next Object in the list.
     */
    Object*                     GetNextObject() const { return mNextObject; }

    /**
     *  Get the previous object in the list of allocated Object.
     *  @brief  Get the previous object in the list of allocated Object.
     *  @return A pointer to the previous Object in the list.
     */
    Object*                     GetPrevObject() const { return mPrevObject; }

protected:
    //! Default constructor.
    Object();

private:
    /**
     *  Add this object to the list of allocated Object. Must be used carefully.
     *  @brief  Add this object to the list of allocated Object.
     */
    void                        Link();

    /**
     *  Remove this object to the list of allocated Object. Must be used carefully.
     *  @brief  Remove this object to the list of allocated Object.
     */
    void                        UnLink();

protected:
    String                  mName;          //!< Name (as string) of this object.
    Object*                 mOwner;         //!< Owner object.
    Bitfield                mFlags;         //!< Object flags.
    
    // Object List Members
    static Object*          mFirstObject;   //!< First object in the list of allocated objects.
    static Object*          mLastObject;    //!< Last object in the list of allocated objects.
    Object*                 mNextObject;    //!< Next object in the list of allocated objects.
    Object*                 mPrevObject;    //!< Previous object in the list of allocated objects.
};


/**
 *  Try to cast an Object pointer into another class.
 *  This cast is completely safe due to the use of the StaticClass() test.
 *  @brief  Safely cast an Object into a class derived from Object.
 *  @param  inObj   the object to cast.
 *  @return NULL if the cast was invalid, or the casted object.
 */
template <class T> INLINE T* Cast( Object* pObject );


} // namespace Gamedesk


#endif  //  _OBJECT_H_
