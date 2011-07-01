/**
 *  @file       Types.h
 *  @brief      Typedefs specific to PSP.
 */
/*
 *  Copyright (C) 2008 Gamedesk
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
#ifndef     _TYPES_H_
#define     _TYPES_H_


// Make sure that NULL is defined as 0
#undef      NULL
#define     NULL            0


namespace Gamedesk {


// Unsigned base types.
typedef unsigned char       UChar;      // 8-bit  unsigned.
typedef unsigned char       Byte;       // 8-bit  unsigned.
typedef unsigned short      UInt16;     // 16-bit unsigned.
typedef unsigned int        UInt32;     // 32-bit unsigned.
typedef unsigned long long  UInt64;     // 64-bit unsigned.

// Signed base types.
typedef char                Int8;       // 8-bit signed.
typedef signed short        Int16;      // 16-bit signed.
typedef signed int          Int32;      // 32-bit signed.
typedef signed long long    Int64;      // 64-bit signed.

// Characters
typedef char                Char;       // An ANSI character.

// Other base types.
typedef bool                Bool;       // Boolean 0 (false) or 1 (true).
typedef float               Float;      // 32-bit IEEE floating point.
typedef double              Double;     // 64-bit IEEE double.

// Bitfield type.
typedef UInt32              Bitfield;   // For bitfields.

// Simple void pointer
typedef void*               Handle;     // Type-less memory pointer


} // namespace Gamedesk


#endif  //  _TYPES_H_
