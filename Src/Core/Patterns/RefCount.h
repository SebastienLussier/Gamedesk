/**
*  @file       RefCount.h
*  @brief	   Reference counted class.
*  @author     S�bastien Lussier.
*  @date       31/07/05.
*/
/*
*  Copyright (C) 2005 Gamedesk
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
#ifndef     _REF_COUNT_H_
#define     _REF_COUNT_H_


namespace Gamedesk {


class RefCounted
{
    RefCounted() :
        mRefCount(1)
    {
    }

    virtual ~RefCounted()
    {
        GD_ASSERT(mCount==0);
    }

    void AddRef()
    {
        mCount++;
    }

    Bool RemoveRef()
    {
        mCount--;
        return mCount == 0;
    }
    UInt32  mCount;
};


} // namespace Gamedesk


#endif  //  _REF_COUNT_H_
