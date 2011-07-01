/**
*  @file       DDSImporter.h
*  @brief      DDS Texture Importer, because DevIL is crap with DDS.
*  @author     Sébastien Lussier.
*  @date       24/04/05.
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
#ifndef     _DDS_IMPORTER_H_
#define     _DDS_IMPORTER_H_


#include "Resource/ResourceManager.h"


class DEVILIMPEXP_API DDSImporter : public ResourceImporter
{
    DECLARE_CLASS(DDSImporter,ResourceImporter);

public:
    DDSImporter();

    virtual Class*    GetResourceClass();
    virtual Resource* Import( const String& pFilename, const String& pParams = "" );
};



#endif  //  _DDS_IMPORTER_H_
