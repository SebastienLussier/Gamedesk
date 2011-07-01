/**
 *  @file       MemoryFile.h
 *  @brief      A memory mMapped file class to speed up file read.
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
#ifndef     _MEMORY_FILE_H_
#define     _MEMORY_FILE_H_


#include "Containers/StringUtils.h"


namespace Gamedesk {
	
	
class CORE_API MemoryFile
{
public:
    MemoryFile();
    MemoryFile( const String& pFilename, Bool pIsReadOnly );
    virtual ~MemoryFile();

    Byte* GetMemory()
    {
        return mMemory;
    }

    UInt32 GetSize()
    {
        return mFileSize;
    }

    void   Open( const String& pFilename, Bool pIsReadOnly );
    void   Close();

private:
    HANDLE mFileHandle;
    HANDLE mMappingHandle;
    Byte*  mMemory;
    UInt32 mFileSize;
};


class CORE_API StringTokenizer
{
public:
    StringTokenizer();
    StringTokenizer(const Char* pWhitespace, const Char* pDelimiters = NULL);
    StringTokenizer(const Char* pText, const Char* pWhitespace, const Char* pDelimiters = NULL);
    StringTokenizer(const Char* pText, UInt32 pLength, const Char* pWhitespace, const Char* pDelimiters = NULL);

    void Init(const Char* pText = 0, UInt32 pLength = 0);

    void SetWhiteSpaces(const Char* pWhitespace);
    void SetDelimiters(const Char* pDelimiters);

    Char GetChar()
    {
        Char val = *mCurrentPos;
        mCurrentPos++;

        mCurToken = 0;
        mCurTokenLength = 0;

        return val;
    }

    void UngetChar()
    {
        if( !AtStart() )
            mCurrentPos--;
    } 

    Char PreviousChar() const
    {
        if( !AtStart() )
            return *(mCurrentPos-1);
        
        return NextChar();
    }

    Char NextChar() const
    {
        if( !AtEnd() )
            return *mCurrentPos;

        return '\0';
    }

    Bool AtStart() const
    {
        return mCurrentPos == mStartPos;
    }

    Bool AtEnd() const
    {
        return mCurrentPos == mEndPos;
    }

    StringTokenizer& GetLine( Char* pStr, UInt32 pBufferSize, Char pDelim = '\n' )
    {
        mCurToken = (Char*)mCurrentPos;
        while( *mCurrentPos != (Byte)pDelim && 
               !AtEnd() &&
               ((UInt32)(mCurrentPos - (Byte*)mCurToken) + 1) != pBufferSize )
               mCurrentPos++;

        mCurTokenLength = mCurrentPos - (Byte*)mCurToken;

        UInt32 len = (UInt32)strlen(pStr);
        memcpy( pStr + len, mCurToken, mCurTokenLength );
        pStr[len + mCurTokenLength] = '\0';

        // Skip delimiter
        if( !AtEnd() )
            mCurrentPos++;

        return *this;
    }

    StringTokenizer& Ignore( UInt32 pCount, Char pDelim = '\0' )
    {
        if( *mCurrentPos == pDelim )
        {
            mCurrentPos++;
            return *this;
        }
   
        for( UInt32 i = 0; i < pCount && !AtEnd(); i++ )
        {
            mCurrentPos++;

            if( *mCurrentPos == pDelim )
            {
                mCurrentPos++;
                return *this;
            }
        }

        return *this;
    }

    void EatWhite()
    {
        while( !AtEnd() && IsWhitespace(*mCurrentPos) )
            mCurrentPos++;
    }

    StringTokenizer& operator>> ( Bool& val )
    {
        GetNextToken();
        val = atoi( GetCurrentToken() ) == 1;
        return *this;
    }
    
    StringTokenizer& operator>> ( Char& val )
    {
        GetNextToken();
        val = atoi( GetCurrentToken() );
        return *this;
    }
    
    StringTokenizer& operator>> ( Byte& val )
    {
        GetNextToken();
        val = atoi( GetCurrentToken() );
        return *this;
    }
    
    StringTokenizer& operator>> ( Int16& val )
    {
        GetNextToken();
        val = atoi( GetCurrentToken() );
        return *this;
    }
    
    StringTokenizer& operator>> ( UInt16& val )
    {
        GetNextToken();
        val = atoi( GetCurrentToken() );
        return *this;
    }
    
    StringTokenizer& operator>> ( Int32& val )
    {
        GetNextToken();
        val = atoi( GetCurrentToken() );
        return *this;
    }
    
    StringTokenizer& operator>> ( UInt32& val )
    {
        GetNextToken();
        val = atoi( GetCurrentToken() );
        return *this;
    }

    StringTokenizer& operator>> ( Int64& val )
    {
        GetNextToken();
        val = StringTo<Int64>( GetCurrentToken() );
        return *this;
    }

    StringTokenizer& operator>> ( UInt64& val )
    {
        GetNextToken();
        val = StringTo<UInt64>( GetCurrentToken() );
        return *this;
    }
    
    StringTokenizer& operator>> ( Float& val )
    {
        Int32 bPointIsDelimiter = IsDelimiter('.');
        if( bPointIsDelimiter )
            mIsDelimiter['.' >> 3] &= ~(1 << ('.' & 7));
        
        GetNextToken();
        val = atof( GetCurrentToken() );
        
        if( bPointIsDelimiter )
            mIsDelimiter['.' >> 3] |= (1 << ('.' & 7));

        return *this;
    }
    
    StringTokenizer& operator>> ( Double& val )
    {
        Int32 bPointIsDelimiter = IsDelimiter('.');
        if( bPointIsDelimiter )
            mIsDelimiter['.' >> 3] &= ~(1 << ('.' & 7));

        GetNextToken();
        val = atof( GetCurrentToken() );

        if( bPointIsDelimiter )
            mIsDelimiter['.' >> 3] |= (1 << ('.' & 7));

        return *this;
    }

    StringTokenizer& operator>> ( String& val )
    {
        GetNextToken();
        val = GetCurrentToken();
        return *this;
    }    

    Int32 IsWhitespace( Byte pCurPos );
    Int32 IsDelimiter( Byte pCurPos );
  
protected:
    void            GetNextToken();
    const Char*     GetCurrentToken();
    void            GetPreviousToken();
    

private:  
    // Parsing stuff
    Char    mIsWhitespace[32];
    Char    mIsDelimiter[32];

    Byte*   mStartPos;
    Byte*   mEndPos;
    Byte*   mCurrentPos;
    UInt32  mLength;

    // Current token
    Char*   mCurToken;
    UInt32  mCurTokenLength;
    //String  mCurTokenStr;
    Char    mCurTokenStr[1024];
};

inline Int32 StringTokenizer::IsWhitespace( Byte pCurPos )
{
    return mIsWhitespace[pCurPos >> 3] & (1 <<(pCurPos & 7));
}

inline Int32 StringTokenizer::IsDelimiter( Byte pCurPos )
{
    return mIsDelimiter[pCurPos >> 3] & (1 <<(pCurPos & 7));
}

inline const Char* StringTokenizer::GetCurrentToken()
{
    if( mCurToken )
    {
        memcpy( mCurTokenStr, mCurToken, mCurTokenLength );
        mCurTokenStr[mCurTokenLength] = '\0';
    }
    else
        mCurTokenStr[0] = '\0';

    return mCurTokenStr;
}


} // namespace Gamedesk


#endif  //  _MEMORY_FILE_H_
