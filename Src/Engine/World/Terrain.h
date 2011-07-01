/**
 *  @file       Terrain.h
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
#ifndef     _TERRAIN_H_
#define     _TERRAIN_H_


#include "Entity.h"
#include "Graphic/Texture/TextureHdl.h"
#include "Input/Keyboard.h"


namespace Gamedesk {


class Shader;
class VertexBuffer;
class IndexBuffer;


class ENGINE_API Terrain : public Entity
{
    DECLARE_CLASS(Terrain, Entity)

public:
    //! Default constructor.
	Terrain();
   
    //! Destructor.
    virtual ~Terrain();

    //! Render the model.
    virtual void Render() const;

    void SetTexture(const String& pTextureName);
    const String& GetTextureName() const;

properties:
    //! Texture name (does nothing...)
    String mTextureName;

private:
    void    LoadRawFile( const String& pHeightMapFile );
    void    BuildBuffers();

    Byte    GetHeight( UInt32 pX, UInt32 pZ );

private:
    HTexture2D          mTexture;
    
    VertexBuffer*       mBufPositions;
    VertexBuffer*       mBufNormals;
    VertexBuffer*       mBufTexCoords;
    IndexBuffer*        mBufIndices; 

    UInt32              mMapSize;
    Float               mMapWidth;
    Byte*               mHeightData;
};


} // namespace Gamedesk


#endif  //  _SKY_DOME_H_