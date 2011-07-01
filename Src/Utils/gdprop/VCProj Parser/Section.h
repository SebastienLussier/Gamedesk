/**
 *  @file      Section.h
 *  @brief	   VCProj section.
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
#ifndef     _VC_SECTION_H_
#define     _VC_SECTION_H_


namespace VC
{
    class Section
    {
    public:
        virtual ~Section()
        {
            for( MultiMap<String,Section*>::iterator it(mSubSections.begin()); it != mSubSections.end(); ++it )
                GD_DELETE(it->second);
        }

        Bool GetPropertyValue( const String& pPath, String& pPropValue )
        {
            size_t posPoint = pPath.find(".");
            if( posPoint != String::npos )
            {
                String subsection = pPath.substr(0, posPoint);

                MultiMap<String,Section*>::const_iterator itSection = mSubSections.find(subsection);
                if( itSection != mSubSections.end() )
                {
                    String remaining = pPath.substr(posPoint+1, String::npos);

                    size_t count = mSubSections.count(subsection);
                    for( size_t i = 0; i < count; i++, ++itSection )
                    {
                        if( itSection->second->GetPropertyValue(remaining,pPropValue) )
                            return true;
                    }
                }

                return false;
            }
            else
            {
                Map<String,String>::const_iterator it = mProperties.find(pPath);
                if( it == mProperties.end() )
                    return false;

                pPropValue = it->second;
            }

            return true;
        }

        Map<String,String>          mProperties;
        MultiMap<String,Section*>   mSubSections;
    };
}


#endif  //   _VC_SECTION_H_
