/**
 *  @file       VertexBuffer.cpp
 *  @brief	    Vertex data buffer to be sent to the graphic card.
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
#include "Engine.h"
#include "VertexBuffer.h"


namespace Gamedesk {
	
	
IMPLEMENT_ABSTRACT_CLASS(VertexBuffer);


VertexBuffer::VertexBuffer() :
    mUsage(Usage_Static),
    mItemCount(0),
    mItemSize(0)
{        
}

void VertexBuffer::Create( UInt32 pItemCount, UInt32 pItemSize, VertexBuffer::Usage pUsage )
{
    mItemCount = pItemCount;
    mItemSize  = pItemSize;
    mUsage     = pUsage;
}

void VertexBuffer::Destroy()
{
    mItemCount = 0;
    mItemSize  = 0;
    mUsage     = Usage_Static;
}

UInt32 VertexBuffer::GetItemCount()
{
    return mItemCount;
}

UInt32 VertexBuffer::GetItemSize()
{
    return mItemSize;
}

VertexBuffer::Usage VertexBuffer::GetUsage()
{
    return mUsage;
}


} // namespace Gamedesk
