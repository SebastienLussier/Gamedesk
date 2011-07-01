/**
 *  @file       FileManager.h
 *  @brief      Utility for debugging and validation (i.e. asserts).
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
#ifndef     _FILE_MANAGER_H_
#define     _FILE_MANAGER_H_


#include "Exception/Exception.h"
#include "Containers/Containers.h"


namespace Gamedesk {


/**
 *  Get information on the file system.
 *  Use this class to get the content of a directory, or find the path to a
 *  default folder.
 *  @author Sebastien Lussier.
 *  @date   27/10/03.
 */
class CORE_API FileManager
{
public:
    /**
     *  Get the default temp folder path.
     *  @return The default temp folder path.
     */
    static String GetTempPath();

    /**
     *  Get the current path.
     *  @return A string, the current path.
     */
    static String GetCurrentPath();

    /**
     *  Set the current path.
     *  @param  pCurrentPath    the current path.
     */
    static void SetCurrentPath( String pCurrentPath );

    /**
     *  Get a list of the files matching a search string in a given directory.
     *  @brief  Search for files in a directory.
     *  @param  pDir                A String, the directory you want the content to be listed.
     *  @param  pEndsWith           The end of the filenames to look for
     *  @param  pFileNames          A reference to a Vector of String that will contain all file names.
     */
    static void FindFiles( String pDir, String pEndsWith, Vector<String>& pFileNames );

    /**
     *  Get the file extension of the given file.
     *  @param  pFilename   The file name.
     *  @return The file extension.
     */
    static String GetFileExtension( const String& pFilename );

    /**
     *  Delete a file from the disk.
     *  @param  pFilename   The file name.
     */
    static void DeleteFile( const String& pFilename );

    /**
     *  Move a file
     *  @param pSource          Source file
     *  @param pDestination     Destination file
     *  @param pReplaceExisting Set this parameter to True if you want to replace an existing file.
     */
    static Bool MoveFile( const String& pSource, const String& pDestination, Bool pReplaceExisting );

    /**
     *  Check if a file exist on disk.
     *  @param  pFilename   The file name.
     *  @return True if the file exist, false otherwise.
     */
    static Bool FileExist( const String& pFilename );

    static Stream* CreateInputStream( const String& pFile );
    static Stream* CreateOutputStream( const String& pFile );
};


class CORE_API FileNotFoundException : public Exception
{
    DECLARE_EXCEPTION(FileNotFoundException);

public:
    FileNotFoundException( const String& pFileName, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage  = "File not found : ";
        mMessage += pFileName;
        mMessage += ".";
        DebugOut();
    }
};


class CORE_API File
{
public:
    static UInt32 ReadAllLines(const char* pFilename, List<String>& pLines);
};


} // namespace Gamedesk


#endif  //  _FILE_MANAGER_H_
