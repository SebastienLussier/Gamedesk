/**
 *  @file       PlatformSpecific.cpp
 *  @brief      Declarations and defines specific to Windows.
 *  @author     Sébastien Lussier.
 *  @date       17/03/05.
 */
/*
 *  Copyright (C) 2005 Gamedesk
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
#include "PlatformSpecific.h"


namespace Gamedesk {
	
	
AssertManager::AssertManager( const Char* pDescription, UInt32 pLine, const Char* pFile, const Char* pFunction, Bool& pIgnoreAlways )
    : mFile(pFile)
    , mFunction(pFunction)
    , mDescription(pDescription)
    , mLine(pLine)
    , mIgnoreAlways(pIgnoreAlways)
{
}

AssertManager::AssertResult AssertManager::ShowAssert()
{
    sprintf( mText, "Assertion failed!\n%s\n\nFile: %s\nLine: %d\nFunction: %s\n\nCall Stack:\n\n\n", mDescription, mFile, mLine, mFunction );

    Int32 result = ::MessageBox( NULL, mText, "Gamedesk - ASSERTION FAILED !", MB_ABORTRETRYIGNORE | MB_ICONERROR );
    AssertResult ar = ASSERT_IGNORE;

    switch( result )
    {
    case IDIGNORE:  mIgnoreAlways = true;
                    ar = ASSERT_IGNORE;
                    break;

    case IDRETRY:   ar = ASSERT_RETRY;
                    break;

    case IDABORT:   ar = ASSERT_ABORT;
                    break;
    }

    if( ar == ASSERT_ABORT )
        exit(0);

    return ar;
}


namespace Core
{
    Handle OpenLibrary( const Char* strName )
    {
        Char strLibName[64];
        strcpy( strLibName, strName );
        strcat( strLibName, LIBRARY_EXTENSION );

        Handle hLib = LoadLibrary(strLibName);

        if( !hLib )
        {
            Char strPluginLibName[128];
            sprintf( strPluginLibName, "Plugins/%s", strLibName );
            hLib = LoadLibrary( strPluginLibName );
        }

        return hLib;
    }

    void CloseLibrary( Handle hDL )
    {
        FreeLibrary( (HINSTANCE)hDL );
    }

    void MsgBox( const char* pStrTitle, const char* pStrTextFormat, ... )
    {
        va_list	    ptrArguments;
	    static char	dlgString[512];

	    va_start( ptrArguments, pStrTextFormat );
		    vsprintf( dlgString, pStrTextFormat, ptrArguments );	
	    va_end( ptrArguments );
        
        ::MessageBox( NULL, dlgString, pStrTitle, 0 );
    }

    // Output debug info to the Output Window
    void DebugOut( const Char* pFormat, ... )
    {
	    va_list	    ptrArguments;
	    static char	debugString[512];

	    va_start( ptrArguments, pFormat );
		    vsprintf( debugString, pFormat, ptrArguments );	
	    va_end( ptrArguments );

        if(IsDebuggerPresent())
	        OutputDebugString( debugString );
        else
            printf(debugString);
    }

    // Output info to the console
    void SystemOut( const Char* pFormat, ... )
    {
        va_list	    ptrArguments;
	    static char	debugString[512];

	    va_start( ptrArguments, pFormat );
		    vsprintf( debugString, pFormat, ptrArguments );	
	    va_end( ptrArguments );

        printf("%s", debugString );
	    OutputDebugString( debugString );
    }
}


} // namespace Gamedesk
