/**
 *  @file       StringUtils.h
 *  @brief	    String utilities.
 *  @author     Sébastien Lussier.
 *  @date       21/10/03.
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
#ifndef     _STRINGUTILS_H_
#define     _STRINGUTILS_H_


///////////////////////////////////////////////////////////////////////////////
// STL includes
#include <string>
#include <sstream>
#include <iomanip>


namespace Gamedesk {
	

///////////////////////////////////////////////////////////////////////////////
// Typedefs (STL wrappers).
typedef     std::string     String;


CORE_API String& ToUpper( String& pString );
CORE_API String& ToLower( String& pString );
CORE_API Int32 ToUpper( Int32 pValue );
CORE_API Int32 ToLower( Int32 pValue );

CORE_API Bool IsLetter( Int32 pValue );
CORE_API Bool IsNumber( Int32 pValue );


///////////////////////////////////////////////////////////////////////////////
// String conversions
extern CORE_API std::stringstream gStringStream;

template <class T>
INLINE String& ToString( const T& pData, String& pString )
{
    gStringStream.clear();
    gStringStream.str("");
    gStringStream << pData;
    pString = gStringStream.str();
    return pString;
}

// Specialization for type Byte
template <>
INLINE String& ToString( const Byte& pData, String& pString )
{
    gStringStream.clear();
    gStringStream.str("");
    gStringStream << (UInt16)pData;
    pString = gStringStream.str();
    return pString;
}

template <class T>
INLINE String ToString( const T& pData )
{
    String str;
    ToString<T>(pData, str);
    return str;
}

template <class T>
INLINE String ToString( const T& pData, std::streamsize pPrecision )
{
    String pString;
    gStringStream.clear();
    gStringStream.str("");
    gStringStream << std::setprecision(pPrecision) << pData;
    pString = gStringStream.str();
    return pString;
}

template <class T>
INLINE T StringTo( const std::string& pString )
{
    gStringStream.clear();
    gStringStream.str(pString);
    
    T  result;

    gStringStream >> result;
    return result;
}

template <>
INLINE Byte StringTo<Byte>( const std::string& pString )
{
    gStringStream.clear();
    gStringStream.str(pString);
    
    UInt16 result;

    gStringStream >> result;
    return result;
}

template <class T>
INLINE Bool StringTo( const std::string& pString, T& result )
{
    gStringStream.clear();
    gStringStream.str(pString);
    
    gStringStream >> result;
    return !gStringStream.fail();
}

template <>
INLINE Bool StringTo( const std::string& pString, Byte& pVal )
{
    gStringStream.clear();
    gStringStream.str(pString);
    
    UInt16 tmpResult;

    gStringStream >> tmpResult;
    pVal = tmpResult;
    
    return !gStringStream.fail();
}

inline UInt32 Hash(const char* pStr)
{
    UInt32 hash = 0;
    for(; *pStr != 0; pStr++)
        hash = 65599 * hash + *pStr;
    return hash ^ (hash >> 16);
}
 
inline UInt32 Hash(const char* pStr, UInt32 pLen)
{
    UInt32 hash = 0;
    for(UINT i = 0; i < pLen; ++i)
        hash = 65599 * hash + pStr[i];
    return hash ^ (hash >> 16);
}

inline UInt32 Hash(const String& pStr)
{
   return Hash(pStr.c_str());
}


} // namespace Gamedesk


#endif  //  _STRINGUTILS_H_
