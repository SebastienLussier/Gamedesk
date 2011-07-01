/**
 *  @file      Reader.h
 *  @brief	   Generic Cpp file parsing.
 *  @author    Sébastien Lussier.
 *  @date      28/02/06.
 */
/*
 *  Copyright (C) 2006 Gamedesk
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
#ifndef     _CPP_READER_H_
#define     _CPP_READER_H_


#include "FileManager/MemoryFile.h"


const UInt32 COMMENT_BUFFER_SIZE = 16*1024;


namespace Cpp
{
    class Reader
    {
    public:
        Reader() 
            : mMemoryFile(NULL)
        {
        }


        virtual ~Reader()
        {
            GD_DELETE(mMemoryFile);
        }

        virtual void Read( const String& pFileName )
        {
            static const char whitespaces[] = { 10, 13, '\t', ' ', 0 };
            static const char delimiters[]  = { '(', ')', '{', '}', '[', ']',
                ';', ':', '+', '-', '*', '/',
                '=', '!', '.', '<', '>', '&',
                '?', '~', '|', '#', '\'', '\"', 
                ',',
                0 };

            if( !mMemoryFile )
                mMemoryFile = GD_NEW(MemoryFile, this, "CppReader::MemoryFile");
            else
                mMemoryFile->Close();

            // Read all the file data into the string stream.
            mMemoryFile->Open( pFileName, true );
            mTokenizer.Init( (Char*)mMemoryFile->GetMemory(), mMemoryFile->GetSize() );
            mTokenizer.SetWhiteSpaces(whitespaces);
            mTokenizer.SetDelimiters(delimiters);
        }

    protected:    
        void MustRead( const String& pValue )
        {
            String tmp;
            ReadToken( tmp );
            if( tmp != pValue )
                throw "Invalid token found!";
        }

        const String& ReadString( String& pValue )
        {
            SkipComments();

            pValue = "";

            Char c;
            c = mTokenizer.GetChar();
            GD_ASSERT( c == '"' );

            c = mTokenizer.GetChar();
            while( c != '"' )
            {
                pValue += c;
                c = mTokenizer.GetChar();
            }

            return pValue;
        }

        Char ConvertEscape( Char pEscape )
        {
            switch( pEscape )
            {
            case 'a':   return '\a';
            case 'b':   return '\b';
            case 't':   return '\t';
            case 'n':   return '\n';
            case 'v':   return '\v';
            case 'f':   return '\f';
            case 'r':   return '\r';
            }

            return pEscape;
        }

        const String& ReadToken( String& pValue )
        {
            SkipComments();
            mTokenizer >> pValue;

            if( strcmp(pValue.c_str(), "\"") == 0 )
            {
                Char c;
                do 
                {
                    c = ReadChar();                    
                    if( c == '\\' )
                        pValue += ConvertEscape(ReadChar());
                    else
                        pValue += c;
                } while( c != '\"' );
            }

            if( strcmp(pValue.c_str(), "\'") == 0 )
            {
                Char c;
                do 
                {
                    c = ReadChar();
                    if( c == '\\' )
                        pValue += ConvertEscape(ReadChar());
                    else
                        pValue += c;
                } while( c != '\'' );
            }

            return pValue;
        }

        String ReadToken()
        {
            String val;
            ReadToken( val );
            return val;
        }

        void SkipComments()
        {
            mTokenizer.EatWhite();

            UInt32 peek;

            if( mTokenizer.NextChar() == '/' )
            {
                mTokenizer.GetChar();
                peek = mTokenizer.NextChar();

                if( peek == '/' )
                {   
                    strcpy_s(mLastComment, COMMENT_BUFFER_SIZE, "/");
                    mTokenizer.GetLine( mLastComment, COMMENT_BUFFER_SIZE );
                    SkipComments();
                }
                else if( peek == '*' )
                {
                    strcpy_s(mLastComment, COMMENT_BUFFER_SIZE, "/*");
                    mTokenizer.GetChar();
                    mTokenizer.GetLine(mLastComment, COMMENT_BUFFER_SIZE, '*');

                    while(1)
                    {                
                        peek = mTokenizer.NextChar();
                        if( peek == '/' )
                        {
                            UInt32 len = (UInt32)strlen(mLastComment);
                            mLastComment[len+1] = mTokenizer.GetChar();
                            mLastComment[len+2] = '\0';
                            break;
                        }
                        else
                            mTokenizer.GetLine(mLastComment, COMMENT_BUFFER_SIZE, '*');

                        if( mTokenizer.AtEnd() )
                            throw("Comment not closed!");
                    }

                    SkipComments();
                }
                else
                {
                    mTokenizer.UngetChar();
                }
            }
        }


        Char PreviousChar()
        {
            SkipComments();
            return mTokenizer.PreviousChar();
        }

        Char ReadChar()
        {
            SkipComments();
            return mTokenizer.GetChar();
        }

        Char NextChar()
        {
            SkipComments();
            return mTokenizer.NextChar();
        }

    protected:
        StringTokenizer mTokenizer;
        Char            mLastComment[COMMENT_BUFFER_SIZE];

    private:
        MemoryFile*     mMemoryFile;    
    };
}


#endif  //  _CPP_READER_H_
