/**
 *  @file       Mutex.cpp
 *  @brief      Mutex synchronisation primitive for Win32.
 *  @author     Sébastien Lussier.
 *  @date       2008/10/17.
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
#include "Core.h"
#include "Thread/Mutex.h"


namespace Gamedesk {
	
	
Mutex::Mutex()
{
    mMutex = ::CreateMutex(NULL, FALSE, NULL);
    GD_ASSERT(mMutex);
}

Mutex::~Mutex()
{
    ::CloseHandle(mMutex);
}

void Mutex::Lock()
{
    DWORD result = ::WaitForSingleObject(mMutex, INFINITE);
    GD_ASSERT(result != WAIT_FAILED);
}

Bool Mutex::TryLock(UInt32 pTimeoutMS)
{
    DWORD result = ::WaitForSingleObject(mMutex, pTimeoutMS);
    GD_ASSERT(result != WAIT_FAILED);
    return result == WAIT_OBJECT_0;
}

Bool Mutex::IsLocked() const
{
    DWORD result = ::WaitForSingleObject(mMutex, 0);
    GD_ASSERT(result != WAIT_FAILED);

    return result == WAIT_TIMEOUT;
}

void Mutex::Unlock()
{
    BOOL result = ::ReleaseMutex(mMutex);
    GD_ASSERT(result == TRUE);
}


} // namespace Gamedesk
