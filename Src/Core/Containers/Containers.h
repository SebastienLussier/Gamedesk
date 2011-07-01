/**
 *  @file       Containers.h
 *  @brief	    Definition of Core Container Classes (STL wrappers for now).
 *  @author     Patrick Girard.
 *  @date       26/08/02.
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
#ifndef     _CONTAINERS_H_
#define     _CONTAINERS_H_


///////////////////////////////////////////////////////////////////////////////
// STL includes
#include <iostream>
#include <iomanip>

#include <algorithm>
#include <list>
#include <vector>
#include <map>
#include <stack>
#include <set>
#include <cctype>
#include <iterator>

#include <fstream>


#include "Core.h"


namespace Gamedesk {


///////////////////////////////////////////////////////////////////////////////
// STL derived classes.


/**
 *  A template vector class based on the STL std::vector class.
 *  @brief  A template vector class.  See this doc for more info.
 *  @author Patrick Girard.
 *  @date   26/08/02.
 */
template < class T, class A = std::allocator<T> >
class Vector : public std::vector<T,A>
{
public:
    virtual ~Vector() {}
    
    Vector<T,A>& append( const Vector<T,A>& pOther )
    {
        reserve( this->size() + pOther.size() );
        
        for( typename Vector::const_iterator itOther = pOther.begin(); itOther != pOther.end(); ++itOther )
            this->push_back(*itOther);

        return *this;
    }

    /**
     *  Serialize the content of this vector to/from a stream.
     *  @param  pStream Stream used for serialization.
     *  @return The stream object.
     */
    friend Stream& operator << ( Stream& pStream, Vector& pVector )
    {
        UInt32 num = pVector.size();
        
        pStream << num;

        if( pStream.In() )
        {
            pVector.resize( num );
        }
        
        for( typename Vector::iterator it = pVector.begin(); it != pVector.end(); ++it )
        {
            pStream << *it;
        }
        
        return pStream;
    }
};


template < class T, class A = std::allocator<T> >
class List : public std::list<T,A>
{
    /**
    *  Serialize the content of this list to/from a stream.
    *  @param  pStream Stream used for serialization.
    *  @return The stream object.
    */
    friend Stream& operator << ( Stream& pStream, List& pList )
    {
        UInt32 num = pList.size();

        pStream << num;

        if( pStream.In() )
        {
            pList.resize( num );
        }

        for( typename List::iterator it = pList.begin(); it != pList.end(); ++it )
        {
            pStream << *it;
        }

        return pStream;
    }
};


/**
 *  A template map class based on the STL std::map class.
 *  @brief  A template map class.  See this doc for more info.
 *  @author Patrick Girard.
 *  @date   26/08/02.
 */
template < class Key, class Value, class Compare = std::less<Key>,
           class Allocator = std::allocator<std::pair<const Key, Value> > >
class Map : public std::map<Key, Value, Compare, Allocator>
{
};



/**
*  A template map class based on the STL std::multimap class.
*  @brief  A template multimap class.  See this doc for more info.
*  @author Sebastien Lussier.
*  @date   12/12/05.
*/
template < class Key, class Value, class Compare = std::less<Key>,
class Allocator = std::allocator<std::pair<const Key, Value> > >
class MultiMap : public std::multimap<Key, Value, Compare, Allocator>
{
};


} // namespace Gamedesk


#endif  //  _CONTAINERS_H_
