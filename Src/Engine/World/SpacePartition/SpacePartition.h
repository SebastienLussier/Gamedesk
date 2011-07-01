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
#ifndef     _SPACE_PARTITION_H_
#define     _SPACE_PARTITION_H_

#include "Maths/Vector2.h"
#include "Maths/Line3.h"
#include "Maths/Frustum.h"
#include "World/Entity.h"


namespace Gamedesk {


class ENGINE_API SpacePartition : public Object
{
	DECLARE_ABSTRACT_CLASS(SpacePartition, Object);

public:
	SpacePartition();
	virtual ~SpacePartition();

	virtual void Initialize(Vector3f pPosition, Vector3f pRootSize, Vector3f pLeafSize) = 0;

	virtual Bool Insert(Entity* pEntity) = 0;
	virtual Bool Remove(Entity* pEntity) = 0;

	virtual UInt32 Query(List<Entity*>& pEntities, Class* pEntityType = 0) const = 0;
	virtual UInt32 Query(const Ray3f& pRay, List<Entity*>& pEntities, Class* pEntityType = 0) const = 0;
	virtual UInt32 Query(const BoundingBox& pBoundingBox, List<Entity*>& pEntities, Class* pEntityType = 0) const = 0;
	virtual UInt32 Query(const Frustum& pFrustum, List<Entity*>& pEntities, Class* pEntityType = 0) const = 0;
};


} // namespace Gamedesk


#endif	// _SPACE_PARTITION_H_