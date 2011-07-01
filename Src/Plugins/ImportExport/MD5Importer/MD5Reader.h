/**
 *  @file       MD5Reader.h
 *  @brief      Read a MD5 file into it's corresponding data structures
 *  @author     Sébastien Lussier.
 *  @date       29/01/04.
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
#ifndef     _MD5_READER_H_
#define     _MD5_READER_H_


#include "MD5Mesh.h"
#include "MD5Shader.h"

#include "FileManager/MemoryFile.h"


namespace MD5
{

class ShaderDirectory
{
    CLASS_DISABLE_COPY(ShaderDirectory);

public:
    static ShaderDirectory* Instance();

    ShaderDirectory();
    ~ShaderDirectory();
    
    void     FillDirectory( const String& pDoomFolder );

    Bool     IsInitialized() const;
    Shader*  GetShader( const String& pShaderName ) const;

    void     AddShader( Shader* pShader );

private:    
    Bool                     mInitialized;
    static ShaderDirectory   mInstance;
    Map<String,Shader*>      mShaderMap;
};


/**
 *  File reader used to parse the MD5 file.
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

    const Vector3f& ReadVector3( Vector3f& pValue );
    const Vector2f& ReadVector2( Vector2f& pValue );

    const Quaternionf& ReadQuaternion( Quaternionf& pQuaternion );

    void SkipComments();
    void EatWhite();
    Char NextChar();
    Char ReadChar();

protected:
    StringTokenizer mTokenizer;

private:
    MemoryFile*     mMemoryFile;    
};


class ShaderReader : public Reader
{
    CLASS_DISABLE_COPY(ShaderReader);

public:
    ShaderReader( ShaderDirectory* pDirectory );
    
    void Read( const String& pFileName );

private:
    void ReadTable();
    void ReadShader( Shader* pShader );
    void ReadSymbol( String& pSymbol );
    void ReadString( String& pString );
    void ReadKeyword( String& pKeyword );

    void SkipLine( Bool bIgnoreClosingBrace = false );
    void ReadBlendBlock( Shader* pShader );
    void ReadTexture( String& pString );

    Renderer::PixelBlendingFactor PixelBlendingConv( const String& symbol );

private:
    ShaderDirectory* mDirectory;
};


class MeshReader : public Reader
{
    CLASS_DISABLE_COPY(MeshReader);

public:
    MeshReader( MeshFile& pMesh );
    
    void Read( const String& pFileName );
    
private:
    void ReadJoint( Joint& joint );
    void ReadMesh( Mesh& mesh );

private:
    MeshFile&       mMeshFile;
};


class AnimReader : public Reader
{
    CLASS_DISABLE_COPY(AnimReader);

public:
    AnimReader( AnimFile& pMesh );
    
    void Read( const String& pFileName );

private:
    void ReadHierarchy();
    void ReadBounds();
    void ReadBaseFrame();
    void ReadFrame( AnimFrame& pFrame  );

private:
    AnimFile&       mAnimFile;
};


class ProcReader : public Reader
{
    CLASS_DISABLE_COPY(ProcReader);

public:
    ProcReader( ProcFile& pProc );

    void Read( const String& pFileName );

private:
    void ReadArea( Area& pArea );
    void ReadSurface( Area::Surface& pSurface );
       
    void ReadPortals();
    void ReadNodes();

    void ReadShadowModel( ShadowModel& pShadowModel );

private:
    ProcFile&       mProcFile;
};


}

#endif  //  _MD5_READER_H_
