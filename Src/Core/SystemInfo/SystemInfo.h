/**
 *  @file       SystemInfo.h
 *  @brief      OS and hardware information about a computer.
 *  @author     Sébastien Lussier.
 *  @date       04/03/02.
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
#ifndef     _SYSTEM_INFO_H_
#define     _SYSTEM_INFO_H_


#include "Time/DateTime.h"


namespace Gamedesk {


/**
 *  Access system information like amount of memory, number of cpu, OS version, etc.
 *  @brief  Access system information.
 *  @author Sebastien Lussier.
 *  @date   04/03/02.
 */
class CORE_API SystemInfo
{
public:

    /**
     *  Factor used to convert to one byte format to another.
     *  @brief  Factor used to convert to one byte format to another.
     */
    static const Int32   BYTE_FORMAT_FACTOR;

    /**
     *  Byte format.
     *  @brief  Byte format size types.
     *  @todo   Move this somewhere else.
     */
    enum ByteFormat
    {
        Byte,           //!< 8 bits.
        Kilobyte,       //!< 1024 bytes,
        Megabyte,       //!< 1024 kylobytes.
        Gigabyte,       //!< 1024 megabytes.
        Petabyte,       //!< 1024 gigabytes.
        Exabyte,        //!< 1024 petabytes.
        Zettabyte,      //!< 1024 exabytes.
        Yottabyte       //!< 1024 zetabytes.
    };

    /**
     *  Obtain the current amount of free physical memory.
     *  @brief  Obtain the current amount of free physical memory.
     *  @param  pByteFormat      The format in wich we want to have the result.
     *  @return The current amount of free physical memory, in the format specified.
     */
    virtual UInt32 GetFreePhysMem( ByteFormat pByteFormat = SystemInfo::Megabyte );

    /**
     *  Obtain the current amount of free virtual memory.
     *  @brief  Obtain the current amount of free virtual memory.
     *  @param  pByteFormat      The format in wich we want to have the result.
     *  @return The current amount of free virtual memory, in the format specified.
     */
    virtual UInt32 GetFreeVirtMem( ByteFormat pByteFormat = SystemInfo::Megabyte );

    /**
     *  Obtain the current amount of used physical memory.
     *  @brief  Obtain the current amount of used physical memory.
     *  @param  pByteFormat      The format in wich we want to have the result.
     *  @return The current amount of used physical memory, in the format specified.
     */
    virtual UInt32 GetUsedPhysMem( ByteFormat pByteFormat = SystemInfo::Megabyte );

    /**
     *  Obtain the current amount of used virtual memory.
     *  @brief  Obtain the current amount of used virtual memory.
     *  @param  pByteFormat      The format in wich we want to have the result.
     *  @return The current amount of used virtual memory, in the format specified.
     */
    virtual UInt32 GetUsedVirtMem( ByteFormat pByteFormat = SystemInfo::Megabyte );

    /**
     *  Obtain the total amount of virtual physical.
     *  @brief  Obtain the total amount of physical memory.
     *  @param  pByteFormat      The format in wich we want to have the result.
     *  @return The total amount of physical memory, in the format specified.
     */
    UInt32 GetTotalPhysMem( ByteFormat pByteFormat = SystemInfo::Megabyte );

    /**
     *  Obtain the total amount of virtual memory.
     *  @brief  Obtain the total amount of virtual memory.
     *  @param  pByteFormat      The format in wich we want to have the result.
     *  @return The total amount of virtual memory, in the format specified.
     */
    UInt32 GetTotalVirtMem( ByteFormat pByteFormat = SystemInfo::Megabyte );

    /**
     *  Obtain the percentage of used physical memory.
     *  @brief  Obtain the percentage of used physical memory.
     *  @return The current percentage of used physical memory (between 0 and 1).
     */
    virtual Float GetMemoryLoad();

    /**
     *  Get the time elapsed since the computer was started.
     *  @brief  Get the time elapsed since the computer was started.
     *  @return A DateTime variable specifing the time elapsed since startup.
     */
    virtual DateTime GetComputerUpTime();

    /**
     *  Get the number of processors in this computer.
     *  @brief  Get the number of processors in this computer.
     *  @return An integer, the number of processors.
     */
    UInt32 GetNumCpu();

    /**
     *  Obtain a string describing the processor model.
     *  @brief  Obtain a string describing the processor model.
     *  @return The processor model.
     */
    String GetCpuModel();

    /**
     *  Does the processor support MMX instructions ?.
     *  @brief  Does the processor support MMX instructions ?.
     *  @return \b true if it support MMX, \b false otherwise.
     */
    Bool CpuSupportMMX();

    /**
     *  Does the processor support SSE instructions ?.
     *  @brief  Does the processor support SSE instructions ?.
     *  @return \b true if it support SSE, \b false otherwise.
     */
    Bool CpuSupportSSE();

    /**
     *  Does the processor support SSE2 instructions ?.
     *  @brief  Does the processor support SSE2 instructions ?.
     *  @return \b true if it support SSE2, \b false otherwise.
     */
    Bool CpuSupportSSE2();

    /**
     *  Does the processor support 3DNOW instructions ?.
     *  @brief  Does the processor support 3DNOW instructions ?.
     *  @return \b true if it support 3DNOW, \b false otherwise.
     */
    Bool CpuSupport3DNow();

    /**
     *  Get the username of the currently logged user of the system.
     *  @brief  Get the username of the currently logged user of the system.
     *  @return The username.
     */
    String GetCurrentUser();

    /**
     *  Get the name of this computer.
     *  @brief  Get the name of this computer.
     *  @return The name of this computer.
     */
    String GetComputerName();

    /**
     *  Obtain a string describing the operating system currently running.
     *  @brief  Obtain a string describing the operating system currently running.
     *  @return A string describing the operating system.
     */
    String GetOSDesc();

    /**
     * Get the number of clock cycles elapsed since the last reboot.
     * @return Number of clock cycles elapsed.
     */
    UInt64 GetCpuCycles();

    /**
     *  Get the processor speed.
     *  @brief  Get the processor speed in hertz.
     *  @return An integer, the number of processors.
     */
    UInt64 GetCpuFrequency();

    UInt32 GetMilliSec();
    UInt64 GetMilliSec64();
    UInt32 GetMicroSec();
    UInt64 GetMicroSec64();

    /**
     * Get the number of seconds elapsed since the last reboot.
     * @return Number of seconds elapsed.
     */
    Double GetSeconds();

    /**
     *  Return the singleton instance of the SystemInfo class.
     *  @brief  Return the singleton instance of the SystemInfo class.
     *  @return the singleton instance of the SystemInfo class.
     */
    static SystemInfo* Instance();


protected:
    /**
     *  Constructor.
     *  @brief  Constructor.
     */
    SystemInfo();

    void GetCPUInfo();
    Bool SupportCPUID();

protected:
    UInt32      mNumCpu;                //!< The number of processors.
    UInt64      mCPUFrequency;          //!< Frequency of the CPU(s)

    String      mCpuModelName;          //!< The processor model.
    Bool        mCpuSupportMMX;         //!< Does this processor support MMX instructions ?
    Bool        mCpuSupportSSE;         //!< Does this processor support SSE instructions ?
    Bool        mCpuSupportSSE2;        //!< Does this processor support SSE2 instructions ?
    Bool        mCpuSupport3DNow;       //!< Does this processor support 3DNow instructions ?

    UInt32      mTotalPhysMem;          //!< The total amount of physical memory, in bytes.
    UInt32      mTotalVirtMem;          //!< The total amount of virtual memory, in bytes.

    String      mComputerName;          //!< The computer name.
    String      mCurrentUser;           //!< The currently logged on user.

    String      mOSDesc;                //!< The OS version.

    static SystemInfo mInstance;        //!< Pointer to singleton.
};


} // namespace Gamedesk


#endif  //  _SYSTEM_INFO_H_
