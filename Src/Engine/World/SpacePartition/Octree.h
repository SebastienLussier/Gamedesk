/**
 *  @file       SpacePartition.h
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       01/12/10.
 */
/*
 *  Copyright (C) 2010 Gamedesk
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
#ifndef     _OCTREE_H_
#define     _OCTREE_H_

#include "World/SpacePartition/SpacePartition.h"


namespace Gamedesk {


class ENGINE_API Octree : public SpacePartition
{
	DECLARE_CLASS(Octree, SpacePartition);

public:
	Octree(void);
	virtual ~Octree(void);

	virtual void Initialize(Vector3f pPosition, Vector3f pRootSize, Vector3f pLeafSize);

	virtual Bool Insert(Entity* pEntity);
	virtual Bool Remove(Entity* pEntity);

	virtual UInt32 Query(List<Entity*>& pEntities, Class* pEntityType = 0) const;
	virtual UInt32 Query(const Ray3f& pRay, List<Entity*>& pEntities, Class* pEntityType = 0) const;
	virtual UInt32 Query(const BoundingBox& pBoundingBox, List<Entity*>& pEntities, Class* pEntityType = 0) const;
	virtual UInt32 Query(const Frustum& pFrustum, List<Entity*>& pEntities, Class* pEntityType = 0) const;

protected:
	struct Node : public BoundingBox
	{
		Node*			mChildrenNodes;
		List<Entity*>	mEntities;

        Node();
        ~Node();
		void BuildTree(Vector3f pLeafSize);
		
		Bool Insert(Entity* pEntity, const BoundingBox& pBoundingBox);
		Bool Remove(Entity* pEntity, const BoundingBox& pBoundingBox);

		void Query(List<Entity*>& pEntities, Class* pEntityType) const;
		void Query(const Ray3f& pRay, List<Entity*>& pEntities, Class* pEntityType) const;
		void Query(const BoundingBox& pBoundingBox, List<Entity*>& pEntities, Class* pEntityType) const;
		void Query(const Frustum& pFrustum, List<Entity*>& pEntities, Class* pEntityType) const;
	};

	Node mRoot;
};


} // namespace Gamedesk


#endif	//	_OCTREE_H_