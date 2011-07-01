 /**
 *  @file       SkeletalMesh.h
 *  @brief      An animated mesh with bones.
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
#ifndef     _SKELETAL_MESH_H_
#define     _SKELETAL_MESH_H_


#include "Mesh.h"


namespace Gamedesk {
	

class SkeletalAnim;


class ENGINE_API SkeletalMesh : public Mesh
{
    DECLARE_CLASS(SkeletalMesh, Mesh);

public:
    class Bone
    {
    public:
        Int32           mParentIndex;
        String          mName;

        Vector3f        mWorldPosition;
        Vector3f        mPosition;
        Quaternionf     mOrientation;
        
        Matrix4f        mTransfo;

        Vector<UInt32>  mChildsIndex;
    };

    class ENGINE_API Section
    {
    public:
        Section();
        ~Section();

        // Shader
        Shader*     mShader;

        // Vertices
        UInt32      mVertexStart;
        UInt32      mVertexCount;
        Vector3f*   mPositions;
        Vector3f*   mVertexNormals;
        
        // Indices
        UInt32      mIndexStart;
        UInt32      mIndexCount;
        Vector3f*   mFaceNormals;

        // Weights
        UInt32      mWeightStart;
        UInt32      mWeightCount;
    };
       
    class Weight
    {
    public:
        UInt32      mBoneIndex;
        Float       mWeight;
        Vector3f    mPosition;
    };

    class VertexInfluence
    {
    public:
        UInt32      mWeightIndex;   
        UInt32      mCount;
    };

public:
    SkeletalMesh();
    virtual ~SkeletalMesh();

    void Init();
    void Update( Double pElapsedTime );
    void Render( Bool pRenderChild = true ) const;
    void AddAnim( SkeletalAnim* pAnim );

	BoundingBox	GetBoundingBox( const Matrix4f& pTransformation = Matrix4f::IDENTITY );

private:
    void RenderNormals() const;
    void RenderBones() const;
    
    void UpdateBone( UInt32 pBoneIndex, Float pTime );
    void ApplyWeight();

    void ComputeTriangleNormals();
    void ComputeVertexNormals();
    
public:
    Vector<Bone>                mBones;
    Vector<Section>             mSections;
    Vector<Weight>              mWeights;
    Vector<VertexInfluence>     mInfluences;

    Vector<Vector3f>            mVertexPositions;
    Vector<Vector3f>            mVertexNormals;
    Vector< Vector<UInt16> >    mVertexTriangles; 
    Vector<Vector3f>            mTriangleNormals;
    
    Vector<SkeletalAnim*>       mAnims;

    Double                      mTimeStart;
    BoundingBox                 mBoundingBox;
};


} // namespace Gamedesk


#endif  //  _SKELETAL_MESH_H_
