/**
 *  @file      ProjReader.h
 *  @brief	   VCProj reader.
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
#ifndef     _VC_PROJ_READER_H_
#define     _VC_PROJ_READER_H_


#include "VCProj Parser/Reader.h"
#include "VCProj Parser/Section.h"


namespace VC
{
    class Proj : public Section
    {
    public:
        String mXmlVersion;
        String mEncoding;
    };

    class ProjReader : public Reader
    {
    public:
        ProjReader()
        {
        }

        virtual ~ProjReader()
        {
        }

        Proj* Read( const String& pFileName )
        {
            Reader::Read( pFileName );

            Proj* proj = GD_NEW(Proj, this, "ProjReader::Proj");

            MustRead("<");
            MustRead("?");
            MustRead("xml");

            MustRead("version");
            MustRead("=");
            ReadString(proj->mXmlVersion);

            MustRead("encoding");
            MustRead("=");
            ReadString(proj->mEncoding);

            MustRead("?");
            MustRead(">");


            while( !mTokenizer.AtEnd() && ReadSection(proj) )
            {
                mTokenizer.EatWhite();
            }

            return proj;
        }

        Section* ReadSection( Section* pParent )
        {
            String name;
            String propertyName;
            String propertyValue;

            MustRead("<");
            ReadToken(name);

            if( strcmp(name.c_str(), "/") == 0 )
                return NULL;

            Section* newSection = GD_NEW(Section, this, "ProjReader::Section");
            pParent->mSubSections.insert(std::pair<String,Section*>(name, newSection));

            // Read Properties
            Char c = NextChar();
            while( c != '>' )
            {
                // '/' Means no subsections
                if( c == '/' )
                {
                    MustRead("/");
                    MustRead(">");
                    return newSection;
                }

                ReadToken(propertyName);
                MustRead("=");
                ReadString(propertyValue);

                newSection->mProperties[propertyName] = propertyValue;

                c = NextChar();
            }
            MustRead(">");

            // Read Subsections
            while( ReadSection(newSection) );

            MustRead(name);
            MustRead(">");

            return newSection;
        }
    };
}


#endif  //  _VC_PROJ_READER_H_
