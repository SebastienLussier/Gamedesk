/**
 *  @file       OGLIndexBuffer.h
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
#ifndef     _OGL_INDEX_BUFFER_H_
#define     _OGL_INDEX_BUFFER_H_


#include "Graphic/Buffer/IndexBuffer.h"


namespace Gamedesk {
	
	
/**
 *  OpenGL index buffer class that use the ARB_vertex_buffer_object extension.
 */
class OGLGRAPHIC_API OGLHWIndexBuffer : public IndexBuffer
{
    DECLARE_CLASS(OGLHWIndexBuffer,IndexBuffer);

public:
    //! Constructor.
    OGLHWIndexBuffer();

    //! Destructor.
    virtual ~OGLHWIndexBuffer();

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
    virtual void* Lock( IndexBuffer::LockType pLockType );

    /**
     *  Release the lock on a previously locked buffer.
     */
    virtual void Unlock();

public:
    /** 
     *  Get the OpenGL buffer ID for this index buffer.
     *  @return     The buffer ID.
     */
    GLuint GetBufferID();

private:
    GLuint          mBufferID;      //!< The OpenGL buffer ID.
};


} // namespace Gamedesk


#endif  //  _OGL_INDEX_BUFFER_H_
