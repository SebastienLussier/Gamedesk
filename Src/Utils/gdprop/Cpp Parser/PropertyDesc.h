/**
 *  @file      PropertyDesc.h
 *  @brief	   Description of a property. Used for parsing.
 *  @author    Sébastien Lussier.
 *  @date      28/02/06.
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
#ifndef     _CPP_PROPERTY_DESC_H_
#define     _CPP_PROPERTY_DESC_H_


#include "Stream/Stream.h"
#include "FileManager/MemoryFile.h"


namespace Cpp
{
    class PropertyDesc
    {
    public:
        PropertyDesc( const String& pVariableName )
            : mVariableName(pVariableName)
        {
            GD_ASSERT( !pVariableName.empty() );

            // Default name
            if( pVariableName.length() > 1 && 
                pVariableName[0] == 'm' &&
                toupper(pVariableName[1]) == pVariableName[1] )
            {
                mName = pVariableName.substr(1, pVariableName.length() - 1);
            }
        }

        void ParseComments( Char* pComments )
        {
            String infoType;

            static const char whitespaces[] = { '\n', '\r', '\t', ' ', '/', '*', '[', ']', '(', ')', ',', 0 };

            StringTokenizer tokenizer( pComments, (UInt32)strlen(pComments), whitespaces );

            while( !tokenizer.AtEnd() )
            {
                tokenizer >> infoType;

                Bool bAdded = false;
                if( infoType.length() > 0 && (infoType[0] == '@' || infoType[0] == '!') )
                {
                    bAdded = AddInfo( infoType, tokenizer );
                    if( !bAdded )
                    {
                        // Do a warning
                        Core::SystemOut("Invalid comment specifier detected (%s)!\n", infoType.c_str());
                    }
                }
                else
                {
                    // Do a warning
                    Core::SystemOut("Wrong comment format detected (%s)!\n", infoType.c_str());
                }

                // Skip rest of line
                tokenizer.Ignore( Number<UInt32>::Max, '\n' );
                tokenizer.EatWhite();
            }
        }

        virtual Bool AddInfo( const String& pInfoType, StringTokenizer& tokenizer )
        {
            if( strcmp(pInfoType.c_str(), "@name") == 0 )
            {
                tokenizer.EatWhite();

                static char bufferLine[2048];
                bufferLine[0] = 0;

                tokenizer.GetLine( bufferLine, 2048, (Char)13 );
                mName = bufferLine;

                if( mName.length() > 1 && mName[mName.length()-1] == '.' )
                {
                    mName = mName.substr(0, mName.length()-1);
                }

                return true;
            }
            else if( strcmp(pInfoType.c_str(), "@desc") == 0 || strcmp(pInfoType.c_str(), "!") == 0 )
            {
                tokenizer.EatWhite();

                static char bufferLine[2048];
                bufferLine[0] = 0;

                tokenizer.GetLine( bufferLine, 2048, (Char)13 );
                mDesc = bufferLine;

                if( mDesc.length() > 1 && mDesc[mDesc.length()-1] == '.' )
                {
                    mDesc = mDesc.substr(0, mDesc.length()-1);
                }

                return true;
            }

            return false;
        }

        virtual void OutputPropertyDescription( TextOutputStream& out, const Char* pClassName )
        {
            out << "    // ";
            out << mVariableName;
            out << "\n";

            out << "    static ";
            out << GetPropertyTypeName().c_str();
            out << " ";
            out << mVariableName;
            out << "Property(\"";
            out << mName;
            out << "\", \"";
            out << mDesc;
            out << "\", (UInt32)&((";
            out << pClassName;
            out << "*)(0))->";
            out << mVariableName;
            out << " );\n";

            out << "    pClass->AddProperty(&";
            out << mVariableName;
            out << "Property);\n";
        }

        virtual const String& GetPropertyTypeName() const = 0;

    public:
        String mVariableName;
        String mName;
        String mDesc;
    };
}


#endif _CPP_PROPERTY_DESC_H_
