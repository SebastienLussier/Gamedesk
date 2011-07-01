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
#ifndef     _LWO_READER_H_
#define     _LWO_READER_H_


#include "Exception/Exception.h"
#include "LWObject.h"

#include "MemoryFile.h"


#define SWAP(a,b)       { a^=b; b^=a; a^=b; }
#define SWAP_16(w)      SWAP( (((BYTE *)&w)[0]), (((BYTE *)&w)[1]) )
#define SWAP_32(l)      { SWAP_16( (((UInt16 *)&l)[0])); SWAP_16( (((UInt16 *)&l)[1])); SWAP( (((UInt16 *)&l)[0]), (((UInt16 *)&l)[1]) ) }


class LWOReader
{
public:
    LWOReader();
    
    void    Read( const String& pFilename, LWObject& pObject );
    
private:
    LWLayer*    GetCurrentLayer();

private:
    ///////////////////////////////////////////////////////////////////
    void    ReadTAGS( UInt32 pChunkSize );
    void    ReadCLIP( UInt32 pChunkSize );
    void    ReadENVL( UInt32 pChunkSize );
    void    ReadLAYR( UInt32 pChunkSize );
    void    ReadPNTS( UInt32 pChunkSize );
    void    ReadBBOX( UInt32 pChunkSize );
    void    ReadPOLS( UInt32 pChunkSize );
    void    ReadPTAG( UInt32 pChunkSize );
    void    ReadVMAP( UInt32 pChunkSize, Bool pPerPoly );
    void    ReadSURF( UInt32 pChunkSize );

    void    ReadBLOK( LWSurface* pSurface, UInt32 pChunkSize );
    void    ReadHEAD( LWSurfaceBlock* pSurfaceBlock, UInt32 pChunkSize );
    void    ReadTMAP( LWTextureMapping* pTextureMapping, UInt32 pChunkSize );

    ///////////////////////////////////////////////////////////////////
    FORCEINLINE UInt32 ReadChunk( UInt32& chunkSize )
    {
        UInt32 id4 = ReadID4();    
        ReadU4( chunkSize );

        return id4;
    }

    FORCEINLINE UInt32 ReadSubChunk( UInt16& pChunkSize, UInt32 pOffset = 0 )
    {
        pOffset;
        UInt32 id4 = ReadID4();
        ReadU2( pChunkSize );

        return id4;
    }

    FORCEINLINE UInt32 ReadID4()
    {
        UInt32 val;
        mLwoFile >> val;
        return val;
    }

    FORCEINLINE UInt32 ReadID4( UInt32& pVal )
    {
        mLwoFile >> pVal;
        return sizeof(pVal);
    }

    FORCEINLINE UInt32 ReadI1( Char& pVal )
    {
        mLwoFile >> pVal;
        return sizeof(pVal);
    }

    FORCEINLINE UInt32 ReadI2( Int16& pVal )
    {
        mLwoFile >> pVal;
        SWAP_16(pVal);
        return sizeof(pVal);
    }

    FORCEINLINE UInt32 ReadI4( Int32& pVal )
    {
        mLwoFile >> pVal;
        SWAP_32(pVal);
        return sizeof(pVal);
    }

    FORCEINLINE UInt32 ReadU1( Byte& pVal )
    {
        mLwoFile >> pVal;
        return sizeof(pVal);
    }

    FORCEINLINE UInt32 ReadU2( UInt16& pVal )
    {
        mLwoFile >> pVal;
        SWAP_16(pVal);
        return sizeof(pVal);
    }

    FORCEINLINE UInt32 ReadU4( UInt32& pVal )
    {
        mLwoFile >> pVal;
        SWAP_32(pVal);
        return sizeof(pVal);
    }

    FORCEINLINE UInt32 ReadF4( Float& pVal )
    {
        mLwoFile >> pVal;
        SWAP_32(pVal);
        return sizeof(pVal);
    }

    FORCEINLINE UInt32 ReadS0( String& pVal )
    {
        Char    ch;

        mLwoFile >> ch;
        while( ch != 0 )
        {
            pVal += ch;
            mLwoFile >> ch;
        }

        if( pVal.length() % 2 == 0 )
        {
            mLwoFile >> ch;
            return pVal.length() + 2;
        }

        return pVal.length() + 1;
    }

    FORCEINLINE UInt32 ReadVX( UInt32& pVal )
    {
        Byte   ch;

        pVal = 0L;

        ReadU1(ch);
        if( ch == 0xff )
        {
            ReadU1(ch);
            pVal |= ch << 16;
            ReadU1(ch);
            pVal |= ch << 8;
            ReadU1(ch);
            pVal |= ch;		
            return sizeof(UInt32);
        }
        else
        {
            pVal |= ch << 8;
            ReadU1(ch);
            pVal |= ch;		
            return sizeof(UInt16);
        }
    }

    FORCEINLINE UInt32 ReadCOL12( Color3f& pVal )
    {
        ReadF4( pVal[0] );
        ReadF4( pVal[1] );
        ReadF4( pVal[2] );
        return sizeof(pVal);
    }

    FORCEINLINE UInt32 ReadVEC12( Vector3f& pVal )
    {
        ReadF4( pVal[0] );
        ReadF4( pVal[1] );
        ReadF4( pVal[2] );

        return sizeof(pVal);
    }

    FORCEINLINE UInt32 ReadFP4( Float& pVal )
    {
        return ReadF4( pVal );
    }

    FORCEINLINE UInt32 ReadANG4( Float& pVal )
    {
        return ReadF4( pVal );
    }

    FORCEINLINE UInt32 ReadFNAM0( String& pVal )
    {
        return ReadS0( pVal );
    }

    FORCEINLINE UInt32 Skip( UInt32 pBytesCount )
    {
        mLwoFile.seekg( pBytesCount, std::ios::cur );
        return pBytesCount;
    }



private:
    LWOReader( const LWOReader& pOther );
    const LWOReader& operator = ( const LWOReader& pOther );

private:
    LWObject*           mObject;
    MemoryFile          mLwoFile;    
};


class LWOIMPEXP_API InvalidLWOFileException : public Exception
{
    DECLARE_EXCEPTION(InvalidLWOFileException);

public:
    InvalidLWOFileException( UInt32 /*pID4Tag*/, CodeLocation pLoc ) : Exception( pLoc )
    {
        mMessage = "Invalid LWO file!";
        DebugOut();
    }
};


#endif  //  _LWO_READER_H_
