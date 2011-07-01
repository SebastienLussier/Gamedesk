/**
 *  @file       MDLReader.cpp
 *  @brief      Read a MDL file into it's corresponding data structures.
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
#include "MDLImporter.h"
#include "MDLReader.h"

#include "Maths/Number.h"
#include "Resource/ResourceManager.h"


Bool                    MDLReader::mSymbolTableInitialized = false;
std::map<String,UInt32> MDLReader::mSymbolTable;


MDLReader::MDLReader()
{
    InitializeSymbolTable();
}

void MDLReader::Read( const String& pFileName, NWN::Model& pModel )
{
    // Read all the file data into the string stream.
    std::ifstream file;
    file.open( pFileName.c_str() );
    mStringStream << file.rdbuf(); 

    // Skip all dependancies
    UInt32 symbol;
    while( true )
    {
        symbol = ReadSymbol();

        if( symbol == NWN::FILEDEPENDANCY_ID )
            ReadString();
        else
            break;
    }

    if( mCurrentSymbol != NWN::NEWMODEL_ID )
        throw ResourceImportException( String("Expected symbol \"") + GetSymbolString(NWN::NEWMODEL_ID) + String("\" Given \"") + mCurrentSymbolString + String("\""), Here );
    
    ReadString( pModel.mName );
    ToLower( pModel.mName );

    String tmpName;

    Bool done = false;
    while( !done )
    {
        switch( ReadSymbol() )
        {
        case NWN::DONEMODEL_ID:
            ReadString( tmpName );
            MatchName( pModel.mName, ToLower( tmpName ) );
            done = true;
            break;
        
        case NWN::SETSUPERMODEL_ID:
            ReadString( tmpName );
            MatchName( pModel.mName, ToLower( tmpName ) );
            ReadString( pModel.mSuperModelName );
            ToLower( pModel.mSuperModelName );
            break;
        
        case NWN::SETANIMATIONSCALE_ID:
            ReadFloat( pModel.mAnimationScale );
            break;
        
        case NWN::CLASSIFICATION_ID:
            ReadString( pModel.mClassification );
            break;
        
        case NWN::BEGINMODELGEOM_ID:
            pModel.mGeometry = ReadGeometryNodes();
            break;

        case NWN::NEWANIM_ID:
            pModel.AddAnimation( ReadModelAnimation() );
            break;
        
        default:
            UnexpectedSymbol();
        }
    }        
}

NWN::GeometryNode* MDLReader::ReadGeometryNodes()
{
    NWN::GeometryNode*              rootNode = NULL;
    NWN::GeometryNode*              newNode  = NULL;
    String                          name;
    std::map<String, NWN::GeometryNode*> geometryNodes;

    name = ReadString();

    Bool done = false;
    while( !done )
    {
        switch( ReadSymbol() )
        {
        case NWN::ENDMODELGEOM_ID:
            MatchName( name, ReadString() );
            done = true;
            break;

        case NWN::NODE_ID:
            newNode = ReadGeometryNode( geometryNodes );
            geometryNodes[ToLower(newNode->mName)] = newNode;

            if( newNode->GetSuperNode() == NULL )
                rootNode = newNode;
            break;

        default:
            UnexpectedSymbol();
        }
    }
    
    return rootNode;
}

NWN::GeometryNode* MDLReader::ReadGeometryNode( const std::map<String, NWN::GeometryNode*>& pGeometryNodes )
{
    NWN::GeometryNode*              parentNode      = NULL;
    NWN::DummyGeometryNode*         dummyNode       = NULL;
    NWN::TrimeshGeometryNode*       trimeshNode     = NULL;
    NWN::EmitterNode*               emitterNode     = NULL;
    NWN::DanglyMeshGeometryNode*    danglyMeshNode  = NULL;
    NWN::AabbGeometryNode*          aabbNode        = NULL;

    Bool    ignoring = false;

    UInt32 type = ReadSymbol();
    String name = ReadString();
    ToLower(name);

    ReadSymbol(NWN::PARENT_ID);
        
    String parentName;
    ReadString( parentName );    
    ToLower( parentName );

    if( parentName != "null" )
    {
        std::map<String,NWN::GeometryNode*>::const_iterator itMap = pGeometryNodes.find( parentName );
        if( itMap != pGeometryNodes.end() )
            parentNode = itMap->second;

        if( !parentNode )
            throw ResourceImportException( String("Could not find parent node \"") + parentName + String("\""), Here );
    }

    switch( type )
    {
        case NWN::TRIMESH_ID:
        case NWN::SKIN_ID:
            trimeshNode = new NWN::TrimeshGeometryNode(parentNode);
            dummyNode = trimeshNode;
            break;
    
        case NWN::DANGLY_MESH_ID:
            danglyMeshNode = new NWN::DanglyMeshGeometryNode(parentNode);
            trimeshNode = danglyMeshNode;
            dummyNode = trimeshNode;
            break;
    
        case NWN::AABB_ID:
            aabbNode = new NWN::AabbGeometryNode(parentNode);
            trimeshNode = aabbNode;
            dummyNode = aabbNode;
            break;
        
        case NWN::DUMMY_ID:
        case NWN::PATCH_ID:
            dummyNode = new NWN::DummyGeometryNode(parentNode);
            break;
                    
        case NWN::EMITTER_ID:
            emitterNode = new NWN::EmitterNode(parentNode);
            dummyNode = emitterNode;
            break;
        
        case NWN::REFERENCE_ID:
        case NWN::LIGHT_ID:
            dummyNode = new NWN::DummyGeometryNode(parentNode);
            ignoring = true;
            break;  
         
        default:
            throw ResourceImportException( String("Unsupported node (type=\"") + GetSymbolString(type) + String("\")"), Here );
    }
    
    dummyNode->mName = name;
    if( parentNode != NULL )
    {
        parentNode->AddChild( dummyNode );
    }
   
    Bool done = false;
    while( !done )
    {
        UInt32 symbol = ReadSymbol();

        switch( symbol )
        {
        case NWN::ENDNODE_ID:          done = true;                            break;
        case NWN::POSITION_ID:         ReadVector3( dummyNode->mPosition );    break;
        case NWN::ORIENTATION_ID:      ReadVector3( dummyNode->mRotationVector ); 
                                       ReadFloat( dummyNode->mRotationAngle ); break;
        case NWN::WIRECOLOR_ID:        ReadColor( dummyNode->mWirecolor );     break;
        case NWN::SCALE_ID:            ReadFloat( dummyNode->mScale );         break;
        default:

            if( trimeshNode != NULL )
            {
                switch( symbol )
                {
                case NWN::AMBIENT_ID:          ReadColor( trimeshNode->mAmbient );                     break;
                case NWN::DIFFUSE_ID:          ReadColor( trimeshNode->mDiffuse );                     break;
                case NWN::SPECULAR_ID:         ReadColor( trimeshNode->mSpecular );                    break;
                case NWN::SHININESS_ID:        ReadFloat( trimeshNode->mShininess );                   break;
                case NWN::SELFILLUMCOLOR_ID:   ReadColor( trimeshNode->mEmissive );                    break;
                case NWN::BITMAP_ID:           ReadString( trimeshNode->mBitmap );                     break;
                case NWN::ALPHA_ID:            ReadFloat( trimeshNode->mAlpha );                       break;
                case NWN::TILEFADE_ID:         ReadFloat( trimeshNode->mTileFade );                    break;
                case NWN::RENDER_ID:           ReadInt( ((Int32&)trimeshNode->mRender) );              break;
                case NWN::TRANSPARENCY_HINT_ID:ReadInt( ((Int32&)trimeshNode->mTransparencyHint) );    break;
                case NWN::VERTS_ID:            ReadVertexList( trimeshNode->mVerts, ReadInt() );       break;
                case NWN::FACES_ID:            ReadFaceList( trimeshNode->mFaces, ReadInt() );         break;
                case NWN::TVERTS_ID:           ReadTexCoordList( trimeshNode->mTexCoords, ReadInt() ); break;
                case NWN::COLORS_ID:           ReadColorList( trimeshNode->mColors, ReadInt() );       break;
                default:
                    if( danglyMeshNode != NULL )
                    {
                        switch( symbol )
                        {
                        case NWN::CONSTRAINTS_ID: ReadConstraintList( danglyMeshNode->mConstraints, ReadInt() );
                        default:
                            IgnoreSymbol( symbol );
                        }
                    }
                    else if( aabbNode != NULL )
                    {
                        switch( symbol )        
                        {
                        case NWN::AABB_ID:     ReadAabbList( *aabbNode );          break;
                        default:
                            IgnoreSymbol( symbol );
                        }
                    }
                    else
                    {
                        IgnoreSymbol( symbol );
                    }
                }
            }
            else if( emitterNode != NULL )
            {
                switch( symbol )
                {
                case NWN::COLORSTART_ID:   ReadColor( emitterNode->mColorStart );  break;
                case NWN::COLOREND_ID:     ReadColor( emitterNode->mColorEnd );    break;
                case NWN::ALPHASTART_ID:   ReadFloat( emitterNode->mAlphaStart );  break;
                case NWN::ALPHAEND_ID:     ReadFloat( emitterNode->mAlphaEnd );    break;
                case NWN::SIZESTART_ID:    ReadFloat( emitterNode->mSizeStart );   break;
                case NWN::SIZEEND_ID:      ReadFloat( emitterNode->mSizeEnd );     break;
                case NWN::SIZESTART_Y_ID:  ReadFloat( emitterNode->mSizeStartY );  break;
                case NWN::SIZESEND_Y_ID:   ReadFloat( emitterNode->mSizeEndY );    break;
                case NWN::FRAMESTART_ID:   ReadInt( emitterNode->mFrameStart );    break;
                case NWN::FRAMEEND_ID:     ReadInt( emitterNode->mFrameEnd );      break;
                case NWN::BIRTHRATE_ID:    ReadFloat( emitterNode->mBirthrate );   break;
                case NWN::LIFEEXP_ID:      ReadFloat( emitterNode->mLifeExp );     break;
                case NWN::MASS_ID:         ReadFloat( emitterNode->mMass );        break;
                case NWN::SPREAD_ID:       ReadFloat( emitterNode->mSpread );      break;
                case NWN::VELOCITY_ID:     ReadFloat( emitterNode->mVelocity );    break;
                case NWN::FPS_ID:          ReadFloat( emitterNode->mFps );         break;
                case NWN::RANDVEL_ID:      ReadFloat( emitterNode->mRandVel );     break;
                case NWN::RENDER_ID:       ReadString( emitterNode->mRender );     break;
                case NWN::BLEND_ID:        ReadString( emitterNode->mBlend );      break;
                case NWN::TEXTURE_ID:      ReadString( emitterNode->mTexture );    break;
                case NWN::XGRID_ID:        ReadInt( emitterNode->mXGrid );         break;
                case NWN::YGRID_ID:        ReadInt( emitterNode->mYGrid );         break;
                case NWN::PARTICLEROT_ID:  ReadFloat( emitterNode->mParticleRot ); break;
                case NWN::XSIZE_ID:        ReadFloat( emitterNode->mXSize );       break;
                case NWN::YSIZE_ID:        ReadFloat( emitterNode->mYSize );       break;
                case NWN::P2P_TYPE_ID:     ReadString( emitterNode->mP2PType );    break;
                case NWN::DRAG_ID:         ReadFloat( emitterNode->mDrag );        break;
                case NWN::GRAV_ID:         ReadFloat( emitterNode->mGrav );        break;
                case NWN::INHERIT_ID:      ReadInt( emitterNode->mInherit );       break;
                case NWN::INHERIT_LOCAL_ID:ReadInt( emitterNode->mInheritLocal );  break;
                case NWN::INHERIT_PART_ID: ReadInt( emitterNode->mInheritPart );   break;
                case NWN::UPDATE_ID:       ReadString( emitterNode->mUpdate );     break;
                default:
                    IgnoreSymbol( symbol );
                }
            }
            else if( ignoring )
            {
                IgnoreSymbol( symbol );
            }
            else
            {
                UnexpectedSymbol();
            }
        }
    }

    return dummyNode;
}

NWN::ModelAnimation* MDLReader::ReadModelAnimation()
{
    String name = ReadString();
    String master = ReadString();
    NWN::ModelAnimation* anim = new NWN::ModelAnimation( name, master );

    ReadSymbol( NWN::LENGTH_ID );
    ReadFloat( anim->mLength );

    ReadSymbol( NWN::TRANSTIME_ID);
    ReadFloat( anim->mBlendTime );
    
    Bool done = false;
    while( !done )
    {        
        switch( ReadSymbol() )
        {
        case NWN::DONEANIM_ID:
            ReadString();
            ReadString();
            done = true;
            break;

        case NWN::ANIMROOT_ID:
            ReadString();
            break;
           
        case NWN::NODE_ID:
            anim->AddAnimationNode( ReadAnimationNode() );
            break;

        case NWN::EVENT_ID:
            ReadFloat();
            ReadString();
            break;
            
        default:
            UnexpectedSymbol();
        }
    }

    return anim;
}

NWN::AnimationNode* MDLReader::ReadAnimationNode()
{
    NWN::AnimationNode*         animationNode = NULL;
    NWN::AnimationEmitterNode*  animationEmitterNode = NULL;
    
    switch( ReadSymbol() )
    {
    case NWN::EMITTER_ID:
        animationEmitterNode = new NWN::AnimationEmitterNode( ReadString() );
        animationNode = animationEmitterNode;
        break;
    
    case NWN::DUMMY_ID:
    case NWN::TRIMESH_ID:
        animationNode = new NWN::AnimationNode( ReadString() );
        break;

    default:
        Core::DebugOut( "MDLReader: Unknown animation type !\n" );
        animationNode = new NWN::AnimationNode( ReadString() );
    }

    ReadSymbol( NWN::PARENT_ID );
    ReadString( animationNode->mParent );

    Bool done = false;
    while( !done )
    {
        UInt32 symbol = ReadSymbol();
        switch( symbol )
        {
        case NWN::ENDNODE_ID:
            done = true;
            break;
        
        case NWN::POSITIONKEY_ID:
            ReadPositionKeys( animationNode );
            break;
        
        case NWN::ORIENTATIONKEY_ID:
            ReadOrientationKeys(animationNode);
            break;
        
        default:
            if( animationEmitterNode && symbol == NWN::BIRTHRATEKEY_ID )
                ReadBirthrateKeys( animationEmitterNode );
            else
                IgnoreSymbol( symbol );
        }
    }

    return animationNode;
}

void MDLReader::ReadVertexList( Vector<Vector3f>& pList, UInt32 pCount )
{
    pList.reserve( pCount );
    Vector3f point;
    for( UInt32 iVertex = 0; iVertex < pCount; iVertex++ )
        pList.push_back( ReadVector3(point) );
}

void MDLReader::ReadFaceList( Vector<NWN::Face>& pList, UInt32 pCount )
{
    NWN::Face face;
    pList.reserve( pCount );

    for( UInt32 iFace = 0; iFace < pCount; iFace++ )
    {
        ReadUInt( face.mV1 );
        ReadUInt( face.mV3 );
        ReadUInt( face.mV2 );        
        ReadUInt( face.mM1 );
        ReadUInt( face.mT1 );
        ReadUInt( face.mT3 );
        ReadUInt( face.mT2 );        
        ReadUInt( face.mM2 );        
        pList.push_back( face );
    }
}

void MDLReader::ReadTexCoordList( Vector<Vector2f>& pList, UInt32 pCount )
{
    pList.reserve( pCount );
    Vector2f coord;
    for( UInt32 iCoord = 0; iCoord < pCount; iCoord++ )
    {
        pList.push_back( ReadVector2(coord) );
        ReadFloat();    // Ignore Z
    }
}

void MDLReader::ReadColorList( Vector<Color4f>& pList, UInt32 pCount )
{
    pList.reserve( pCount );
    Color4f color;
    for( UInt32 iColor = 0; iColor < pCount; iColor++ )
        pList.push_back( ReadColor(color) );
}

void MDLReader::ReadConstraintList( Vector<Float>& pList, UInt32 pCount )
{
    pList.reserve( pCount );
    for( UInt32 iConstraint = 0; iConstraint < pCount; iConstraint++ )
        pList.push_back( ReadFloat() );
}

void MDLReader::ReadAabbList( NWN::AabbGeometryNode& node )
{
    Float x1;
    Float y1;
    Float z1;
    Float x2;
    Float y2;
    Float z2;
    Int32 i;

    EatWhite();
    while( !IsLetter( mStringStream.peek() ) )
    {
        x1 = ReadFloat();
        z1 = ReadFloat();
        y1 = ReadFloat();        
        x2 = ReadFloat();
        z2 = ReadFloat();
        y2 = ReadFloat();        
        i  = ReadInt();

        node.AddAabbEntry( x1, y1, z1, x2, y2, z2, i );

        EatWhite();
    }
}


void MDLReader::ReadPositionKeys( NWN::AnimationNode* pAnimatioNode )
{
    Float       time;
    Vector3f    position;
    UInt32      count;

    EatWhite( false );
    if( mStringStream.peek() != '\n' )
    {
        ReadUInt( count );
     
        // Read the given number of keys.
        for( UInt32 i = 0; i < count; i++ )
        {
            ReadFloat( time );
            ReadVector3( position );
            pAnimatioNode->AddPositionKey( time, position );
        }
    }
    else
    {
        // Read until we find a character.
        while( true )
        {
            EatWhite();

            if( IsLetter( mStringStream.peek() ) )
                break;                
            
            ReadFloat( time );
            ReadVector3( position );
            pAnimatioNode->AddPositionKey( time, position );
        }

        ReadSymbol( NWN::ENDLIST_ID );
    }
}


void MDLReader::ReadOrientationKeys( NWN::AnimationNode* pAnimatioNode )
{
    Float       time;
    Vector3f    axis;
    Float       angle;
    UInt32      count;

    EatWhite( false );
    if( mStringStream.peek() != '\n' )
    {
        ReadUInt( count );
     
        // Read the given number of keys.
        for( UInt32 i = 0; i < count; i++ )
        {
            ReadFloat( time );
            ReadVector3( axis );
            ReadFloat( angle );
            pAnimatioNode->AddOrientationKey( time, axis, angle );
        }
    }
    else    
    {
        // Read until we find a character.
        while( true )
        {
            EatWhite();

            if( IsLetter( mStringStream.peek() ) )
                break;
            
            ReadFloat( time );
            ReadVector3( axis );
            ReadFloat( angle );
            pAnimatioNode->AddOrientationKey( time, axis, angle );
        }

        ReadSymbol( NWN::ENDLIST_ID );
    }
}

void MDLReader::ReadBirthrateKeys( NWN::AnimationEmitterNode* pAnimatioEmitterNode )
{
    Float       time;
    Float       birthrate;
    UInt32      count;

    EatWhite( false );
    if( mStringStream.peek() != '\n' )
    {
        ReadUInt( count );
     
        // Read the given number of keys.
        for( UInt32 i = 0; i < count; i++ )
        {
            ReadFloat( time );
            ReadFloat( birthrate );
            pAnimatioEmitterNode->AddBirthrateKey( time, birthrate );
        }
    }
    else
    {
        // Read until we find a character.
        while( true )
        {
            EatWhite();

            if( IsLetter( mStringStream.peek() ) )
                break;

            ReadFloat( time );
            ReadFloat( birthrate );
            pAnimatioEmitterNode->AddBirthrateKey( time, birthrate );
        }

        ReadSymbol( NWN::ENDLIST_ID );
    }    
}

const String& MDLReader::ReadString( String& pValue )
{
    SkipComments();
    mStringStream >> pValue;
    //Core::DebugOut( "MDLReader: %s\n", pValue.c_str() );
    return pValue;
}

String MDLReader::ReadString()
{
    String val;
    ReadString( val );
    return val;
}

const Float& MDLReader::ReadFloat( Float& pValue )
{
    SkipComments();
    mStringStream >> pValue;
    return pValue;
}

Float MDLReader::ReadFloat()
{
    Float val;
    ReadFloat( val );
    return val;
}

const Int32& MDLReader::ReadInt( Int32& pValue )
{
    SkipComments();
    mStringStream >> pValue;
    return pValue;
}
Int32 MDLReader::ReadInt()
{
    Int32 val;
    ReadInt( val );
    return val;
}

const UInt32& MDLReader::ReadUInt( UInt32& pValue )
{
    SkipComments();
    mStringStream >> pValue;
    return pValue;
}
UInt32 MDLReader::ReadUInt()
{
    UInt32 val;
    ReadUInt( val );
    return val;
}

const Vector3f& MDLReader::ReadVector3( Vector3f& pValue )
{
    ReadFloat( pValue.x );
    ReadFloat( pValue.z );
    ReadFloat( pValue.y );    
    return pValue;
}

const Vector2f& MDLReader::ReadVector2( Vector2f& pValue )
{
    ReadFloat( pValue.x );
    ReadFloat( pValue.y );
    return pValue;
}

const Color4f& MDLReader::ReadColor( Color4f& pValue )
{
    ReadFloat( pValue.R );
    ReadFloat( pValue.G );
    ReadFloat( pValue.B );
    pValue.A = 1.0f;
    return pValue;
}

UInt32 MDLReader::ReadSymbol()
{
    ReadString( mCurrentSymbolString );

    mCurrentSymbol = NWN::UNKNOWN_ID;
    std::map<String,UInt32>::const_iterator itMap = mSymbolTable.find(mCurrentSymbolString);
    if( itMap != mSymbolTable.end() )
        mCurrentSymbol = itMap->second;

    return mCurrentSymbol;
}

void MDLReader::ReadSymbol( UInt32 pSymbolWanted )
{
    ReadSymbol();
    if( mCurrentSymbol != pSymbolWanted )
        throw ResourceImportException( String("Expected symbol \"") + GetSymbolString(pSymbolWanted) + String("\" Given \"") + mCurrentSymbolString + String("\""), Here );
}

void MDLReader::UnexpectedSymbol() const
{
    throw ResourceImportException( String("Unexpected symbol \"") + mCurrentSymbolString + String("\""), Here );
}

void MDLReader::MatchName( const String& pWanted, const String& pGiven ) const
{
    if( pGiven != pWanted )
        throw ResourceImportException( String("Expected string \"") + pWanted + String("\" Given \"") + pGiven + String("\""), Here );
}

void MDLReader::IgnoreSymbol( UInt32 /*pSymbol*/ )
{
    mStringStream.ignore( Number<Int32>::Max, '\n' );
    //Core::DebugOut( "MDLReader: Ignoring symbol %s (%ul)\n", GetSymbolString(pSymbol).c_str(), pSymbol );
}

String MDLReader::GetSymbolString( UInt32 pSymbol ) const
{
    for( std::map<String,UInt32>::const_iterator itMap = mSymbolTable.begin(); itMap != mSymbolTable.end(); ++itMap )
    {
        if( itMap->second == pSymbol )
            return itMap->first;
    }

    return "Unknown Symbol";
}    

void MDLReader::SkipComments()
{
    EatWhite();
    
    if( mStringStream.peek() == '#' )
    {
        mStringStream.ignore( Number<Int32>::Max, '\n' );
        SkipComments();
    }
    else
    {
        mStringStream.unget();
    }
}

void MDLReader::EatWhite( Bool pEOLIsWhite )
{
    UInt32 read;
    
    if( pEOLIsWhite )
    {
        do 
        {
            read = mStringStream.get();
        } while( read == '\n' || read == ' ' || read == '\t' );
    }
    else
    {
        do 
        {
            read = mStringStream.get();
        } while( read == ' ' || read == '\t' );
    }

    mStringStream.unget();
}

void MDLReader::InitializeSymbolTable()
{
    if( mSymbolTableInitialized )
        return;

    mSymbolTable[NWN::FILEDEPENDANCY]    = NWN::FILEDEPENDANCY_ID;
    mSymbolTable[NWN::NEWMODEL]          = NWN::NEWMODEL_ID;
    mSymbolTable[NWN::DONEMODEL]         = NWN::DONEMODEL_ID;
    mSymbolTable[NWN::SETSUPERMODEL]     = NWN::SETSUPERMODEL_ID;
    mSymbolTable[NWN::CLASSIFICATION]    = NWN::CLASSIFICATION_ID;
    mSymbolTable[NWN::SETANIMATIONSCALE] = NWN::SETANIMATIONSCALE_ID;
    mSymbolTable[NWN::BEGINMODELGEOM]    = NWN::BEGINMODELGEOM_ID;
    mSymbolTable[NWN::ENDMODELGEOM]      = NWN::ENDMODELGEOM_ID;
    mSymbolTable[NWN::NEWANIM]           = NWN::NEWANIM_ID;
    mSymbolTable[NWN::DONEANIM]          = NWN::DONEANIM_ID;
    mSymbolTable[NWN::NODE]              = NWN::NODE_ID;
    mSymbolTable[NWN::ENDNODE]           = NWN::ENDNODE_ID;
    mSymbolTable[NWN::BITMAP]            = NWN::BITMAP_ID;
    mSymbolTable[NWN::VERTS]             = NWN::VERTS_ID;
    mSymbolTable[NWN::FACES]             = NWN::FACES_ID;
    mSymbolTable[NWN::TVERTS]            = NWN::TVERTS_ID;
    mSymbolTable[NWN::PARENT]            = NWN::PARENT_ID;
    mSymbolTable[NWN::POSITION]          = NWN::POSITION_ID;
    mSymbolTable[NWN::ORIENTATION]       = NWN::ORIENTATION_ID;
    mSymbolTable[NWN::WIRECOLOR]         = NWN::WIRECOLOR_ID;
    mSymbolTable[NWN::AMBIENT]           = NWN::AMBIENT_ID;
    mSymbolTable[NWN::DIFFUSE]           = NWN::DIFFUSE_ID;
    mSymbolTable[NWN::SPECULAR]          = NWN::SPECULAR_ID;
    mSymbolTable[NWN::SHININESS]         = NWN::SHININESS_ID;
    mSymbolTable[NWN::LENGTH]            = NWN::LENGTH_ID;
    mSymbolTable[NWN::TRANSTIME]         = NWN::TRANSTIME_ID;
    mSymbolTable[NWN::ANIMROOT]          = NWN::ANIMROOT_ID;
    mSymbolTable[NWN::POSITIONKEY]       = NWN::POSITIONKEY_ID;
    mSymbolTable[NWN::ORIENTATIONKEY]    = NWN::ORIENTATIONKEY_ID;
    mSymbolTable[NWN::ENDLIST]           = NWN::ENDLIST_ID;
    mSymbolTable[NWN::EVENT]             = NWN::EVENT_ID;
    mSymbolTable[NWN::COLORS]            = NWN::COLORS_ID;
    mSymbolTable[NWN::SCALE]             = NWN::SCALE_ID;
    mSymbolTable[NWN::SELFILLUMCOLOR]    = NWN::SELFILLUMCOLOR_ID;
    mSymbolTable[NWN::ALPHA]             = NWN::ALPHA_ID;
    mSymbolTable[NWN::TRANSPARENCY_HINT] = NWN::TRANSPARENCY_HINT_ID;
    mSymbolTable[NWN::CONSTRAINTS]       = NWN::CONSTRAINTS_ID;
    mSymbolTable[NWN::COLORSTART]        = NWN::COLORSTART_ID;
    mSymbolTable[NWN::COLOREND]          = NWN::COLOREND_ID;
    mSymbolTable[NWN::ALPHASTART]        = NWN::ALPHASTART_ID;
    mSymbolTable[NWN::ALPHAEND]          = NWN::ALPHAEND_ID;
    mSymbolTable[NWN::SIZESTART]         = NWN::SIZESTART_ID;
    mSymbolTable[NWN::SIZEEND]           = NWN::SIZEEND_ID;
    mSymbolTable[NWN::SIZESTART_Y]       = NWN::SIZESTART_Y_ID;
    mSymbolTable[NWN::SIZESEND_Y]        = NWN::SIZESEND_Y_ID;
    mSymbolTable[NWN::FRAMESTART]        = NWN::FRAMESTART_ID;
    mSymbolTable[NWN::FRAMEEND]          = NWN::FRAMEEND_ID;
    mSymbolTable[NWN::BIRTHRATE]         = NWN::BIRTHRATE_ID;
    mSymbolTable[NWN::LIFEEXP]           = NWN::LIFEEXP_ID;
    mSymbolTable[NWN::MASS]              = NWN::MASS_ID;
    mSymbolTable[NWN::SPREAD]            = NWN::SPREAD_ID;
    mSymbolTable[NWN::VELOCITY]          = NWN::VELOCITY_ID;
    mSymbolTable[NWN::RANDVEL]           = NWN::RANDVEL_ID;
    mSymbolTable[NWN::FPS]               = NWN::FPS_ID;
    mSymbolTable[NWN::BLEND]             = NWN::BLEND_ID;
    mSymbolTable[NWN::TEXTURE]           = NWN::TEXTURE_ID;
    mSymbolTable[NWN::XGRID]             = NWN::XGRID_ID;
    mSymbolTable[NWN::YGRID]             = NWN::YGRID_ID;
    mSymbolTable[NWN::PARTICLEROT]       = NWN::PARTICLEROT_ID;
    mSymbolTable[NWN::XSIZE]             = NWN::XSIZE_ID;
    mSymbolTable[NWN::YSIZE]             = NWN::YSIZE_ID;
    mSymbolTable[NWN::RENDER]            = NWN::RENDER_ID;
    mSymbolTable[NWN::P2P_TYPE]          = NWN::P2P_TYPE_ID;
    mSymbolTable[NWN::DRAG]              = NWN::DRAG_ID;
    mSymbolTable[NWN::GRAV]              = NWN::GRAV_ID;
    mSymbolTable[NWN::INHERIT]           = NWN::INHERIT_ID;
    mSymbolTable[NWN::INHERIT_LOCAL]     = NWN::INHERIT_LOCAL_ID;
    mSymbolTable[NWN::INHERIT_PART]      = NWN::INHERIT_PART_ID;
    mSymbolTable[NWN::UPDATE]            = NWN::UPDATE_ID;
    mSymbolTable[NWN::BIRTHRATEKEY]      = NWN::BIRTHRATEKEY_ID;
    mSymbolTable[NWN::TILEFADE]          = NWN::TILEFADE_ID;

    mSymbolTable[NWN::TRIMESH]           = NWN::TRIMESH_ID;
    mSymbolTable[NWN::DANGLY_MESH]       = NWN::DANGLY_MESH_ID;
    mSymbolTable[NWN::AABB]              = NWN::AABB_ID;
    mSymbolTable[NWN::DUMMY]             = NWN::DUMMY_ID;
    mSymbolTable[NWN::PATCH]             = NWN::PATCH_ID;
    mSymbolTable[NWN::EMITTER]           = NWN::EMITTER_ID;
    mSymbolTable[NWN::REFERENCE]         = NWN::REFERENCE_ID;
    mSymbolTable[NWN::LIGHT]             = NWN::LIGHT_ID;
    mSymbolTable[NWN::SKIN]              = NWN::SKIN_ID;

    mSymbolTableInitialized = true;
}