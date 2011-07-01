#include "Engine.h"
#include "BSP.h"

#include "Maths/Vector2.h"
#include "Maths/Number.h"
#include "Maths/Frustum.h"

#include "Graphic/Mesh/Mesh.h"
#include "Graphic/Buffer/IndexBuffer.h"
#include "Graphic/Buffer/VertexBuffer.h"
#include "Graphic/Buffer/VertexFormat.h"

#include "Graphic/Font/Font.h"
#include "Graphic/Font/FontHdl.h"

#include "Graphic/Texture/Texture.h"
#include "Graphic/Renderer.h"
#include "Graphic/GraphicSubsystem.h"


namespace Gamedesk {
	
	
IMPLEMENT_CLASS(Bsp);


Bsp::Bsp()
	: mBufPositions(NULL)
	, mBufNormals(NULL)
	, mBufIndices(NULL)
    , mPackedLightmaps( 1024, Image::Format_R8G8B8 )
{
}

Bsp::~Bsp()
{
}

__inline UInt32 NextPow2(UInt32 pNumber)
{
    UInt32 pow2 = 1;
    
#if GD_PLATFORM == GD_PLATFORM_WIN32
    __asm
    {
        xor ecx, ecx
        bsr ecx, pNumber
        inc ecx
        shl pow2, cl
    }
#else
	debugBreak();
#endif

    return pow2;
}

void Bsp::Init()
{
    mFacesDrawn.SetSize( mFaces.size() );

    // Calculate faces extent
    for( UInt32 i = 0; i < mFaces.size(); i++ )
        CalculateFaceExtent(i);

    // Create all the lightmaps (pack them in 2048x2048 textures)
    mPackedLightmaps.BeginPacking();

    for( UInt32 i = 0; i < mFaces.size(); i++ )
        CreateFaceLightmap(i);

    mPackedLightmaps.EndPacking();
}

void Bsp::CalculateFaceExtent( UInt32 iFace )
{
    BSPFace& face = mFaces[iFace];
    const BSPTexInfo& texInfo = mTextureInfo[face.mTextureInfo];

	Vector2f        min, max;
    Vector2<Int16>  bmin, bmax;
    
    min = Vector2f( Number<Float>::Max, Number<Float>::Max );
    max = Vector2f( Number<Float>::Min, Number<Float>::Min );

	for( UInt32 j = 0; j < face.mNumEdges; j++ )
	{
 		Int32 faceEdge = mFacesEdge[face.mFirstEdgeIndex+j];
		const BSPEdge& edge = mEdges[faceEdge >= 0 ? faceEdge : -faceEdge];
        const Vector3f& v = mVertices[edge.mPoints[faceEdge >= 0 ? 0 : 1]];
		
        Vector2f texCoord( (v dot texInfo.mAxisU) + texInfo.mOffsetU,
                           (v dot texInfo.mAxisV) + texInfo.mOffsetV );
   
        if( texCoord.x < min.x )
            min.x = texCoord.x;

        if( texCoord.x > max.x )
            max.x = texCoord.x;

        if( texCoord.y < min.y )
            min.y = texCoord.y;

        if( texCoord.y > max.y )
            max.y = texCoord.y;
	}

	bmin.x = floor(min.x/16);
    bmin.y = floor(min.y/16);

	bmax.x = ceil(max.x/16);
    bmax.y = ceil(max.y/16);

	face.mTextureMin = bmin * 16;
	face.mExtent = (bmax - bmin) * 16;
}

void Bsp::CreateFaceLightmap( UInt32 iFace )
{
    BSPFace&          face    = mFaces[iFace];
    const BSPTexInfo& texInfo = mTextureInfo[face.mTextureInfo];

    if( face.mNumEdges == 0 )
        return;

    if( texInfo.mFlags & (SURF_SKY|SURF_TRANS33|SURF_TRANS66|SURF_WARP) )
        return;    

    UInt32 lightmapWidth  = (face.mExtent.x>>4)+1;
	UInt32 lightmapHeight = (face.mExtent.y>>4)+1;
    
    Image imgLight;
    imgLight.Create( lightmapWidth, lightmapHeight, Image::Format_R8G8B8 );
    
    Byte* dst = imgLight.GetData();
    Byte* src = &mLightmapData[face.mLightmapOffset];
    memcpy( dst, src, Image::GetSize(Image::Format_R8G8B8, lightmapWidth, lightmapHeight) );
    
    imgLight.ChangeGamma( 2 );

    mPackedLightmaps.InsertImage( imgLight, face.mLightmapInfo );
}

const Bsp::BSPLeaf& Bsp::FindLeafContaining( const Vector3f& pPoint ) const
{
    Int32 currentNode = 0;
	
	// Loop until we find a negative index
	while( currentNode >= 0 )
	{
		// If the point is in front of the plane for this node, assign i to be the front node
        if( mPlanes[mNodes[currentNode].mPlaneIndex].DistanceTo(pPoint) >= 0 )
			currentNode = mNodes[currentNode].mFrontChild;
		else
			currentNode = mNodes[currentNode].mBackChild;
	}

	// Return leaf
	return mLeaves[~currentNode];
}

Bool Bsp::IsPotentiallyVisible( UInt32 pFromCluster, UInt32 pTestCluster ) const
{
    if( pFromCluster == NOT_VISIBLE ) 
        return true;

    if( pTestCluster == NOT_VISIBLE )
        return false;

    return mClusters[pFromCluster].mPVS.CheckBit( pTestCluster );
}

void Bsp::Render()
{
	Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT(renderer);

    const BSPLeaf& cameraLeaf = FindLeafContaining( renderer->GetViewPos() );

    FontHdl font( "Data/Fonts/tahoma.ttf", 14 );
    
    int numVisibleFaces = 0;
    int numVisibleLeaves = 0;

    mFacesDrawn.ClearAllBits();

    renderer->SetRenderState( Renderer::Lighting, false );
    renderer->SetCulling( Renderer::CullFrontFace );

    Matrix4f modelViewMatrix;
    Matrix4f projectionMatrix;
    Frustum  frustum;
    Vector2f texCoord;

    renderer->GetModelViewMatrix(modelViewMatrix);
	renderer->GetProjectionMatrix(projectionMatrix);
    frustum.CalculateFrustum(projectionMatrix, modelViewMatrix);

    static UInt32 currentCluster = NOT_VISIBLE;
    if( cameraLeaf.mCluster != NOT_VISIBLE )
        currentCluster = cameraLeaf.mCluster;

    UInt32 lastLightMapPage = 0xFFFFFFFF;

    // Go through all the leafs and check their visibility
    int i = mLeaves.size();
	while(i--)
	{
		// Get the current leaf that is to be tested for visibility from our camera's leaf
		const BSPLeaf& leaf = mLeaves[i];

		// If the current leaf can't be seen from our cluster, go to the next leaf
		if( !IsPotentiallyVisible(currentCluster, leaf.mCluster) ) 
			continue;

		// If the current leaf is not in the camera's frustum, go to the next leaf
        if( !frustum.BoxInFrustum(leaf.mBBox) )
			continue;
		
        numVisibleLeaves++;

		// If we get here, the leaf we are testing must be visible in our camera's view.
		// Get the number of faces that this leaf is in charge of.
		int faceCount = leaf.mNumFaces;

		// Loop through and render all of the faces in this leaf
		while(faceCount--)
		{
			// Grab the current face index from our leaf faces array
			int faceIndex = mLeafFaces[leaf.mFirstFaceIndex + faceCount];
            
    		// Since many faces are duplicated in other leafs, we need to
			// make sure this face already hasn't been drawn.
			if( mFacesDrawn.CheckBit(faceIndex) ) 
			    continue;

		    const BSPFace& face = mFaces[faceIndex];
            const BSPTexInfo& texInfo = mTextureInfo[face.mTextureInfo];

            if( texInfo.mFlags & SURF_SKY )
                continue;

            // Set this face as drawn and render it
			mFacesDrawn.SetBit(faceIndex);

            // Increase the rendered face count to display for fun
			numVisibleFaces++;

            // Set face texture
            renderer->GetTextureStage(0)->SetTexture( *texInfo.mTexture );
            
            // Set face lightmap
            if( !face.mLightmapInfo.mValid )
            {
                if( lastLightMapPage != 0xFFFFFFFF )
                { 
                    lastLightMapPage = 0xFFFFFFFF;
                    renderer->GetTextureStage(1)->ResetTexture(); 
                }
            }
            else if( face.mLightmapInfo.mTextureIndex != lastLightMapPage )
            {
                lastLightMapPage = face.mLightmapInfo.mTextureIndex;
                renderer->GetTextureStage(1)->SetTexture( mPackedLightmaps.GetTexture(lastLightMapPage) ); 
            }                        

            // Render face as a triangle fan
            renderer->BeginScene( Renderer::TriangleFan );
            {
                if( face.mPlaneSide )
                    renderer->SetNormal( -mPlanes[face.mPlaneIndex].GetNormal() );
                else
                    renderer->SetNormal( mPlanes[face.mPlaneIndex].GetNormal() );

		        for( UInt32 j = 0; j < face.mNumEdges; j++ )
		        {
			        Int32 faceEdge = mFacesEdge[face.mFirstEdgeIndex+j];
			        const BSPEdge& edge = mEdges[faceEdge > 0 ? faceEdge : -faceEdge];
                    const Vector3f& v = mVertices[edge.mPoints[faceEdge > 0 ? 0 : 1]];
                    
                    texCoord.x = (v dot texInfo.mAxisU) + texInfo.mOffsetU;
                    texCoord.x /= texInfo.mTexture->GetWidth();
		            texCoord.y = (v dot texInfo.mAxisV) + texInfo.mOffsetV;
		            texCoord.y /= texInfo.mTexture->GetHeight();
                    renderer->SetUV( texCoord ); 
                    
                    if( face.mLightmapInfo.mValid )
                    {
                        texCoord.x  = (v dot texInfo.mAxisU) + texInfo.mOffsetU;
                        texCoord.x += face.mLightmapInfo.mOffsetU*16;
                        texCoord.x -= face.mTextureMin.x;
                        texCoord.x += 8;
                        texCoord.x /= 1024*16;

                        texCoord.y  = (v dot texInfo.mAxisV) + texInfo.mOffsetV;
                        texCoord.y += face.mLightmapInfo.mOffsetV*16;
                        texCoord.y -= face.mTextureMin.y;
                        texCoord.y += 8;
                        texCoord.y /= 1024*16;

                        renderer->SetUV( 1, texCoord );
                    }

                    renderer->SetVertex( v );
		        }
            }
		    renderer->EndScene();
		}			
	}

    renderer->SetRenderState( Renderer::Lighting, true );
    renderer->GetTextureStage(0)->ResetTexture();
    renderer->GetTextureStage(1)->ResetTexture(); 
    renderer->SetCulling( Renderer::CullBackFace );
    
    font->DrawString( 6, 20, "Pos: %f %f %f", renderer->GetViewPos().x, renderer->GetViewPos().y, renderer->GetViewPos().z );
}


} // namespace Gamedesk
