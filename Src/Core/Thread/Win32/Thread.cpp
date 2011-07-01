/**
 *  @file       Thread.cpp
 *  @brief      Thread support for Win32.
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
#include "Thread/Thread.h"


namespace Gamedesk {
	
	
const Int32 GDToWin32ThreadPriority[] = 
        {
            THREAD_PRIORITY_IDLE,           // PriorityIdle,
            THREAD_PRIORITY_LOWEST,         // PriorityLowest,
            THREAD_PRIORITY_BELOW_NORMAL,   // PriorityBelowNormal,
            THREAD_PRIORITY_NORMAL,         // PriorityNormal,
            THREAD_PRIORITY_ABOVE_NORMAL,   // PriorityAboveNormal,
            THREAD_PRIORITY_TIME_CRITICAL   // PriorityTimeCritical
        };

void Thread::Start(Priority pPriority, UInt32 stackSize)
{
    unsigned int threadId;
    mHandle = (HANDLE)_beginthreadex(NULL, stackSize, &Thread::Win32ThreadEntryPoint, this, CREATE_SUSPENDED, &threadId);

    mId = threadId;

    ::SetThreadPriority(mHandle, GDToWin32ThreadPriority[pPriority]);
    ::ResumeThread(mHandle);
}

unsigned int WINAPI Thread::Win32ThreadEntryPoint(void* pParam)
{
    Thread* thread = (Thread*)pParam;
    thread->EntryPoint();
    return 0;
}


} // namespace Gamedesk
