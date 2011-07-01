/**
 *  @file       BSPImporter.h
 *  @brief      Doom 3 BSP mesh/anim importer.
 *  @author     Sébastien Lussier.
 *  @date       12/08/04.
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
#ifndef     _BSPIMPORTER_H_
#define     _BSPIMPORTER_H_

#include "World/SpacePartition/BSP.h"
#include "Resource/ResourceManager.h"

#include "BSPFile.h"



class BSPIMPORTER_API BSPImporter : public ResourceImporter
{
    DECLARE_CLASS(BSPImporter,ResourceImporter);
    
public:
    BSPImporter();

    virtual Class*    GetResourceClass();
    virtual Resource* Import( const String& pFilename, const String& pParams = "" );

private:
	Bsp*	CreateBSP( BSP::BSPFile& bspFile );
    void    DecompressVisibility( Bitset& pClusterPVS, Byte* pIn );
};


#endif  //  _BSPIMPORTER_H_
