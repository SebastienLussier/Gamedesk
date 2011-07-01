
#ifndef     _INTERSECTION_H_
#define     _INTERSECTION_H_


#include "Vector3.h"
#include "Line3.h"
#include "Plane3.h"
#include "BoundingBox.h"


namespace Gamedesk {


/**
 *  Test for an intesection between this plane and a line.
 *  @param  pLine           Line to use for the test.
 *  @param  ptIntersection  A reference to a vector that will contain the intersection point in case there is one.
 *  @param  pDistance       A reference to a T that will containt the distance between the origin of the line and the intersection point.
 *  @return False if the line is parallel to this plane, or true otherwise.
 */
template <typename T>
Bool Intersect( const Ray3<T>& pRay, const Plane3<T>& pPlane, Vector3<T>& pPtIntersection, T& pDistance )
{
    pDistance = pPlane.GetNormal() dot pRay.GetDirection();
	
	if( pDistance != static_cast<T>(0.0) )
    {
        pDistance = -((pPlane.GetNormal() dot pRay.GetOrigin()) + pPlane.GetConstant()) / pDistance;
		pPtIntersection = pRay.GetOrigin() + pRay.GetDirection()*pDistance;
		return true;
    }

	return false;
}

/**
 *  Test for an intesection between this plane and another plane.
 *  @param  pOther          Plane to use for the test.
 *  @param  pLine           A reference to a line that will contain the intersection of the two plane, in case there is one.
 *  @return False if the planes are parallel or the same, or true otherwise.
 */
template <typename T>
Bool Intersect( const Plane3<T>& pPlane1, const Plane3<T>& pPlane2, Ray3<T>& pLine )
{
    T n00 = pPlane1.GetNormal().GetLengthSqr();
    T n01 = pPlane1.GetNormal() dot pPlane2.GetNormal();
    T n11 = pPlane2.GetNormal().GetLengthSqr();
    T det = n00*n11 - n01*n01;

    if( Maths::Abs(det) < Maths::EPSILON )
        return false;

    T invDet = static_cast<T>(1.0)/det;
    T c0 = (n11*pPlane1.GetConstant() - n01*pPlane2.GetConstant())*invDet;
    T c1 = (n00*pPlane2.GetConstant() - n01*pPlane1.GetConstant())*invDet;

    pLine.SetDirection( pPlane1.GetNormal() cross pPlane2.GetNormal() );
    pLine.SetOrigin( pPlane1.GetNormal()*c0 + pPlane2.GetNormal()*c1 );

    return true;
}

INLINE Bool Intersect( const Ray3f& pRay, const BoundingBox& pBox )
{
    Float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin  = (pBox(    pRay.GetSign(0)).x - pRay.GetOrigin().x) * pRay.GetInvDirection().x;
    tmax  = (pBox(1 - pRay.GetSign(0)).x - pRay.GetOrigin().x) * pRay.GetInvDirection().x;
    tymin = (pBox(    pRay.GetSign(1)).y - pRay.GetOrigin().y) * pRay.GetInvDirection().y;
    tymax = (pBox(1 - pRay.GetSign(1)).y - pRay.GetOrigin().y) * pRay.GetInvDirection().y;
    
    if( tmin > tymax || tymin > tmax ) 
        return false;

    if( tymin > tmin )
        tmin = tymin;

    if( tymax < tmax )
        tmax = tymax;

    tzmin = (pBox(    pRay.GetSign(2)).z - pRay.GetOrigin().z) * pRay.GetInvDirection().z;
    tzmax = (pBox(1 - pRay.GetSign(2)).z - pRay.GetOrigin().z) * pRay.GetInvDirection().z;

    if( tmin > tzmax || tzmin > tmax ) 
        return false;

    /*
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
    return ( (tmin < t1) && (tmax > t0) );
    */

    return true;
}

INLINE Bool IntersectTriangle( const Vector3f& pVert0, const Vector3f& pVert1, const Vector3f& pVert2, const Ray3f& pRay )
{
    Vector3f edge1, edge2, tvec, pvec, qvec;
    Float    det,inv_det;
    Float    t, u, v;

    // Find vectors for two edges sharing pVert0.
    edge1 = pVert1 - pVert0;
    edge2 = pVert2 - pVert0;

    // Begin calculating determinant - also used to calculate U parameter.
    pvec = pRay.GetDirection() cross edge2;

    // If determinant is near zero, ray lies in plane of triangle.
    det = edge1 dot pvec;

    // Define TEST_CULL if culling is desired.
#ifdef TEST_CULL          
    if( det < Maths::EPSILON )
        return false;

    // Calculate distance from pVert0 to ray origin.
    tvec = pRay.GetOrigin() - pVert0;

    // Calculate U parameter and test bounds.
    u = tvec dot pvec;

    if( u < 0.0 || u > det )
        return false;

    // Prepare to test V parameter.
    qvec = tvec cross edge1;

    // Calculate V parameter and test bounds.
    v = pRay.GetDirection() dot qvec;
    if( v < 0.0 || u + v > det )
        return false;

    // Calculate t, scale parameters, ray intersects triangle.
    t = edge2 dot qvec;
    inv_det = 1.0 / det;
    t *= inv_det;
    u *= inv_det;
    v *= inv_det;

    // The non-culling branch.
#else                    
    if( det > -Maths::EPSILON && det < Maths::EPSILON )
        return false;

    inv_det = 1.0 / det;

    // Calculate distance from pVert0 to ray origin.
    tvec = pRay.GetOrigin() - pVert0;

    // Calculate U parameter and test bounds.
    u = (tvec dot pvec) * inv_det;
    if( u < 0.0 || u > 1.0 )
        return false;

    // Prepare to test V parameter.
    qvec = tvec cross edge1;

    // Calculate V parameter and test bounds.
    v = (pRay.GetDirection() dot qvec) * inv_det;
    if( v < 0.0 || u + v > 1.0 )
        return false;

    // Calculate t, ray intersects triangle.
    t = (edge2 dot qvec) * inv_det;
#endif

    return true;
}


} // namespace Gamedesk


#endif  //  _INTERSECTION_H_
