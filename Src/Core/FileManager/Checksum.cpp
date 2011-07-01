/**
 *  @file       Checksum.cpp
 *  @brief      Utility class to compute checksum.
 *  @author     Sébastien Lussier.
 *  @date       01/03/06.
 *  @note       Derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm
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
#include "Core.h"
#include "Checksum.h"


namespace Gamedesk {
	
	
// MD5 simple initialization method
MD5Checksum::MD5Checksum()
{
    Init();
}

MD5Checksum::MD5Checksum(FILE *file)
{
    Init();  // must be called be all constructors
    Update(file);
    Finalize ();
}


MD5Checksum::MD5Checksum(std::istream& stream)
{
    Init();  // must called by all constructors
    Update(stream);
    Finalize();
}


MD5Checksum::MD5Checksum(std::ifstream& stream)
{
    Init();  // must called by all constructors
    Update(stream);
    Finalize();
}


void MD5Checksum::Init()
{
    mFinalized=0;  // we just started!

    // Nothing counted, so mCount=0
    mCount[0] = 0;
    mCount[1] = 0;

    // Load magic initialization constants.
    mDigest.A = 0x67452301;
    mDigest.B = 0xefcdab89;
    mDigest.C = 0x98badcfe;
    mDigest.D = 0x10325476;
}


// MD5 block Update operation. Continues an MD5 message-digest
// operation, processing another message block, and updating the
// context.
void MD5Checksum::Update(Byte* input, UInt32 input_length)
{
    UInt32 input_index, buffer_index;
    UInt32 buffer_space;                // how much space is left in mBuffer

    GD_ASSERT( !mFinalized );
    
    // Compute number of bytes mod 64
    buffer_index = (UInt32)((mCount[0] >> 3) & 0x3F);

    // Update number of bits
    if (  (mCount[0] += ((UInt32) input_length << 3))<((UInt32) input_length << 3) )
        mCount[1]++;

    mCount[1] += ((UInt32)input_length >> 29);


    buffer_space = 64 - buffer_index;  // how much space is left in mBuffer

    // Transform as many times as possible.
    if (input_length >= buffer_space)
    {   
        // ie. we have enough to fill the mBuffer
        // fill the rest of the mBuffer and transform
        memcpy(mBuffer + buffer_index, input, buffer_space);
        Transform(mBuffer);

        // Now, Transformeach 64-byte piece of the input, bypassing the mBuffer
        for (input_index = buffer_space; input_index + 63 < input_length; input_index += 64)
            Transform(input+input_index);

        buffer_index = 0;  // so we can mBuffer remaining
    }
    else
    {
        input_index=0;     // so we can mBuffer the whole input
    }

    // And here we do the buffering:
    memcpy(mBuffer+buffer_index, input+input_index, input_length-input_index);
}


// MD5 Update for files.
// Like above, except that it works on files (and uses above as a primitive.)
void MD5Checksum::Update(FILE *file)
{
    Byte mBuffer[1024];

    Int32 len = fread(mBuffer, 1, 1024, file);
    while( len!= 0 )
    {
        Update(mBuffer, len);
        len = fread(mBuffer, 1, 1024, file);
    }

    fclose(file);
}


// MD5 Update for istreams.
// Like Update for files; see above.
void MD5Checksum::Update(std::istream& pStream)
{
    Byte mBuffer[1024];
    Int32 len;

    while( pStream.good() )
    {
        pStream.read((char*)mBuffer, 1024); // note that return value of read is unusable.
        len=pStream.gcount();
        Update(mBuffer, len);
    }
}


// MD5 Update for ifstreams.
// Like Update for files; see above.
void MD5Checksum::Update(std::ifstream& pStream)
{
    Byte mBuffer[1024];
    Int32 len;

    while( pStream.good() )
    {
        pStream.read((char*)mBuffer, 1024); // note that return value of read is unusable.
        len = pStream.gcount();
        Update(mBuffer, len);
    }
}


// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.
void MD5Checksum::Finalize()
{
    Byte bits[8];
    UInt32 index, padLen;

    static Byte PADDING[64]=
    {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    GD_ASSERT(!mFinalized);
    
    // Save number of bits
    Encode(bits, mCount, 8);

    // Pad out to 56 mod 64.
    index = (UInt32) ((mCount[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    Update(PADDING, padLen);

    // Append length (before padding)
    Update(bits, 8);

    // Zeroize sensitive information
    memset(mBuffer, 0, sizeof(*mBuffer));

    mFinalized=1;
}


const MD5Checksum::Value& MD5Checksum::Digest() const
{
    GD_ASSERT( mFinalized );
    return mDigest;
}


Char* MD5Checksum::HexDigest(Char* pBuffer, UInt32 pBufferLen) const
{
    if(pBufferLen < 33)
        return 0;

    GD_ASSERT( mFinalized );
    
    for( Int32 i = 0; i < 16; i++ )
        sprintf(pBuffer+i*2, "%02x", mDigest[i]);

    return pBuffer;
}


// Constants for MD5Transform routine.
// Although we could use C++ style constants, defines are actually better,
// since they let us easily evade scope clashes.
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21


// MD5 basic transformation. Transforms mDigest based on block.
void MD5Checksum::Transform(Byte block[64])
{

  UInt32 a = mDigest.A;
  UInt32 b = mDigest.B;
  UInt32 c = mDigest.C;
  UInt32 d = mDigest.D;
  UInt32 x[16];

  Decode(x, block, 64);

  GD_ASSERT(!mFinalized);  // not just a user error, since the method is private

  /* Round 1 */
  FF(a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
  FF(d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
  FF(c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
  FF(b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
  FF(a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
  FF(d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
  FF(c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
  FF(b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
  FF(a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
  FF(d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
  FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
  FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
  FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
  FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
  FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
  FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

 /* Round 2 */
  GG(a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
  GG(d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
  GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
  GG(b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
  GG(a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
  GG(d, a, b, c, x[10], S22,  0x2441453); /* 22 */
  GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
  GG(b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
  GG(a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
  GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
  GG(c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
  GG(b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
  GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
  GG(d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
  GG(c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
  GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

  /* Round 3 */
  HH(a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
  HH(d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
  HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
  HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
  HH(a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
  HH(d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
  HH(c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
  HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
  HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
  HH(d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
  HH(c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
  HH(b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
  HH(a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
  HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
  HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
  HH(b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

  /* Round 4 */
  II(a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
  II(d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
  II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
  II(b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
  II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
  II(d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
  II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
  II(b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
  II(a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
  II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
  II(c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
  II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
  II(a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
  II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
  II(c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
  II(b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

  mDigest.A += a;
  mDigest.B += b;
  mDigest.C += c;
  mDigest.D += d;

  // Zeroize sensitive information.
  memset( (Byte* ) x, 0, sizeof(x) );
}


// Encodes input (UInt32) into output (Byte). Assumes len is
// a multiple of 4.
void MD5Checksum::Encode(Byte* output, UInt32* input, UInt32 len)
{
    UInt32 i, j;

    for(i = 0, j = 0; j < len; i++, j += 4) 
    {
        output[j+0] = (Byte) ((input[i] >>  0) & 0xFF);
        output[j+1] = (Byte) ((input[i] >>  8) & 0xFF);
        output[j+2] = (Byte) ((input[i] >> 16) & 0xFF);
        output[j+3] = (Byte) ((input[i] >> 24) & 0xFF);
    }
}


// Decodes input (Byte) into output (UINT4). Assumes len is
// a multiple of 4.
void MD5Checksum::Decode(UInt32* output, Byte* input, UInt32 len)
{
    UInt32 i, j;

    for(i = 0, j = 0; j < len; i++, j += 4)
    {
        output[i] = (((UInt32)input[j+0]) <<  0) | 
                    (((UInt32)input[j+1]) <<  8) |
                    (((UInt32)input[j+2]) << 16) | 
                    (((UInt32)input[j+3]) << 24);
    }
}


// ROTATE_LEFT rotates x left n bits.
inline UInt32 MD5Checksum::RotateLeft(UInt32 x, UInt32 n)
{
    return (x << n) | (x >> (32-n));
}


// F, G, H and I are basic MD5 functions.
inline UInt32 MD5Checksum::F(UInt32 x, UInt32 y, UInt32 z)
{
    return (x & y) | (~x & z);
}

inline UInt32 MD5Checksum::G(UInt32 x, UInt32 y, UInt32 z)
{
    return (x & z) | (y & ~z);
}

inline UInt32 MD5Checksum::H(UInt32 x, UInt32 y, UInt32 z)
{
    return x ^ y ^ z;
}

inline UInt32 MD5Checksum::I(UInt32 x, UInt32 y, UInt32 z)
{
    return y ^ (x | ~z);
}


// FF, GG, HH, and IItransformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
inline void MD5Checksum::FF(UInt32& a, UInt32 b, UInt32 c, UInt32 d, UInt32 x, UInt32 s, UInt32 ac)
{
    a += F(b, c, d) + x + ac;
    a = RotateLeft(a, s) + b;
}

inline void MD5Checksum::GG(UInt32& a, UInt32 b, UInt32 c, UInt32 d, UInt32 x, UInt32 s, UInt32 ac)
{
    a += G(b, c, d) + x + ac;
    a = RotateLeft(a, s) + b;
}

inline void MD5Checksum::HH(UInt32& a, UInt32 b, UInt32 c, UInt32 d, UInt32 x, UInt32 s, UInt32 ac)
{
    a += H(b, c, d) + x + ac;
    a = RotateLeft(a, s) + b;
}

inline void MD5Checksum::II(UInt32& a, UInt32 b, UInt32 c, UInt32 d, UInt32 x, UInt32 s, UInt32 ac)
{
    a += I(b, c, d) + x + ac;
    a = RotateLeft(a, s) + b;
}


} // namespace Gamedesk
