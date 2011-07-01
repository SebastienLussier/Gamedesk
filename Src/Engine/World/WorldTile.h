/**
 *  @file       WorldTile.h
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       02/05/05.
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
#ifndef     _WORLD_TILE_H_
#define     _WORLD_TILE_H_


#include "Entity.h"
#include "Graphic/Texture/TextureHdl.h"
#include "Input/Keyboard.h"

#include "Graphic/Buffer/VertexList.h"
#include "Graphic/Buffer/TriangleBatch.h"


namespace Gamedesk {


class VertexBuffer;
class IndexBuffer;
class ShaderProgram;
class ShaderObject;


class ENGINE_API WorldTile : public Entity
{
    DECLARE_CLASS(WorldTile, Entity)

public:
    class ENGINE_API TerrainChunk
    {
    public:
        TriangleBatch& GetHiResTriangles();
        TriangleBatch& GetLowResTriangles();

		void AddTextureLayer( const HTexture2D& pTexture, Texture2D* pAlphaMap = 0 );

        UInt32     GetLayerCount() const;
        Texture2D& GetTexture( UInt32 pLayer );
        Texture2D& GetAlphaMap( UInt32 pLayer );

    private:
        TriangleBatch   mHiResTriangles;
        TriangleBatch   mLowResTriangles;
        UInt32          mOffsetHi;
        UInt32          mOffsetLow;
		
    private:
        class TextureLayer
        {
        public:
            HTexture2D	mTexture;
            Texture2D*  mAlphaMap;
        };

        Vector<TextureLayer> mTextureLayers;
    };

public:
    //! Default constructor.
	WorldTile();
   
    //! Destructor.
    virtual ~WorldTile();

    virtual void Init();

    //! Render the model.
    virtual void Render() const;

    VertexList& GetVertexList();
    Vector<TerrainChunk*>& GetTerrainChunks();

private:
    VertexList              mVertexList;

    VertexBuffer*           mTerrainPositions;
    VertexBuffer*           mTerrainNormals;
    VertexBuffer*           mTerrainTexCoords;
    VertexBuffer*           mTerrainTexCoords_2;
    IndexBuffer*            mTerrainIndices;     
    Vector<TerrainChunk*>   mTerrainChunks;

    ShaderObject*           mPixelShader;
    ShaderObject*           mVertexShader;
    ShaderProgram*          mShaderProgram;
};


} // namespace Gamedesk


#endif  //  _WORLD_TILE_H_
