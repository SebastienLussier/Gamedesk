/**
 *  @file      Reader.h
 *  @brief	   Generic VCProj reader.
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
#ifndef     _VC_READER_H_
#define     _VC_READER_H_


#include "Stream/Stream.h"
#include "FileManager/MemoryFile.h"


namespace VC
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

        void Read( const String& pFileName )
        {
            static const char whitespaces[] = { 10, 13, '\t', ' ', 0 };
            static const char delimiters[]  = { '<', '>', '/', '=', '\"', '?', 0 };

            if( !mMemoryFile )
                mMemoryFile = GD_NEW(MemoryFile, this, "MemoryFile");
            else
                mMemoryFile->Close();

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
            mTokenizer.EatWhite();

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

        const String& ReadToken( String& pValue )
        {
            mTokenizer.EatWhite();
            mTokenizer >> pValue;
            return pValue;
        }

        String ReadToken()
        {
            String val;
            ReadToken( val );
            return val;
        }

        Char NextChar()
        {
            mTokenizer.EatWhite();
            return mTokenizer.NextChar();
        }

        Char ReadChar()
        {
            mTokenizer.EatWhite();
            return mTokenizer.GetChar();
        }

    protected:
        StringTokenizer mTokenizer;

    private:
        MemoryFile*     mMemoryFile;    
    };
}


#endif  //  _VC_READER_H_
