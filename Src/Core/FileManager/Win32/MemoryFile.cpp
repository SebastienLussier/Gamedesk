/**
 *  @file       MemoryFile.h
 *  @brief      A memory mapped file class to speed up file read.
 *  @author     Sébastien Lussier.
 *  @date       24/10/04.
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
#include "FileManager/MemoryFile.h"


namespace Gamedesk {
	
	
MemoryFile::MemoryFile()
    : mFileHandle(INVALID_HANDLE_VALUE)
    , mMappingHandle(INVALID_HANDLE_VALUE)
    , mMemory(NULL)
    , mFileSize(0)
{
}

MemoryFile::MemoryFile( const String& pFilename, Bool pIsReadOnly )
    : mFileHandle(INVALID_HANDLE_VALUE)
    , mMappingHandle(INVALID_HANDLE_VALUE)
    , mMemory(NULL)
    , mFileSize(0)
{
    Open( pFilename, pIsReadOnly );
}

MemoryFile::~MemoryFile()
{
    Close();
}

void MemoryFile::Open( const String& pFilename, Bool pIsReadOnly )
{
    // Open file on disk  
    UInt32 flags = pIsReadOnly ? GENERIC_READ : (GENERIC_READ|GENERIC_WRITE);

    mFileHandle = CreateFile( pFilename.c_str(), flags, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    if( mFileHandle == INVALID_HANDLE_VALUE )  
    {
        UInt32 error = GetLastError();
        GD_ASSERT( error != S_OK );
    }
    
    mFileSize = ::GetFileSize( mFileHandle, NULL );

    // Create file mapping  
    UInt32 accessFlags = pIsReadOnly ? PAGE_READONLY : PAGE_READWRITE;
    mMappingHandle = CreateFileMapping( mFileHandle, NULL, accessFlags, 0, mFileSize, 0 );
    if( mMappingHandle == INVALID_HANDLE_VALUE )  
    {
        UInt32 error = GetLastError();
        GD_ASSERT( error != S_OK );
    }

    // Map view of file
    UInt32 viewFlag = pIsReadOnly ? FILE_MAP_READ : FILE_MAP_WRITE;
    mMemory =  (Byte*)MapViewOfFile( mMappingHandle, viewFlag, 0, 0, 0 );
    if( !mMemory )
    {
        UInt32 error = GetLastError();
        GD_ASSERT( error != S_OK );
    }
}

void MemoryFile::Close()
{
    if( !mMemory )
        return;

    UnmapViewOfFile( mMemory );
    CloseHandle( mMappingHandle ); 
    CloseHandle( mFileHandle ); 

    mMemory = NULL;
    mFileHandle = INVALID_HANDLE_VALUE;
    mMappingHandle = INVALID_HANDLE_VALUE;
    mFileSize = 0;
}

StringTokenizer::StringTokenizer()
{
    memset( mIsWhitespace, 0, sizeof(mIsWhitespace) );
    memset( mIsDelimiter, 0, sizeof(mIsDelimiter) );
    Init();
}

StringTokenizer::StringTokenizer(const Char* pWhitespace, const Char* pDelimiters)
{
    SetWhiteSpaces(pWhitespace);
    SetDelimiters(pDelimiters);
    Init();
}

StringTokenizer::StringTokenizer(const Char* pText, const Char* pWhitespace, const Char* pDelimiters)
{
    SetWhiteSpaces(pWhitespace);
    SetDelimiters(pDelimiters);
    Init(pText, 0);
}

StringTokenizer::StringTokenizer(const Char* pText, UInt32 pLength, const Char* pWhitespace, const Char* pDelimiters)
{
    SetWhiteSpaces(pWhitespace);
    SetDelimiters(pDelimiters);
    Init(pText, pLength);
}

void StringTokenizer::Init(const Char* pText, UInt32 pLength)
{
    mCurrentPos     = (Byte*)pText;
    mStartPos       = (Byte*)pText;
    mCurTokenLength = 0;
    mLength         = pLength != 0 ? pLength : pText ? strlen(pText) : 0;
    mEndPos         = mStartPos + mLength;
}

void StringTokenizer::SetWhiteSpaces(const Char* pWhitespaces)
{
    // initialise whitespace table
    memset( mIsWhitespace, 0, sizeof(mIsWhitespace) );
    if( pWhitespaces )
    {
        for( Byte* ctrl = (Byte*)pWhitespaces; *ctrl; *ctrl++ )
            mIsWhitespace[*ctrl >> 3] |= (1 << (*ctrl & 7));
    }    
}

void StringTokenizer::SetDelimiters(const Char* pDelimiters)
{
    // initialise delimiter table
    memset( mIsDelimiter, 0, sizeof(mIsDelimiter) );
    if( pDelimiters )
    {
        for( Byte* ctrl = (Byte*)pDelimiters; *ctrl; *ctrl++ )
            mIsDelimiter[*ctrl >> 3] |= (1 << (*ctrl & 7));
    }
}

void StringTokenizer::GetNextToken()
{
    // skip till end of token
    while( !AtEnd() && IsWhitespace(*mCurrentPos) )
        mCurrentPos++;

    // this is the start of the token
    mCurToken = (char*)mCurrentPos;

    // skip till end of token
    for(; !AtEnd(); ++mCurrentPos)
    {
        if( IsWhitespace(*mCurrentPos) )
            break;

        if( IsDelimiter(*mCurrentPos) )
        {
            if( mCurrentPos == (Byte*)mCurToken )
                ++mCurrentPos;
            break;
        }
    }

    // end of search condition
    if( mCurrentPos == (Byte*)mCurToken )  
    {
        mCurToken = 0;
        mCurTokenLength = 0;  
    }
    else
    {
        mCurTokenLength = mCurrentPos - (Byte*)mCurToken;
    }
}

void StringTokenizer::GetPreviousToken()
{
    Byte* pEndOfToken;

    // move to end of string if we are out of it.
    if( *mCurrentPos == 0 )
	    --mCurrentPos;

    // skip till end of previous token
    while( mCurrentPos > mStartPos && IsWhitespace(*mCurrentPos) )
        mCurrentPos--;  

    pEndOfToken = mCurrentPos;

    // skip till start of previous token
    for( ; mCurrentPos > mStartPos; --mCurrentPos )
    {
        if( IsWhitespace(*mCurrentPos) )
            break;

        if( IsDelimiter(*mCurrentPos) )
        {
            if( pEndOfToken == mCurrentPos )
                --mCurrentPos;
            break;
        }
    }


    if( pEndOfToken == mCurrentPos )  
    {
	    mCurToken = 0;
	    mCurTokenLength = 0;  
    }
    else
    {
	    mCurToken = (char*)mCurrentPos+1;
        mCurTokenLength = pEndOfToken - mCurrentPos;
    }
}


} // namespace Gamedesk
