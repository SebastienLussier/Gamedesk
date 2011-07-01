/**
 *  @file      EnumDesc.h
 *  @brief	   Description of a Cpp enum. Used for parsing.
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
#ifndef     _CPP_ENUM_DESC_H_
#define     _CPP_ENUM_DESC_H_


namespace Cpp
{
    class EnumDesc
    {
    public:
        EnumDesc( const String& pEnumName, const String& pContext, const String& /*pComment*/ )
            : mName(pEnumName)
            , mNamespace(pContext)
        {
        }

        void OutputEnumHeaderInfo( TextOutputStream& out )
        {
            UInt32 nsCount = OutputNamespaceStart(out);

            OutputIndent(out, nsCount);
            out << "class ENGINE_API ";
            out << mName;
            out << "\n";
            OutputIndent(out, nsCount);
            out << "{\n";
            OutputIndent(out, nsCount);
            out << "    DECLARE_ENUM_INFO();\n";
            OutputIndent(out, nsCount);
            out << "};\n";

            OutputNamespaceEnd(out, nsCount);
            out << "\n";
        }

        void OutputEnumCppInfo( TextOutputStream& out )
        {
            UInt32 nsCount = OutputNamespaceStart(out);

            // Strings
            OutputIndent(out, nsCount);
            out << "const char* ";
            out << mName;
            out << "::mStrings[] =\n";
            OutputIndent(out, nsCount);
            out << "{\n";
            {
                for( Map<String, String>::const_iterator it(mValues.begin()); it != mValues.end(); ++it )
                {
                    if( it != mValues.begin() )
                        out << ",\n";

                    OutputIndent(out, nsCount);
                    out << "    \"";
                    out << it->first.c_str();
                    out << "\"";
                }
                out << "\n";
            }
            OutputIndent(out, nsCount);
            out << "};\n";

            // Values
            OutputIndent(out, nsCount);
            out << "const UInt32 ";
            out << mName;
            out << "::mValues[] =\n";
            OutputIndent(out, nsCount);
            out << "{\n";
            {
                for( Map<String, String>::const_iterator it(mValues.begin()); it != mValues.end(); ++it )
                {
                    if( it != mValues.begin() )
                        out << ",\n";

                    OutputIndent(out, nsCount);
                    out << "    ::";
                    out << mNamespace.c_str();
                    out << it->first.c_str();
                }
                out << "\n";
            }
            OutputIndent(out, nsCount);
            out << "};\n";

            UInt32 enumCount = (UInt32)mValues.size();
            OutputIndent(out, nsCount);
            out << "const UInt32 ";
            out << mName;
            out << "::mSize = ";
            out << enumCount;
            out << ";\n";

            OutputNamespaceEnd(out, nsCount);

            out << "\n";
        }

        UInt32 OutputNamespaceStart( TextOutputStream& out )
        {
            UInt32 nsCount = 1;
            UInt32 currentToken = 0;
            UInt32 nextToken = 0;
            String nsString;

            while( (nextToken = (UInt32)mNamespace.find("::", currentToken)) != String::npos )
            {
                nsString = mNamespace.substr(currentToken, nextToken);

                OutputIndent(out, nsCount);
                out << "namespace ";
                out << nsString;
                out << "\n";

                OutputIndent(out, nsCount);
                out << "{\n";

                nsCount++;
                currentToken = nextToken + 2;
            }

            return nsCount;
        }

        void OutputNamespaceEnd( TextOutputStream& out, UInt32 pNSCount )
        {
            while( pNSCount > 1 )
            {
                pNSCount--;
                OutputIndent(out, pNSCount);
                out << "}\n"; 
            }
        }

        void OutputIndent( TextOutputStream& out, UInt32 indentCount )
        {
            while( indentCount > 0 )
            {
                out << "    ";
                indentCount--;
            }
        }

        Map<String, String> mValues;
        String              mName;
        String              mNamespace;
    };

}


#endif  //  _CPP_ENUM_DESC_H_
