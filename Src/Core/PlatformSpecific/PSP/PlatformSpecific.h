/**
 *  @file       PlatformSpecific.h
 *  @brief      Declaration and defines specific to the PSP.
 *  @author     Sebastien Lussier.
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

#define DLL_EXPORT                      extern "C"
#define DLL_IMPORT
#define STDCALL

#define INLINE                          inline
#define FORCEINLINE                     inline

#define GD_PLATFORM_NAME                "PSP"

const char LIBRARY_PREFIX[]             = "lib";
const char LIBRARY_EXTENSION[]          = ".prx";
const char LIBRARY_EXTENSION_DEBUG[]    = ".prx";

// Path Separator
#define PATH_SEPARATOR ("/")

// Secure functions not available on this platform
#define fopen_s(pFile, filename, mode)      *(pFile) = fopen((filename), (mode))
#define sscanf_s                            sscanf
#define vsprintf_s                          vsprintf


namespace Gamedesk {


INLINE void debugBreak()
{
    // #TODO - Code this
    *((int*)0) = 0xBADDC0DE;
}



namespace Core
{

///////////////////////////////////////////////////////////////////////////////
/// Open Library
///////////////////////////////////////////////////////////////////////////////
INLINE Handle OpenLibrary( const Char* /*pName*/ )
{
    // #TODO-PSP - Manage PRX here
    debugBreak();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// Close Library
///////////////////////////////////////////////////////////////////////////////
INLINE void CloseLibrary( Handle /*pHandle*/ )
{
    // #TODO-PSP - Manage PRX here
    debugBreak();
}

INLINE void MsgBox( const char* pTitle, const char* pText )
{
    printf( "%s, %s", pTitle, pText );
}

// Output debug info to the Output Window
INLINE void CORE_API DebugOut( const Char* pFormat, ... )
{
    va_list args;
    va_start( args, pFormat );
    vprintf(pFormat, args);
    va_end( args );
}

// Output info to the console
INLINE void CORE_API SystemOut( const Char* pFormat, ... )
{
    va_list args;
    va_start( args, pFormat );
    vprintf(pFormat, args);
    va_end( args );
}

} // Namespace Core


class CORE_API AssertManager
{
public:
    enum AssertResult
    {
        ASSERT_IGNORE,
        ASSERT_RETRY,
        ASSERT_ABORT
    };

    AssertManager( const Char* pDescription, UInt32 pLine, const Char* pFile, const Char* pFunction, Bool& pIgnoreAlways )
        : mFile(pFile)
        , mFunction(pFunction)
        , mDescription(pDescription)
        , mLine(pLine)
        , mIgnoreAlways(pIgnoreAlways)
    {
    }

    AssertResult ShowAssert()
    {
        Core::DebugOut( "ASSERTION FAILED!\n%s\n\nFile: %s\nLine: %d\nFunction: %s\n\nCall Stack:\n\n\n", mDescription, mFile, mLine, mFunction );
        debugBreak();
        return ASSERT_IGNORE;
    }

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


} // namespace Gamedesk


#endif  //  _PLATFORM_SPECIFIC_H_
