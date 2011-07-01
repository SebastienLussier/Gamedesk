/**
 *  @file       LWObject.h
 *  @brief      Lightwave objects representation.
 *  @author     Sébastien Lussier.
 *  @date       08/01/04.
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
#ifndef     _LWOBJECT_H_
#define     _LWOBJECT_H_


#include "LWOTags.h"

#include "Maths/Vector2.h"
#include "Maths/Vector3.h"

#include "Graphic/Color3.h"
#include "Graphic/Color4.h"

#include "Graphic/Texture/Texture.h"


typedef Float               LWPercent;      //!< FP4
typedef UInt32              LWIndex;        //!< VX
typedef String              LWTag;          //!< TAGS


struct LWUV
{
    Float U;
    Float V;
};

struct LWColor3
{
    Float R;
    Float G;
    Float B;
};

struct LWColor4
{
    Float R;
    Float G;
    Float B;
    Float A;
};

struct LWVertex
{    
    Bool operator == ( const LWVertex& pOther ) const
    {
        return mNormal.EqualEps( pOther.mNormal ) && mPos.EqualEps( pOther.mPos ) && mColor.EqualEps( pOther.mColor ) && mUV.EqualEps( pOther.mUV );
    }
    
    Vector3f        mPos;
    Vector3f        mNormal;
    Color4f         mColor;
    Vector2f        mUV;
};

struct LWTriangle
{
    LWIndex         mPt1;
    LWIndex         mPt2;
    LWIndex         mPt3;
    struct LWSurface* mSurface;
};

union LWMapValue
{
    LWColor3    mRGB;
    LWColor4    mRGBA;
    Float       mWeight;
    UInt32      mPick;
    LWUV        mUV;
};


/**
 *  VMAP, VMAD
 *  Associates a set of floating-point vectors with a set of points. 
 *  VMAPs begin with a type, a dimension (vector length) and a name. 
 *  These are followed by a list of vertex/vector pairs. 
 *  The vertex is given as an index into the most recent PNTS chunk, in VX format. 
 *  The vector contains dimension floating-point values. There can be any number of 
 *  these chunks, but they should all have different types or names. 
 */
struct LWVertexMap
{
    /**
     *  PICK - Selection set. This is a VMAP of dimension 0 that marks points for quick selection by name during modeling. It has no effect on the geometry of the object. 
     *  WGHT - Weight maps have a dimension of 1 and are generally used to alter the influence of deformers such as bones. Weights can be positive or negative, and the default weight for unmapped vertices is 0.0. 
     *  MNVW - Subpatch weight maps affect the shape of geometry created by subdivision patching. 
     *  TXUV - UV texture maps have a dimension of 2. 
     *  RGB, RGBA - Color maps, with a dimension of 3 or 4. 
     *  MORF - These contain vertex displacement deltas. 
     *  SPOT - These contain absolute vertex displacements (alternative vertex positions).
     */
    UInt32                  mType; 
    String                  mName;

    Bool                    mPerPoly;        //!< (VMAD) Per polygon.

    Vector<LWIndex>         mPolygonIndex;   //!< (VMAD) Per poly.
    Vector<LWIndex>         mVertexIndex;    //!< (VMAP) Per vertex.
    Vector<LWMapValue>      mValues;
};

typedef Vector<LWVertexMap*> LWVertexMapList;


struct LWVMapPt 
{
    LWVMapPt() :
        mVMap(NULL),
        mIndex(0)
    {
    }
        
    LWVertexMap*            mVMap;           //!< Pointer to vertex mapping
    LWIndex                 mIndex;          //!< Index in this vertex map
};


//! BBOX
struct LWBoundingBox
{
    LWBoundingBox() :
        mMin(0.0f, 0.0f, 0.0f),
        mMax(0.0f, 0.0f, 0.0f)
    {
    }

    Vector3f    mMin;
    Vector3f    mMax;
};


/**
 *  CLIP
 *  The CLIP chunk contains a series of subchunks describing a single, possibly time-varying image.
 */
struct LWClip
{
    LWIndex     mIndex;
    
    String      mStillImage;        //!< STIL (The source is a single still image referenced by a filename in neutral path format).
    
    LWIndex     mXRefIndex;         //!< XREF (The source is a copy, or instance, of another clip, given by the index).
    String      mXRefInstName;      //!< XREF (The string is a unique name for this instance of the clip).
};


/**
 *  TMAP
 *  Image map and procedural textures employ the TMAP subchunk to define the 
 *  mapping they use to get from object or world coordinate space to texture 
 *  space. 
 */
struct LWTextureMapping
{
    LWTextureMapping() : 
        mValid(false),
        mCenter(0.0f, 0.0f, 0.0f),
        mRotation(0.0f, 0.0f, 0.0f),
        mCoordinateSys(0) {}

    Bool        mValid;
    
    Vector3f    mCenter;            //!< CNTR (In meters).
    Vector3f    mRotation;          //!< ROTA (Vector of heading, pitch and bank in radians).
    Vector3f    mSize;              //!< SIZE (In meters).
    
    UInt16      mFalloffType;       //!< FALL (0 - Cubic, 1 - Spherical, 2 - Linear X, 3 - Linear Y, 4 - Linear Z).
    Vector3f    mFalloffVector;     //!< FALL (Rate per unit distance along each axis).

    UInt16      mCoordinateSys;     //!< CSYS (0 for object coordinates (the default if the chunk is missing) or 1).

    String      mReferenceObject;   //!< OREF (Specifies a reference object for the texture).
};


/**
 *  IMAP
 *  Texture blocks with a header type of IMAP are image maps. These use an 
 *  image to modulate one of the surface channels. In addition to the basic
 *  parameters listed below, the block may also contain a TMAP chunk. 
 */
struct LWImageMap
{
    LWImageMap() : 
        mValid(false),
        mImageMap(0),
        mWrapModeWidth(1),
        mWrapModeHeight(1)
    {
    }

    Bool        mValid;
    
    /**
     *  PROJ (The projection defines how 2D coordinates in the image are transformed into 3D coordinates in the scene).
     *  0 - Planar, 1 - Cylindrical, 2 - Spherical, 3 - Cubic, 4 - Front Projection, 5 - UV 
     *
     */    
    UInt16      mProjectionMode;
    UInt16      mProjectionAxis;    //!< AXIS (The major axis used for planar, cylindrical and spherical projections. The value is 0, 1 or 2 for the X, Y or Z axis). 
    
    LWIndex     mImageMap;          //!< IMAG (The CLIP index of the mapped image).
    
    /**
     *  WRAP (Specifies how the color of the texture is derived for areas outside the image).
     *  0 - Reset, 1 - Repeat, 2 - Mirror, 3 - Edge 
     */
    UInt16      mWrapModeWidth;
    UInt16      mWrapModeHeight;

    String      mUVVertexMapName;   //!< VMAP (name of the TXUV vertex map that contains the coordinates).
};


/**
 *  BLOK
 *  A surface may contain any number of blocks which hold texture layers or 
 *  shaders. Each block is defined by a subchunk with the following format.
 */
struct LWSurfaceBlock
{
    LWSurfaceBlock() : 
        mType(ID_IMAP),
        mTextureChannel(ID_COLR),
        mEnableState(1),
        mOpacity(1.0f),
        mOpacityType(7)
    {
    }
        
    //! IMAP = an image map texture, PROC = a procedural texture, GRAD = a gradient texture, SHDR = a shader plug-in.
    UInt32      mType;    
    
    //! Ordinal Strings (to order blocks)
    String      mOrdinalString;

    //! CHAN (Can have a value of COLR, DIFF, LUMI, SPEC, GLOS, REFL, TRAN, RIND, TRNL, or BUMP).
    UInt32      mTextureChannel;

    //! ENAB (True if the texture layer or shader should be evaluated during rendering). 
    UInt16      mEnableState;           

    /**
     * OPAC (Specifies how opaque the layer is with respect to the layers before it (beneath it) on the same channel).
     * 0 - Normal
     * 1 - Subtractive
     * 2 - Difference
     * 3 - Multiply
     * 4 - Divide
     * 5 - Alpha
     * 6 - Texture Displacement
     * 7 - Additive 
     */
    UInt16      mOpacityType;
    LWPercent   mOpacity;

    LWImageMap          mImageMap;
    LWTextureMapping    mTextureMapping;
};


/**
 * SURF
 * Base characteristics of a surface.
 * Missing: Envelopes, Refraction, Glow, Outline.
 */
struct LWSurface
{
    LWSurface() :
        mBaseColor(0.78431f,0.78431f,0.78431f),
        mDiffuse(1),
        mLuminosity(0),
        mSpecular(0),
        mReflection(0),
        mTransparency(0),
        mTranslucency(0),
        mSpecularGlossiness(0.4f),
        mDiffuseSharpness(0),
        mBumpStrength(1),
        mPolySide(1),
        mMaxSmoothingAngle(0),
        mReflectionOptions(0),
        mReflectionMapImage(0),
        mReflMapSeamAngle(0),
        mReflBlurPercent(0),
        mTransparencyOptions(0),
        mColorHighlights(0),
        mColorFilter(0),
        mAdditiveTransparency(0),
        mAlphaMode(2),
        mAlphaValue(1)
    {
    }

    ~LWSurface()
    {
        // Delete all blocks.
        Vector<LWSurfaceBlock*>::iterator    itBlocks;
        for( itBlocks = mBlocks.begin(); itBlocks != mBlocks.end(); ++itBlocks )
            GD_DELETE(*itBlocks);
    }
        
    String      mSurfaceName;
    String      mSurfaceFile;
    Color3f     mBaseColor;             //!< COLR
    LWPercent   mDiffuse;               //!< DIFF
    LWPercent   mLuminosity;            //!< LUMI
    LWPercent   mSpecular;              //!< SPEC
    LWPercent   mReflection;            //!< REFL
    LWPercent   mTransparency;          //!< TRAN
    LWPercent   mTranslucency;          //!< TRNL
    LWPercent   mSpecularGlossiness;    //!< GLOS (Glossiness controls the falloff of specular highlights).
    LWPercent   mDiffuseSharpness;      //!< SHRP (Diffuse sharpness models non-Lambertian surfaces).
    LWPercent   mBumpStrength;          //!< BUMP (Bump strength scales the height of the bumps in the gradient calculation).
    UInt16      mPolySide;              //!< SIDE (The sidedness of a polygon can be 1 for front-only, or 3 for front and back).
    Float       mMaxSmoothingAngle;     //!< SMAN (The maximum angle between adjacent polygons that will be smooth shaded).
    UInt16      mReflectionOptions;     //!< RFOP (0=Backdrop Only, 1=Raytracing+Backdrop, 2=Spherical Map, 3=Raytracing+Spherical Map).
    LWIndex     mReflectionMapImage;    //!< RIMG (A surface reflects this image as if it were spherically wrapped around the scene).
    Float       mReflMapSeamAngle;      //!< RSAN (This angle is the heading angle of the reflection map seam).
    LWPercent   mReflBlurPercent;       //!< RBLR (The amount of blurring of reflections).
    UInt16      mTransparencyOptions;   //!< TROP (0=Backdrop Only, 1=Raytracing+Backdrop, 2=Spherical Map, 3=Raytracing+Spherical Map).
    LWPercent   mColorHighlights;       //!< CLRH (A higher color highlight value blends more of the surface color and less of the incident light color).
    LWPercent   mColorFilter;           //!< CLRF (The color filter percentage determines the amount by which rays passing through a transparent surface are tinted by the color of the surface).
    LWPercent   mAdditiveTransparency;  //!< ADTR (The color of the surface is added to the color of the scene elements behind it).
    UInt16      mAlphaMode;             //!< ALPH (0=Unaffected by Surface, 1=Constant Value, 2=Surface Opacity, 3=Shadow Density).
    LWPercent   mAlphaValue;            //!< ALPH (The alpha mode defines the alpha channel output options for the surface).

    Vector<LWSurfaceBlock*>  mBlocks; //!< BLOK (Hold texture layers or shaders).
};


struct LWPoint
{
    LWPoint() :
        mPolygons(NULL),
        mPolygonsCount(0),
        mVertexMaps(NULL),
        mVertexMapsCount(0)
    {
    }

    ~LWPoint()
    {
        if( mPolygons )
            GD_DELETE_ARRAY(mPolygons);

        if( mVertexMaps )
            GD_DELETE_ARRAY(mVertexMaps);
    }

    Vector3f            mPos;               // Position of the point.
    
    LWIndex*            mPolygons;          // Indices of polygons sharing this point.
    UInt32              mPolygonsCount;     // Number of polygons sharing this point.

    LWVMapPt*           mVertexMaps;        // Vertex mappings for this point.
    UInt32              mVertexMapsCount;   // Number of vertex mappings.
};


//! PNTS  
struct LWPointList
{
    LWPointList() :
        mPoints(NULL),
        mPointsCount(0)
    {
    }

    ~LWPointList()
    {
        if( mPoints )
            GD_DELETE_ARRAY(mPoints);
    }

    LWPoint*            mPoints;        
    UInt32              mPointsCount;
};


struct LWPolygonVertex 
{
    LWPolygonVertex() :
        mIndex(0),
        mNewIndex(0),
        mVertexMaps(NULL),
        mVertexMapsCount(0)
    {
        mUV.U = 0;
        mUV.V = 0;
    }

    ~LWPolygonVertex()
    {
        if( mVertexMaps )
            GD_DELETE_ARRAY(mVertexMaps);
    }
        
    LWIndex             mIndex;             // Index into the point array
    LWIndex             mNewIndex;          // Index into the duplicated point array
    Vector3f            mNormal;            // Normal of this vertex
    LWUV                mUV;                // Texture coordinate for this vertex.
   
    LWVMapPt*           mVertexMaps;        // Vertex mappings for this vertex.
    UInt32              mVertexMapsCount;   // Number of vertex mappings.
};


struct LWPolygon
{
    LWPolygon() :
        mVertex(NULL),
        mVertexCount(0),
        mSurface(NULL),
        mNormal(0,0,0),
        mFlags(0),
        mType(0),
        mSurfaceIndex(0),
        mPartIndex(0),
        mSmoothingGroupIndex(0)
    {
    }

    ~LWPolygon()
    {
        if( mVertex )
            GD_DELETE_ARRAY(mVertex);
    }

    LWPolygonVertex*    mVertex;
    UInt32              mVertexCount;
    
    LWSurface*          mSurface;
    
    Vector3f            mNormal;

    UInt32              mFlags;
    UInt32              mType;                  //!< Can be FACE, CURV, PTCH, MBAL or BONE.

    UInt32              mSurfaceIndex;          //!< PTAG (SURF)
    UInt32              mPartIndex;             //!< PTAG (PART)
    UInt32              mSmoothingGroupIndex;   //!< PTAG (SMGP)
};


//! POLS
struct LWPolygonList
{
    ~LWPolygonList()
    {
        // Delete all poly.
        Vector<LWPolygon*>::iterator        itPolys;
        for( itPolys = mPolygons.begin(); itPolys != mPolygons.end(); ++itPolys )
            GD_DELETE(*itPolys);
    }
    
    Vector<LWPolygon*>   mPolygons;
};


/**
 *  LAYR
 *  Signals the start of a new layer. All the data chunks which follow will be 
 *  included in this layer until another layer chunk is encountered. If data is
 *  encountered before a layer chunk, it goes into an arbitrary layer. If the 
 *  least significant bit of flags is set, the layer is hidden. The parent index 
 *  indicates the default parent for this layer and can be -1 or missing to 
 *  indicate no parent. 
 */
struct LWLayer
{
    LWLayer() :
        mNumber(0),
        mFlags(0),
        mPivot(0,0,0),
        mParent(0xFFFF)
    {
    }

    ~LWLayer()
    {
        // Delete all vertex mappings.
        Vector<LWVertexMap*>::iterator   itVertexMap;
        for( itVertexMap = mVertexMaps.begin(); itVertexMap != mVertexMaps.end(); ++itVertexMap )
            GD_DELETE(*itVertexMap);
    }

    UInt16                      mNumber;
    UInt16                      mFlags;
    Vector3f                    mPivot;
    String                      mName;
    UInt16                      mParent;

    LWPointList                 mPointList;     //!< PNTS
    LWPolygonList               mPolygonList;   //!< POLY
    
    LWVertexMapList             mVertexMaps;    //!< VMAP, VMAD
        
    LWBoundingBox               mBoundingBox;   //!< BBOX
    
    String                      mDescription;   //!< DESC    
    String                      mComments;      //!< TEXT
};


//! LWO2
struct LWObject
{
    ~LWObject();

    /**
     *  Calculate the bounding box for a point list, but only if the bounding
     *  box hasn't already been initialized.
     */
    void GetBoundingBox( LWPointList& pPointList, LWBoundingBox& pBoundingBox  );
    
    /**
     *  Calculate the polygon normals.  By convention, LW's polygon normals
     *  are found as the cross product of the first and last edges.  It's
     *  undefined for one- and two-point polygons.
     */
    void GetPolyNormals( LWPointList& pPointList, LWPolygonList& pPolygonList );
    
    /**
     *  For each point, fill in the indexes of the polygons that share the
     *  point.  
     */
    void GetPointPolygons( LWPointList& pPointList, LWPolygonList& pPolygonList );
    
    /**
     *  Convert tag indexes into actual LWSurface pointers.  If any polygons
     *  point to tags for which no corresponding surface can be found, a
     *  default surface is created.
     */
    void ResolvePolySurfaces( LWPolygonList& pPolygonList );
    
    /**
     *  Calculate the vertex normals.  For each polygon vertex, sum the
     *  normals of the polygons that share the point.  If the normals of the
     *  current and adjacent polygons form an angle greater than the max
     *  smoothing angle for the current polygon's surface, the normal of the
     *  adjacent polygon is excluded from the sum.  It's also excluded if the
     *  polygons aren't in the same smoothing group.
     *  
     *  Assumes that GetPointPolygons(), GetPolyNormals() and
     *  ResolvePolySurfaces() have already been called.
     */
    void GetVertNormals( LWPointList& pPointList, LWPolygonList& pPolygonList );
    
    /**
     *  Fill in the LWVMapPt structure for each point.
     */
    void GetPointVMaps( LWPointList& pPointList, LWVertexMapList& pVertexMapList );
    
    /**
     *  Fill in the lwVMapPt structure for each polygon vertex.
     */
    void GetPolyVMaps( LWPolygonList& pPolygonList, LWVertexMapList& pVertexMapList );

    
    Vector<LWLayer*>         mLayers;       //!< LAYR
    Vector<LWClip*>          mClips;        //!< CLIP
    Vector<LWSurface*>       mSurfaces;     //!< SURF
    Vector<LWTag>            mTags;         //!< TAGS
};


#endif  //  _LWOBJECT_H_
