/**
 *  @file       LWOReader.cpp
 *  @brief      Lightwave model reader.
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
#include "LWOImpExp.h"
#include "LWOReader.h"

#include "LWOTags.h"
#include "FileManager/FileManager.h"


LWOReader::LWOReader() :
    mObject(NULL)
{
}

void LWOReader::Read( const String& pFilename, LWObject& pObject )
{
    UInt32 dataSize     = 0;
    UInt32 dataRead     = 0;
    UInt32 chunkSize    = 0;
    UInt32 chunkTag     = 0;

    mObject = &pObject;

    mLwoFile.open( pFilename.c_str() );
    if( mLwoFile.fail() )
    {
        mLwoFile.close();
        throw FileNotFoundException( pFilename, Here );
    }

    try
    {
        if( ReadChunk( dataSize ) != ID_FORM )
            throw InvalidLWOFileException( ID_FORM, Here );

        if( ReadID4() != ID_LWO2 )
            throw InvalidLWOFileException( ID_LWO2, Here );
        
        dataRead += sizeof(ID_LWO2);
        while( dataRead < dataSize )
        {            
            chunkTag = ReadChunk(chunkSize);
            dataRead += sizeof(UInt32);     // Chunk tag.
            dataRead += sizeof(UInt32);     // Chunk size.

            switch( chunkTag )
            {
            case ID_TAGS:	    ReadTAGS( chunkSize );  break;
            case ID_CLIP:	    ReadCLIP( chunkSize );  break;
            case ID_ENVL:	    ReadENVL( chunkSize );  break;
            case ID_LAYR:	    ReadLAYR( chunkSize );  break;
            case ID_PNTS:	    ReadPNTS( chunkSize );  break;
            case ID_BBOX:	    ReadBBOX( chunkSize );  break;
            case ID_POLS:	    ReadPOLS( chunkSize );  break;
            case ID_PTAG:	    ReadPTAG( chunkSize );  break;
            case ID_VMAP:	    
            case ID_VMAD:	    ReadVMAP( chunkSize, chunkTag == ID_VMAD );  break;
            case ID_SURF:	    ReadSURF( chunkSize );  break;
            default:            Skip( chunkSize );      break;
            }
            
            dataRead += chunkSize;          // Chunk bytes.
        }        
    }
    catch( Exception& /*e*/ )
    {
        mLwoFile.close();
        throw;
    }
    
    // Fill data structure
    mLwoFile.close();

    LWLayer*                    layer;
    Vector<LWLayer*>::iterator  itLayer;
    for( itLayer = mObject->mLayers.begin(); itLayer != mObject->mLayers.end(); ++itLayer )
    {
        layer = (*itLayer);

        mObject->GetBoundingBox     ( layer->mPointList, layer->mBoundingBox );
        mObject->GetPolyNormals     ( layer->mPointList, layer->mPolygonList );
        mObject->GetPointPolygons   ( layer->mPointList, layer->mPolygonList );
        mObject->ResolvePolySurfaces( layer->mPolygonList );
        mObject->GetVertNormals     ( layer->mPointList, layer->mPolygonList );
        mObject->GetPointVMaps      ( layer->mPointList, layer->mVertexMaps );
        mObject->GetPolyVMaps       ( layer->mPolygonList, layer->mVertexMaps );
    }

    mObject = NULL;
}

LWLayer* LWOReader::GetCurrentLayer()
{
    if( mObject->mLayers.size() == 0 )
        mObject->mLayers.push_back( GD_NEW(LWLayer, this, "Layer") );

    return mObject->mLayers.back();
}

void LWOReader::ReadTAGS( UInt32 pChunkSize )
{
    UInt32  dataRead = 0;
    String  newTag;

    while( dataRead < pChunkSize )
    {
        newTag = "";
        dataRead += ReadS0( newTag );
        mObject->mTags.push_back( newTag );
    }
}

void LWOReader::ReadCLIP( UInt32 pChunkSize )
{
    UInt32  dataRead = 0;
    UInt32  subChunkTag;
    UInt16  subChunkSize;
    UInt32  bytesHold;    

    LWClip* newClip = GD_NEW(LWClip, this, "Clip");

    dataRead += ReadU4( newClip->mIndex );

    // Read all subchunks
    while( dataRead < pChunkSize )
    {
        if( (pChunkSize - dataRead) < 6 )
        {
            dataRead += Skip( pChunkSize - dataRead );
            return;
	    }

        subChunkTag = ReadSubChunk(subChunkSize);
        dataRead += sizeof(subChunkTag);   // Subchunk tag.
        dataRead += sizeof(subChunkSize);  // Subchunk size.

        bytesHold = dataRead;

        switch( subChunkTag )
        {
        case ID_STIL:
            dataRead += ReadS0( newClip->mStillImage ); 
            break;

        case ID_XREF:   
            dataRead += ReadVX( newClip->mXRefIndex ); 
            dataRead += ReadS0( newClip->mXRefInstName ); 
            break;
            
        case ID_ISEQ:
        case ID_ANIM:
        case ID_STCC:
        case ID_TIME:
        case ID_CONT:
        case ID_BRIT:
        case ID_SATR:
        case ID_HUE:
        case ID_GAMM:
        case ID_NEGA:
        case ID_IFLT:
        case ID_PFLT:
        default:        
            dataRead += Skip( subChunkSize );   
            break;
        }

        // Make sure we've read all subchunk bytes (given by the size).
        if( (subChunkSize - dataRead + bytesHold) > 0 )
            dataRead += Skip( subChunkSize - dataRead + bytesHold );
    }

    mObject->mClips.push_back( newClip );
}

void LWOReader::ReadENVL( UInt32 pChunkSize )
{
    // Not supported yet.
    Skip( pChunkSize );
}

void LWOReader::ReadLAYR( UInt32 pChunkSize )
{
    UInt32      dataRead = 0;
    LWLayer*    newLayer = GD_NEW(LWLayer, this, "Layer");
    
    dataRead += ReadU2( newLayer->mNumber );
    dataRead += ReadU2( newLayer->mFlags );
    dataRead += ReadVEC12( newLayer->mPivot );
    dataRead += ReadS0( newLayer->mName );

    if( dataRead < pChunkSize )
        dataRead += ReadU2( newLayer->mParent );

    mObject->mLayers.push_back( newLayer );
}

void LWOReader::ReadPNTS( UInt32 pChunkSize )
{
    UInt32      numPoints = pChunkSize / sizeof(Vector3f);
    LWPoint*    points;
    
    GetCurrentLayer()->mPointList.mPoints      = GD_NEW_ARRAY(LWPoint, numPoints, this, "Point");
    GetCurrentLayer()->mPointList.mPointsCount = numPoints;

    points = GetCurrentLayer()->mPointList.mPoints;

    for( UInt32 iPoint = 0; iPoint < numPoints; iPoint++ )
        ReadVEC12( points[iPoint].mPos );
}

void LWOReader::ReadBBOX( UInt32 /*pChunkSize*/ )
{
    ReadVEC12( GetCurrentLayer()->mBoundingBox.mMin );
    ReadVEC12( GetCurrentLayer()->mBoundingBox.mMax );
}

void LWOReader::ReadPOLS( UInt32 pChunkSize )
{
    UInt32      dataRead = 0;
    UInt16      numVertex;
    UInt32      polygonType;
    LWPolygon*  newPolygon;
    
    dataRead += ReadID4( polygonType );

    while( dataRead < pChunkSize )
    {   
        newPolygon = GD_NEW(LWPolygon, this, "Polygon");

        dataRead += ReadU2( numVertex );

        newPolygon->mFlags       = (0xFC00 & numVertex) >> 10;                		
		numVertex                = 0x03FF & numVertex;
		        
        newPolygon->mType        = polygonType;
        newPolygon->mVertex      = GD_NEW_ARRAY(LWPolygonVertex, numVertex, this, "Polygon::Vertex");
        newPolygon->mVertexCount = numVertex;

        // Read indices and assign them to each vertex
		for( UInt32 iIndex = 0; iIndex < newPolygon->mVertexCount; iIndex++ )
            dataRead += ReadVX( newPolygon->mVertex[iIndex].mIndex );
        
        GetCurrentLayer()->mPolygonList.mPolygons.push_back( newPolygon );
    }
}

void LWOReader::ReadPTAG( UInt32 pChunkSize )
{
    UInt32      dataRead = 0;
    LWIndex     index;
    LWIndex     val;
    UInt32      tagType;

    dataRead += ReadID4( tagType );

    if( tagType != ID_SURF && tagType != ID_PART && tagType != ID_SMGP )
    {
        Skip( pChunkSize - dataRead );
        return;
    }

    while( dataRead < pChunkSize ) 
    {
        dataRead += ReadVX( index );
        dataRead += ReadVX( val );
                
        switch( tagType )
        {
        case ID_SURF:  GetCurrentLayer()->mPolygonList.mPolygons[index]->mSurfaceIndex        = val;  break;
        case ID_PART:  GetCurrentLayer()->mPolygonList.mPolygons[index]->mPartIndex           = val;  break;
        case ID_SMGP:  GetCurrentLayer()->mPolygonList.mPolygons[index]->mSmoothingGroupIndex = val;  break;
        }
    }
}

void LWOReader::ReadVMAP( UInt32 pChunkSize, Bool pPerPoly )
{
    UInt32              dataRead = 0;
    UInt16              dimension;
    LWIndex             vertexIndex;
    LWIndex             polygonIndex;
    
    LWVertexMap*        newVertexMap = GD_NEW(LWVertexMap, this, "VertexMap");

    newVertexMap->mPerPoly = pPerPoly;

    dataRead += ReadID4( newVertexMap->mType );
    dataRead += ReadU2( dimension );
    dataRead += ReadS0( newVertexMap->mName );

    while( dataRead < pChunkSize )
    {
        dataRead += ReadVX( vertexIndex );
        newVertexMap->mVertexIndex.push_back( vertexIndex );

        if( pPerPoly )
        {
            dataRead += ReadVX( polygonIndex );
            newVertexMap->mPolygonIndex.push_back( polygonIndex );
        }

        LWMapValue mapping;

        switch( newVertexMap->mType )
        {
        case ID_PICK:
            mapping.mPick = vertexIndex;
            break;

        case ID_WGHT:
            dataRead += ReadF4( mapping.mWeight );
            break;

        case ID_TXUV:   
            dataRead += ReadF4( mapping.mUV.U );
            dataRead += ReadF4( mapping.mUV.V );
            break;

        case ID_RGB:
            dataRead += ReadF4( mapping.mRGB.R );
            dataRead += ReadF4( mapping.mRGB.G );
            dataRead += ReadF4( mapping.mRGB.B );
            break;

        case ID_RGBA:
            dataRead += ReadF4( mapping.mRGBA.R );
            dataRead += ReadF4( mapping.mRGBA.G );
            dataRead += ReadF4( mapping.mRGBA.B );
            dataRead += ReadF4( mapping.mRGBA.A ); 
            break;

        case ID_MORF:
        case ID_SPOT:
        case ID_MNVW:   
        default:       
            dataRead += Skip( dimension * sizeof(Float) );      
            break;
        }   

        newVertexMap->mValues.push_back( mapping );
    }

    GetCurrentLayer()->mVertexMaps.push_back( newVertexMap );
}

void LWOReader::ReadSURF( UInt32 pChunkSize )
{
    UInt32      dataRead = 0;
    UInt16      subChunkSize;
    UInt32      subChunkTag;
    UInt32      bytesHold;
    LWIndex     env;
    
    LWSurface*  newSurface = GD_NEW(LWSurface, this, "Surface");

    dataRead += ReadS0( newSurface->mSurfaceName );
    dataRead += ReadS0( newSurface->mSurfaceFile );

    // Read all subchunks
    while( dataRead < pChunkSize )
    {
        if( (pChunkSize - dataRead) < 6 )
        {
            dataRead += Skip( pChunkSize - dataRead );
            return;
	    }

        subChunkTag = ReadSubChunk(subChunkSize);
        dataRead += sizeof(subChunkTag);   // Subchunk tag.
        dataRead += sizeof(subChunkSize);  // Subchunk size.

        bytesHold = dataRead;

        switch( subChunkTag )
        {
        case ID_COLR:   
            dataRead += ReadCOL12( newSurface->mBaseColor );    
            dataRead += ReadVX( env );
            break;
            
        case ID_DIFF:
            dataRead += ReadFP4( newSurface->mDiffuse );    
            dataRead += ReadVX( env );
            break;

        case ID_LUMI:
            dataRead += ReadFP4( newSurface->mLuminosity );    
            dataRead += ReadVX( env );
            break;

        case ID_SPEC:
            dataRead += ReadFP4( newSurface->mSpecular );    
            dataRead += ReadVX( env );
            break;

        case ID_REFL:
            dataRead += ReadFP4( newSurface->mReflection );    
            dataRead += ReadVX( env );
            break;

        case ID_TRAN:
            dataRead += ReadFP4( newSurface->mTransparency );    
            dataRead += ReadVX( env );
            break;

        case ID_TRNL:
            dataRead += ReadFP4( newSurface->mTranslucency );    
            dataRead += ReadVX( env );
            break;
            
        case ID_GLOS:
            dataRead += ReadFP4( newSurface->mSpecularGlossiness );    
            dataRead += ReadVX( env );
            break;

        case ID_SHRP:
            dataRead += ReadFP4( newSurface->mDiffuseSharpness );    
            dataRead += ReadVX( env );
            break;

        case ID_BUMP:
            dataRead += ReadFP4( newSurface->mBumpStrength );    
            dataRead += ReadVX( env );
            break;
            
        case ID_SIDE:
            dataRead += ReadU2( newSurface->mPolySide );    
            break;
            
        case ID_SMAN:
            dataRead += ReadANG4( newSurface->mMaxSmoothingAngle );
            break;
            
        case ID_RFOP:
            dataRead += ReadU2( newSurface->mReflectionOptions );
            break;
            
        case ID_RIMG:
            dataRead += ReadVX( newSurface->mReflectionMapImage );
            break;
            
        case ID_RSAN:
            dataRead += ReadANG4( newSurface->mReflMapSeamAngle );
            dataRead += ReadVX( env );
            break;

        case ID_RBLR:
            dataRead += ReadFP4( newSurface->mReflBlurPercent );
            dataRead += ReadVX( env );
            break;

        case ID_CLRH:
            dataRead += ReadFP4( newSurface->mColorHighlights );
            dataRead += ReadVX( env );
            break;

        case ID_TROP:
            dataRead += ReadU2( newSurface->mTransparencyOptions );
            dataRead += Skip( subChunkSize - sizeof(UInt16) );
            break;

        case ID_CLRF:
            dataRead += ReadFP4( newSurface->mColorFilter );
            dataRead += ReadVX( env );
            break;

        case ID_ADTR:
            dataRead += ReadFP4( newSurface->mAdditiveTransparency );
            dataRead += ReadVX( env );
            break;         
        
        case ID_ALPH:
            dataRead += ReadU2( newSurface->mAlphaMode );
            dataRead += ReadFP4( newSurface->mAlphaValue );
            break;

        case ID_BLOK:
            ReadBLOK( newSurface, subChunkSize );
            dataRead += subChunkSize;
            break;
        
        case ID_VCOL:
        case ID_LINE:
        case ID_GLOW:
        case ID_TBLR:
        case ID_TIMG:
        case ID_RIND:
        default:    dataRead += Skip( subChunkSize );   break;
        }

        // Make sure we've read all subchunk bytes (given by the size).
        if( (subChunkSize - dataRead + bytesHold) > 0 )
            dataRead += Skip( subChunkSize - dataRead + bytesHold );
    }

    mObject->mSurfaces.push_back( newSurface );
}

void LWOReader::ReadBLOK( LWSurface* pSurface, UInt32 pChunkSize )
{
    UInt32      dataRead = 0;
    UInt16      subChunkSize;
    UInt32      subChunkTag;
    UInt32      bytesHold;

    LWSurfaceBlock*     newSurfaceBlock = GD_NEW(LWSurfaceBlock, this, "SurfaceBlock");

    //dataRead += ReadS0( newSurfaceBlock->mOrdinalString );

    // Read all subchunks
    while( dataRead < pChunkSize )
    {
        subChunkTag = ReadSubChunk(subChunkSize,1);
        dataRead += sizeof(subChunkTag);   // Subchunk tag.
        dataRead += sizeof(subChunkSize);  // Subchunk size.

        bytesHold = dataRead;

        switch( subChunkTag )
        {
        case ID_IMAP:
        case ID_PROC:
        case ID_GRAD:
        case ID_SHDR:
            newSurfaceBlock->mType = subChunkTag;
	        ReadHEAD( newSurfaceBlock, subChunkSize );
            dataRead += subChunkSize;
            break;

        case ID_TMAP:
            ReadTMAP( &newSurfaceBlock->mTextureMapping, subChunkSize );
            dataRead += subChunkSize;
            break;

        case ID_PROJ:
            dataRead += ReadU2( newSurfaceBlock->mImageMap.mProjectionMode );
            break;

        case ID_AXIS:
            dataRead += ReadU2( newSurfaceBlock->mImageMap.mProjectionAxis );
            break;

        case ID_IMAG:
            newSurfaceBlock->mImageMap.mValid = true;
            dataRead += ReadVX( newSurfaceBlock->mImageMap.mImageMap );
            break;

        case ID_WRAP:
            dataRead += ReadU2( newSurfaceBlock->mImageMap.mWrapModeWidth );
            dataRead += ReadU2( newSurfaceBlock->mImageMap.mWrapModeHeight );
            break;

        case ID_VMAP:
            dataRead += ReadS0( newSurfaceBlock->mImageMap.mUVVertexMapName );
            break;

        default:
           dataRead += Skip( subChunkSize );   
           break;
        }

        // Make sure we've read all subchunk bytes (given by the size).
        if( (subChunkSize - dataRead + bytesHold) > 0 )
            dataRead += Skip( subChunkSize - dataRead + bytesHold );
    }

    pSurface->mBlocks.push_back( newSurfaceBlock );
}

void LWOReader::ReadHEAD( LWSurfaceBlock* pSurfaceBlock, UInt32 pChunkSize )
{
    UInt32      dataRead = 0;
    UInt16      subChunkSize;
    UInt32      subChunkTag;
    UInt32      bytesHold;
    LWIndex     env;

    dataRead += ReadS0( pSurfaceBlock->mOrdinalString );

    while( dataRead < pChunkSize )
    {
        subChunkTag = ReadSubChunk(subChunkSize,2);
        dataRead += sizeof(subChunkTag);   // Subchunk tag.
        dataRead += sizeof(subChunkSize);  // Subchunk size.

        bytesHold = dataRead;

        switch( subChunkTag )
        {
        case ID_CHAN:
            dataRead += ReadID4( pSurfaceBlock->mTextureChannel );
            break;

        case ID_ENAB:
            dataRead += ReadU2( pSurfaceBlock->mEnableState );
            break;

        case ID_OPAC:
            dataRead += ReadU2( pSurfaceBlock->mOpacityType );
            dataRead += ReadFP4( pSurfaceBlock->mOpacity );
            dataRead += ReadVX( env );
            break;

        default:
            dataRead += Skip( subChunkSize );
            break;
        }

        // Make sure we've read all subchunk bytes (given by the size).
        if( (subChunkSize - dataRead + bytesHold) > 0 ) 
            dataRead += Skip( subChunkSize - dataRead + bytesHold );
    }
}


void LWOReader::ReadTMAP( LWTextureMapping* pTextureMapping, UInt32 pChunkSize )
{
    UInt32      dataRead = 0;
    UInt16      subChunkSize;
    UInt32      subChunkTag;
    UInt32      bytesHold;
    LWIndex     env;

    while( dataRead < pChunkSize )
    {
        subChunkTag = ReadSubChunk(subChunkSize,2);
        dataRead += sizeof(subChunkTag);   // Subchunk tag.
        dataRead += sizeof(subChunkSize);  // Subchunk size.

        bytesHold = dataRead;

        switch( subChunkTag )
        {
        case ID_CNTR:
            dataRead += ReadVEC12( pTextureMapping->mCenter );
            dataRead += ReadVX( env );
            break;

        case ID_SIZE:
            dataRead += ReadVEC12( pTextureMapping->mSize );
            dataRead += ReadVX( env );
            break;

        case ID_ROTA:
            dataRead += ReadVEC12( pTextureMapping->mRotation );
            dataRead += ReadVX( env );
            break;

        case ID_OREF:
            dataRead += ReadS0( pTextureMapping->mReferenceObject );
            break;

        case ID_FALL:
            dataRead += ReadU2( pTextureMapping->mFalloffType );
            dataRead += ReadVEC12( pTextureMapping->mFalloffVector );
            dataRead += ReadVX( env );
            break;

        case ID_CSYS:
            dataRead += ReadU2( pTextureMapping->mCoordinateSys );
            break;

        default:
            dataRead += Skip( subChunkSize );
            break;
        }

        // Make sure we've read all subchunk bytes (given by the size).
        if( (subChunkSize - dataRead + bytesHold) > 0 ) 
            dataRead += Skip( subChunkSize - dataRead + bytesHold );
    }
}

