/**
 *  @file       FileManager.cpp
 *  @brief	    Utility for debugging and validation (i.e. asserts).
 *  @author     Sébastien Lussier.
 *  @date       15/07/02.
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
#include "FileManager.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS( FileNotFoundException );


String FileManager::GetTempPath()
{
    return "";
}

String FileManager::GetCurrentPath()
{
    return "";
}

void FileManager::SetCurrentPath( String pCurrentPath )
{
}

void FileManager::GetFileListing( String pDir, Vector<String>& pFileNames, String pExtensionFilter, Bool pRecursive )
{
}

void FileManager::GetDirListing( String pDir, Vector<String>& pLstDirNames, Bool pRecursive )
{
}


String FileManager::GetFileExtension( const String& pFilename )
{
    String  strExtension;
    int     position = pFilename.rfind( '.' );

    if( position != String::npos )
    {
        strExtension = pFilename.substr( position+1 );
    }

    return strExtension;
}

void FileManager::DeleteFile( const String& pFilename )
{
}


} // namespace Gamedesk
