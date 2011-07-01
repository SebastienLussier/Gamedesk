/**
*  @file       VertexList.h
*  @brief      Array of vertex.
*  @author     Sébastien Lussier.
*  @date       04/05/05.
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
#ifndef     _VERTEX_LIST_H_
#define     _VERTEX_LIST_H_


#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Graphic/Color4.h"

#include "VertexFormat.h"


namespace Gamedesk {


class TriangleBatch;


class ENGINE_API VertexList
{
public:
    VertexList();
    ~VertexList();

    void                Allocate( UInt32 pVertexCount, VertexFormat pVertexFormat );

    UInt32              GetVertexCount() const;
    VertexFormat        GetVertexFormat() const;

    Vector3f*           GetPositions();
    Vector3f*           GetNormals();
    Color4f*            GetColors();
    Vector2f*           GetTextureCoords();
    Vector2f*           GetTextureCoords_2();

    void                Reorder( TriangleBatch& pUnorderedStrip );

private:
    void                Free();

private:
    UInt32              mVertexCount;
    VertexFormat        mVertexFormat;

    Vector3f*           mPositions;
    Vector3f*           mNormals;
    Color4f*            mColors;
    Vector2f*           mTexCoords;
    Vector2f*           mTexCoords_2;
};


} // namespace Gamedesk


#endif  // _VERTEX_LIST_H_
