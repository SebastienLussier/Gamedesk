/**
 *  @file       SoftwareVertexBuffer.h
 *  @brief	    Software Vertex data buffer to be sent to the graphic card.
 *  @author     Sébastien Lussier.
 *  @date       11/09/2008.
 */
/*
 *  Copyright (C) 2008 Gamedesk
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
#ifndef     _SOFTWARE_VERTEX_BUFFER_H_
#define     _SOFTWARE_VERTEX_BUFFER_H_


#include "Graphic/Buffer/VertexBuffer.h"


namespace Gamedesk {


/**
 *  Software VertexBuffer class
 */
class ENGINE_API SoftwareVertexBuffer : public VertexBuffer
{
    DECLARE_CLASS(SoftwareVertexBuffer,VertexBuffer);

public:
    //! Constructor.
    SoftwareVertexBuffer();

    //! Destructor.
    virtual ~SoftwareVertexBuffer();

    /**
     *  Create a vertex buffer by specifying it's properties.
     *  @param  pItemCount  The number of items this buffer will contain.
     *  @param  pItemSize   The size of each item, in bytes.
     *  @param  pUsage      Define the way this buffer will be used.
     */
    virtual void    Create( UInt32 pItemCount, UInt32 pItemSize, Usage pUsage = Usage_Static );

    /**
     *  Destroy the buffer, releasing the memory associated to it.
     */
    virtual void    Destroy();

    /**
     *  Provides a pointer corresponding to the memory mapped area of this buffer.
     *  @param  pLockType   Specify the kind of lock that should be applied on the buffer.
     *  @return A pointer to the mapped area of memory for this buffer, of (GetItemCount()*GetItemSize()) bytes.
     */
    virtual void*   Lock( LockType pLockType );

    /**
     *  Release the lock on a previously locked buffer.
     */
    virtual void    Unlock();

public:
    /**
     *  Get a pointer to the buffer memory.
     *  @return     An array of bytes (itemsize * itemcount bytes).
     */
    void*   GetData();
    
private:
    Byte*   mData;
};


} // namespace Gamedesk


#endif  //  _SOFTWARE_VERTEX_BUFFER_H_
