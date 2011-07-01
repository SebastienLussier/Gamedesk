/**
 *  @file       PlatformSpecific.h
 *  @brief      Declaration and defines specific to Linux.
 *  @author     Patrick Girard.
 *  @date       20/01/02.
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
#ifndef     _PLATFORM_SPECIFIC_H_
#define     _PLATFORM_SPECIFIC_H_


#include <sys/utsname.h>                // System Identification
#include <dlfcn.h>

#define GD_LINUX                        1

#define DLL_EXPORT                      extern "C"
#define DLL_IMPORT
#define STDCALL

#define INLINE                          __inline        // Inline code
#define FORCEINLINE                     __inline

#define GD_PLATFORM_NAME                "Linux"

const char LIBRARY_PREFIX[]             = "lib";
const char LIBRARY_EXTENSION[]          = ".so";
const char LIBRARY_EXTENSION_DEBUG[]    = ".so";

// Path Separator
#define PATH_SEPARATOR ("/")


namespace Gamedesk {
	

INLINE void debugBreak()
{
    asm("int $03");
}

namespace Core
{

///////////////////////////////////////////////////////////////////////////////
/// Open Library
///////////////////////////////////////////////////////////////////////////////
INLINE Handle OpenLibrary( const Char* pName )
{
    Char libName[64];
    strcpy( LibName, LIBRARY_PREFIX );
    strcat( libName, pName );

    for( Int32 i = 3; i < strlen(libName); i++ )
    {
        libName[i] = ToLower(libName[i]);
    }
    strcat( libName, LIBRARY_EXTENSION );

    Handle hLib = dlopen( libName, RTLD_NOW );

    if( !hLib )
    {
        Char pluginLibName[128];
        sprintf( pluginLibName, "plugins/%s", libName );
        hLib = dlopen( pluginLibName, RTLD_NOW );
    }

    return hLib;
}

///////////////////////////////////////////////////////////////////////////////
/// Close Library
///////////////////////////////////////////////////////////////////////////////
INLINE void CloseLibrary( Handle pHandle )
{
    dlclose(pHandle);
}

INLINE void MsgBox( const char* pTitle, const char* pText )
{
    printf( "%s, %s", pTitle, pText );
}


} // namespace Core
} // namespace Gamedesk


#endif  //  _PLATFORM_SPECIFIC_H_
