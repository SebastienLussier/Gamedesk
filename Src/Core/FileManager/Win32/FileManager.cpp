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
#include "FileManager/FileManager.h"


namespace Gamedesk {
	
	
String FileManager::GetTempPath()
{
    Char buffer[MAX_PATH];

    if( ::GetTempPath( MAX_PATH, buffer ) != 0 )
        return buffer;

    return "";
}

String FileManager::GetCurrentPath()
{
    Char buffer[MAX_PATH];

    if( ::GetCurrentDirectory( MAX_PATH, buffer ) != 0 )
        return buffer;

    return "";
}

void FileManager::SetCurrentPath( String pCurrentPath )
{
    ::SetCurrentDirectory( pCurrentPath.c_str() );
}

void FileManager::FindFiles( String pDir, String pEndsWith, Vector<String>& pFileNames )
{
    Bool              done;                 // Done searching for files?
    HANDLE            handleFound = NULL;   // Handle to find data.
    WIN32_FIND_DATA   findData;             // Info on file found.

    // Add trailing backslash if needed
    if( pDir.at(pDir.size()-1) != '/' && pDir.at(pDir.size()-1) != '\\' )
        pDir += '/';

    std::string searchPath = pDir;
    searchPath += "*";
    searchPath += pEndsWith;

    handleFound = FindFirstFile( searchPath.c_str(), &findData );
    done = (handleFound == INVALID_HANDLE_VALUE);

    while( !done )
    {
        pFileNames.push_back( findData.cFileName );
        done = !FindNextFile( handleFound, &findData );
    }

    if( handleFound != INVALID_HANDLE_VALUE )
        FindClose( handleFound );
}

String FileManager::GetFileExtension( const String& pFilename )
{
    String strExtension;
    UInt32 position = pFilename.rfind( '.' );

    if( position != String::npos )
    {
		strExtension = pFilename.substr( position+1 );
    }

    return strExtension;
}

void FileManager::DeleteFile( const String& pFilename )
{
	::DeleteFile( pFilename.c_str() );
}

Bool FileManager::MoveFile( const String& pSource, const String& pDestination, Bool pReplaceExisting )
{
    return ::MoveFileEx( pSource.c_str(), pDestination.c_str(), pReplaceExisting ? MOVEFILE_REPLACE_EXISTING : 0 ) == TRUE;
}

Bool FileManager::FileExist( const String& pFilename )
{
    return _access( pFilename.c_str(), _A_NORMAL ) == 0;
}



class StdFileInputStream : public InputStream
{
    friend class FileManager;

public:
    virtual ~StdFileInputStream()
    {
        Close();
    }

    Bool Open( const String& pPath )
    {
        mFile = fopen( pPath.c_str(), "rb" );
        mIsValid = (mFile != NULL);

        // Get file size.
        if( mIsValid )
        {
            fseek( mFile, 0, SEEK_END );
            mSize = ftell( mFile );
            fseek( mFile, 0, SEEK_SET );
        }

        return mIsValid;
    }

    Bool Close()
    {
        GD_ASSERT_M( mIsValid, "Trying to access an invalid stream!" );
        mIsValid = false;

        return fclose( mFile ) == 0;
    }

    UInt32 Size()
    {
        GD_ASSERT_M( mIsValid, "Trying to access an invalid stream!" );
        return mSize;
    }

    UInt32 Pos()
    {
        GD_ASSERT_M( mIsValid, "Trying to access an invalid stream!" );
        return ftell( mFile );
    }

    void Serialize( void* pData, UInt32 pLen )
    {
        GD_ASSERT_M( mIsValid, "Trying to access an invalid stream!" );
        UInt32 len = fread( pData, pLen, 1, mFile );
        GD_ASSERT_M( len == 1, "Serialization failed: could not read requested number of bytes!" );
    }

protected:
    StdFileInputStream()
        : mSize(0)
        , mFile(NULL)
    {
    }

private:
    UInt32 mSize;
    FILE*  mFile;
};


class StdFileOutputStream : public OutputStream
{
    friend class FileManager;

public:
    virtual ~StdFileOutputStream()
    {
        Close();
    }

    Bool Open( const String& pPath )
    {
        mFile = fopen( pPath.c_str(), "wb" );
        mIsValid = (mFile != NULL);

        // Get file size.
        mSize = 0;

        return mIsValid;
    }

    Bool Close()
    {
        if( mIsValid )
        {
            mIsValid = false;
            return fclose( mFile ) == 0;
        }

        return true;
    }

    UInt32 Size()
    {
        GD_ASSERT_M( mIsValid, "Trying to access an invalid stream!" );
        return mSize;
    }

    UInt32 Pos()
    {
        GD_ASSERT_M( mIsValid, "Trying to access an invalid stream!" );
        return ftell( mFile );
    }

    void Serialize( void* pData, UInt32 pLen )
    {
        GD_ASSERT_M( mIsValid, "Trying to access an invalid stream!" );

        if( pLen != 0 )
        {
            UInt32 itemsWritten = fwrite( pData, pLen, 1, mFile );
            GD_ASSERT_M( itemsWritten == 1, "Serialization failed: could not write requested number of bytes!" );
            mSize += pLen;
        }
    }

protected:
    StdFileOutputStream()
        : mSize(0)
        , mFile(NULL)
    {
    }

private:
    UInt32 mSize;
    FILE*  mFile;
};


Stream* FileManager::CreateInputStream( const String& pFile )
{
    InputStream* fileInputStream = GD_NEW(StdFileInputStream, 0, "Core::FileManager::InputStream");
    GD_ASSERT_M( fileInputStream, "Could not allocate a new StdFileInputStream" );

    if( fileInputStream )
    {
        fileInputStream->Open( pFile );

        if( !fileInputStream->IsValid() )
        {
            GD_DELETE(fileInputStream);
            fileInputStream = NULL;
        }
    }

    return fileInputStream;
}

Stream* FileManager::CreateOutputStream( const String& pFile )
{
    OutputStream* fileOutputStream = GD_NEW(StdFileOutputStream, 0, "Core::FileManager::OutputStream");
    GD_ASSERT_M( fileOutputStream, "Could not allocate a new StdFileOutputStream" );

    if( fileOutputStream )
    {
        fileOutputStream->Open( pFile );

        if( !fileOutputStream->IsValid() )
        {
            GD_DELETE(fileOutputStream);
            fileOutputStream = NULL;
        }
    }

    return fileOutputStream;
}

UInt32 File::ReadAllLines(const char* pFilename, List<String>& pLines)
{
    std::ifstream ifs( pFilename );
    
    String line;
    UInt32 lineCount = 0;
    while( getline( ifs, line ) )
    {
        lineCount++;
        pLines.push_back( line );
    }

    return lineCount;
}


} // namespace Gamedesk
