/**
 *  @file       MD5Reader.cpp
 *  @brief      Read a MD5 file into it's corresponding data structures.
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
#include "MD5ImporterDll.h"
#include "MD5Reader.h"

#include "FileManager/FileManager.h"
#include "Resource/ResourceManager.h"

namespace MD5
{


ShaderDirectory::ShaderDirectory()
    : mInitialized(false)
{
}

ShaderDirectory::~ShaderDirectory()
{
    for( Map<String,Shader*>::iterator it  = mShaderMap.begin();
                                       it != mShaderMap.end();
                                       ++it )
    {
        GD_DELETE(it->second);
    }
}

void ShaderDirectory::FillDirectory( const String& pShaderDirectory )
{
    if( mInitialized )
        return;

    Vector<String>              files;
    Vector<String>::iterator    itList;

    String fullDir = pShaderDirectory + String("\\materials");
       
    FileManager::FindFiles( fullDir, "*.mtr", files );
    ShaderReader reader(this);

    for( itList = files.begin(); itList != files.end(); ++itList )
       reader.Read( fullDir + String("\\") + *itList );

    mInitialized = true;
}

void ShaderDirectory::AddShader( Shader* pShader )
{
    mShaderMap[pShader->mName] = pShader;
}


Bool ShaderDirectory::IsInitialized() const
{
    return mInitialized;
}

Shader* ShaderDirectory::GetShader( const String& pShaderName ) const
{
    Map<String,Shader*>::const_iterator itFind = mShaderMap.find(pShaderName);

    if( itFind != mShaderMap.end() )
        return itFind->second;

    return NULL;
}

ShaderReader::ShaderReader( ShaderDirectory* pDirectory )
    : mDirectory(pDirectory)
{
}

void ShaderReader::Read( const String& pFile )
{
    Reader::Read( pFile );

    String lastSymbol;

    while( !mTokenizer.AtEnd() )
    {
        String symbol;
        ReadString( symbol );

        if( symbol.size() == 0 )
            break;

        if( symbol == "table" )
        {
            ReadString( symbol );            
            ReadTable();
        }
        else
        {
            if( symbol == "material" || symbol == "skin" || symbol == "particle" )
                ReadString( symbol );
                       
            Shader* shader = GD_NEW(Shader, this, "Shader");
            shader->mName = symbol;
            
            ReadShader( shader );
            mDirectory->AddShader( shader );
        }

        lastSymbol = symbol;
    }
}

void ShaderReader::ReadTable()
{
    String symbol;

    UInt32 numBrace = 0;

    if( NextChar() == '{' )
    {
        ReadChar();
        numBrace++;
    }

    
    while( numBrace != 0 )
    {
        if( NextChar() == '{' )
            numBrace++;
    
        if( NextChar() == '}' )
            numBrace--;

        ReadChar();
    }
}
void ShaderReader::ReadShader( Shader* pShader )
{
    String symbol;
    
    if( ReadChar() != '{' )
        debugBreak();
    
    while( NextChar() != '}' )
    {
        if( NextChar() == '{' )
        {
            ReadBlendBlock( pShader );
        }
        else
        {
            ReadSymbol( symbol );

            if( symbol == "bumpmap" )
                ReadTexture( pShader->mTextures[Shader::TexBump] );
            else if( symbol == "specularmap" )
                ReadTexture( pShader->mTextures[Shader::TexSpecular] );
            else if( symbol == "diffusemap" )
                ReadTexture( pShader->mTextures[Shader::TexDiffuse] );
            else if( symbol == "nonsolid" )
                pShader->mNonSolid = true;
            else
                SkipLine();
        }
    }

    if( ReadChar() != '}' )
        debugBreak();
}

void ShaderReader::ReadBlendBlock( Shader* pShader )
{
    String symbol;

    if( ReadChar() != '{' )
        debugBreak();

    Shader::TextureType CurrentTex = Shader::TexDiffuse;
    
    while( NextChar() != '}' )
    {
        ReadKeyword( symbol );
        
        if( symbol == "blend" )
        {
            ReadKeyword( symbol );
            
            if( symbol == "add" )
            {
                pShader->mBlendSrc[CurrentTex] = Renderer::BlendOne;
                pShader->mBlendDst[CurrentTex] = Renderer::BlendOne;
            }
            else if( symbol == "blend" )
            {
                pShader->mBlendSrc[CurrentTex] = Renderer::BlendZero;
                pShader->mBlendDst[CurrentTex] = Renderer::BlendSrcColor;
            }
            else if( symbol == "filter" )
            {
                pShader->mBlendSrc[CurrentTex] = Renderer::BlendSrcAlpha;
                pShader->mBlendDst[CurrentTex] = Renderer::BlendInvSrcAlpha;
            }
            else if( symbol == "diffusemap" ) 
            {
                CurrentTex = Shader::TexDiffuse;
            }
            else if( symbol == "specularmap" )
            {
                CurrentTex = Shader::TexSpecular;
            }
            else if( symbol == "bumpmap" )
            {
                CurrentTex = Shader::TexBump;
            }            
            else
            {
                pShader->mBlendSrc[CurrentTex] = PixelBlendingConv( symbol );
                
                if( ReadChar() != ',' )
                    debugBreak();

                ReadKeyword( symbol );
                pShader->mBlendDst[CurrentTex] = PixelBlendingConv( symbol );
            }
        }
        else if( symbol == "map" )
        {
            ReadTexture( pShader->mTextures[CurrentTex] );
        }
        else
        {
            SkipLine();
        }
    }

    if( ReadChar() != '}' )
        debugBreak();
}

void ShaderReader::ReadTexture( String& pString )
{
    ReadString( pString );

    if( pString == "addnormals" || 
        pString == "heightmap"  || 
        pString == "makealpha" )
    {
        if( ReadChar() != '(' )
            debugBreak();

        ReadString( pString );
    }

    SkipLine();
}

Renderer::PixelBlendingFactor ShaderReader::PixelBlendingConv( const String& symbol )
{
    if( symbol == "gl_zero" )
        return Renderer::BlendZero;
    else if( symbol == "gl_one" )
        return Renderer::BlendOne;
    else if( symbol == "gl_src_color" )
        return Renderer::BlendSrcColor;
    else if( symbol == "gl_one_minus_src_color" )
        return Renderer::BlendInvSrcColor;
    else if( symbol == "gl_dst_color" )
        return Renderer::BlendDstColor;
    else if( symbol == "gl_one_minus_dst_color" )
        return Renderer::BlendInvDstColor;
    else if( symbol == "gl_src_alpha" )
        return Renderer::BlendSrcAlpha;
    else if( symbol == "gl_one_minus_src_alpha" )
        return Renderer::BlendInvSrcAlpha;
    else if( symbol == "gl_dst_alpha" )
        return Renderer::BlendDstAlpha;
    else if( symbol == "gl_one_minus_dst_alpha" )
        return Renderer::BlendInvDstAlpha;
    else if( symbol == "gl_src_alpha_saturate" )
        return Renderer::BlendSrcAlphaSaturate;

    debugBreak();
    return Renderer::BlendZero; 
}

void ShaderReader::ReadString( String& pString )
{
    SkipComments();

    pString = "";

    Int32 peek = mTokenizer.NextChar();

    while( peek != ' ' && peek != '\n' && peek != 13 && peek != '\t' && peek != '}' && peek != '{' &&  peek != '(' && peek != ')' && peek != 0 && peek != -1 )
    {
        peek = mTokenizer.GetChar();
        if( peek == '/' && (mTokenizer.NextChar() == '/' || mTokenizer.NextChar() == '*') )
        {
             mTokenizer.UngetChar();
             break;
        }
        
        pString += peek;
        peek = mTokenizer.NextChar();
    }

    if( pString.size() == 0 && peek != 0 && peek != -1 )
        debugBreak();
}

void ShaderReader::ReadSymbol( String& pString )
{
    SkipComments();

    pString = "";

    Int32 peek = mTokenizer.NextChar();

    while( (peek >= 'A' && peek <= 'Z') ||
           (peek >= 'a' && peek <= 'z') ||
           (peek >= '0' && peek <= '9') ||
           (peek == '_') || (peek == '*') )
    {
        if( pString.size() == 0 && (peek >= '0' && peek <= '9') )
            debugBreak();

        pString += peek;

        mTokenizer.GetChar();
        peek = mTokenizer.NextChar();
    }

    if( pString.size() == 0 )
        debugBreak();
}

void ShaderReader::ReadKeyword( String& pKeyword )
{
    ReadSymbol( pKeyword );
    ToLower( pKeyword );
}

void ShaderReader::SkipLine( Bool bIgnoreBrace )
{
    Int32 peek = mTokenizer.NextChar();
    while( peek != '\n' )
    {
        if( (peek == '{' || peek == '}') && !bIgnoreBrace )
            break;
        
        mTokenizer.GetChar();
        peek = mTokenizer.NextChar();
    }

    if( peek == '\n' )
        mTokenizer.GetChar();
}

MeshReader::MeshReader( MeshFile& pMeshFile )
    : mMeshFile(pMeshFile)
{
}

void MeshReader::Read( const String& pFileName )
{
    Reader::Read( pFileName );

    UInt32 numJoints;
    UInt32 numMeshes;

    MustRead("MD5Version");
    ReadUInt(mMeshFile.mVersion);

    MustRead("commandline");
    ReadStringQuote(mMeshFile.mCommandLine);

    // numJoints
    MustRead("numJoints");
    ReadUInt(numJoints);
    mMeshFile.mJoints.resize(numJoints);

    // numMeshes
    MustRead("numMeshes");
    ReadUInt(numMeshes);
    mMeshFile.mMeshes.resize(numMeshes);

    MustRead("joints");
    MustRead("{");
    for( Vector<Joint>::iterator it = mMeshFile.mJoints.begin(); it != mMeshFile.mJoints.end(); ++it )
        ReadJoint(*it);
    MustRead("}");
    
    for( Vector<Mesh>::iterator it = mMeshFile.mMeshes.begin(); it != mMeshFile.mMeshes.end(); ++it )
        ReadMesh(*it);
}

void MeshReader::ReadJoint( Joint& joint )
{
    ReadStringQuote( joint.mName );
    ReadInt( joint.mParentIdx );
    ReadVector3( joint.mPosition );
    ReadQuaternion( joint.mOrientation );
}

void MeshReader::ReadMesh( Mesh& mesh )
{
    UInt32 numVerts;
    UInt32 numTris;
    UInt32 numWeights;

    MustRead("mesh");
    MustRead("{");

    MustRead("shader");
    ReadStringQuote(mesh.mShader);
    
    MustRead("numverts");
    ReadUInt(numVerts);
    mesh.mVertices.resize(numVerts);
    for( Vector<Vertex>::iterator it = mesh.mVertices.begin(); it != mesh.mVertices.end(); ++it )
    {
        Vertex& vertex = (*it);

        MustRead("vert");
        ReadUInt(); // Index
        ReadVector2( vertex.mUV );
        ReadUInt( vertex.mWeightStart );
        ReadUInt( vertex.mWeightCount );
    }    


    MustRead("numtris");
    ReadUInt(numTris);
    mesh.mTriangles.resize(numTris);
    for( Vector<Triangle>::iterator it = mesh.mTriangles.begin(); it != mesh.mTriangles.end(); ++it )
    {
        Triangle& triangle = (*it);
        MustRead("tri");
        ReadUInt(); // Index
        ReadUInt( triangle.mVertices[0] );
        ReadUInt( triangle.mVertices[1] );
        ReadUInt( triangle.mVertices[2] );
    }

    MustRead("numweights");
    ReadUInt(numWeights);
    mesh.mWeights.resize(numWeights);
    for( Vector<Weight>::iterator it = mesh.mWeights.begin(); it != mesh.mWeights.end(); ++it )
    {
        Weight& weight = (*it);

        MustRead("weight");
        ReadUInt(); // Index
        ReadUInt( weight.mJointIndex );
        ReadFloat( weight.mWeight );
        ReadVector3( weight.mPosition );
    }
    
    MustRead("}");
}

///////////////////////////////////////////////////////////////////////////////
AnimReader::AnimReader( AnimFile& pAnimFile ) 
    : mAnimFile(pAnimFile)
{
}

    
void AnimReader::Read( const String& pFileName )
{
    Reader::Read( pFileName );

    UInt32 numJoints;
    UInt32 numFrames;

    MustRead("MD5Version");
    ReadUInt(mAnimFile.mVersion);

    MustRead("commandline");
    ReadStringQuote(mAnimFile.mCommandLine);

    // numFrames 80
    // numJoints 71
    // frameRate 24
    // numAnimatedComponents 52

    // numFrames
    MustRead("numFrames");
    ReadUInt(numFrames);
    mAnimFile.mAnimFrames.resize(numFrames);

    // numJoints
    MustRead("numJoints");
    ReadUInt(numJoints);
    mAnimFile.mJointInfos.resize(numJoints);

    // frameRate
    MustRead("frameRate");
    ReadUInt(mAnimFile.mFrameRate);

    // numAnimatedComponents
    MustRead("numAnimatedComponents");
    ReadUInt(mAnimFile.mNumAnimatedComponents);
    
    ReadHierarchy( );
    ReadBounds();
    ReadBaseFrame();

    for( UInt32 iFrame = 0; iFrame < numFrames; iFrame++ )
        ReadFrame( mAnimFile.mAnimFrames[iFrame] );
}

void AnimReader::ReadHierarchy()
{
    MustRead("hierarchy");
    MustRead("{");

    for( UInt32 iJoint = 0; iJoint < mAnimFile.mJointInfos.size(); iJoint++ )
    {
        ReadStringQuote( mAnimFile.mJointInfos[iJoint].mName );
        ReadInt( mAnimFile.mJointInfos[iJoint].mParentIdx );
        ReadUInt( mAnimFile.mJointInfos[iJoint].mAnimatedComponents );
        ReadUInt( mAnimFile.mJointInfos[iJoint].mFrameDataOffset );
    }

    MustRead("}");
}

void AnimReader::ReadBounds()
{
    MustRead("bounds");
    MustRead("{");

    for( UInt32 iFrame = 0; iFrame < mAnimFile.mAnimFrames.size(); iFrame++ )
    {
        ReadVector3( mAnimFile.mAnimFrames[iFrame].mBoundingBox(0) );
        ReadVector3( mAnimFile.mAnimFrames[iFrame].mBoundingBox(1) );
    }

    MustRead("}");
}

void AnimReader::ReadBaseFrame()
{
    MustRead("baseframe");
    MustRead("{");

    for( UInt32 iJoint = 0; iJoint < mAnimFile.mJointInfos.size(); iJoint++ )
    {
        ReadVector3( mAnimFile.mJointInfos[iJoint].mInitialPos );
        ReadQuaternion( mAnimFile.mJointInfos[iJoint].mInitialRot );
    }
    MustRead("}");
}

void AnimReader::ReadFrame( AnimFrame& pFrame )
{
    Int32 frameNum;

    MustRead("frame");
    ReadInt( frameNum );
    MustRead("{");

    pFrame.mData.resize( mAnimFile.mNumAnimatedComponents );
    for( UInt32 iData = 0; iData < mAnimFile.mNumAnimatedComponents; iData++ )
        ReadFloat( pFrame.mData[iData] );

    MustRead("}");
}


///////////////////////////////////////////////////////////////////////////////
ProcReader::ProcReader( ProcFile& pProcFile )
    : mProcFile(pProcFile)
{
}

void ProcReader::Read( const String& pFileName )
{
    Reader::Read( pFileName );

    MustRead("mapProcFile003");
    
    while( 1 )
    {
        String section = ReadString();

        if( section == "model" )
        {
            mProcFile.mAreas.resize( mProcFile.mAreas.size() + 1 );
            ReadArea( mProcFile.mAreas[mProcFile.mAreas.size()-1] );
        }
        else if( section == "shadowModel" )
        {
            mProcFile.mShadowModels.resize( mProcFile.mShadowModels.size() + 1 );
            ReadShadowModel( mProcFile.mShadowModels[mProcFile.mShadowModels.size()-1] );
        }
        else if( section == "interAreaPortals" )
        {
            ReadPortals();
        }
        else if( section == "nodes" )
        {
            ReadNodes();
        }
        else
        {
            break;
        }
    }
}


void ProcReader::ReadArea( Area& pArea )
{
    MustRead( "{" );
    ReadStringQuote( pArea.mName );

    UInt32 numSurfaces;
    ReadUInt( numSurfaces );

    pArea.mSurfaces.resize( numSurfaces );

    Vector<Area::Surface>::iterator it;
    for( it = pArea.mSurfaces.begin(); it != pArea.mSurfaces.end(); ++it )
        ReadSurface( *it );

    MustRead( "}" );
}

void ProcReader::ReadSurface( Area::Surface& pSurface )
{
    MustRead( "{" );
    ReadStringQuote( pSurface.mName );

    UInt32 numVertices;
    UInt32 numIndices;

    ReadUInt( numVertices );
    ReadUInt( numIndices );

    pSurface.mVertices.resize( numVertices );
    pSurface.mIndices.resize( numIndices );

    Vector<Area::Surface::Vertex>::iterator itVertex;
    for( itVertex = pSurface.mVertices.begin(); itVertex != pSurface.mVertices.end(); ++itVertex )
    {
        MustRead( "(" );

        ReadFloat( itVertex->mPosition.x );
        ReadFloat( itVertex->mPosition.y );
        ReadFloat( itVertex->mPosition.z );

        ReadFloat( itVertex->mTexCoord.x );
        ReadFloat( itVertex->mTexCoord.y );

        ReadFloat( itVertex->mNormal.x );
        ReadFloat( itVertex->mNormal.y );
        ReadFloat( itVertex->mNormal.z );

        MustRead( ")" );
    }

    Vector<UInt32>::iterator itIndex;
    for( itIndex = pSurface.mIndices.begin(); itIndex != pSurface.mIndices.end(); ++itIndex )
    {
        ReadUInt( *itIndex );
    }

    MustRead( "}" );
}
    
void ProcReader::ReadPortals()
{
    MustRead( "{" );

    UInt32 numArea;
    UInt32 numPortals;

    ReadUInt( numArea );
    ReadUInt( numPortals );

    mProcFile.mPortals.resize( numPortals );
    
    Vector<Portal>::iterator it;
    for( it = mProcFile.mPortals.begin(); it != mProcFile.mPortals.end(); ++it )
    {
        UInt32 numPoints;
        ReadUInt( numPoints );
        ReadUInt( it->mPositiveSideArea );
        ReadUInt( it->mNegativeSideArea );

        it->mPoints.resize( numPoints );
        
        Vector<Vector3f>::iterator itPoint;
        for( itPoint = it->mPoints.begin(); itPoint != it->mPoints.end(); ++itPoint )
            ReadVector3( *itPoint );
    }

    MustRead( "}" );
}

void ProcReader::ReadNodes()
{
    MustRead( "{" );

    UInt32 numNodes;
    ReadUInt( numNodes );

    mProcFile.mNodes.resize( numNodes );

    Vector<Node>::iterator it;
    for( it = mProcFile.mNodes.begin(); it != mProcFile.mNodes.end(); ++it )
    {
        Float A,B,C,D;

        MustRead( "(" );
        ReadFloat( A );
        ReadFloat( B );
        ReadFloat( C );
        ReadFloat( D );
        MustRead( ")" );

        it->mPlane[0] = A;
        it->mPlane[1] = B;
        it->mPlane[2] = C;
        it->mPlane[3] = D;

        ReadInt( it->mPositiveChild );
        ReadInt( it->mNegativeChild );
    }
    
    MustRead( "}" );
}

void ProcReader::ReadShadowModel( ShadowModel& pShadowModel )
{
    MustRead( "{" );

    ReadStringQuote( pShadowModel.mName );
    
    UInt32  numVertices;
    UInt32  numIndices;

    ReadUInt( numVertices );
    ReadUInt( pShadowModel.mNumCaps );
    ReadUInt( pShadowModel.mNumFrontCaps );
    ReadUInt( numIndices );
    ReadUInt( pShadowModel.mPlaneBits );

    pShadowModel.mVertices.resize( numVertices );
    pShadowModel.mIndices.resize( numIndices );

    Vector<Vector3f>::iterator itVertex;
    for( itVertex = pShadowModel.mVertices.begin(); itVertex != pShadowModel.mVertices.end(); ++itVertex )
        ReadVector3( *itVertex );

    Vector<UInt32>::iterator itIndex;
    for( itIndex = pShadowModel.mIndices.begin(); itIndex != pShadowModel.mIndices.end(); ++itIndex )
        ReadUInt( *itIndex );

    MustRead( "}" );
}


///////////////////////////////////////////////////////////////////////////////
Reader::Reader() 
    : mMemoryFile(NULL)
{
}

Reader::~Reader()
{
    GD_DELETE(mMemoryFile);
}

void Reader::Read( const String& pFileName )
{
    const char whitespaces[] = { 10, 13, '\t', ' ', 0 };

    // Read all the file data into the string stream.
    mMemoryFile = GD_NEW(MemoryFile, this, "MD5Reader MemoryFile")( pFileName, true );
    mTokenizer.Init( (Char*)mMemoryFile->GetMemory(), mMemoryFile->GetSize() );
    mTokenizer.SetWhiteSpaces(whitespaces);
}

void Reader::MustRead( const String& pValue )
{
    String tmp;
    ReadString( tmp );
    GD_ASSERT( tmp == pValue );
}

const String& Reader::ReadStringQuote( String& pValue )
{
    SkipComments();

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
    SkipComments();
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
    SkipComments();
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
    SkipComments();
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
    SkipComments();
    mTokenizer >> pValue;
    return pValue;
}
UInt32 Reader::ReadUInt()
{
    UInt32 val;
    ReadUInt( val );
    return val;
}

const Vector3f& Reader::ReadVector3( Vector3f& pValue )
{
    MustRead("(");
    ReadFloat( pValue.x );
    ReadFloat( pValue.z );
    ReadFloat( pValue.y );    
    MustRead(")");

    pValue *= 0.05f;

    return pValue;
}

const Vector2f& Reader::ReadVector2( Vector2f& pValue )
{
    MustRead("(");
    ReadFloat( pValue.x );
    ReadFloat( pValue.y );
    MustRead(")");
    
    return pValue;
}

const Quaternionf& Reader::ReadQuaternion( Quaternionf& pQuaternion )
{
    MustRead("(");
    ReadFloat( pQuaternion.x );
    ReadFloat( pQuaternion.z );
    ReadFloat( pQuaternion.y );   
    MustRead(")");
   
    Float term = 1.0000f - ((pQuaternion.x*pQuaternion.x) + (pQuaternion.y*pQuaternion.y) + (pQuaternion.z*pQuaternion.z));
    pQuaternion.w = term < 0.0f ? 0.0f : Maths::Sqrt( term );
    
    return pQuaternion;
}

void Reader::SkipComments()
{
    EatWhite();

    static Char buf[512];
    
    UInt32 peek;

    if( mTokenizer.NextChar() == '/' )
    {
        mTokenizer.GetChar();
        peek = mTokenizer.NextChar();

        if( peek == '/' )
        {   
            strcpy_s(buf, 512, "");
            mTokenizer.GetLine( buf, 512 );
            SkipComments();
        }
        else if( peek == '*' )
        {
            mTokenizer.GetChar();
            
            mTokenizer.Ignore( Number<UInt32>::Max, '*' );
            while(1)
            {                
                peek = mTokenizer.NextChar();
                if( peek == '/' )
                {
                    mTokenizer.GetChar();
                    break;
                }
                else
                    mTokenizer.Ignore( Number<UInt32>::Max, '*' );
            }
            SkipComments();
        }
        else
        {
            mTokenizer.UngetChar();
        }
    }
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
    SkipComments();
    return mTokenizer.NextChar();
}

Char Reader::ReadChar()
{
    SkipComments();
    return mTokenizer.GetChar();
}

}
