/**
 *  @file       ASEFile.h
 *  @brief      ASE file content representation.
 *  @author     Sébastien Lussier.
 *  @date       03/12/2010.
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
#ifndef     _ASE_FILE_H_
#define     _ASE_FILE_H_


#include "Maths/Vector2.h"
#include "Maths/Vector3.h"

#include "Maths/Matrix4.h"

#include "Graphic/Color3.h"
#include "Graphic/Color4.h"

#include "Graphic/Texture/Texture.h"


struct ASEFile
{
	UInt32 mVersion;
	String mComment;

	struct Scene
	{
		String	mFilename;
		UInt32	mFirstFrame;
		UInt32	mLastFrame;
		UInt32	mFrameSpeed;
		UInt32	mTicksPerFrame;
		Color3f mBackgroundStatic;
		Color3f mAmbientStatic;
	} mScene;

	struct Material
	{ 
		String	mName;
		String	mClass;
		Color3f mAmbient;
		Color3f mDiffuse;
		Color3f mSpecular;
		Float	mShine;
		Float	mShineStrength;
		Float	mTransparency;
		Float	mWireSize;
		String	mShading;
		Float	mXPFallOff;
		Float	mSelfIllum;
		String	mFallOff;
		String	mXPType;

		struct Map
		{
			String	mName;
			String	mClass;
			UInt32	mSubNo;
			Float	mAmount;
			String	mBitmap;
			String	mType;
			Float	mUVW_UOffset;
			Float	mUVW_VOffset;
			Float	mUVW_UTiling;
			Float	mUVW_VTiling;
			Float	mUVWAngle;
			Float	mUVWBlur;
			Float	mUVWBlurOffset;
			Float	mUVWNouseAMT;
			Float	mUVWNoiseSize;
			Float	mUVWNoiseLevel;
			Float	mUVWNoisePhase;
			String	mBitmapFilter;
		};

        Map mMapGeneric;
		Map mMapDiffuse;
        Map mMapSpecular;
        Map mMapShine;
        Map mMapShineStrength;
        Map mMapSelfIllum;

        Vector<Material> mSubMaterials;
	};
    typedef Vector<Material> MaterialList;
	MaterialList mMaterialList;

	struct GeomObject
	{
		String  mNodeName;

        struct NodeTM
        {
            String      mNodeName;
            Vector3f    mInheritPos;
            Vector3f    mInheritRot;
            Vector3f    mInheritScl;
            Matrix4f    mTransform;
            Vector3f    mPos;
            Vector3f    mRotAxis;
            Float       mRotAngle;
            Vector3f    mScale;
            Vector3f    mScaleAxis;
            Float       mScaleAxisAng;
        } mNodeTM;

        struct Mesh
        {
            struct Face
            {
                UInt32 mIndices[3];
                UInt32 mMaterialID;
            };

            struct FaceNormal
            {
                Vector3f mFaceNormal;
                Vector3f mVertexNormals[3];
            };

            Float               mTimeValue;
            UInt32              mNumVertex;
            UInt32              mNumFaces;
            Vector<Vector3f>    mVertexList;
            Vector<Face>        mFaceList;
            UInt32              mNumTVertex;
            Vector<Vector3f>    mTVertList;
            UInt32              mNumTVFaces;
            Vector<Face>        mTVFaceList;
            Vector<FaceNormal>  mNormals;
        } mMesh;

        UInt32              mMotionBlur;
        UInt32              mCastShadow;
        UInt32              mRecvShadow;
        UInt32              mMaterialRef;
	};
    typedef List<GeomObject*> GeomObjectList;
	GeomObjectList mGeomObjects;
	

};


#endif  //  _ASE_FILE_H_
