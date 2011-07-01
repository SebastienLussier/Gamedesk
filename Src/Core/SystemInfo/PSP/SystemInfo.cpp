/**
 *  @file       SystemInfo.cpp
 *  @brief      PSP System information.
 *  @author     Sébastien Lussier.
 *  @date       13/09/2008.
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
#include "SystemInfo/SystemInfo.h"

#include <psptypes.h>
#include <psprtc.h>


namespace Gamedesk {
	
	
SystemInfo      SystemInfo::mInstance;
const Int32     SystemInfo::BYTE_FORMAT_FACTOR = 1024;

SystemInfo* SystemInfo::Instance()
{
    return &mInstance;
}

SystemInfo::SystemInfo()
{
    // Memory
    mTotalPhysMem = 32 * 1024 * 1024;
    mTotalVirtMem = 32 * 1024 * 1024;

    // CPU
    mNumCpu = 1;

    // Computer name
    mComputerName = "";

    // User name
    mCurrentUser = "";

    // Windows version
    mOSDesc = "PSP";
}

UInt32 SystemInfo::GetNumCpu()
{
    return 1;
}

String SystemInfo::GetCpuModel()
{
    return "";
}

Bool SystemInfo::CpuSupportMMX()
{
    return false;
}

Bool SystemInfo::CpuSupportSSE()
{
    return false;
}

Bool SystemInfo::CpuSupportSSE2()
{
    return false;
}

Bool SystemInfo::CpuSupport3DNow()
{
    return false;
}

UInt32 SystemInfo::GetTotalPhysMem( ByteFormat pByteFormat )
{
    return mTotalPhysMem / pow( (Float)BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}

UInt32 SystemInfo::GetTotalVirtMem( ByteFormat pByteFormat )
{
    return mTotalVirtMem / pow( (Float)BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}

Float SystemInfo::GetMemoryLoad()
{
    return 0.0f;
}

String SystemInfo::GetCurrentUser()
{
    return mCurrentUser;
}

String SystemInfo::GetComputerName()
{
    return mComputerName;
}

UInt32 SystemInfo::GetFreePhysMem( ByteFormat /*pByteFormat*/ )
{
    return 0;
}

UInt32 SystemInfo::GetFreeVirtMem( ByteFormat /*pByteFormat*/ )
{
    return 0;
}

UInt32 SystemInfo::GetUsedPhysMem( ByteFormat /*pByteFormat*/ )
{
    return 0;
}

UInt32 SystemInfo::GetUsedVirtMem( ByteFormat /*pByteFormat*/ )
{
    return 0;
}

DateTime SystemInfo::GetComputerUpTime()
{
    return 0;
}

String SystemInfo::GetOSDesc()
{
    return "";
}

Bool SystemInfo::SupportCPUID()
{
    return false;
}

void SystemInfo::GetCPUInfo()
{
}

UInt64 SystemInfo::GetCpuCycles()
{
    UInt64 ticks;
    int res = sceRtcGetCurrentTick(&ticks);
    GD_ASSERT(res == 0);

    res = sceRtcTickAddYears (&ticks,&ticks,-2000);
    GD_ASSERT(res == 0);

    return ticks;
}

UInt64 SystemInfo::GetCpuFrequency()
{
    return 1000000;
}

UInt32 SystemInfo::GetMilliSec()
{
    return ((10 * GetCpuCycles()) / (GetCpuFrequency() / 100));
}

UInt64 SystemInfo::GetMilliSec64()
{
    return ((10 * GetCpuCycles()) / (GetCpuFrequency() / 100));
}

UInt32 SystemInfo::GetMicroSec()
{
    return ((1000 * GetCpuCycles()) / (GetCpuFrequency() / 1000));
}

UInt64 SystemInfo::GetMicroSec64()
{
    return ((1000 * GetCpuCycles()) / (GetCpuFrequency() / 1000));
}

Double SystemInfo::GetSeconds()
{
    return GetMicroSec64() / 1000000.0f;
}


} // namespace Gamedesk
