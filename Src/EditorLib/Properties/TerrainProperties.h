/**
 *  @file       TerrainProperties.h
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       18/02/04.
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
#ifndef     _TERRAIN_PROPERTIES_H_
#define     _TERRAIN_PROPERTIES_H_


#include "EntityProperties.h"


class Terrain;


/**
 *  TerrainProperties class.  This object is used as a facade to a model3D.
 *  As a PropertiesSender and PropertiesListener, it can be connected with
 *  other Senders/Listeners to keep the data up to date.
 *  @brief  TerrainProperties class.
 *  @author Marco Arsenault.
 *  @date   06/02/04.
 */
class EDITORLIB_API TerrainProperties : public EntityProperties
{
    DECLARE_CLASS(TerrainProperties, EntityProperties);

public:
    //! Constructor.
	TerrainProperties();

    //! Destructor.
    virtual ~TerrainProperties();

    //! Initialize all properties.
    virtual void InitProperties();

    void SetTexture(const String& pTextureName);
    const String& GetTexture() const;

private:
    PropertyString  mTextureName;
};


#endif  //  _TERRAIN_PROPERTIES_H_
