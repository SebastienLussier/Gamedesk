/**
 *  @file       Bitset.h
 *  @brief	    Bitset of n bits.
 *  @author     Sébastien Lussier.
 *  @date       02/03/05.
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
#ifndef     _BITSET_H_
#define     _BITSET_H_


namespace Gamedesk {


class Bitset 
{

public:
    Bitset() 
        : mBits(NULL)
        , mSize(0)
        , mBitCount(0)
    {
    }

    Bitset( UInt32 pBitCount ) 
        : mBits(NULL)
    {
        SetSize(pBitCount);
    }

	~Bitset() 
	{
        if( mBits )
		    GD_DELETE(mBits);
	}

	// Resizes the bitset
	void SetSize( UInt32 pBitCount ) 
	{ 
		// Get the size of integers we need
        mBitCount = pBitCount;
		mSize = (mBitCount + 7) >> 3;

		if( mBits ) 
		{
			GD_DELETE(mBits);
			mBits = NULL;
		}

		// Allocate the bits and initialize them
		if( mSize > 0 )
        {
            mBits = GD_NEW_ARRAY(Byte, mSize, this, "Core::Container::Bitset::Bits");
		    ClearAllBits();
        }
	}

    UInt32 GetSize() const
    {
        return mBitCount;
    }

    // Check if the desired bit slot is a 1 or a 0
	Bool CheckBit( UInt32 pBit ) const
	{
		return (mBits[pBit >> 3] & (1 << (pBit & 7))) != 0;
	}

	// Set a bit to 1
	void SetBit( UInt32 pBit ) 
	{
		mBits[pBit >> 3] |= (1 << (pBit & 7));
	}

    // Clear a bit to 0
	void ClearBit( UInt32 pBit ) 
	{
		mBits[pBit >> 3] &= ~(1 << (pBit & 7));
	}	

	// Set all bits to 0
	void ClearAllBits() 
	{
		memset(mBits, 0x00, sizeof(Byte) * mSize);
	}

    // Set all bits to 1
	void SetAllBits() 
	{
		memset(mBits, 0xFF, sizeof(Byte) * mSize);
	}

    Byte& operator [] ( UInt32 pByte )
    {
        return mBits[pByte];
    }

    Byte* operator * ()
    {
        return mBits;
    }

    /**
     *  Serialize the content of this vector to/from a stream.
     *  @param  pStream Stream used for serialization.
     *  @return The stream object.
     */
    Stream& operator << ( Stream& pStream )
    {
        pStream << mSize;
        pStream << mBitCount;
        pStream.Serialize( mBits, mSize );
        
        return pStream;
    }

private:
	Byte*   mBits;
	UInt32  mSize;
  UInt32  mBitCount;
};


} // namespace Gamedesk


#endif  //  _BIT_SET_H_
