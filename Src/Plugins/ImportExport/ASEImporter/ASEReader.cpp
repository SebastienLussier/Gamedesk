/**
 *  @file       ASEReader.cpp
 *  @brief      ASE model reader.
 *  @author     Sébastien Lussier.
 *  @date       03/12/10.
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
#include "ASEImporter.h"
#include "ASEReader.h"

#include "FileManager/FileManager.h"

namespace ASE
{

MeshReader::MeshReader( ASEFile& pMesh ) : mMeshFile(pMesh)
{
    ASEFile::GeomObjectList::iterator it;
    ASEFile::GeomObjectList::iterator itBegin = mMeshFile.mGeomObjects.begin();
    ASEFile::GeomObjectList::iterator itEnd = mMeshFile.mGeomObjects.end();
    for(it = itBegin; it != itEnd; ++it)
    {
        GD_DELETE(*it);
    }
}
    
void MeshReader::Read( const String& pFileName )
{
    const UInt32 _3DSMAX_ASCIIEXPORT = Hash("*3DSMAX_ASCIIEXPORT");
    const UInt32 COMMENT = Hash("*COMMENT");
    const UInt32 SCENE = Hash("*SCENE");
    const UInt32 MATERIAL_LIST = Hash("*MATERIAL_LIST");
    const UInt32 GEOMOBJECT = Hash("*GEOMOBJECT");

	Reader::Read( pFileName );

    while(GetNextLine())
    {
        String attrib = ReadString();
        UInt32 attribHash = Hash(attrib);

        if     (attribHash == _3DSMAX_ASCIIEXPORT)  Read(mMeshFile.mVersion);
        else if(attribHash == COMMENT)              Read(mMeshFile.mComment);
        else if(attribHash == SCENE)                Read(mMeshFile.mScene);
        else if(attribHash == MATERIAL_LIST)        Read(mMeshFile.mMaterialList);
        else if(attribHash == GEOMOBJECT)           
        {
            ASEFile::GeomObject* geomObject = GD_NEW(ASEFile::GeomObject, this, "ASEFile::GeomObject");
            Read(*geomObject);
            mMeshFile.mGeomObjects.push_back(geomObject);
        }
    }
}

void MeshReader::Read(ASEFile::Scene& pScene)
{
    const UInt32 SCENE_FILENAME = Hash("*SCENE_FILENAME");
    const UInt32 SCENE_FIRSTFRAME = Hash("*SCENE_FIRSTFRAME");
    const UInt32 SCENE_LASTFRAME = Hash("*SCENE_LASTFRAME");
    const UInt32 SCENE_FRAMESPEED = Hash("*SCENE_FRAMESPEED");
    const UInt32 SCENE_TICKSPERFRAME = Hash("*SCENE_TICKSPERFRAME");
    const UInt32 SCENE_BACKGROUND_STATIC = Hash("*SCENE_BACKGROUND_STATIC");
    const UInt32 SCENE_AMBIENT_STATIC = Hash("*SCENE_AMBIENT_STATIC");
    const UInt32 END_BRACKET = Hash("}");

    MustRead("{");
    
    while(GetNextLine())
    {
        String attrib = ReadString();
        UInt32 attribHash = Hash(attrib);

        if     (attribHash == SCENE_FILENAME)           Read(pScene.mFilename);
        else if(attribHash == SCENE_FIRSTFRAME)         Read(pScene.mFirstFrame);
        else if(attribHash == SCENE_LASTFRAME)          Read(pScene.mLastFrame);
        else if(attribHash == SCENE_FRAMESPEED)         Read(pScene.mFrameSpeed);
        else if(attribHash == SCENE_TICKSPERFRAME)      Read(pScene.mTicksPerFrame);
        else if(attribHash == SCENE_BACKGROUND_STATIC)  Read(pScene.mBackgroundStatic);
        else if(attribHash == SCENE_AMBIENT_STATIC)     Read(pScene.mAmbientStatic);
        else if(attribHash == END_BRACKET)              return;
    }
}

void MeshReader::Read(ASEFile::MaterialList& pMaterialList)
{
    const UInt32 MATERIAL = Hash("*MATERIAL");
    const UInt32 END_BRACKET = Hash("}");

    MustRead("{");

    GetNextLine();

    UInt32 materialCount;
    UInt32 currentIdx = 0;
    MustRead("*MATERIAL_COUNT");
    Read(materialCount);

    pMaterialList.resize(materialCount);

    while(GetNextLine())
    {
        String attrib = ReadString();
        UInt32 attribHash = Hash(attrib.c_str());

        if(attribHash == MATERIAL)                 
        {
            UInt32 idx = ReadUInt();
            if(idx != currentIdx)
                throw ResourceImportException(String("Unexpected material found"), Here);

            Read(pMaterialList[currentIdx]);
            currentIdx++;
        }
        else if(attribHash == END_BRACKET)        
        {
            if(currentIdx != materialCount)
                throw ResourceImportException(String("Missing materials"), Here);

            return;
        }
    }
}

void MeshReader::Read(ASEFile::Material& pMat)
{
    const UInt32 MATERIAL_NAME = Hash("*MATERIAL_NAME");
    const UInt32 MATERIAL_CLASS = Hash("*MATERIAL_CLASS");
    const UInt32 MATERIAL_AMBIENT = Hash("*MATERIAL_AMBIENT");
    const UInt32 MATERIAL_DIFFUSE = Hash("*MATERIAL_DIFFUSE");
    const UInt32 MATERIAL_SPECULAR = Hash("*MATERIAL_SPECULAR");
    const UInt32 MATERIAL_SHINE = Hash("*MATERIAL_SHINE");
    const UInt32 MATERIAL_SHINESTRENGTH = Hash("*MATERIAL_SHINESTRENGTH");
    const UInt32 MATERIAL_TRANSPARENCY = Hash("*MATERIAL_TRANSPARENCY");
    const UInt32 MATERIAL_WIRESIZE = Hash("*MATERIAL_WIRESIZE");
    const UInt32 MATERIAL_SHADING = Hash("*MATERIAL_SHADING");
    const UInt32 MATERIAL_XP_FALLOFF = Hash("*MATERIAL_XP_FALLOFF");
    const UInt32 MATERIAL_SELFILLUM = Hash("*MATERIAL_SELFILLUM");
    const UInt32 MATERIAL_FALLOFF = Hash("*MATERIAL_FALLOFF");
    const UInt32 MATERIAL_XP_TYPE = Hash("*MATERIAL_XP_TYPE");
    const UInt32 MAP_GENERIC = Hash("*MAP_GENERIC");
    const UInt32 MAP_DIFFUSE = Hash("*MAP_DIFFUSE");
    const UInt32 MAP_SPECULAR = Hash("*MAP_SPECULAR");
    const UInt32 MAP_SHINE = Hash("*MAP_SHINE");
    const UInt32 MAP_SHINESTRENGTH = Hash("*MAP_SHINESTRENGTH");
    const UInt32 MAP_SELFILLUM = Hash("*MAP_SELFILLUM");
    const UInt32 NUMSUBMTLS = Hash("*NUMSUBMTLS");
    const UInt32 SUBMATERIAL = Hash("*SUBMATERIAL");
    const UInt32 END_BRACKET = Hash("}");

    MustRead("{");

    UInt32 currentSubMaterialIdx = 0;

    while(GetNextLine())
    {
        String attrib = ReadString();
        UInt32 attribHash = Hash(attrib);

        if     (attribHash == MATERIAL_NAME)            Read(pMat.mName);
        else if(attribHash == MATERIAL_CLASS)           Read(pMat.mClass);
        else if(attribHash == MATERIAL_AMBIENT)         Read(pMat.mAmbient);
        else if(attribHash == MATERIAL_DIFFUSE)         Read(pMat.mDiffuse);
        else if(attribHash == MATERIAL_SPECULAR)        Read(pMat.mSpecular);
        else if(attribHash == MATERIAL_SHINE)           Read(pMat.mShine);
        else if(attribHash == MATERIAL_SHINESTRENGTH)   Read(pMat.mShineStrength);
        else if(attribHash == MATERIAL_TRANSPARENCY)    Read(pMat.mTransparency);
        else if(attribHash == MATERIAL_WIRESIZE)        Read(pMat.mWireSize);
        else if(attribHash == MATERIAL_SHADING)         Read(pMat.mShading);
        else if(attribHash == MATERIAL_XP_FALLOFF)      Read(pMat.mXPFallOff);
        else if(attribHash == MATERIAL_SELFILLUM)       Read(pMat.mSelfIllum);
        else if(attribHash == MATERIAL_FALLOFF)         Read(pMat.mFallOff);
        else if(attribHash == MATERIAL_XP_TYPE)         Read(pMat.mXPType);
        else if(attribHash == MAP_GENERIC)              Read(pMat.mMapGeneric);
        else if(attribHash == MAP_DIFFUSE)              Read(pMat.mMapDiffuse);
        else if(attribHash == MAP_SPECULAR)             Read(pMat.mMapSpecular);
        else if(attribHash == MAP_SHINE)                Read(pMat.mMapShine);
        else if(attribHash == MAP_SHINESTRENGTH)        Read(pMat.mMapShineStrength);
        else if(attribHash == MAP_SELFILLUM)            Read(pMat.mMapSelfIllum);
        else if(attribHash == NUMSUBMTLS)               
        {
            UInt32 numSubMtls = ReadUInt();
            pMat.mSubMaterials.resize(numSubMtls);
        }
        else if(attribHash == SUBMATERIAL)
        {
            UInt32 idx = ReadUInt();
            if(idx != currentSubMaterialIdx)
                throw ResourceImportException(String("Unexpected material found"), Here);

            Read(pMat.mSubMaterials[currentSubMaterialIdx]);
            currentSubMaterialIdx++;
        }
        else if(attribHash == END_BRACKET)
        {
            if(currentSubMaterialIdx != pMat.mSubMaterials.size())
                throw ResourceImportException(String("Missing sub materials"), Here);

            return;
        }
    }
}

void MeshReader::Read(ASEFile::Material::Map& pMap)
{
    const UInt32 MAP_NAME = Hash("*MAP_NAME");
    const UInt32 MAP_CLASS = Hash("*MAP_CLASS");
    const UInt32 MAP_SUBNO = Hash("*MAP_SUBNO");
    const UInt32 MAP_AMOUNT = Hash("*MAP_AMOUNT");
    const UInt32 BITMAP = Hash("*BITMAP");
    const UInt32 MAP_TYPE = Hash("*MAP_TYPE");
    const UInt32 UVW_U_OFFSET = Hash("*UVW_U_OFFSET");
    const UInt32 UVW_V_OFFSET = Hash("*UVW_V_OFFSET");
    const UInt32 UVW_U_TILING = Hash("*UVW_U_TILING");
    const UInt32 UVW_V_TILING = Hash("*UVW_V_TILING");
    const UInt32 UVW_ANGLE = Hash("*UVW_ANGLE");
    const UInt32 UVW_BLUR = Hash("*UVW_BLUR");
    const UInt32 UVW_BLUR_OFFSET = Hash("*UVW_BLUR_OFFSET");
    const UInt32 UVW_NOUSE_AMT = Hash("*UVW_NOUSE_AMT");
    const UInt32 UVW_NOISE_SIZE = Hash("*UVW_NOISE_SIZE");
    const UInt32 UVW_NOISE_LEVEL = Hash("*UVW_NOISE_LEVEL");
    const UInt32 UVW_NOISE_PHASE = Hash("*UVW_NOISE_PHASE");
    const UInt32 BITMAP_FILTER = Hash("*BITMAP_FILTER");
    const UInt32 END_BRACKET = Hash("}");

    MustRead("{");

    while(GetNextLine())
    {
        String attrib = ReadString();
        UInt32 attribHash = Hash(attrib);

        if     (attribHash == MAP_NAME)         Read(pMap.mName);
        else if(attribHash == MAP_CLASS)        Read(pMap.mClass);
        else if(attribHash == MAP_SUBNO)        Read(pMap.mSubNo);
        else if(attribHash == MAP_AMOUNT)       Read(pMap.mAmount);
        else if(attribHash == BITMAP)           Read(pMap.mBitmap);
        else if(attribHash == MAP_TYPE)         Read(pMap.mType);
        else if(attribHash == UVW_U_OFFSET)     Read(pMap.mUVW_UOffset);
        else if(attribHash == UVW_V_OFFSET)     Read(pMap.mUVW_VOffset);
        else if(attribHash == UVW_U_TILING)     Read(pMap.mUVW_UTiling);
        else if(attribHash == UVW_V_TILING)     Read(pMap.mUVW_VTiling);
        else if(attribHash == UVW_ANGLE)        Read(pMap.mUVWAngle);
        else if(attribHash == UVW_BLUR)         Read(pMap.mUVWBlur);
        else if(attribHash == UVW_BLUR_OFFSET)  Read(pMap.mUVWBlurOffset);
        else if(attribHash == UVW_NOUSE_AMT)    Read(pMap.mUVWNouseAMT);
        else if(attribHash == UVW_NOISE_SIZE)   Read(pMap.mUVWNoiseSize);
        else if(attribHash == UVW_NOISE_LEVEL)  Read(pMap.mUVWNoiseLevel);
        else if(attribHash == UVW_NOISE_PHASE)  Read(pMap.mUVWNoisePhase);
        else if(attribHash == BITMAP_FILTER)    Read(pMap.mBitmapFilter);
        else if(attribHash == END_BRACKET)      return;
    }
}

void MeshReader::Read(ASEFile::GeomObject& pGeomObject)
{
    const UInt32 NODE_NAME = Hash("*NODE_NAME");
    const UInt32 NODE_TM = Hash("*NODE_TM");
    const UInt32 MESH = Hash("*MESH");
    const UInt32 PROP_MOTIONBLUR = Hash("*PROP_MOTIONBLUR");
    const UInt32 PROP_CASTSHADOW = Hash("*PROP_CASTSHADOW");
    const UInt32 PROP_RECVSHADOW = Hash("*PROP_RECVSHADOW");
    const UInt32 MATERIAL_REF = Hash("*MATERIAL_REF");
    const UInt32 END_BRACKET = Hash("}");

    MustRead("{");

    while(GetNextLine())
    {
        String attrib = ReadString();
        UInt32 attribHash = Hash(attrib);

        if     (attribHash == NODE_NAME)        Read(pGeomObject.mNodeName);
        else if(attribHash == NODE_TM)          Read(pGeomObject.mNodeTM);
        else if(attribHash == MESH)             Read(pGeomObject.mMesh);
        else if(attribHash == PROP_MOTIONBLUR)  Read(pGeomObject.mMotionBlur);
        else if(attribHash == PROP_CASTSHADOW)  Read(pGeomObject.mCastShadow);
        else if(attribHash == PROP_RECVSHADOW)  Read(pGeomObject.mRecvShadow);
        else if(attribHash == MATERIAL_REF)     Read(pGeomObject.mMaterialRef);
        else if(attribHash == END_BRACKET)  return;
    }
}

void MeshReader::Read(ASEFile::GeomObject::NodeTM& pNodeTM)
{
    const UInt32 NODE_NAME = Hash("*NODE_NAME");
    const UInt32 INHERIT_POS = Hash("*INHERIT_POS");
    const UInt32 INHERIT_ROT = Hash("*INHERIT_ROT");
    const UInt32 INHERIT_SCL = Hash("*INHERIT_SCL");
    //const UInt32 TM_ROW0 = Hash("*TM_ROW0");
    //const UInt32 TM_ROW1 = Hash("*TM_ROW1");
    //const UInt32 TM_ROW2 = Hash("*TM_ROW2");
    //const UInt32 TM_ROW3 = Hash("*TM_ROW3");
    const UInt32 TM_ROTAXIS = Hash("*TM_ROTAXIS");
    const UInt32 TM_ROTANGLE = Hash("*TM_ROTANGLE");
    const UInt32 TM_SCALE = Hash("*TM_SCALE");
    const UInt32 TM_SCALEAXIS = Hash("*TM_SCALEAXIS");
    const UInt32 TM_SCALEAXISANG = Hash("*TM_SCALEAXISANG");
    const UInt32 END_BRACKET = Hash("}");

    MustRead("{");

    while(GetNextLine())
    {
        String attrib = ReadString();
        UInt32 attribHash = Hash(attrib);

        if     (attribHash == NODE_NAME)        Read(pNodeTM.mNodeName);
        else if(attribHash == INHERIT_POS)      Read(pNodeTM.mInheritPos);
        else if(attribHash == INHERIT_ROT)      Read(pNodeTM.mInheritRot);
        else if(attribHash == INHERIT_SCL)      Read(pNodeTM.mInheritScl);
        //else if(attribHash == TM_ROW0)          Read(pNodeTM.mTransform.);
        //else if(attribHash == TM_ROW1)          Read(pNodeTM.mInheritRot);
        //else if(attribHash == TM_ROW2)          Read(pNodeTM.mInheritRot);
        //else if(attribHash == TM_ROW3)          Read(pNodeTM.mInheritRot);
        else if(attribHash == TM_ROTAXIS)       Read(pNodeTM.mRotAxis);
        else if(attribHash == TM_ROTANGLE)      Read(pNodeTM.mRotAngle);
        else if(attribHash == TM_SCALE)         Read(pNodeTM.mScale);
        else if(attribHash == TM_SCALEAXIS)     Read(pNodeTM.mScaleAxis);
        else if(attribHash == TM_SCALEAXISANG)  Read(pNodeTM.mScaleAxisAng);
        else if(attribHash == END_BRACKET)      return;
    }
}

void MeshReader::Read(ASEFile::GeomObject::Mesh& pMesh)
{
    const UInt32 TIMEVALUE = Hash("*TIMEVALUE");
    const UInt32 MESH_NUMVERTEX = Hash("*MESH_NUMVERTEX");
    const UInt32 MESH_NUMFACES = Hash("*MESH_NUMFACES");
    const UInt32 MESH_VERTEX_LIST = Hash("*MESH_VERTEX_LIST");
    const UInt32 MESH_FACE_LIST = Hash("*MESH_FACE_LIST");
    const UInt32 MESH_NUMTVERTEX = Hash("*MESH_NUMTVERTEX");
    const UInt32 MESH_TVERTLIST = Hash("*MESH_TVERTLIST");
    const UInt32 MESH_NUMTVFACES = Hash("*MESH_NUMTVFACES");
    const UInt32 MESH_TFACELIST = Hash("*MESH_TFACELIST");
    const UInt32 MESH_NORMALS = Hash("*MESH_NORMALS");
    const UInt32 END_BRACKET = Hash("}");

    MustRead("{");

    while(GetNextLine())
    {
        String attrib = ReadString();
        UInt32 attribHash = Hash(attrib);

        if     (attribHash == TIMEVALUE)        Read(pMesh.mTimeValue);
        else if(attribHash == MESH_NUMVERTEX)   Read(pMesh.mNumVertex);
        else if(attribHash == MESH_NUMFACES)    Read(pMesh.mNumFaces);
        else if(attribHash == MESH_VERTEX_LIST) Read(pMesh.mVertexList, pMesh.mNumVertex);
        else if(attribHash == MESH_FACE_LIST)   Read(pMesh.mFaceList, pMesh.mNumFaces, true);
        else if(attribHash == MESH_NUMTVERTEX)  Read(pMesh.mNumTVertex);
        else if(attribHash == MESH_TVERTLIST)   Read(pMesh.mTVertList, pMesh.mNumTVertex);
        else if(attribHash == MESH_NUMTVFACES)  Read(pMesh.mNumTVFaces);
        else if(attribHash == MESH_TFACELIST)   Read(pMesh.mTVFaceList, pMesh.mNumTVFaces, false);
        else if(attribHash == MESH_NORMALS)     Read(pMesh.mNormals, pMesh.mNumFaces);
        else if(attribHash == END_BRACKET)      return;
    }
}

void MeshReader::Read(Vector<Vector3f>& pVector3List, UInt32 pSize)
{
    MustRead("{");

    pVector3List.resize(pSize);

    UInt32 currentIdx = 0;
    while(GetNextLine())
    {
        String attrib = ReadString();
        if(attrib == "}")
        {
            if(currentIdx != pSize)
                throw ResourceImportException(String("Missing data"), Here);

            return;
        }
        
        UInt32 idx = ReadUInt();
        if(idx != currentIdx)
            throw ResourceImportException(String("Unexpected data found"), Here);

        Read(pVector3List[idx]);
        
        currentIdx++;
    }
}

void MeshReader::Read(Vector<ASEFile::GeomObject::Mesh::Face>& pFaceList, UInt32 pSize, Bool isFaceList)
{
    MustRead("{");

    pFaceList.resize(pSize);

    UInt32 currentIdx = 0;
    while(GetNextLine())
    {
        String attrib = ReadString();
        if(attrib == "}")
        {
            if(currentIdx != pSize)
                throw ResourceImportException(String("Missing data"), Here);

            return;
        }
        
        UInt32 idx = ReadUInt();
        if(idx != currentIdx)
            throw ResourceImportException(String("Unexpected data found"), Here);

        if(isFaceList)
        {
            MustRead("A:");
            Read(pFaceList[idx].mIndices[0]);
            MustRead("B:");
            Read(pFaceList[idx].mIndices[1]);
            MustRead("C:");
            Read(pFaceList[idx].mIndices[2]);

            MustRead("AB:");
            ReadUInt();
            MustRead("BC:");
            ReadUInt();
            MustRead("CA:");
            ReadUInt();

            MustRead("*MESH_SMOOTHING");
            String res = ReadString();

            if(res != "*MESH_MTLID")
                MustRead("*MESH_MTLID");

            Read(pFaceList[idx].mMaterialID);
        }
        else
        {
            Read(pFaceList[idx].mIndices[0]);
            Read(pFaceList[idx].mIndices[1]);
            Read(pFaceList[idx].mIndices[2]);
        }
        
        currentIdx++;
    }
}

void MeshReader::Read(Vector<ASEFile::GeomObject::Mesh::FaceNormal>& pFaceNormals, UInt32 pSize)
{
    MustRead("{");

    pFaceNormals.resize(pSize);

    UInt32 currentIdx = 0;
    while(GetNextLine())
    {
        String attrib = ReadString();
        if(attrib == "}")
        {
            if(currentIdx != pSize)
                throw ResourceImportException(String("Missing data"), Here);

            return;
        }
                
        UInt32 idx = ReadUInt();
        if(idx != currentIdx)
            throw ResourceImportException(String("Unexpected data found"), Here);

        Read(pFaceNormals[idx].mFaceNormal);

        for(int i = 0; i < 3; i++)
        {
            if(!GetNextLine())
                throw ResourceImportException(String("Missing data"), Here);

            MustRead("*MESH_VERTEXNORMAL");
            ReadUInt();
            Read(pFaceNormals[idx].mVertexNormals[i]);
        }
                
        currentIdx++;
    }
}

Reader::Reader()
{
}

Reader::~Reader()
{
}

void Reader::Read( const String& pFileName )
{
    const char whitespaces[] = { 10, 13, '\t', ' ', 0 };

    mTokenizer.SetWhiteSpaces(whitespaces);
    
    File::ReadAllLines(pFileName.c_str(), mLines);

    mCurrentLine = mLines.begin();
}

void Reader::MustRead( const String& pValue )
{
    String tmp;
    ReadString( tmp );
    GD_ASSERT( tmp == pValue );
}

const String& Reader::ReadStringQuote( String& pValue )
{
    EatWhite();

    pValue = "";
    
    Char c;
    c = mTokenizer.GetChar();
    GD_ASSERT( c == '"' );
    
    c = mTokenizer.GetChar();
    while( c != '"' )
    {
        pValue += c;
        c = mTokenizer.GetChar();
    }

    return pValue;
}

const String& Reader::ReadString( String& pValue )
{
    EatWhite();

	if(NextChar() == '\"')
		return ReadStringQuote(pValue);
	
	mTokenizer >> pValue;
    return pValue;
}

String Reader::ReadString()
{
    String val;
    ReadString( val );
    return val;
}

const Float& Reader::ReadFloat( Float& pValue )
{
    EatWhite();
    mTokenizer >> pValue;
    return pValue;
}

Float Reader::ReadFloat()
{
    Float val;
    ReadFloat( val );
    return val;
}

const Int32& Reader::ReadInt( Int32& pValue )
{
    EatWhite();
    mTokenizer >> pValue;
    return pValue;
}
Int32 Reader::ReadInt()
{
    Int32 val;
    ReadInt( val );
    return val;
}

const UInt32& Reader::ReadUInt( UInt32& pValue )
{
    EatWhite();
    mTokenizer >> pValue;
    return pValue;
}

UInt32 Reader::ReadUInt()
{
    UInt32 val;
    ReadUInt( val );
    return val;
}

const Color3f& Reader::ReadColor( Color3f& pValue )
{
    ReadFloat( pValue.R );
    ReadFloat( pValue.G );
    ReadFloat( pValue.B );        
    return pValue;
}

const Vector3f& Reader::ReadVector3( Vector3f& pValue )
{
    ReadFloat( pValue.x );
    ReadFloat( pValue.y );
    ReadFloat( pValue.z );        
    return pValue;
}

const Vector2f& Reader::ReadVector2( Vector2f& pValue )
{
    ReadFloat( pValue.x );
    ReadFloat( pValue.y );
    
    return pValue;
}

void Reader::EatWhite()
{
    Int32 peek = mTokenizer.NextChar();
    while( peek == ' '  || peek == '\n' || peek == 13 || peek == '\t' )
    {
        mTokenizer.GetChar();
        peek = mTokenizer.NextChar();
    }
}

Char Reader::NextChar()
{
    EatWhite();
    return mTokenizer.NextChar();
}

Char Reader::ReadChar()
{
    EatWhite();
    return mTokenizer.GetChar();
}

Bool Reader::GetNextLine()
{
    mCurrentLine++;
    Bool bGotLine = mCurrentLine != mLines.end();
    if(bGotLine)
        mTokenizer.Init(mCurrentLine->c_str(), mCurrentLine->size());
    return bGotLine;
}

}
