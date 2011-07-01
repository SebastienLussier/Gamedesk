/**
 *  @file       SystemInfo.cpp
 *  @brief      Windows OS and Hardware info.
 *  @author     Sébastien Lussier.
 *  @date       24/02/02.
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
#include "Core.h"
#include "SystemInfo/SystemInfo.h"


namespace Gamedesk {
	
	
SystemInfo      SystemInfo::mInstance;
const Int32     SystemInfo::BYTE_FORMAT_FACTOR = 1024;

SystemInfo* SystemInfo::Instance()
{
    return &mInstance;
}

SystemInfo::SystemInfo()
{
    SYSTEM_INFO     systemInfo;
    MEMORYSTATUS	memoryInfo;

    Char        computerName[MAX_COMPUTERNAME_LENGTH + 1];
    Char        userName[257];
    ULONG		len;

    GetSystemInfo( &systemInfo );
    GlobalMemoryStatus( &memoryInfo );

    // Memory
    mTotalPhysMem = memoryInfo.dwTotalPhys;
    mTotalVirtMem = memoryInfo.dwTotalVirtual;

    // CPU
    GetCPUInfo();
    mNumCpu = systemInfo.dwNumberOfProcessors;
    
    // Frequency
    GetCpuFrequency();

    // Computer name
    len = MAX_COMPUTERNAME_LENGTH+1;
    ::GetComputerName( computerName, &len );
    mComputerName = computerName;

    // User name
    len = 257;
    ::GetUserName( userName, &len );
    mCurrentUser = userName;

    // Windows version
    mOSDesc = GetOSDesc();
}

UInt32 SystemInfo::GetNumCpu()
{
    return mNumCpu;
}

String SystemInfo::GetCpuModel()
{
    return mCpuModelName;
}

Bool SystemInfo::CpuSupportMMX()
{
    return mCpuSupportMMX;
}

Bool SystemInfo::CpuSupportSSE()
{
    return mCpuSupportSSE;
}

Bool SystemInfo::CpuSupportSSE2()
{
    return mCpuSupportSSE2;
}

Bool SystemInfo::CpuSupport3DNow()
{
    return mCpuSupport3DNow;
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
    return GetUsedPhysMem( SystemInfo::Byte ) / (Float)GetTotalPhysMem( SystemInfo::Byte );
}

String SystemInfo::GetCurrentUser()
{
    return mCurrentUser;
}

String SystemInfo::GetComputerName()
{
    return mComputerName;
}

UInt32 SystemInfo::GetFreePhysMem( ByteFormat pByteFormat )
{
    MEMORYSTATUS        memoryInfo;
    GlobalMemoryStatus( &memoryInfo );
    return memoryInfo.dwAvailPhys / pow( (Float)BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}

UInt32 SystemInfo::GetFreeVirtMem( ByteFormat pByteFormat )
{
    MEMORYSTATUS        memoryInfo;
    GlobalMemoryStatus( &memoryInfo );
    return memoryInfo.dwAvailVirtual / pow( (Float)BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}

UInt32 SystemInfo::GetUsedPhysMem( ByteFormat pByteFormat )
{
    MEMORYSTATUS        memoryInfo;
    GlobalMemoryStatus( &memoryInfo );
    return (mTotalPhysMem - memoryInfo.dwAvailPhys) / pow( (Float)BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}

UInt32 SystemInfo::GetUsedVirtMem( ByteFormat pByteFormat )
{
    MEMORYSTATUS        memoryInfo;
    GlobalMemoryStatus( &memoryInfo );
    return (mTotalVirtMem - memoryInfo.dwAvailVirtual) / pow( (Float)BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}

DateTime SystemInfo::GetComputerUpTime()
{
    return GetTickCount() / 1000;   // We want it in seconds
}

String SystemInfo::GetOSDesc()
{
    OSVERSIONINFOEX  versionInfo;

    if( !mOSDesc.empty() )
        return mOSDesc;

    memset( &versionInfo, 0, sizeof(OSVERSIONINFO) );
    versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    if( !GetVersionEx( (OSVERSIONINFO*)&versionInfo ) )
        return "";

    switch( versionInfo.dwPlatformId )
    {
    // Tests for Windows NT product family.
    case VER_PLATFORM_WIN32_NT:
        {
            // Test for the product.
        if( versionInfo.dwMajorVersion <= 4 )
            mOSDesc += "Microsoft Windows NT ";
        else if( versionInfo.dwMajorVersion == 5 && versionInfo.dwMinorVersion == 0 )
            mOSDesc += "Microsoft Windows 2000 ";

        else if( versionInfo.dwMajorVersion == 5 && versionInfo.dwMinorVersion == 1 )
            mOSDesc += "Microsoft Windows XP ";

        HKEY    key;
        Char    productType[80];
        SIZE_T  bufLen;

        RegOpenKeyEx( HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_QUERY_VALUE, &key );
        RegQueryValueEx( key, "ProductType", NULL, NULL, (LPBYTE)productType, &bufLen );
        RegCloseKey( key );

        if( lstrcmpi("WINNT", productType) == 0 )
            mOSDesc += "Professional ";

        if( lstrcmpi("LANMANNT", productType) == 0 )
            mOSDesc += "Server ";

        if( lstrcmpi("SERVERNT", productType) == 0 )
            mOSDesc += "Advanced Server ";

        String buildNumber = ToString( (Int32)(versionInfo.dwBuildNumber & 0xFFFF) );

        // Display version, service pack (if any), and build number.
        if( versionInfo.dwMajorVersion <= 4 )
        {
            mOSDesc += "version ";
            mOSDesc += versionInfo.dwMajorVersion + ".";
            mOSDesc += versionInfo.dwMinorVersion + " ";
            mOSDesc += versionInfo.szCSDVersion;
            mOSDesc += " (Build " +
            mOSDesc += buildNumber;
            mOSDesc += ")";
        }
        else
        {
            mOSDesc += versionInfo.szCSDVersion;
            mOSDesc += " (Build ";
            mOSDesc += buildNumber;
            mOSDesc += ")";
        }
        break;
        }

    // Test for the Windows 95 product family.
    case VER_PLATFORM_WIN32_WINDOWS:
        if( versionInfo.dwMajorVersion == 4 && versionInfo.dwMinorVersion == 0 )
        {
            mOSDesc += "Microsoft Windows 95 ";

            if( versionInfo.szCSDVersion[1] == 'C' || versionInfo.szCSDVersion[1] == 'B' )
                mOSDesc += "OSR2 ";
        }

        if( versionInfo.dwMajorVersion == 4 && versionInfo.dwMinorVersion == 10 )
        {
            mOSDesc += "Microsoft Windows 98 ";

            if( versionInfo.szCSDVersion[1] == 'A' )
                mOSDesc += "SE ";
        }

        if( versionInfo.dwMajorVersion == 4 && versionInfo.dwMinorVersion == 90 )
        {
            mOSDesc += "Microsoft Windows Millennium Edition ";
        }

        break;
    }

    return mOSDesc;
}

// This function checks if the CPUID command is available on the current
// processor
Bool SystemInfo::SupportCPUID()
{
    UInt32 bitChanged = 0;

    // We've to check if we can toggle the flag register bit 21
    // If we can't the processor does not support the CPUID command
    __asm pushfd
    __asm pop eax
    __asm mov ebx, eax
    __asm xor eax, 0x00200000
    __asm push eax
    __asm popfd
    __asm pushfd
    __asm pop eax
    __asm xor eax,ebx
    __asm mov bitChanged, eax

    return ((bitChanged) ? true : false);
}


// Obtain all cpu info
void SystemInfo::GetCPUInfo()
{
    UInt32 processorVendor;
    UInt32 eaxReg, ebxReg, edxReg;

    // First of all we check if the CPUID command is available
    if( !SupportCPUID() )
        return;

    // We read the standard CPUID level 0x00000000 which should
    // be available on every x86 processor
    __asm mov eax, 0
    __asm cpuid
    __asm mov processorVendor, ebx

    // Turn off features
    mCpuSupport3DNow = false;
    mCpuSupportMMX   = false;
    mCpuSupportSSE   = false;
    mCpuSupportSSE2  = false;
    mCpuSupport3DNow = false;

    // Then we switch to the specific processor vendors
    switch( processorVendor )
    {
        // VendorID = GenuineIntel
        case 0x756E6547:
        {
            // Now we get the CPUID standard level 0x00000001
            __asm mov eax, 1
            __asm cpuid
            __asm mov eaxReg, eax
            __asm mov ebxReg, ebx

            // Then get the cpu model, family, type, stepping and brand id by masking
            // the eax and ebx register
            UInt32 model    = (eaxReg >> 4) & 0xF;
            UInt32 family   = (eaxReg >> 8) & 0xF;
            UInt32 brandID  = ebxReg & 0xF;

            UInt32 extendedModel;

            switch( family )
            {
            case 3: mCpuModelName = "Intel i386";                               break;
            case 4: mCpuModelName = "Intel i486";                               break;
            case 5: mCpuModelName = "Intel Pentium";                            break;
            case 6:
                switch( model )
                {
                case 0: mCpuModelName = "Intel Pentium Pro";                    break;
                case 1: mCpuModelName = "Intel Pentium Pro";                    break;
                case 3: mCpuModelName = "Intel Pentium II";                     break;
                case 5: mCpuModelName = "Intel Pentium II Model 5/Xeon/Celeron";break;
                case 6: mCpuModelName = "Intel Celeron";                        break;
                case 7: mCpuModelName = "Intel Pentium III/Pentium III Xeon";   break;
                case 8:
                case 0xA:
                    switch( brandID )
                    {
                    case 1: mCpuModelName = "Intel Celeron";                    break;
                    case 2: mCpuModelName = "Intel Pentium III";                break;
                    case 3: mCpuModelName = "Intel Pentium III Xeon";           break;
                    default:mCpuModelName = "Intel Pentium III";                break;
                    }
                    break;

                case 0xB:
                    switch( brandID )
                    {
                    case 3: mCpuModelName = "Intel Celeron";                    break;
                    case 4: mCpuModelName = "Intel Pentium III";                break;
                    case 7: mCpuModelName = "Intel Celeron Mobile";             break;
                    default:mCpuModelName = "Intel Pentium III";                break;
                    }
                    break;

                default:mCpuModelName = "Unknown Intel Pentium Pro";            break;
                }
                break;
            case 15:
                extendedModel = (eaxReg >> 16) & 0xFF;
                switch( model )
                {
                case 0: mCpuModelName = brandID == 8 ? "Intel Pentium 4" : "Intel Pentium 4 Xeon"; break;
                case 1: mCpuModelName = brandID == 8 ? "Intel Pentium 4" : "Intel Pentium 4 Xeon"; break;
                case 2: mCpuModelName = brandID == 9 ? "Intel Pentium 4" : "Intel Pentium 4 Xeon"; break;
                default:mCpuModelName = "Intel Pentium 4";                                           break;
                }
                break;

            default:
                mCpuModelName = "Intel (Unknown model)";                                             break;
            }
        }
        break;

        // VendorID = AuthenticAMD
        case 0x68747541:
        {
            // Now we get the CPUID standard level 0x00000001
            __asm mov eax, 1
            __asm cpuid
            __asm mov eaxReg, eax

            // Then we mask the model, family, stepping and type (AMD does not support brand id)
            UInt32 model    = (eaxReg >> 4) & 0xF;
            UInt32 family   = (eaxReg >> 8) & 0xF;

            switch( family )
            {
            case 4:
                switch( model )
                {
                case 3:
                case 7:
                case 8:
                case 9:     mCpuModelName = "AMD 486";          break;
                case 0xE:
                case 0xF:   mCpuModelName = "AMD 586";          break;
                default:    mCpuModelName = "AMD 486 or 586";   break;
                }
                break;

            case 5:
                switch( model )
                {
                case 0:     mCpuModelName = "AMD K5 SSA5 (PR75, PR90, PR100)";      break;
                case 1:     mCpuModelName = "AMD K5 5k86 (PR120, PR133)";           break;
                case 2:     mCpuModelName = "AMD K5 5k86 (PR166)";                  break;
                case 3:     mCpuModelName = "AMD K5 5k86 (PR200)";                  break;
                case 6:     mCpuModelName = "AMD K6 (0.30 µm)";                     break;
                case 7:     mCpuModelName = "AMD K6 (0.25 µm)";                     break;
                case 8:     mCpuModelName = "AMD K6-2";                             break;
                case 9:     mCpuModelName = "AMD K6-III";                           break;
                case 0xD:   mCpuModelName = "AMD K6-2+ or K6-III+ (0.18 µm)";       break;
                default:    mCpuModelName = "Unknown AMD K5 or K6 model";           break;
                }
                break;
            case 6:
                switch( model )
                {
                case 1:     mCpuModelName = "AMD Athlon (0.25 µm)";                 break;
                case 2:     mCpuModelName = "AMD Athlon (0.18 µm)";                 break;
                case 3:     mCpuModelName = "AMD Duron (Spitfire)";                 break;
                case 4:     mCpuModelName = "AMD Athlon (Thunderbird)";             break;
                case 6:     mCpuModelName = "AMD Athlon MP/Mobile Athlon (Palomino)";break;
                case 7:     mCpuModelName = "AMD Mobile Duron (Morgan)";            break;
                default:    mCpuModelName = "Unknown AMD K7 model";                 break;
                }
                break;
            default:        mCpuModelName = "Unknown AMD model";                    break;
            }

            // Then we read the extended CPUID level 0x80000000
            __asm mov eax, 0x80000000
            __asm cpuid
            __asm mov eaxReg, eax

            // ...to check the max. supportted extended CPUID level
            UInt32 maxSupportedExtendedLevel = eaxReg;

            // Then we check if theres an extended CPUID level support
            if( maxSupportedExtendedLevel >= 0x80000001 )
            {
                // If we can access the extended CPUID level 0x80000001 we get the
                // edx register
                __asm mov eax, 0x80000001
                __asm cpuid
                __asm mov edxReg, edx

                // Now we can mask some AMD specific cpu extensions
                mCpuSupport3DNow = BitCheck(edxReg, 31);
            }
        }
        break;

        default:
        {
            // First of all we read the standard CPUID level 0x00000001
            // This level should be available on every x86-processor clone
            __asm mov eax, 1
            __asm cpuid
            __asm mov eaxReg, eax
            __asm mov ebxReg, ebx


            // Then we mask the processor model, family, type and stepping
            UInt32 model    = (eaxReg >> 4) & 0xF;
            UInt32 family   = (eaxReg >> 8) & 0xF;
            UInt32 type     = (eaxReg >> 12) & 0x3;

            // To have complete information we also mask the brand id
            UInt32 brandID  = ebxReg & 0xF;

            // Fill cpu model string with the informations we got
            mCpuModelName = "Unknown CPU (model=";
            mCpuModelName += model;
            mCpuModelName += ", family=";
            mCpuModelName += family;
            mCpuModelName += ", brand=";
            mCpuModelName += brandID;
            mCpuModelName += ", type=";
            mCpuModelName += type;
            mCpuModelName += ") ";
        }
        break;

    }

    // Now we get the standard processor extensions

    // We just get the standard CPUID level 0x00000001 which should be
    // available on every x86 processor
    __asm mov eax, 1
    __asm cpuid
    __asm mov edxReg, edx

    // Then we mask some bits - we don't need all of these for now... so I leave
    // some commented out.
    //mCpuSupportFPU                                            = CheckBit(edxReg, 0);
    //CPUInfo._Ext.VME_Virtual8086ModeEnhancements              = CheckBit(edxReg, 1);
    //CPUInfo._Ext.DE_DebuggingExtensions                       = CheckBit(edxReg, 2);
    //CPUInfo._Ext.PSE_PageSizeExtensions                       = CheckBit(edxReg, 3);
    //CPUInfo._Ext.TSC_TimeStampCounter                         = CheckBit(edxReg, 4);
    //CPUInfo._Ext.MSR_ModelSpecificRegisters                   = CheckBit(edxReg, 5);
    //CPUInfo._Ext.PAE_PhysicalAddressExtension                 = CheckBit(edxReg, 6);
    //CPUInfo._Ext.MCE_MachineCheckException                    = CheckBit(edxReg, 7);
    //CPUInfo._Ext.CX8_COMPXCHG8B_Instruction                   = CheckBit(edxReg, 8);
    //CPUInfo._Ext.APIC_AdvancedProgrammableInterruptController = CheckBit(edxReg, 9);
    //CPUInfo._Ext.APIC_ID                                      = (ebxReg >> 24) & 0xFF;
    //CPUInfo._Ext.SEP_FastSystemCall                           = CheckBit(edxReg, 11);
    //CPUInfo._Ext.MTRR_MemoryTypeRangeRegisters                = CheckBit(edxReg, 12);
    //CPUInfo._Ext.PGE_PTE_GlobalFlag                           = CheckBit(edxReg, 13);
    //CPUInfo._Ext.MCA_MachineCheckArchitecture                 = CheckBit(edxReg, 14);
    //CPUInfo._Ext.CMOV_ConditionalMoveAndCompareInstructions   = CheckBit(edxReg, 15);
    //CPUInfo._Ext.FGPAT_PageAttributeTable                     = CheckBit(edxReg, 16);
    //CPUInfo._Ext.PSE36_36bitPageSizeExtension                 = CheckBit(edxReg, 17);
    //CPUInfo._Ext.PN_ProcessorSerialNumber                     = CheckBit(edxReg, 18);
    //CPUInfo._Ext.CLFSH_CFLUSH_Instruction                     = CheckBit(edxReg, 19);
    //CPUInfo._Ext.CLFLUSH_InstructionCacheLineSize             = (ebxReg >> 8) & 0xFF;
    //CPUInfo._Ext.DS_DebugStore                                = CheckBit(edxReg, 21);
    //CPUInfo._Ext.ACPI_ThermalMonitorAndClockControl           = CheckBit(edxReg, 22);
    mCpuSupportMMX                                              = BitCheck(edxReg, 23);
    //CPUInfo._Ext.FXSR_FastStreamingSIMD_ExtensionsSaveRestore = CheckBit(edxReg, 24);
    mCpuSupportSSE                                              = BitCheck(edxReg, 25);
    mCpuSupportSSE2                                             = BitCheck(edxReg, 26);
    //CPUInfo._Ext.SS_SelfSnoop                                 = CheckBit(edxReg, 27);
    //CPUInfo._Ext.HT_HyperThreading                            = CheckBit(edxReg, 28);
    //CPUInfo._Ext.HT_HyterThreadingSiblings                    = (ebxReg >> 16) & 0xFF;
    //CPUInfo._Ext.TM_ThermalMonitor                            = CheckBit(edxReg, 29);
    //CPUInfo._Ext.IA64_Intel64BitArchitecture                  = CheckBit(edxReg, 30);
}

UInt64 SystemInfo::GetCpuCycles()
{
    LARGE_INTEGER Counter;
    QueryPerformanceCounter(&Counter);
    return Counter.QuadPart;
}

UInt64 SystemInfo::GetCpuFrequency()
{
    if( mCPUFrequency == 0 )
    {
        LARGE_INTEGER Freq;
        QueryPerformanceFrequency(&Freq);
        mCPUFrequency = Freq.QuadPart;
    }

    return mCPUFrequency;
}

UInt32 SystemInfo::GetMilliSec()
{    
    return ((10 * GetCpuCycles()) / (mCPUFrequency / 100));
}

UInt64 SystemInfo::GetMilliSec64()
{
    return ((10 * GetCpuCycles()) / (mCPUFrequency / 100));
}

UInt32 SystemInfo::GetMicroSec()
{
    return ((1000 * GetCpuCycles()) / (mCPUFrequency / 1000));
}

UInt64 SystemInfo::GetMicroSec64()
{
    return ((1000 * GetCpuCycles()) / (mCPUFrequency / 1000));
}

Double SystemInfo::GetSeconds()
{
    return GetMicroSec64() / 1000000.0f;
}


} // namespace Gamedesk
