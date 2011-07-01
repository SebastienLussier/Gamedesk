/**
 *  @file       IndexBuffer.h
 *  @brief	    Polygon index data buffer to be sent to the graphic card.
 *  @author     Sébastien Lussier.
 *  @date       07/01/04.
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
#ifndef     _INDEX_BUFFER_H_
#define     _INDEX_BUFFER_H_


#include "Resource/Resource.h"


namespace Gamedesk {


/**
 *  IndexBuffer are memory buffers that contain index data.
 *  Index data, or indices, are integer offsets into vertex buffers and are
 *  used to render primitives using the Renderer::DrawIndexedPrimitive method.
 *
 *  To obtain a new IndexBuffer, call the GraphicSubsystem::Create method
 *  by specifying IndexBuffer::StaticClass() as the resource class.
 *
 *  @see GraphicSubsystem::Create().
 *  @see Renderer::SetIndices().
 *  @see Renderer::DrawIndexedPrimitive().
 *
 *  @author Sébastien Lussier.
 *  @date   21/01/04
 */
class ENGINE_API IndexBuffer : public Resource
{
    DECLARE_ABSTRACT_CLASS(IndexBuffer,Resource);

public:
    //! Define the way this buffer is going to be used in is lifetime.
    enum Usage
    {
        Usage_Static,   //!< Specified once and used multiple time for rendering.
        Usage_Dynamic,  //!< Updated frequently, but used multiple time for rendering.     
        Usage_Stream    //!< Updated each time it is used for rendering.
    };

    //! Specify which kind of lock will be performed on the buffer.
    enum LockType
    {
        Lock_Read,      //!< We want to read data from the buffer (can be slow).
        Lock_Write,     //!< We want to write data to the buffer.
        Lock_Full       //!< We want to read and also write data to the buffer (can also be slow).
    };
    
public:
    /**
     *  Create an index buffer by specifying it's properties.
     *  @param  pItemCount  The number of indices this buffer will contain.
     *  @param  pItemSize   The size of each item, in bytes.
     *  @param  pUsage      Define the way this buffer will be used.
     */
    virtual void Create( UInt32 pItemCount, UInt32 pItemSize, Usage pUsage = Usage_Static );

    /**
     *  Destroy the buffer, releasing the memory associated to it.
     */
    virtual void Destroy();

    /**
     *  Provides a pointer corresponding to the memory mapped area of this buffer.
     *  @param  pLockType   Specify the kind of lock that should be applied on the buffer.
     *  @return A pointer to the mapped area of memory for this buffer, of (GetItemCount()*GetItemSize()) bytes.
     */
    virtual void* Lock( IndexBuffer::LockType pLockType ) = 0;

    /**
     *  Release the lock on a previously locked buffer.
     */
    virtual void Unlock() = 0;

    /**
     *  Get the usage of this index buffer.
     *  @return The usage of this buffer.
     */
    Usage GetUsage();

    /**
     *  Get the number of indices in the buffer.
     *  @return The total number of indices in the buffer.
     */
    UInt32 GetItemCount();

    /**
     *  Get the size of each indices contained in the buffer.
     *  @return The size of each indices, in bytes.
     */
    UInt32 GetItemSize();    

protected:
    //! Constructor.
    IndexBuffer();
    
protected:
    Usage           mUsage;         //!< The usage of this index buffer.
    UInt32          mItemCount;     //!< The maximum number of indices that can fit in the buffer.
    UInt32          mItemSize;      //!< The size of each indices, in bytes.
};


} // namespace Gamedesk


#endif  //  _INDEX_BUFFER_H_
