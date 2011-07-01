#include "Engine.h"
#include "Octree.h"

#include "Maths/Intersection.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Octree);

Octree::Octree(void)
{
   // Initialize(Vector3f(0,0,0), Vector3f(1000.0f, 1000.0f, 1000.0f), Vector3f(10.0f, 10.0f, 10.0f));
}

Octree::~Octree(void)
{
}

void Octree::Initialize(Vector3f pPosition, Vector3f pRootSize, Vector3f pLeafSize)
{
	mRoot.SetMin(pPosition - (pRootSize / 2));
	mRoot.SetMax(pPosition + (pRootSize / 2));
	mRoot.BuildTree(pLeafSize);
}

Bool Octree::Insert(Entity* pEntity)
{
	const BoundingBox& bb = pEntity->GetBoundingBox();
	
	return mRoot.Insert(pEntity, bb);
}

Bool Octree::Remove(Entity* pEntity)
{
	const BoundingBox& bb = pEntity->GetBoundingBox();
	return mRoot.Remove(pEntity, bb);
}

UInt32 Octree::Query(List<Entity*>& pEntities, Class* pEntityType) const
{
	mRoot.Query(pEntities, pEntityType);
	return pEntities.size();
}

UInt32 Octree::Query(const Ray3f& pRay, List<Entity*>& pEntities, Class* pEntityType) const
{
	mRoot.Query(pRay, pEntities, pEntityType);
	return pEntities.size();
}

UInt32 Octree::Query(const BoundingBox& pBoundingBox, List<Entity*>& pEntities, Class* pEntityType) const
{
	mRoot.Query(pBoundingBox, pEntities, pEntityType);
	return pEntities.size();
}

UInt32 Octree::Query(const Frustum& pFrustum, List<Entity*>& pEntities, Class* pEntityType) const
{
	mRoot.Query(pFrustum, pEntities, pEntityType);
	return pEntities.size();
}

Octree::Node::Node() : mChildrenNodes(0)
{
}

Octree::Node::~Node()
{
    GD_DELETE_ARRAY(mChildrenNodes);
}

void Octree::Node::BuildTree(Vector3f pLeafSize)
{
	Vector3f extends = mBounds[1] - mBounds[0];

	if(extends > pLeafSize)
	{
		Vector3f center = mBounds[0] + (extends / 2);
		Float t = GetExtend();

		Vector3f transforms[8];
		transforms[0] = Vector3f(0,0,0);
		transforms[1] = Vector3f(0,0,t);
		transforms[2] = Vector3f(0,t,0);
		transforms[3] = Vector3f(0,t,t);
		transforms[4] = Vector3f(t,0,0);
		transforms[5] = Vector3f(t,0,t);
		transforms[6] = Vector3f(t,t,0);
		transforms[7] = Vector3f(t,t,t);

		mChildrenNodes = GD_NEW_ARRAY(Octree::Node, 8, this, "Octree::Node");
		for(UInt32 i = 0; i < 8; i++)
		{
			mChildrenNodes[i].SetMin(mBounds[0] + transforms[i]);
			mChildrenNodes[i].SetMax(center + transforms[i]);
			mChildrenNodes[i].BuildTree(pLeafSize);
		}		
	}
    else
    {
        mChildrenNodes = 0;
    }
}

Bool Octree::Node::Insert(Entity* pEntity, const BoundingBox& pBoundingBox)
{
	if(Contains(pBoundingBox))
	{
		Bool inserted = false;

		if(mChildrenNodes)
		{
			inserted = mChildrenNodes[0].Insert(pEntity, pBoundingBox);
			if(!inserted) inserted = mChildrenNodes[1].Insert(pEntity, pBoundingBox);
			if(!inserted) inserted = mChildrenNodes[2].Insert(pEntity, pBoundingBox);
			if(!inserted) inserted = mChildrenNodes[3].Insert(pEntity, pBoundingBox);
			if(!inserted) inserted = mChildrenNodes[4].Insert(pEntity, pBoundingBox);
			if(!inserted) inserted = mChildrenNodes[5].Insert(pEntity, pBoundingBox);
			if(!inserted) inserted = mChildrenNodes[6].Insert(pEntity, pBoundingBox);
			if(!inserted) inserted = mChildrenNodes[7].Insert(pEntity, pBoundingBox);
		}

		if(!inserted)
			mEntities.push_back(pEntity);

		return true;
	}

	return false;
}

Bool Octree::Node::Remove(Entity* pEntity, const BoundingBox& pBoundingBox)
{
	if(Contains(pBoundingBox))
	{
		Bool removed = false;
		
		if(mChildrenNodes)
		{	
			removed = mChildrenNodes[0].Remove(pEntity, pBoundingBox);
			if(!removed) removed = mChildrenNodes[1].Remove(pEntity, pBoundingBox);
			if(!removed) removed = mChildrenNodes[2].Remove(pEntity, pBoundingBox);
			if(!removed) removed = mChildrenNodes[3].Remove(pEntity, pBoundingBox);
			if(!removed) removed = mChildrenNodes[4].Remove(pEntity, pBoundingBox);
			if(!removed) removed = mChildrenNodes[5].Remove(pEntity, pBoundingBox);
			if(!removed) removed = mChildrenNodes[6].Remove(pEntity, pBoundingBox);
			if(!removed) removed = mChildrenNodes[7].Remove(pEntity, pBoundingBox);
		}

		if(!removed)
			mEntities.push_back(pEntity);

		return true;
	}

	return false;
}

void Octree::Node::Query(List<Entity*>& pEntities, Class* pEntityType) const
{
	std::insert_iterator<List<Entity*> > insert_it(pEntities, pEntities.end());
	std::copy(mEntities.begin(), mEntities.end(), insert_it);

	if(mChildrenNodes)
	{
		mChildrenNodes[0].Query(pEntities, pEntityType);
		mChildrenNodes[1].Query(pEntities, pEntityType);
		mChildrenNodes[2].Query(pEntities, pEntityType);
		mChildrenNodes[3].Query(pEntities, pEntityType);
		mChildrenNodes[4].Query(pEntities, pEntityType);
		mChildrenNodes[5].Query(pEntities, pEntityType);
		mChildrenNodes[6].Query(pEntities, pEntityType);
		mChildrenNodes[7].Query(pEntities, pEntityType);
	}
}

void Octree::Node::Query(const Ray3f& pRay, List<Entity*>& pEntities, Class* pEntityType) const
{
	if(Intersect(pRay, *this))
	{
		std::insert_iterator<List<Entity*> > insert_it(pEntities, pEntities.end());
		std::copy(mEntities.begin(), mEntities.end(), insert_it);

		if(mChildrenNodes)
		{
			mChildrenNodes[0].Query(pRay, pEntities, pEntityType);
			mChildrenNodes[1].Query(pRay, pEntities, pEntityType);
			mChildrenNodes[2].Query(pRay, pEntities, pEntityType);
			mChildrenNodes[3].Query(pRay, pEntities, pEntityType);
			mChildrenNodes[4].Query(pRay, pEntities, pEntityType);
			mChildrenNodes[5].Query(pRay, pEntities, pEntityType);
			mChildrenNodes[6].Query(pRay, pEntities, pEntityType);
			mChildrenNodes[7].Query(pRay, pEntities, pEntityType);
		}
	}
}

void Octree::Node::Query(const BoundingBox& pBoundingBox, List<Entity*>& pEntities, Class* pEntityType) const
{
	if(pBoundingBox.Contains(*this))
	{
		std::insert_iterator<List<Entity*> > insert_it(pEntities, pEntities.end());
		std::copy(mEntities.begin(), mEntities.end(), insert_it);

		if(mChildrenNodes)
		{
			mChildrenNodes[0].Query(pBoundingBox, pEntities, pEntityType);
			mChildrenNodes[1].Query(pBoundingBox, pEntities, pEntityType);
			mChildrenNodes[2].Query(pBoundingBox, pEntities, pEntityType);
			mChildrenNodes[3].Query(pBoundingBox, pEntities, pEntityType);
			mChildrenNodes[4].Query(pBoundingBox, pEntities, pEntityType);
			mChildrenNodes[5].Query(pBoundingBox, pEntities, pEntityType);
			mChildrenNodes[6].Query(pBoundingBox, pEntities, pEntityType);
			mChildrenNodes[7].Query(pBoundingBox, pEntities, pEntityType);
		}
	}
}

void Octree::Node::Query(const Frustum& pFrustum, List<Entity*>& pEntities, Class* pEntityType) const
{
	if(pFrustum.BoxInFrustum(*this))
	{
		std::insert_iterator<List<Entity*> > insert_it(pEntities, pEntities.end());
		std::copy(mEntities.begin(), mEntities.end(), insert_it);

		if(mChildrenNodes)
		{
			mChildrenNodes[0].Query(pFrustum, pEntities, pEntityType);
			mChildrenNodes[1].Query(pFrustum, pEntities, pEntityType);
			mChildrenNodes[2].Query(pFrustum, pEntities, pEntityType);
			mChildrenNodes[3].Query(pFrustum, pEntities, pEntityType);
			mChildrenNodes[4].Query(pFrustum, pEntities, pEntityType);
			mChildrenNodes[5].Query(pFrustum, pEntities, pEntityType);
			mChildrenNodes[6].Query(pFrustum, pEntities, pEntityType);
			mChildrenNodes[7].Query(pFrustum, pEntities, pEntityType);
		}
	}
}


} // namespace Gamedesk
