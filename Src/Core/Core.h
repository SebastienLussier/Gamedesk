/**
 *  @file       Core.h
 *  @brief      Definition and standard includes for the Core module.
 *  @author     Sébastien Lussier.
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
#ifndef     _CORE_H_
#define     _CORE_H_


#ifndef CORE_API
    #define CORE_API        DLL_IMPORT
#endif


///////////////////////////////////////////////////////////////////////////////
// Ansi C++ includes.
#include <errno.h>          // System wide error numbers
#include <float.h>          // Constants for floating point values
#include <malloc.h>         // Declarations and definitions for memory allocation functions
#include <math.h>           // Definitions and declarations for math library
#include <new>              // New standard header
#include <stdarg.h>         // Defines ANSI-style macros for variable argument functions
#include <stdio.h>          // Definitions/declarations for standard I/O routines
#include <stdlib.h>         // Declarations/definitions for commonly used library functions
#include <string.h>         // Declarations for string manipulation functions
#include <sys/stat.h>       // Defines structure used by stat() and fstat()
#include <sys/types.h>      // Types returned by system level calls for file and time info
#include <time.h>           // Definitions/declarations for time routines
#include <limits.h>         // Integers limits constants


///////////////////////////////////////////////////////////////////////////////
// Build options.
#include "BuildOptions.h"


///////////////////////////////////////////////////////////////////////////////
// Platform specific inclusions.
#include "Types/Types.h"
#include "PlatformSpecific/PlatformSpecific.h"


///////////////////////////////////////////////////////////////////////////////
// Memory.
#include "Memory/Memory.h"


///////////////////////////////////////////////////////////////////////////////
// Debugging utils.
#include "Debug/DebugUtil.h"


///////////////////////////////////////////////////////////////////////////////
// String utils.
#include "Containers/StringUtils.h"


///////////////////////////////////////////////////////////////////////////////
// Stream base classes.
#include "Stream/Stream.h"


///////////////////////////////////////////////////////////////////////////////
// STL containers.
#include "Containers/Containers.h"


///////////////////////////////////////////////////////////////////////////////
// Object/Class definition.
#include "Object/Object.h"
#include "Object/ObjectIterator.h"
#include "Object/Class.h"


#define BitCheck(var, bit)   ((var & (1 << bit)) ? true : false)
#define BitSet(var, bit)     ((var &= (1 << bit)))

namespace Gamedesk
{
    void StaticInitCoreClasses();
}


#endif  //  _CORE_H_
