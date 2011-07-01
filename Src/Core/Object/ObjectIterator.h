/**
 *  @file       ObjectIterator.h
 *  @brief      TODO.
 *  @author     Sébastien Lussier.
 *  @date       28/02/02.
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
#ifndef     _OBJECT_ITERATOR_H_
#define     _OBJECT_ITERATOR_H_


namespace Gamedesk {


/**
 *  Iterator class used to iterate trough all object of a subclass of Object.
 *  @brief  Object list iterator.
 *  @author Sebastien Lussier.
 *  @date   10/03/02.
 */
template <class T> class CORE_API ObjectIterator
{
public:
    /**
     *  Default constructor. Will initialize the iterator to the first object of type T.
     *  @brief  Constructor.
     */
    ObjectIterator() : mObject( Object::GetFirstObject() ), mValid(false)
    {
        if( mObject && mObject->IsA( T::StaticClass() ) )
            mValid = true;
        else
            ++(*this);
    }

    /**
     *  Look for the next object of type T in the object list.
     *  @brief  Look for the next object of type T in the object list.
     *  @return A reference to this iterator.
     */
    ObjectIterator& operator ++ ()
    {
        mValid = false;

        if( mObject )
            mObject = mObject->GetNextObject();

        while( mObject && !mValid )
        {
            if( mObject->IsA( T::StaticClass() ) )
                mValid = true;
            else
                mObject = mObject->GetNextObject();
        }

        return *this;
    }

    /**
     *  Test if this iterator is still valid.
     *  @brief  Test if this iterator is still valid.
     *  @return \b True if the iterator still point at a valid Object.
     */
    Bool IsValid() const
    {
        return mObject && mValid;
    }

    /**
     *  Test if this iterator is still valid.
     *  @brief  Test if this iterator is still valid.
     *  @return \b True if the iterator still point at a valid Object.
     */
    operator Bool() const
    {
        return IsValid();
    }

    /**
     *  Get the object pointed by the iterator.
     *  @brief  Get the object pointed by the iterator.
     *  @return A pointer to the object pointed by the iterator.
     */
    T* operator* ()   const     { return Cast<T>(mObject); }

private:
    Object*     mObject;        //!< Content of the iterator.
    Bool        mValid;         //!< \b True if the iterator is valid, \b false otherwise.
};


} // namespace Gamedesk


#endif  //  _OBJECT_ITERATOR_H_
