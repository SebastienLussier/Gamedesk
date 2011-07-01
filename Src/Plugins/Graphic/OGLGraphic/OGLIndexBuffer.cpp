/**
 *  @file       OGLIndexBuffer.cpp
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
#include "OGLGraphic.h"
#include "OGLIndexBuffer.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(OGLHWIndexBuffer);


const GLenum    GDToGLIndexBufferUsage[] =
                    {
                        GL_STATIC_DRAW_ARB,     // Usage_Static
                        GL_DYNAMIC_DRAW_ARB,    // Usage_Dynamic
                        GL_STREAM_DRAW_ARB      // Usage_Stream
                    };

const GLenum    GDToGLIndexBufferLockType[] =
                    {
                        GL_READ_ONLY_ARB,       // Lock_Read
                        GL_WRITE_ONLY_ARB,      // Lock_Write
                        GL_READ_WRITE_ARB       // Lock_Full
                    };


OGLHWIndexBuffer::OGLHWIndexBuffer() :
    mBufferID(NULL)
{
}

OGLHWIndexBuffer::~OGLHWIndexBuffer()
{
    Destroy();
}

void OGLHWIndexBuffer::Create( UInt32 pItemCount, UInt32 pItemSize, IndexBuffer::Usage pUsage )
{
    Super::Create( pItemCount, pItemSize, pUsage );

    // Allocate a Buffer object.
    glGenBuffersARB( 1, &mBufferID );                       

    // Set the actual buffer to use.
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferID );      

    // Setup the usage and amount of memory this vertex buffer will need.
    glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mItemSize * mItemCount, NULL, GDToGLIndexBufferUsage[mUsage] );
}

void OGLHWIndexBuffer::Destroy()
{
    if( mBufferID )
    {
        glDeleteBuffersARB( 1, &mBufferID );
        mBufferID = NULL;
    }

    Super::Destroy();
}

void* OGLHWIndexBuffer::Lock( IndexBuffer::LockType pLockType )
{
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferID );  
    return glMapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GDToGLIndexBufferLockType[pLockType] );
}

void OGLHWIndexBuffer::Unlock()
{
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mBufferID );     
    glUnmapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB );
}

GLuint OGLHWIndexBuffer::GetBufferID()
{
    return mBufferID;
}


} // namespace Gamedesk
