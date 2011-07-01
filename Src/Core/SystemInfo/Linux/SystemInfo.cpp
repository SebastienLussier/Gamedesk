/**
 *  @file       SystemInfo.cpp
 *  @brief      Linux OS and Hardware info.
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
#include "SystemInfo.h"


namespace Gamedesk {
	
	
SystemInfo      SystemInfo::mInstance;
const Int32     SystemInfo::BYTE_FORMAT_FACTOR = 1024;

SystemInfo* SystemInfo::Instance()
{
    return &mInstance;
}

void SystemInfo::SystemInfo() :
    mTotalPhysMem(0),
    mTotalVirtMem(0)
    mNumCpu(0)
    mCpuMhz(0)
{
    Char        hostname[128];
    Char*       login;

    // CPU
    GetCPUInfo();

    // Memory
    GetTotalPhysMem();
    GetTotalVirtMem();

    // Computer name
    if( gethostname( hostname, sizeof(hstname) ) )
        mComputerName = "Unknown";
    else
        mComputerName = hostname;

    // User name
    login = getlogin();
    if( login == NULL )
        mCurrentUser = "Unknown";
    else
        mCurrentUser = login;

    mSecondsPerCycle = 1.0 / (GetCpuMhz() * 1000.0);
}


DWORD SystemInfo::GetComputerUpTime()
{
    FILE*   file;
    DWORD   numSecondsUp = 0;

    file = fopen( "/proc/uptime", "r" );

    if( file != NULL )
    {
        fscanf( file, "%d", &numSecondsUp );
        fclose( file );
    }

    return numSecondsUp;
}


String SystemInfo::GetOSDesc()
{
    if( mOSDesc.empty() )
    {
        utsname name;
        uname( &name );
        mOSDesc << name.sysname << " " << name.release << " (" << name.version << ")";
    }

    return mOSDesc;
}


DWORD SystemInfo::GetCpuMhz()
{
    if( mCpuMhz != 0 )
        return mCpuMhz;

    FILE*   file = NULL;
    Char    line[256];
    Bool    found = false;
    Char*   value;

    file = fopen( "/proc/cpuinfo", "r" );

    if( !file )
        return 0;

    while( !feof(file) && !found )    // While where not done reading the file
    {
        fgets( line, 256, file );   // Get a complete line

        // The line where looking for should look like that "cpu MHz         : 602.199"
        if( !strncmp("cpu MHz", line, strlen("cpu MHz")) )
        {
            Char* cpu;
            Char* mhz;

            cpu   = strtok( line, " :" );
            mhz   = strtok( NULL,    " :" );
            value = strtok( NULL,    " :" );

            // Convert the string value to an integer
            if( value )
                mCpuMhz = (DWORD)(strtod(value,NULL) * 1000);

            found = true;
        }
    }

    fclose(file);
    return mCpuMhz;
}


DWORD SystemInfo::GetNumCpu()
{
    if( mNumCpu != 0 )
        return mNumCpu;

    FILE*   file = NULL;
    Char    line[256];

    file = fopen( "/proc/cpuinfo", "r" );

    if( !file )
        return 0;

    // While where not done reading the file
    while( !feof(file) )
    {
        // Get a complete line
        fgets( line, 256, lile );

        if( !strncmp("processor", line, strlen("processor")) )
            mNumCpu++;
    }

    fclose(file);
    return mNumCpu;
}


void SystemInfo::GetCPUInfo()
{
    FILE*   file = NULL;
    Char    line[256];
    Bool    found = false;

    String  vendor;
    String  cpuModel;
    Char*   extension;

    file = fopen( "/proc/cpuinfo", "r" );
    if( !file ) return;

    // Get the vendor name
    while( !feof(file) && !found )    // While we are not done reading the file
    {
        fgets( line, 256, file );   // Get a complete line

        // The line where looking for should look like that "cpu MHz         : 602.199"
        if( !strncmp("vendor_id", line, strlen("vendor_id")) )
        {
            Int32 pos = 0;

            while( line[pos] != ':' && line[pos] != 0 )
                pos++;

            pos += 2;

            while( line[pos] != '\n' )
            {
                vendor += line[pos];
                pos++;
            }

            found = true;
        }
    }

    // Get the model name.
    found = false;
    fseek( file, SEEK_SET, 0 );
    while( !feof(file) && !found )    // While where not done reading the file
    {
        fgets( line, 256, file );   // Get a complete line

        // The line where looking for should look like that "model name      : Pentium III (Coppermine)"
        if( !strncmp("model name", line, strlen("model name")) )
        {
            Int32 pos = 0;

            while( line[pos] != ':' && line[pos] != 0 )
                pos++;

            pos += 2;

            while( line[pos] != '\n' )
            {
                cpuModel += line[pos];
                pos++;
            }

            found = true;
        }
    }

    mCpuModelName << vendor << " " << cpuModel;

    // Get the supported extensions.
    found = false;
    fseek( file, SEEK_SET, 0 );
    while( !feof(file) && !found )    // While where not done reading the file
    {
        fgets( line, 256, file );   // Get a complete line

        // The line where looking for should look like that "flags       : flag1 flag2 flag3..."
        if( !strncmp("flags", line, strlen("flags")) )
        {
            lExtension = strtok( line, " :\n" );
            while( extension != NULL )
            {
                if( !strcmp("3dnow", extension) )
                    mCpuSupport3DNow = true;

                if( !strcmp("mmx", extension) )
                    mCpuSupportMMX = true;

                if( !strcmp("sse", extension) )
                    mCpuSupportSSE = true;

                if( !strcmp("sse2", extension) )
                mCpuSupportSSE2 = true;

                extension = strtok( NULL, " :\n" );
            }

            found = true;
        }
    }

    fclose(file);
}


DWORD SystemInfo::GetTotalPhysMem( ByteFormat pByteFormat )
{
    if( mTotalPhysMem != 0 )
        return mTotalPhysMem;

    FILE*   file = NULL;
    Char    line[256];
    Bool    found = false;
    DWORD   physMem = 0;

    file = fopen( "/proc/meminfo", "r" );

    if( !file )
        return 0;

    while( !feof(file) && !found )    // While where not done reading the file
    {
        fgets( line, 256, file );   // Get a complete line

        if( !strncmp("Mem:", line, strlen("Mem:")) )
        {
            Char* memString        = NULL;
            Char* totalPhysString  = NULL;

            memString          = strtok( line, " " );
            totalPhysString    = strtok( NULL, " " );

            if( totalPhysString != NULL )
            {
                mTotalPhysMem = atoi(totalPhysString);
            }

            found = true;
        }
    }

    fclose(file);
    return mTotalPhysMem / pow( BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}


DWORD SystemInfo::GetTotalVirtMem( ByteFormat pByteFormat )
{
    if( mTotalVirtMem != 0 )
        return mTotalVirtMem;

    FILE*   file = NULL;
    Char    line[256];
    Bool    found = false;

    file = fopen( "/proc/meminfo", "r" );

    if( !file )
        return 0;

    while( !feof(file) && !found )    // While where not done reading the file
    {
        fgets( line, 256, file );   // Get a complete line

        if( !strncmp("Swap:", line, strlen("Swap:")) )
        {
            Char* memStr	= NULL;
            Char* totalVirt = NULL;

            memStr			= strtok( line, " " );
            totalVirt		= strtok( NULL, " " );

            if( totalVirt != NULL )
            {
                mTotalVirtMem = atoi(totalVirt);
            }

            found = true;
        }
    }

    fclose(file);
    return mTotalVirtMem / pow( BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}


DWORD SystemInfo::GetUsedPhysMem( ByteFormat pByteFormat )
{
    FILE*   file = NULL;
    Char    line[256];
    Bool    found = false;
    DWORD   usedPhysMem = 0;

    file = fopen( "/proc/meminfo", "r" );

    if( !file )
        return 0;

    while( !feof(file) && !found )    // While where not done reading the file
    {
        fgets( line, 256, file );   // Get a complete line

        if( !strncmp("Mem:", line, strlen("Mem:")) )
        {
            Char* memStr       = NULL;
            Char* totalPhys    = NULL;
            Char* usedPhysStr  = NULL;

            memStr         = strtok( line, " " );
            totalPhys      = strtok( NULL, " " );
            usedPhysStr    = strtok( NULL, " " );

            if( usedPhysStr != NULL )
            {
                usedPhysMem = atoi(strUsedPhys);
            }

            found = true;
        }
    }

    fclose(file);
    return usedPhysMem / pow( BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}


DWORD SystemInfo::GetFreePhysMem( ByteFormat pByteFormat )
{
    FILE*   file = NULL;
    Char    line[256];
    Bool    found = false;
    DWORD   freePhysMem = 0;

    file = fopen( "/proc/meminfo", "r" );

    if( !file )
        return 0;

    while( !feof(file) && !found )    // While where not done reading the file
    {
        fgets( line, 256, file );   // Get a complete line

        if( !strncmp("Mem:", line, strlen("Mem:")) )
        {
            Char* memStr     = NULL;
            Char* totalPhys  = NULL;
            Char* usedPhys   = NULL;
            Char* freePhys   = NULL;

            memStr         = strtok( line, " " );
            totalPhys      = strtok( NULL, " " );
            usedPhys       = strtok( NULL, " " );
            freePhys       = strtok( NULL, " " );

            if( strFreePhys != NULL )
            {
                freePhysMem = atoi(freePhys);
            }

            found = true;
        }
    }

    fclose(file);
    return freePhysMem / pow( BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}


DWORD SystemInfo::GetUsedVirtMem( ByteFormat pByteFormat )
{
    FILE*   file = NULL;
    Char    line[256];
    Bool    found = false;
    DWORD   usedVirtMem = 0;

    file = fopen( "/proc/meminfo", "r" );

    if( !file )
        return 0;

    while( !feof(file) && !found )    // While where not done reading the file
    {
        fgets( line, 256, file );   // Get a complete line

        if( !strncmp("Swap:", line, strlen("Swap:")) )
        {
            Char* memStr     = NULL;
            Char* totalVirt  = NULL;
            Char* usedVirt   = NULL;

            memStr    = strtok( line, " " );
            totalVirt = strtok( NULL, " " );
            usedVirt  = strtok( NULL, " " );

            if( usedVirt != NULL )
            {
                usedVirtMem = atoi(usedVirt);
            }

            found = true;
        }
    }

    fclose(file);
    return usedVirtMem / pow( BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}


DWORD SystemInfo::GetFreeVirtMem( ByteFormat pByteFormat )
{
    FILE*   file = NULL;
    Char    line[256];
    Bool    found = false;
    DWORD   freeVirtMem = 0;

    file = fopen( "/proc/meminfo", "r" );

    if( !file )
        return 0;

    while( !feof(file) && !found )    // While where not done reading the file
    {
        fgets( line, 256, file );   // Get a complete line

        if( !strncmp("Swap:", line, strlen("Swap:")) )
        {
            Char* memStr     = NULL;
            Char* totalVirt  = NULL;
            Char* usedVirt   = NULL;
            Char* freeVirt   = NULL;

            memStr    = strtok( line, " " );
            totalVirt = strtok( NULL, " " );
            usedVirt  = strtok( NULL, " " );
            freeVirt  = strtok( NULL, " " );

            if( strFreeVirt != NULL )
            {
                freeVirtMem = atoi(freeVirt);
            }

            found = true;
        }
    }

    fclose(file);
    return freeVirtMem / pow( BYTE_FORMAT_FACTOR, pByteFormat ) + 0.5f;
}


Float SystemInfo::GetMemoryLoad()
{
    return GetUsedPhysMem( SystemInfo::Byte ) / (Float)GetTotalPhysMem( SystemInfo::Byte );
}


UInt64 SystemInfo::GetCycles()
{
    UInt64 x;
    __asm__ __volatile__ (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}


Double SystemInfo::GetSeconds()
{
    UInt32 L,H;
    asm("rdtsc" : "=a" (L), "=d" (H));
    return ((Double)L +  4294967296.0 * (Double)H) * mSecondsPerCycle;
}


} // namespace Gamedesk
