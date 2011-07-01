/**
 *  @file       MDLReader.h
 *  @brief      Read a MDL file into it's corresponding data structures
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
#ifndef     _MDL_READER_H_
#define     _MDL_READER_H_


#include "MDLModel.h"


/**
 *  File reader used to parse the MDL file and fill a NWN::Model object with it's content.
 */
class MDLReader
{
public:
    MDLReader();

    void Read( const String& pFileName, NWN::Model& pModel );

private:
    NWN::GeometryNode*      ReadGeometryNodes();
    NWN::GeometryNode*      ReadGeometryNode( const std::map<String,NWN::GeometryNode*>& pGeometryNodes );
    NWN::ModelAnimation*    ReadModelAnimation();
    NWN::AnimationNode*     ReadAnimationNode();

    void            ReadVertexList( Vector<Vector3f>& pList, UInt32 pCount );
    void            ReadFaceList( Vector<NWN::Face>& pList, UInt32 pCount );
    void            ReadTexCoordList( Vector<Vector2f>& pList, UInt32 pCount );
    void            ReadColorList( Vector<Color4f>& pList, UInt32 pCount );
    void            ReadConstraintList( Vector<Float>& pList, UInt32 pCount );
    void            ReadAabbList( NWN::AabbGeometryNode& pList );

    void            ReadPositionKeys( NWN::AnimationNode* pAnimatioNode );
    void            ReadOrientationKeys( NWN::AnimationNode* pAnimatioNode );
    void            ReadBirthrateKeys( NWN::AnimationEmitterNode* pAnimatioEmitterNode );

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
    const Color4f&  ReadColor( Color4f& pValue );
    
    UInt32          ReadSymbol();
    void            ReadSymbol( UInt32 pSymbolWanted );    

    // Error detection
    void UnexpectedSymbol() const;
    void MatchName( const String& pWanted, const String& pGiven ) const;

    void IgnoreSymbol( UInt32 pSymbol );

    String GetSymbolString( UInt32 pSymbol ) const;

    void SkipComments();
    void EatWhite( Bool pEOLIsWhite = true );

    void InitializeSymbolTable();
    
private:
    MDLReader( const MDLReader& pOther );
    const MDLReader& operator = ( const MDLReader& pOther );

private:
    UInt32                          mCurrentSymbol;
    String                          mCurrentSymbolString;

    std::stringstream               mStringStream;

    static Bool                     mSymbolTableInitialized;
    static std::map<String,UInt32>  mSymbolTable;
};


#endif  //  _MDL_READER_H_
