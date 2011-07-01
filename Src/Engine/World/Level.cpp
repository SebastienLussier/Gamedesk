/**
 *  @file       Level.cpp
 *  @brief	    LensFlare effect.
 *  @author     Sébastien Lussier
 *  @date       01/03/05.
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
#include "Engine.h"
#include "Level.h"

/*
#include "Graphic/Mesh.h"

#include "Resource/ResourceManager.h"
#include "WorldTile.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Level);


Level::Level()
{
    ResourceImporter* importer = ResourceManager::Instance()->GetImporterForFile( "Data/World/Maps/Kalimdor/Kalimdor_23_40.adt", WorldTile::StaticClass() );

    for( UInt32 y = 59; y <= 59; y++ )
    {
        for( UInt32 x = 8; x <= 8; x++ )
        {
	        const String filename = String("Data/World/Maps/Azeroth/Azeroth_") + ToString(x) + String("_") + ToString(y) + String(".adt");
	        
            WorldTile* tile = Cast<WorldTile>( importer->Import(filename) );
	        tile->Init();

            mTiles.push_back(tile);
        }
    }
}

Level::~Level()
{
    for( Vector<WorldTile*>::iterator it = mTiles.begin(); it != mTiles.end(); ++it )
	    GD_DELETE(*it);
}

void Level::Render() const
{
    for( Vector<WorldTile*>::const_iterator it = mTiles.begin(); it != mTiles.end(); ++it )
        (*it)->Render();
}

} // namespace Gamedesk

*/

#include "World/SpacePartition/BSP.h"
#include "Resource/ResourceManager.h"


namespace Gamedesk {


IMPLEMENT_CLASS(Level);


Level::Level()
{
	const String filename = "Data/Quake2/maps/base1.bsp";
	ResourceImporter* importer = ResourceManager::Instance()->GetImporterForFile( filename, Bsp::StaticClass() );
    mBSP = Cast<Bsp>(importer->Import( filename ));
	mBSP->Init();

	for( UInt32 i = 0; i < mBSP->mVertices.size(); i++ )
		mBoundingBox.Grow( mBSP->mVertices[i] );
}

Level::~Level()
{
	GD_DELETE(mBSP);
}

void Level::Render() const
{
	if( mBSP )
		mBSP->Render();
}


} // namespace Gamedesk
