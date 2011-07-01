/**
*  @file       ClassDesc.h
*  @brief	   Description of a Cpp class. Used for parsing.
*  @author     Sébastien Lussier.
*  @date       28/02/06.
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
#ifndef     _CPP_CLASS_DESC_H_
#define     _CPP_CLASS_DESC_H_


#include "Cpp Parser/PropertyDesc.h"


namespace Cpp
{
    class ClassDesc
    {
    public:
        ClassDesc( const Char* pHeaderFile, const String& pContext )
        {
            mHeaderFile = pHeaderFile;
            mNamespace = pContext;
        }

        virtual ~ClassDesc()
        {
            List<PropertyDesc*>::iterator it(mProperties.begin());
            for( ; it != mProperties.end(); ++it )
                GD_DELETE(*it);
        }

        void AddProperty( PropertyDesc* pNewProperty )
        {
            mProperties.push_back(pNewProperty);
        }

        void OutputProperties( TextOutputStream& out )
        {
            if( mProperties.empty() )
                return;

            out << "#include \"";
            out << mHeaderFile;
            out << "\"\n";

            out << "void ";
            out << mNamespace;
            out << mName;
            out << "::StaticRegisterProperties( Class* pClass )\n";
            out << "{\n";
            out << "    if( pClass != ";
            out << mName;
            out << "::StaticClass() )\n";
            out << "        return;\n";
            out << "\n";

            for( List<PropertyDesc*>::iterator it(mProperties.begin()); it != mProperties.end(); ++it )
            {
                (*it)->OutputPropertyDescription(out, mName.c_str());
                out << "\n";
            }

            out << "}\n";
            out << "\n";
        }

    public:
        String               mName;
        String               mNamespace;
        String               mHeaderFile;

    private:
        List<PropertyDesc*>  mProperties;
    };
}


#endif  //  _CPP_CLASS_DESC_H_
