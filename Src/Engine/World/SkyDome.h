/**
 *  @file       SkyDome.h
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       12/02/04.
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
#ifndef     _SKY_DOME_H_
#define     _SKY_DOME_H_


#include "Entity.h"
#include "Graphic/Texture/TextureHdl.h"


namespace Gamedesk {


class Shader;
class IndexBuffer;
class VertexBuffer;


class ENGINE_API SkyDome : public Entity
{
    DECLARE_CLASS(SkyDome, Entity)

public:
    //! Default constructor.
	SkyDome();
   
    //! Destructor.
    virtual ~SkyDome();

    //! Render the model.
    virtual void Render() const;

    void SetTexture(const String& pTextureName);
    const String& GetTextureName() const;

properties:
    //! Texture name (currently does nothing...)
    String              mTextureName;

private:
    HTexture2D          mTexture;
        
    VertexBuffer*       mBufPositions;
    VertexBuffer*       mBufTexCoords;
    IndexBuffer*        mBufIndices; 
};


} // namespace Gamedesk


#endif  //  _SKY_DOME_H_