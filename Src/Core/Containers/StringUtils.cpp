/**
 *  @file       StringUtils.cpp
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
#include "Core.h"
#include "StringUtils.h"


namespace Gamedesk {
	

std::stringstream gStringStream;                 // create a stream

String& ToUpper( String& pString )
{
    std::transform(pString.begin(), pString.end(), pString.begin(), toupper);   return pString;
}

String& ToLower( String& pString )
{
    std::transform(pString.begin(), pString.end(), pString.begin(), tolower);   return pString;
}

Int32 ToUpper( Int32 pValue )
{
    return (pValue >= 'a' && pValue <= 'z') ? (pValue + 32) : pValue;
}

Int32 ToLower( Int32 pValue )
{
    return (pValue >= 'A' && pValue <= 'Z')? (pValue - 32) : pValue;
}

Bool IsLetter( Int32 pValue )
{
    return (pValue >= 'A' && pValue <= 'Z') || (pValue >= 'a' && pValue <= 'z');
}

Bool IsNumber( Int32 pValue )
{
    return pValue >= '0' && pValue <= '9';
}


} // namespace Gamedesk
