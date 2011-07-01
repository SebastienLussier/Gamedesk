/**
 *  @file       Exception.cpp
 *  @brief      Definition of the base Exception class.
 *  @author     Sébastien Lussier.
 *  @date       19/01/03.
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
#include "Core.h"
#include "Exception.h"


namespace Gamedesk {
	
	
CodeLocation::CodeLocation( const Char* pFileName, const Char* pFunctionName, UInt32 pLineNumber ) :
  mFileName(pFileName),
  mFunctionName(pFunctionName),
  mLineNumber(pLineNumber)
{
}

INLINE const String& CodeLocation::File() const
{
    return mFileName;
}

INLINE const String& CodeLocation::Function() const
{
    return mFunctionName;
}

INLINE UInt32 CodeLocation::Line() const
{
    return mLineNumber;
}


Exception::Exception( CodeLocation pLocation ) :
    mLocation(pLocation)
{
}

const CodeLocation& Exception::GetLocation() const
{
    return mLocation;
}

const String& Exception::GetFile() const
{
    return mLocation.File();
}

const String& Exception::GetFunction() const
{
    return mLocation.Function();
}

UInt32 Exception::GetLine() const
{
    return mLocation.Line();
}

const String& Exception::GetMessage() const
{
    return mMessage;
}

String Exception::ToString() const
{
    String str(GetName());

    str += " thrown at ";
    str += GetFile();
    str += ":";
    str += Gamedesk::ToString( GetLine() );
    str += "\n";
    str += GetMessage();

    return str;
}

void Exception::BuildStack( const Char* pFunc )
{
    mCallStack.push_back( pFunc );
}

Exception& Exception::DebugOut()
{
    Core::DebugOut( "%s(%s) : %s : %s\n", 
                    mLocation.File().c_str(), 
                    Gamedesk::ToString( mLocation.Line() ).c_str(),
                    GetName(),
                    mMessage.c_str() );
 
    return *this;
}


} // namespace Gamedesk
