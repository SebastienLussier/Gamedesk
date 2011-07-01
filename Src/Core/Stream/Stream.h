/**
 *  @file       Stream.h
 *  @brief      Data stream base class (in input or output).
 *  @author     Sébastien Lussier.
 *  @date       18/03/05.
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
#ifndef     _STREAM_H_
#define     _STREAM_H_


#include "Core.h"


namespace Gamedesk {


class CORE_API Stream
{
public:
    virtual ~Stream()               { }

    virtual Bool Open( const String& /*pPath*/ )    { return false; }
    virtual Bool Close()                            { return false; }

    virtual Bool IsValid() const    { return mIsValid; }

    virtual UInt32 Size() const     { return 0; }
    virtual UInt32 Pos() const      { return 0; }

    virtual Bool In() const         { return false; }
    virtual Bool Out() const        { return false; }

    virtual void Serialize( void* pData, UInt32 pLen ) = 0;

    virtual Stream& operator << ( Char& pVal )      { Serialize( &pVal, sizeof(pVal) ); return *this; }
    virtual Stream& operator << ( Int16& pVal )     { Serialize( &pVal, sizeof(pVal) ); return *this; }
    virtual Stream& operator << ( Int32& pVal )     { Serialize( &pVal, sizeof(pVal) ); return *this; }
    virtual Stream& operator << ( Int64& pVal )     { Serialize( &pVal, sizeof(pVal) ); return *this; }
    virtual Stream& operator << ( Byte& pVal )      { Serialize( &pVal, sizeof(pVal) ); return *this; }
    virtual Stream& operator << ( UInt16& pVal )    { Serialize( &pVal, sizeof(pVal) ); return *this; }
    virtual Stream& operator << ( UInt32& pVal )    { Serialize( &pVal, sizeof(pVal) ); return *this; }
    virtual Stream& operator << ( UInt64& pVal )    { Serialize( &pVal, sizeof(pVal) ); return *this; }
    virtual Stream& operator << ( Bool& pVal )      { Serialize( &pVal, sizeof(pVal) ); return *this; }
    virtual Stream& operator << ( Float& pVal )     { Serialize( &pVal, sizeof(pVal) ); return *this; }
    virtual Stream& operator << ( Double& pVal )    { Serialize( &pVal, sizeof(pVal) ); return *this; }
    virtual Stream& operator << ( class Object*& )  { return *this; }
    virtual Stream& operator << ( String& pVal )
    {
        UInt32 len = (UInt32)pVal.length();

        (*this) << len;

        if( In() )
        {
            pVal.resize( len );
            Serialize( (Char*)pVal.c_str(), len + 1 );
        }
        else
        {
            Serialize( (Char*)pVal.c_str(), len + 1 );
        }

        return *this;
    }

protected:
    Stream()
        : mIsValid(false)
    {
    }

protected:
    Bool mIsValid;
};


class CORE_API InputStream : public Stream
{
public:
    Bool In() const
    {
        return true;
    }
};


class CORE_API OutputStream : public Stream
{
public:
    Bool Out() const
    {
        return true;
    }
};


class CORE_API TextInputStream : public InputStream
{
public:
    TextInputStream(InputStream& pStream) :
        mInternalStream(pStream)
    {
    }

    Stream& operator << ( Char& pVal )
    {
        Serialize( &pVal, sizeof(pVal) );
        return *this;
    }

    Stream& operator << ( Int16& pVal )
    {
        String str;
        (*this) << str;
        StringTo<Int16>( str, pVal );
        return *this;
    }

    Stream& operator << ( Int32& pVal )
    {
        String str;
        (*this) << str;
        StringTo<Int32>( str, pVal );
        return *this;
    }

    Stream& operator << ( Int64& pVal )
    {
        String str;
        (*this) << str;
        StringTo<Int64>( str, pVal );
        return *this;
    }

    Stream& operator << ( Byte& pVal )
    {
        String str;
        (*this) << str;
        StringTo<Byte>( str, pVal );
        return *this;
    }

    Stream& operator << ( UInt16& pVal )
    {
        String str;
        (*this) << str;
        StringTo<UInt16>( str, pVal );
        return *this;
    }

    Stream& operator << ( UInt32& pVal )
    {
        String str;
        (*this) << str;
        StringTo<UInt32>( str, pVal );
        return *this;
    }

    Stream& operator << ( UInt64& pVal )
    {
        String str;
        (*this) << str;
        StringTo<UInt64>( str, pVal );
        return *this;
    }

    Stream& operator << ( Bool& pVal )
    {
        String str;
        (*this) << str;
        StringTo<Bool>( str, pVal );
        return *this;
    }

    Stream& operator << ( Float& pVal )
    {
        String str;
        (*this) << str;
        StringTo<Float>( str, pVal );
        return *this;
    }

    Stream& operator << ( Double& pVal )
    {
        String str;
        (*this) << str;
        StringTo<Double>( str, pVal );
        return *this;
    }

    Stream& operator << ( String& pVal )
    {
        pVal = "";

        Char c;
        (*this) << c;

        while( c != ' ' && c != 0 )
        {
            static const Char CR = '\r';
            if( c != CR )
                pVal += c;

            (*this) << c;
        }

        return *this;
    }

    Stream& operator << ( class Object*& )
    {
        return *this;
    }

    UInt32 Size() const     { return mInternalStream.Size(); }
    UInt32 Pos() const      { return mInternalStream.Pos(); }

    void Serialize( void* pData, UInt32 pLen )
    {
        mInternalStream.Serialize( pData, pLen );
    }

private:
    TextInputStream( const TextInputStream& pOther );
    const TextInputStream& operator = ( const TextInputStream& pOther );

private:
    InputStream&        mInternalStream;
};


class CORE_API TextOutputStream : public OutputStream
{
public:
    TextOutputStream(OutputStream& pStream) :
        mInternalStream(pStream)
    {
    }

    Stream& operator << ( Char& pVal )
    {
        Serialize( &pVal, sizeof(pVal) );
        return *this;
    }

    Stream& operator << ( Int16& pVal )
    {
        String str;
        ToString<Int16>( pVal, str );
        (*this) << str;
        return *this;
    }

    Stream& operator << ( Int32& pVal )
    {
        String str;
        ToString<Int32>( pVal, str );
        (*this) << str;
        return *this;
    }

    Stream& operator << ( Int64& pVal )
    {
        String str;
        ToString<Int64>( pVal, str );
        (*this) << str;
        return *this;
    }

    Stream& operator << ( Byte& pVal )
    {
        String str;
        ToString<Byte>( pVal, str );
        (*this) << str;
        return *this;
    }

    Stream& operator << ( UInt16& pVal )
    {
        String str;
        ToString<UInt16>( pVal, str );
        (*this) << str;
        return *this;
    }

    Stream& operator << ( UInt32& pVal )
    {
        String str;
        ToString<UInt32>( pVal, str );
        (*this) << str;
        return *this;
    }

    Stream& operator << ( UInt64& pVal )
    {
        String str;
        ToString<UInt64>( pVal, str );
        (*this) << str;
        return *this;
    }

    Stream& operator << ( Bool& pVal )
    {
        String str;
        ToString<Bool>( pVal, str );
        (*this) << str;
        return *this;
    }

    Stream& operator << ( Float& pVal )
    {
        String str;
        ToString<Float>( pVal, str );
        (*this) << str;
        return *this;
    }

    Stream& operator << ( Double& pVal )
    {
        String str;
        ToString<Double>( pVal, str );
        (*this) << str;
        return *this;
    }

    Stream& operator << ( String& pVal )
    {
        Char* pStr = const_cast<Char*>(pVal.c_str());

        while( *pStr != 0 )
        {
            if( *pStr == '\n' )
            {
                static Char CR = '\r';
                (*this) << CR;
            }

            (*this) << *pStr;

            ++pStr;
        }

        return *this;
    }

    Stream& operator << ( const Char* pVal )
    {
        Serialize( const_cast<Char*>(pVal), (UInt32)strlen(pVal) );
        return *this;
    }

    Stream& operator << ( class Object*& )
    {
        return *this;
    }

    UInt32 Size() const     { return mInternalStream.Size(); }
    UInt32 Pos() const      { return mInternalStream.Pos(); }

    void Serialize( void* pData, UInt32 pLen )
    {
        mInternalStream.Serialize( pData, pLen );
    }

private:
    TextOutputStream( const TextOutputStream& pOther );
    const TextOutputStream& operator = ( const TextOutputStream& pOther );

private:
    OutputStream&        mInternalStream;
};


} // namespace Gamedesk


#endif  //  _STREAM_H_
