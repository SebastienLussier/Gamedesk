/**
 *  @file       Mesh.h
 *  @brief      Base class for all types of meshes.
 *  @author     Sébastien Lussier.
 *  @date       15/01/03.
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
#ifndef     _MESH_H_
#define     _MESH_H_


#include "Graphic/Material/Surface.h"
#include "Resource/Resource.h"

#include "Maths/Vector3.h"
#include "Maths/Quaternion.h"
#include "Maths/BoundingBox.h"
#include "Maths/Line3.h"

#include "Graphic/Color3.h"
#include "Graphic/Color4.h"
#include "Graphic/Texture/TextureHdl.h"

#include "Graphic/Buffer/VertexList.h"
#include "Graphic/Buffer/TriangleBatch.h"

#include "Collision/kDOPTree.h"


namespace Gamedesk {


class VertexBuffer;
class IndexBuffer;
class Shader;


class ENGINE_API Mesh : public Resource
{
    DECLARE_CLASS(Mesh, Resource);
    
public:
    Mesh();
    virtual ~Mesh();

    virtual void Init();
    virtual void Update(Double pElapsedTime);
    virtual void Render(Bool pRenderChild = true) const;
    
    const BoundingBox& GetBoundingBox() const;

    VertexList&         GetVertexList();
    TriangleBatch&      GetTriangles();

    virtual Bool LineCheck( const Ray3f& pRay ) const;
	
    //! Adds a child to this mesh.
    void AddChild(Mesh* pMesh);
    
public:
    VertexList          mVertexList;
    TriangleBatch       mTriangles;
    
    Shader*             mShader;

    VertexBuffer*       mBufPositions;
    VertexBuffer*       mBufNormals;
    VertexBuffer*       mBufColors;
    VertexBuffer*       mBufTexCoords;
    IndexBuffer*        mBufIndices; 

protected:
    Vector<Mesh*>		mChildMeshes;

private: 
    BoundingBox         mBoundingBox;
    kDOPTree            mkDOPTree;
};


} // namespace Gamedesk


#endif  //  _MESH_H_
