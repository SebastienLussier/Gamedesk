/**
 *  @file       WoWImporter.h
 *  @brief      WoW mesh/anim importer.
 *  @author     S�bastien Lussier.
 *  @date       30/04/05.
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
#ifndef     _WoWIMPORTER_H_
#define     _WoWIMPORTER_H_


#include "Resource/ResourceManager.h"

#include "WoWFiles.h"


class WoWIMPORTER_API WoWImporter : public ResourceImporter
{
    DECLARE_CLASS(WoWImporter,ResourceImporter);
    
public:
    WoWImporter();

    virtual Class*    GetResourceClass();
    virtual Resource* Import( const String& pFilename, const String& pParams = "" );
};

class WoWIMPORTER_API BLPImporter : public ResourceImporter
{
	DECLARE_CLASS(BLPImporter,ResourceImporter);

public:
	BLPImporter();

	virtual Class*    GetResourceClass();
	virtual Resource* Import( const String& pFilename, const String& pParams = "" );
};


#endif  //  _WoWIMPORTER_H_
