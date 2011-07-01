/**
 *  @file       StaticMesh.h
 *  @brief      Mesh with no animated part.
 *  @author     Sébastien Lussier.
 *  @date       18/08/04.
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
#ifndef     _STATIC_MESH_H_
#define     _STATIC_MESH_H_


#include "Mesh.h"


namespace Gamedesk {

class ENGINE_API StaticMesh : public Mesh
{
    DECLARE_CLASS(StaticMesh, Mesh);
    
public:
    StaticMesh();
    virtual ~StaticMesh();

    void Init();
    void Render( Bool pRenderChild = true ) const;
};


} // namespace Gamedesk


#endif  //  _STATIC_MESH_H_
