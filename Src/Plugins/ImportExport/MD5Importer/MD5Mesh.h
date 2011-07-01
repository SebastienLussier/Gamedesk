/**
 *  @file       MD5Model.h
 *  @brief      Defines data structures to store MD5 mesh info.
 *  @author     Sébastien Lussier.
 *  @date       12/08/04.
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
#ifndef     _MD5_MESH_H_
#define     _MD5_MESH_H_


#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Maths/Quaternion.h"
#include "Maths/BoundingBox.h"
#include "Maths/Plane3.h"
#include "Graphic/Color4.h"


namespace MD5
{

class Joint
{
public:
    String      mName;
    Int32       mParentIdx;
    Vector3f    mPosition;
    Quaternionf mOrientation;
};

class Vertex
{
public:
    Vector2f    mUV;
    UInt32      mWeightStart;
    UInt32      mWeightCount;
};

class Triangle
{
public:
    UInt32      mVertices[3];
};

class Weight
{
public:
    UInt32      mJointIndex;
    Float       mWeight;
    Vector3f    mPosition;
};

/**
 *  This is the object representing the whole MD5Mesh file.
 */
class Mesh
{
public:
    Mesh()
    {
        
    }

    ~Mesh()
    {
    }

public:
    String              mShader;
    Vector<Vertex>      mVertices;
    Vector<Triangle>    mTriangles;
    Vector<Weight>      mWeights;
};


class MeshFile
{
public:
    Vector<Joint>       mJoints;
    Vector<Mesh>        mMeshes;
    
    UInt32              mVersion;
    String              mCommandLine;
};


struct JointAnim
{
    String          mName;
    Int32           mParentIdx;
    UInt32          mAnimatedComponents;
    UInt32          mFrameDataOffset;

    Vector3f        mInitialPos;
    Quaternionf     mInitialRot;
};

struct AnimFrame
{
    BoundingBox     mBoundingBox;
    Vector<Float>   mData;
};

class AnimFile
{
public:
    enum Component
    {
        TranslationX    = 0x00000001,
        TranslationY    = 0x00000002,
        TranslationZ    = 0x00000004,
        RotationX       = 0x00000008,
        RotationY       = 0x00000010,
        RotationZ       = 0x00000020,
        AllComponents   = 0x00000038

    };    

    String              mName;
    
    UInt32              mNumAnimatedComponents;
    UInt32              mFrameRate;
    Vector<JointAnim>   mJointInfos;
    Vector<AnimFrame>   mAnimFrames;

    UInt32              mVersion;
    String              mCommandLine;
};


class Area
{
public:
    String              mName;

    class Surface
    {
    public:
    
        class Vertex
        {
        public:
            Vector3f    mPosition;
            Vector2f    mTexCoord;
            Vector3f    mNormal;
        };

        String          mName;
        Vector<Vertex>  mVertices;
        Vector<UInt32>  mIndices;
    };

    Vector<Surface>     mSurfaces;
};


class Portal
{
public:
    UInt32              mPositiveSideArea;
    UInt32              mNegativeSideArea;
    Vector<Vector3f>    mPoints;
};


class Node
{
public:
    Plane3f             mPlane;
    Int32               mPositiveChild;
    Int32               mNegativeChild;
};


class ShadowModel
{
public:
    String              mName;
    UInt32              mNumCaps;
    UInt32              mNumFrontCaps;
    UInt32              mPlaneBits;
    
    Vector<Vector3f>    mVertices;
    Vector<UInt32>      mIndices;
};


class ProcFile
{
public:
    String              mName;
    Vector<Area>        mAreas;
    Vector<Portal>      mPortals;
    Vector<Node>        mNodes;
    Vector<ShadowModel> mShadowModels;
};


};

#endif  //  _MD5_MESH_H_
