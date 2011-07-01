/**
 *  @file       MDLModel.h
 *  @brief      Defines data structures to store MDL model info.
 *  @author     Sébastien Lussier.
 *  @date       30/01/04.
 *  @see        http://www.wnwn.net/mdl/
 *  @see        http://sourceforge.net/projects/nwn-j3d/
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
#ifndef     _MDL_MODEL_H_
#define     _MDL_MODEL_H_


#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Graphic/Color4.h"


#include "MDLTags.h"


namespace NWN
{

/**
 *  Base class of all nodes.
 */
class Node
{
public:
    Node( Node* pSuperNode ) :
        mSuperNode(pSuperNode)
    {
        
    }

    virtual ~Node()
    {
        for( std::map<String,Node*>::iterator itChild = mChilds.begin(); itChild != mChilds.end(); ++itChild )
            GD_DELETE(itChild->second);
    }
        
    virtual UInt32 GetType() const = 0;
    
    void AddChild( Node* pNode )
    {
        mChilds[ToLower(pNode->mName)] = pNode;
    }

    Bool ContainsChild( const String& pName ) const
    {
        return FindChild( pName ) != NULL;
    }

    Node* FindChild( const String& pName ) const
    {
        Node*                                   childNode = NULL;
        std::map<String,Node*>::const_iterator  itChild;

        itChild = mChilds.find( pName );
        if( itChild != mChilds.end() )
            return itChild->second;

        for( itChild = mChilds.begin(); itChild != mChilds.end(); ++itChild )
        {
            childNode = itChild->second->FindChild( pName );
            if( childNode )
                return childNode;
        }

        return NULL;
    }

public:
    String                  mName;
    Node*                   mSuperNode;
    std::map<String,Node*>  mChilds;
};


/**
 *  Base class of all nodes found in the geometry section.
 */
class GeometryNode : public Node
{
public:
    GeometryNode( GeometryNode* pParent ) : Node( pParent )
    {
    }

    GeometryNode* GetSuperNode()
    {
        return (GeometryNode*)mSuperNode;
    }
};


/** 
 *  "Dummy" geometry node that contain only a transformation (no polygons).
 */
class DummyGeometryNode : public GeometryNode
{
public:
    DummyGeometryNode( GeometryNode* pParent ) : GeometryNode( pParent ),
        mScale(1.0f)
    {}

    UInt32 GetType() const
    {
        return NWN::DUMMY_ID;
    }

public:
    Vector3f        mPosition;
    Vector3f        mRotationVector;
    Float           mRotationAngle;
    Color4f         mWirecolor;
    Float           mScale;
};

class Face
{
public:
    UInt32          mV1;
    UInt32          mV2;
    UInt32          mV3;
    UInt32          mM1;
    UInt32          mT1;
    UInt32          mT2;
    UInt32          mT3;
    UInt32          mM2;
};


/**
 *  Contains all the 
 */
class TrimeshGeometryNode : public DummyGeometryNode
{
public:
    TrimeshGeometryNode( GeometryNode* pParent ) : DummyGeometryNode( pParent ),
        mAmbient(0.2f,0.2f,0.2f,1.0f),
        mDiffuse(0.8f,0.8f,0.8f,1.0f),
        mSpecular(0.0f,0.0f,0.0f,1.0f),
        mEmissive(0.0f,0.0f,0.0f,1.0f),
        mShininess(0.0f),
        mAlpha(1.0f),
        mRender(true),
        mTransparencyHint(false)
    {}

    virtual UInt32 GetType() const
    {
        return NWN::TRIMESH_ID;
    }

    // Transform the geometry indices and vertices so that we can use it effectively with index and vertex buffers.
    void Indexify();

public:
    Color4f             mAmbient;
    Color4f             mDiffuse;
    Color4f             mSpecular;
    Color4f             mEmissive;
    Float               mShininess;
    Float               mAlpha;
    String              mBitmap;
    Float               mTileFade;
    Bool                mRender;
    Bool                mTransparencyHint;
    
    Vector<Vector3f>    mVerts;
    Vector<Face>        mFaces;
    Vector<Vector2f>    mTexCoords;
    Vector<Color4f>     mColors;
    Vector<Vector3f>    mNormals;
    Vector<Float>       mConstraints;    
};

/**
 *  Definition of a particle emitter.
 */
class EmitterNode : public DummyGeometryNode
{
public:
    EmitterNode( GeometryNode* pParent ) : DummyGeometryNode( pParent ) {}

    virtual UInt32 GetType() const
    {
        return NWN::EMITTER_ID;
    }

public:
    Color4f     mColorStart;
    Color4f     mColorEnd;
    Float       mAlphaStart;
    Float       mAlphaEnd;
    Float       mSizeStart;
    Float       mSizeEnd;
    Float       mSizeStartY;
    Float       mSizeEndY;
    Int32       mFrameStart;
    Int32       mFrameEnd;
    Float       mBirthrate;
    // spawnType 0 
    Float       mLifeExp;
    Float       mMass;
    Float       mSpread;
    Float       mParticleRot;
    Float       mVelocity;
    Float       mRandVel;
    Float       mFps;
    // random 1
    Int32       mInherit;
    Int32       mInheritLocal;
    Int32       mInheritPart;
    // inheritvel 0 
    Float       mXSize;
    Float       mYSize;
    // bounce 1 
    // bounce_co 0.2
    // loop 0 
    String      mUpdate;
    String      mRender;
    String      mBlend;
    // update_sel 1 
    // render_sel 1 
    // blend_sel 3 
    // deadspace 0.0 
    // opacity 0.5 
    // blurlength 10.0 
    // lightningDelay 0.0 
    // lightningRadius 0.0 
    // lightningScale 0.0 
    // blastRadius 0.0 
    // blastLength 0.0 
    // twosidedtex 0 
    // p2p 0 
    // p2p_sel 1 
    // p2p_type Bezier 
    String      mP2PType;
    // p2p_bezier2 0.0 
    // p2p_bezier3 0.0 
    // combinetime 0.0 
    Float       mDrag;
    Float       mGrav;
    // threshold 0.0 
    String      mTexture;
    Int32       mXGrid;
    Int32       mYGrid;
    // affectedByWind false 
    // m_isTInt32ed 0 
    // renderorder 0 
    // Splat 0 
};


/**
 *  Not really sure what this thing is for... we threat it as a trimeshgeomnode for now.
 */
class DanglyMeshGeometryNode : public TrimeshGeometryNode
{
public:
    DanglyMeshGeometryNode( GeometryNode* pParent ) : TrimeshGeometryNode( pParent ) {}

    virtual UInt32 GetType() const
    {
        return NWN::DANGLY_MESH_ID;
    }

    void AddConstraint( Float pConstraint )
    {
        mConstraints.push_back( pConstraint );
    }
};


/**
 *  Must be used for axis aligned bounding box...
 */
class AabbGeometryNode : public TrimeshGeometryNode
{
public:
    class Entry
    {
    public:
        Entry( Float x1, Float y1, Float z1, Float x2, Float y2, Float z2, Int32 face )
        {
            mCornerA = Vector3f( x1, y1, z1 );
            mCornerB = Vector3f( x2, y2, z2 );
            mFace    = face;
        }

    public:
        Vector3f mCornerA;
        Vector3f mCornerB;
        Int32    mFace;
    };

    AabbGeometryNode( GeometryNode* pParent ) : TrimeshGeometryNode( pParent ) {}

    virtual ~AabbGeometryNode()
    {
        for( std::list<Entry*>::iterator itList = mListEntry.begin(); itList != mListEntry.end(); ++itList )
            GD_DELETE(*itList);
    }

    virtual UInt32 GetType() const
    {
        return NWN::AABB_ID;
    }

    void AddAabbEntry( Float x1, Float y1, Float z1, Float x2, Float y2, Float z2, Int32 face )
    {
        mListEntry.push_back( new Entry( x1, y1, z1, x2, y2, z2, face ) );
    }

public:
    std::list<Entry*>    mListEntry;
};


/**
 *  Position key for animations.
 */
class PositionKey
{
public:
    PositionKey( Float pTime, const Vector3f& pPosition ) :
        mTime(pTime),
        mPosition(pPosition)
    {           
    }

public:
    Float       mTime;
    Vector3f    mPosition;
};


class AxisAngle 
{
public:
    AxisAngle( const Vector3f& pAxis, Float pAngle ) :
        mAxis(pAxis),
        mAngle(pAngle)
    {
    }

public:
    Vector3f    mAxis;
    Float       mAngle;
};


/**
 *  Orientation key for animations.
 */
class OrientationKey
{
public:
    OrientationKey( Float pTime, const AxisAngle& mOrientation ) :
        mTime(pTime),
        mOrientation(mOrientation)
    {           
    }

public:
    Float       mTime;
    AxisAngle   mOrientation;
};


/**
 *  Animation node containing a list of position and rotation keys.
 */
class AnimationNode
{
public:
    AnimationNode( const String& pName ) :
        mName( pName )
    {
    }

    virtual ~AnimationNode()
    {
        for( std::list<PositionKey*>::iterator itList = mPositionKeys.begin(); itList != mPositionKeys.end(); ++itList )
            GD_DELETE(*itList);

        for( std::list<OrientationKey*>::iterator itList = mOrientationKeys.begin(); itList != mOrientationKeys.end(); ++itList )
            GD_DELETE(*itList);
    }

    void AddPositionKey( Float pTime, const Vector3f& pPosition )
    {
        mPositionKeys.push_back( new PositionKey(pTime, pPosition) );
    }

    void AddOrientationKey( Float pTime, const Vector3f& pAxis, Float pAngle )
    {
        mOrientationKeys.push_back( new OrientationKey(pTime, AxisAngle(pAxis,pAngle)) );
    }

public:
    String mName;
    String mParent;

    std::list<PositionKey*>     mPositionKeys;
    std::list<OrientationKey*>  mOrientationKeys;
};


/**
 *  Birthrate keys for particle emitter animations.
 *  @todo   Support more particle emitter keys.
 */
class BirthrateKey
{
public:
    BirthrateKey( Float pTime, Float pRate ) :
        mTime(pTime),
        mRate(pRate)
    {           
    }

public:
    Float   mTime;
    Float   mRate;
};


/**
 *  Animation node for a particle emitter.
 */
class AnimationEmitterNode : public AnimationNode
{
public:
    AnimationEmitterNode( const String& pName ) : AnimationNode( pName )
    {
        for( std::list<BirthrateKey*>::iterator itList = mBirthrateKey.begin(); itList != mBirthrateKey.end(); ++itList )
            GD_DELETE(*itList);
    }

    void AddBirthrateKey( Float pTime, Float pRate )
    {
        mBirthrateKey.push_back( new BirthrateKey( pTime, pRate ) );
    }
   
public:
    std::list<BirthrateKey*>    mBirthrateKey;
};


/**
 *  Definition of a model animation. Contains a list of animation nodes.
 */ 
class ModelAnimation
{
public:
    ModelAnimation( const String& pName, const String& pMaster ) : 
        mName( pName ),
        mMaster( pMaster )
    {
        ToLower(mName);
    }

    virtual ~ModelAnimation()
    {
        for( std::list<AnimationNode*>::iterator itList = mAnimNodes.begin(); itList != mAnimNodes.end(); ++itList )
            GD_DELETE(*itList);
    }

    void AddAnimationNode( AnimationNode* pAnimNode )
    {
        mAnimNodes.push_back( pAnimNode );
    }

public:
    String                      mName;
    String                      mMaster;
    String                      mModel;
    Float                       mLength;
    float                       mBlendTime;
    String                      mAnimRoot;
    std::list<AnimationNode*>   mAnimNodes;
};


/**
 *  This is the object representing the whole MDL file.
 */
class Model
{
public:
    Model( const String& pName ) :
        mName( pName )
    {
        ToLower(mName);
    }

    ~Model()
    {
        if( mGeometry )
            GD_DELETE(mGeometry);

        for( std::map<String,ModelAnimation*>::iterator itMap = mAnimations.begin(); itMap != mAnimations.end(); ++itMap )
            GD_DELETE(itMap->second);
    }

    void AddAnimation( ModelAnimation* pAnimation )
    {
        mAnimations[ToLower(pAnimation->mName)] = pAnimation;
    }

    ModelAnimation* FindAnimation( const String& pName )
    {
        std::map<String,ModelAnimation*>::iterator itAnim;
        itAnim = mAnimations.find( pName );

        if( itAnim != mAnimations.end() )
            return itAnim->second;
        
        return NULL;
    }   

public:
    String                              mName;
    String                              mSuperModelName;
    Float                               mAnimationScale;
    String                              mClassification;
    GeometryNode*                       mGeometry;
    std::map<String,ModelAnimation*>    mAnimations;
};


};


#endif  //  _MDL_MODEL_H_
