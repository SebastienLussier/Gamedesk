/**
 *  @file       BuildOptions.h
 *  @brief	    Compilation options.
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
#ifndef     _BUILD_OPTIONS_H_
#define     _BUILD_OPTIONS_H_


// Supported platforms
#define GD_PLATFORM_WIN32           1   //! Win32
#define GD_PLATFORM_PSP             2   //! PSP
#define GD_PLATFORM_LINUX           3   //! Linux

// Supported compilers
#define GD_COMPILER_VC9             1   //! Visual Studio 2008
#define GD_COMPILER_VC8             2   //! Visual Studio 2005
#define GD_COMPILER_VC7             3   //! Visual Studio 2003
#define GD_COMPILER_GCC             4   //! GCC


// Autodetect platform
#if defined(WIN32)
    #define GD_PLATFORM             GD_PLATFORM_WIN32
#elif defined(PSP)
    #define GD_PLATFORM             GD_PLATFORM_PSP
#elif defined(LINUX)
    #define GD_PLATFORM             GD_PLATFORM_LINUX
#else
    #error "Unsupported platform!"
#endif


// Autodetect compiler
#if defined(_MSC_VER)
    #if _MSC_VER >= 1700
        #error "Unsupported compiler: Microsoft Visual C++ 11 or above"
	#elif _MSC_VER >= 1600
		#define GD_COMPILER         GD_COMPILER_VC10
    #elif _MSC_VER >= 1500
        #define GD_COMPILER         GD_COMPILER_VC9
    #elif _MSC_VER >= 1400
        #define GD_COMPILER         GD_COMPILER_VC8     
    #elif _MSC_VER >= 1300
        #define GD_COMPILER         GD_COMPILER_VC7
    #else
        #error "Unsupported compiler: Microsoft Visual C++ 6 or below"
    #endif
#elif defined(__GNUC__)
    #define GD_COMPILER             GD_COMPILER_GCC
#else
    #error "Couldn't detect the compiler !"
#endif


#define GD_DISABLED                 1
#define GD_ENABLED                  2


//! Enable GD_ASSERT ?
#define GD_CFG_USE_ASSERT           GD_ENABLED      


//! Do GD_ASSERT in debug mode ?
#define GD_CFG_USE_ASSERT_DEBUG     GD_ENABLED


//! Do guard ?
#define GD_CFG_USE_GUARD            GD_ENABLED       


//! Do guard in debug mode ?
//! Slowdown the engine alot, should only be enable for debugging.
#define GD_CFG_USE_GUARD_DEBUG      GD_ENABLED


//! Enable stack tracing ?
//! Slowdown the engine alot, should only be enable for debugging.
#define GD_CFG_USE_STACK_TRACING    GD_DISABLED


//! Enable in-code profiling.
#define GD_CFG_USE_PERF_MONITOR     GD_ENABLED


#define GD_CFG_USE_PROPERTIES       GD_ENABLED


#endif  //  _BUILD_OPTIONS_H_
