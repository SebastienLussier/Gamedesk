/**
 *  @file       ASEReader.h
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
#ifndef     _ASE_READER_H_
#define     _ASE_READER_H_


#include "Exception/Exception.h"
#include "ASEFile.h"

#include "FileManager/MemoryFile.h"

namespace ASE
{

/**
 *  File reader used to parse the ASE file.
 */
class Reader
{
    CLASS_DISABLE_COPY(Reader);

public:
    Reader();
    ~Reader();

    virtual void    Read( const String& pFileName );

protected:    
    void            MustRead( const String& pValue );
    
    const String&   ReadStringQuote( String& pValue );

    const String&   ReadString( String& pValue );
    String          ReadString();

    const Float&    ReadFloat( Float& pValue );
    Float           ReadFloat();

    const Int32&    ReadInt( Int32& pValue );
    Int32           ReadInt();
    
    const UInt32&   ReadUInt( UInt32& pValue );
    UInt32          ReadUInt();

    const Color3f&  ReadColor( Color3f& pValue );
    const Vector3f& ReadVector3( Vector3f& pValue );
    const Vector2f& ReadVector2( Vector2f& pValue );

    void EatWhite();
    Char NextChar();
    Char ReadChar();
	Char PeekChar();

    Bool GetNextLine();

private:
    typedef List<String>                StringList;
    typedef StringList::const_iterator  StringListIterator;

    StringTokenizer         mTokenizer;
    StringList              mLines;
    StringListIterator      mCurrentLine;
};


class MeshReader : public Reader
{
    CLASS_DISABLE_COPY(MeshReader);

public:
    MeshReader( ASEFile& pMesh );
    
    void Read( const String& pFileName );

private:
    void Read(ASEFile::Scene& pScene);
    void Read(ASEFile::MaterialList& pMaterialList);
    void Read(ASEFile::Material& pMat);
    void Read(ASEFile::Material::Map& pMap);
    void Read(ASEFile::GeomObject& pGeomObject);
    void Read(ASEFile::GeomObject::NodeTM& pNodeTM);
    void Read(ASEFile::GeomObject::Mesh& pMesh);
    void Read(Vector<Vector3f>& pVector3List, UInt32 pSize);
    void Read(Vector<ASEFile::GeomObject::Mesh::Face>& pFaceList, UInt32 pSize, Bool isFaceList = true);
    void Read(Vector<ASEFile::GeomObject::Mesh::FaceNormal>& pFaceNormals, UInt32 pSize);
    
    template <class T>
    inline void Read(T& pAttributeValue);
    
private:
    ASEFile&       mMeshFile;
};

template <>
inline void MeshReader::Read<Float>(Float& pAttributeValue)
{
    ReadFloat(pAttributeValue);
}

template <>
inline void MeshReader::Read<UInt32>(UInt32& pAttributeValue)
{
    ReadUInt(pAttributeValue);
}

template <>
inline void MeshReader::Read<Int32>(Int32& pAttributeValue)
{
    ReadInt(pAttributeValue);
}

template <>
inline void MeshReader::Read<String>(String& pAttributeValue)
{
    ReadString(pAttributeValue);
}

template <>
inline void MeshReader::Read<Vector2f>(Vector2f& pAttributeValue)
{
    ReadVector2(pAttributeValue);
}

template <>
inline void MeshReader::Read<Vector3f>(Vector3f& pAttributeValue)
{
    ReadVector3(pAttributeValue);
}

template <>
inline void MeshReader::Read<Color3f>(Color3f& pAttributeValue)
{
    ReadColor(pAttributeValue);
}

}

#endif  //  _ASE_READER_H_
