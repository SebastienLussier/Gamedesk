/**
 *  @file       LWOReader.h
 *  @brief      Lightwave model reader.
 *  @author     Sébastien Lussier.
 *  @date       08/01/04.
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


class MemoryFile
{
public:
    MemoryFile();
    MemoryFile( const String& pFilename );
    ~MemoryFile();

    void open( const String& pFilename );
    void close();

    MemoryFile& operator>> ( Bool& val );
    MemoryFile& operator>> ( Char& val );
    MemoryFile& operator>> ( Byte& val );
    MemoryFile& operator>> ( Int16& val );
    MemoryFile& operator>> ( UInt16& val );
    MemoryFile& operator>> ( Int32& val );
    MemoryFile& operator>> ( UInt32& val );
    MemoryFile& operator>> ( Float& val );
    MemoryFile& operator>> ( Double& val );
 
    Bool fail() const;
    void clear() const;

    MemoryFile& read( Char* s, std::streamsize n );

    MemoryFile& seekg( std::streampos pos );
    MemoryFile& seekg( std::streamoff off, std::ios_base::seekdir dir );

    std::streampos tellg();
 
private:
    Bool                mEOF;
    Bool                mFailed;
    UInt32              mFileCursor;
    UInt32              mFileSize;
    Byte*               mFileBytes;
};


#endif  //  _MEMORY_FILE_H_
