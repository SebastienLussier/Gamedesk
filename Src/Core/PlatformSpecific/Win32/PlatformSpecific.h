/**
 *  @file       PlatformSpecific.h
 *  @brief      Declarations and defines specific to Windows.
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
#ifndef     _PLATFORM_SPECIFIC_H_
#define     _PLATFORM_SPECIFIC_H_


#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <scrnsave.h>
#include <io.h>


///////////////////////////////////////////////////////////////////////////////
// Disable unwanted warnings
#pragma warning (disable:4097) // typedef-name X used as synonym for class-name Y
#pragma warning (disable:4127) // conditional expression is constant
#pragma warning (disable:4201) // nonstandard extension used : nameless struct/union
#pragma warning (disable:4244) // conversion from 'double' to 'float', possible loss of data
#pragma warning (disable:4251) // class X needs to have dll-interface to be used by clients of class Y
#pragma warning (disable:4275) // non dll-interface class X used as base for dll-interface class Y
#pragma warning (disable:4660) // template-class specialization 'X<T>' is already instantiated

#ifndef GD_DEBUG
    // only activated in debug: in release gives warning for every try{} catch{} block...
    #pragma warning (disable:4702) // unreachable code 
#endif

#pragma warning (disable:4710) // inline function not expanded
#pragma warning (disable:4711) // function selected for automatic inlining
#pragma warning (disable:4786) // identifier was truncated to '255' characters in the debug information


///////////////////////////////////////////////////////////////////////////////
// Defines
#define GD_WINDOWS  1                       // We are under Windows

#define DLL_IMPORT      __declspec(dllimport)   // Import from DLL
#define DLL_EXPORT      __declspec(dllexport)   // Export to DLL
#define STDCALL         __stdcall               // Standard calling convention

#define INLINE          __inline                // Inline code
#define FORCEINLINE     __forceinline           // Force code to be inline
    
#define GD_PLATFORM_NAME                "Win32"

#if _MSC_VER < 1300
    // Not supported by VC6 and below
    #define __FUNCTION__                "??"
#endif

// Library extensions
#ifdef GD_DEBUG
    static const char LIBRARY_EXTENSION[]       = "_d.dll";
    static const char LIBRARY_EXTENSION_DEBUG[] = "_d.dll";
#else
    static const char LIBRARY_EXTENSION[]       = ".dll";
    static const char LIBRARY_EXTENSION_DEBUG[] = "_d.dll";
#endif

// Compiler
#ifdef GD_DEBUG
    #define COMPILER "Visual C++ - Debug Build"
#else
    #define COMPILER "Visual C++ - Release Build"
#endif


namespace Gamedesk {


FORCEINLINE void debugBreak()
{
    __debugbreak();
}


class CORE_API AssertManager
{
public:
    enum AssertResult
    {
        ASSERT_IGNORE,
        ASSERT_RETRY,
        ASSERT_ABORT
    };

    AssertManager( const Char* pDescription, UInt32 pLine, const Char* pFile, const Char* pFunction, Bool& pIgnoreAlways );
    AssertResult ShowAssert();

private:
    AssertManager( const AssertManager& pOther );
    const AssertManager& operator = ( const AssertManager& pOther );

private:
    const Char* mFile;
    const Char* mFunction;
    const Char* mDescription;
    UInt32      mLine;
    Bool&       mIgnoreAlways;
        
    Char        mText[1024];
};


namespace Core
{
    Handle CORE_API OpenLibrary( const Char* strName );
    void   CORE_API CloseLibrary( Handle hDL );

    void CORE_API MsgBox( const char* pStrTitle, const char* pStrTextFormat, ... );

    // Output debug info to the Output Window
    void CORE_API DebugOut( const Char* pFormat, ... );

    // Output info to the console
    void CORE_API SystemOut( const Char* pFormat, ... );
}


} // namespace Gamedesk


#endif  //  _PLATFORM_SPECIFIC_H_
