/**
 *  @file       WoWReader.h
 *  @brief      Read a WoW file into it's corresponding data structures
 *  @author     Sébastien Lussier.
 *  @date       30/04/05.
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
#ifndef     _WoW_READER_H_
#define     _WoW_READER_H_


#include "WoWFiles.h"
#include "FileManager/MemoryFile.h"


namespace WoW
{


class WoWID
{
public:
    WoWID( const WoWID& pOther )
    {
        mID = pOther.mID;
    }

    WoWID( const Char* pStr )
    {
        mID = (pStr[0] << 24) + (pStr[1] << 16) + (pStr[2] << 8) + (pStr[3] << 0);
    }

    Bool operator == ( const WoWID& pOther ) const
    {
        return mID == pOther.mID;
    }

    Bool operator != ( const WoWID& pOther) const
    {
        return mID != pOther.mID;
    }

    Stream& operator << ( Stream& pStream )
    {
        pStream << mID;
    }

private:
    UInt32  mID;
};


class WowFileStream : public InputStream
{
public:
    WowFileStream()
        : mMemFile(NULL)
        , mSize(0)
        , mPos(0)
    {
    }

    virtual ~WowFileStream()
    {
        Close();
    }

    Bool Open( const String& pPath )
    {
        mMemFile = GD_NEW(MemoryFile, this, "WowImporter::Memory File")( pPath, true );
        mIsValid = (mMemFile->GetMemory() != NULL);

        // Get file size.
        if( mIsValid )
            mSize = mMemFile->GetSize();

        mPos = 0;

        return mIsValid;
    }

    Bool Close()
    {
        if( mMemFile )
            GD_DELETE(mMemFile);

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
        return mPos;
    }

    void Serialize( void* pData, UInt32 pLen )
    {
        GD_ASSERT_M( mIsValid, "Trying to access an invalid stream!" );
        GD_ASSERT_M( mPos + pLen <= mSize, "Trying to read past file buffer!" );

        memcpy( pData, mMemFile->GetMemory() + mPos, pLen );
        mPos += pLen;
    }

    void SeekRel( Int32 pSize )
    {
        GD_ASSERT_M( (mPos + pSize <= mSize) && (mPos + pSize >= 0), "Trying to go past file buffer!" );
        mPos += pSize;
    }

    void Seek( UInt32 pPos )
    {
        GD_ASSERT_M( pPos <= mSize, "Trying to go past file buffer!" );
        mPos = pPos;
    }

private:
    MemoryFile* mMemFile;
    UInt32      mSize;
    UInt32      mPos;
};


class ADTFileReader
{
    CLASS_DISABLE_COPY(ADTFileReader);

public:
    ADTFileReader( ADTFile& pFile );

    void Read( const String& pFileName );

private:
    void ReadMapChunk( WowFileStream& pStream, ADTFile::MapChunk& pMapChunk, UInt32 pChunkSize );

private:
    ADTFile&       mADT;
};


class BLPFileReader
{
	CLASS_DISABLE_COPY(BLPFileReader);

public:
	BLPFileReader( BLPFile& pFile );

	void Read( const String& pFileName );

private:
	BLPFile&       mBLP;
};


}

#endif  //  _WoW_READER_H_
