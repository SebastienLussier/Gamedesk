/**
 *  @file       DevILImporter.h
 *  @brief      Texture Importer that uses DevIL Image Library.
 *  @author     Sébastien Lussier.
 *  @date       01/12/03.
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
#ifndef     _DEVIL_IMPORTER_H_
#define     _DEVIL_IMPORTER_H_


#include "Resource/ResourceManager.h"


class DEVILIMPEXP_API DevILImporter : public ResourceImporter
{
    DECLARE_CLASS(DevILImporter,ResourceImporter);

public:
    DevILImporter();

    virtual Class*    GetResourceClass();
    virtual Resource* Import( const String& pFilename, const String& pParams = "" );
};



#endif  //  _DEVIL_IMPORTER_H_
