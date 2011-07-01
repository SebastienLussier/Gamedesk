/**
 *  @file       Event.cpp
 *  @brief      Event synchronisation primitive for Win32.
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
#include "Thread/Event.h"


namespace Gamedesk {
	
	
Event::Event(Bool pAutoReset, Bool pInitiallyLocked)
{
    mEvent = ::CreateEvent(NULL, !pAutoReset, !pInitiallyLocked, NULL);
    GD_ASSERT(mEvent);
}

Event::~Event()
{
    ::CloseHandle(mEvent);
}

void Event::SetDone()
{
    BOOL result = ::SetEvent(mEvent);
    GD_ASSERT(result == TRUE);
}

void Event::Wait()
{
    DWORD result = ::WaitForSingleObject(mEvent, INFINITE);
    GD_ASSERT(result != WAIT_FAILED);
}

Bool Event::TryWait(UInt32 pTimeoutMS)
{
    DWORD result = ::WaitForSingleObject(mEvent, pTimeoutMS);
    GD_ASSERT(result != WAIT_FAILED);
    return result == WAIT_OBJECT_0;
}

Bool Event::IsDone() const
{
    DWORD result = ::WaitForSingleObject(mEvent, 0);
    GD_ASSERT(result != WAIT_FAILED);
    return result == WAIT_TIMEOUT;
}

void Event::Reset()
{
    ::ResetEvent(mEvent);
}


} // namespace Gamedesk
