/**
 *  @file       SoftwareIndexBuffer.h
 *  @brief	    Software Polygon index data buffer to be sent to the graphic card.
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
#include "Engine.h"
#include "Graphic/Buffer/SoftwareIndexBuffer.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(SoftwareIndexBuffer);


SoftwareIndexBuffer::SoftwareIndexBuffer() :
    mData(NULL)
{
}

SoftwareIndexBuffer::~SoftwareIndexBuffer()
{
    Destroy();
}

void SoftwareIndexBuffer::Create( UInt32 pItemCount, UInt32 pItemSize, IndexBuffer::Usage pUsage )
{
    Super::Create( pItemCount, pItemSize, pUsage );

    mData = GD_NEW_ARRAY(Byte, mItemSize * mItemCount, this, "OGLGraphic::SoftwareIndexBuffer");
}

void SoftwareIndexBuffer::Destroy()
{
    if( mData )
    {
        GD_DELETE_ARRAY(mData);
        mData = NULL;
    }

    Super::Destroy();
}

void* SoftwareIndexBuffer::Lock( IndexBuffer::LockType /*pLockType*/ )
{
    return mData;
}

void SoftwareIndexBuffer::Unlock()
{
}

void* SoftwareIndexBuffer::GetData()
{
    return mData;
}


} // namespace Gamedesk
