/**
 *  @file       Checksum.h
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
#ifndef     _CHECKSUM_H_
#define     _CHECKSUM_H_


namespace Gamedesk {


class CORE_API MD5Checksum
{
public:
    class Value
    {
    public:
        UInt32  A;
        UInt32  B;
        UInt32  C;
        UInt32  D;

        Value() {}
        Value( UInt32 a, UInt32 b, UInt32 c, UInt32 d ) : A(a), B(b), C(c), D(d) {}
        Value( const Value& pOther ) : A(pOther.A), B(pOther.B), C(pOther.C), D(pOther.D) {}

        Bool operator == ( const Value& pOther ) const
        {
            return A == pOther.A && B == pOther.B && C == pOther.C && D == pOther.D;
        }

        Bool operator != ( const Value& pOther ) const
        {
            return A != pOther.A || B != pOther.B || C != pOther.C || D != pOther.D;
        }

        operator Byte* ()
        {
            return (Byte*)this;
        }

        operator Byte* () const
        {
            return (Byte*)this;
        }
    };

    // Methods for controlled operation:
    MD5Checksum();                              // Simple initializer

    // Constructors for special circumstances.  All these constructors finalize the MD5 context.
    MD5Checksum(Byte* string);                  // Digest string, finalize
    MD5Checksum(std::istream& stream);          // Digest stream, finalize
    MD5Checksum(FILE *file);                    // Digest file, close, finalize
    MD5Checksum(std::ifstream& stream);         // Digest stream, close, finalize

    void Update(Byte* input, UInt32 input_length);
    void Update(std::istream& stream);
    void Update(FILE *file);
    void Update(std::ifstream& stream);
    void Finalize();

    // Methods to acquire finalized result
    const Value&    Digest() const;
    Char*           HexDigest(Char* pBuffer, UInt32 pBufferLen) const;  // Digest as a 33-byte ascii-hex string

private:
    Value   mDigest;
    UInt32  mCount[2];                  // Number of *bits*, mod 2^64
    Byte    mBuffer[64];                // Input buffer
    Bool    mFinalized;

    // Last, the private methods, mostly static:
    void Init();                        // Called by all constructors
    void Transform(Byte *buffer);       // Does the real update work.  Note that length is implied to be 64.

    static void Encode(Byte *dest, UInt32 *src, UInt32 length);
    static void Decode(UInt32 *dest, Byte *src, UInt32 length);

    static inline UInt32 RotateLeft(UInt32 x, UInt32 n);
    
    static inline UInt32 F(UInt32 x, UInt32 y, UInt32 z);
    static inline UInt32 G(UInt32 x, UInt32 y, UInt32 z);
    static inline UInt32 H(UInt32 x, UInt32 y, UInt32 z);
    static inline UInt32 I(UInt32 x, UInt32 y, UInt32 z);

    static inline void FF(UInt32& a, UInt32 b, UInt32 c, UInt32 d, UInt32 x, UInt32 s, UInt32 ac);
    static inline void GG(UInt32& a, UInt32 b, UInt32 c, UInt32 d, UInt32 x, UInt32 s, UInt32 ac);
    static inline void HH(UInt32& a, UInt32 b, UInt32 c, UInt32 d, UInt32 x, UInt32 s, UInt32 ac);
    static inline void II(UInt32& a, UInt32 b, UInt32 c, UInt32 d, UInt32 x, UInt32 s, UInt32 ac);
};


} // namespace Gamedesk


#endif  //  _CHECKSUM_H_
