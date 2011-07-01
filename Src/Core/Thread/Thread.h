/**
 *  @file       Thread.h
 *  @brief      Thread support.
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
#ifndef     _THREAD_H_
#define     _THREAD_H_


#include "Thread/Event.h"


namespace Gamedesk {


class CORE_API Thread
{
public:
    enum Priority
    {
        PriorityIdle,
        PriorityLowest,
        PriorityBelowNormal,
        PriorityNormal,
        PriorityAboveNormal,
        PriorityTimeCritical
    };

    inline Thread();

    void Start(Priority pPriority = PriorityNormal, UInt32 stackSize = 32*1024);
    
    virtual void Run() = 0;

    inline Bool WaitUntilStarted(UInt32 pTimeout = INFINITE);
    inline Bool WaitUntilStopped(UInt32 pTimeout = INFINITE);

    inline Handle  GetHandle() const;
    inline UInt32  GetId() const;

private:
    static unsigned int WINAPI Win32ThreadEntryPoint(void* pParam);

    inline void EntryPoint();

private:
    HANDLE          mHandle;
    UInt32          mId;
    Event           mStartEvent;
    Event           mStopEvent;
};


///////////////////////////////////////////////////////////////////////////////


Thread::Thread() 
    : mHandle((HANDLE)0xFFFFFFFF)
    , mId(0xFFFFFFFF)
    , mStartEvent(true, true)
    , mStopEvent(true, true)
{
}

Bool Thread::WaitUntilStarted(UInt32 pTimeout)
{
    return mStartEvent.TryWait(pTimeout);
}

Bool Thread::WaitUntilStopped(UInt32 pTimeout)
{
    return mStopEvent.TryWait(pTimeout);
}

Handle Thread::GetHandle() const   
{
    return mHandle;
}

UInt32 Thread::GetId() const
{
    return mId;
}

void Thread::EntryPoint()
{
    mStartEvent.SetDone();

    Run();

    mStopEvent.SetDone();
}


} // namespace Gamedesk


#endif
