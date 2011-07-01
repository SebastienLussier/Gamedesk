/**
 *  @file       LensFlare.cpp
 *  @brief	    LensFlare effect.
 *  @author     Sébastien Lussier
 *  @date       04/05/04.
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
#include "Engine.h"
#include "LensFlare.h"

#include "Maths/Maths.h"
#include "Maths/Vector3.h"
#include "Maths/Matrix4.h"

#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"


namespace Gamedesk {
	
	
LensFlare::LensFlare()
{
    mFlare0.GetTexture( "Data/Flares/Flare0.tga" );
    mFlare1.GetTexture( "Data/Flares/Flare1.tga" );
    mFlare2.GetTexture( "Data/Flares/Flare2.tga" );
    mFlare3.GetTexture( "Data/Flares/Flare3.tga" );
    mFlare4.GetTexture( "Data/Flares/Flare4.tga" );
}

void LensFlare::Render() const
{
    Matrix4f modelViewMatrix;
    Matrix4f projectionMatrix;
    Int32    viewport[4];
    
    Vector3f vFlare;

    Renderer* renderer = GraphicSubsystem::Instance()->GetRenderer();
    GD_ASSERT( renderer );

    renderer->GetViewport( viewport );
    vFlare = renderer->WorldToScreen( mPosition ); 

    Float brightnessFactor = 1.0f;

    if( vFlare.z > 1.0f )
        return;

    if( vFlare.x < 0 || vFlare.y < 0 )
    {
        float maxVal = Maths::Min( vFlare.x, vFlare.y );
    
        if( maxVal < -100 )
            return;

        brightnessFactor = 1.0f + (maxVal/100.0f);
    }
    else if( vFlare.x >= viewport[2] || vFlare.y >= viewport[3] )
    {
        float maxVal = Maths::Max( vFlare.x-viewport[2], vFlare.y-viewport[3] );

        if( maxVal > 100 )
            return;

        brightnessFactor = 1.0f - (maxVal/100.0f);
    }

    renderer->SetRenderState( Renderer::Lighting, false );
    renderer->SetBlendFunc( Renderer::BlendSrcAlpha, Renderer::BlendInvSrcAlpha );
    renderer->SetRenderState( Renderer::Blend, true );
    
    vFlare.x /= viewport[2];
    vFlare.x -= 0.5f;
    vFlare.x *= 2;

    vFlare.y /= viewport[3];
    vFlare.y -= 0.5f;
    vFlare.y *= 2;  

    renderer->Begin2DProjection( -1, 1, -1, 1, -1, 1 );

    const Float MAX_BRIGHTNESS = 0.2f;
    Float screenLight = MAX_BRIGHTNESS * (Maths::Sqrt(2.0f)-Maths::Sqrt(vFlare.x*vFlare.x + vFlare.y*vFlare.y));
    renderer->SetColor( 1.0f, 1.0f, 1.0f, Maths::Clamp( screenLight, 0.0f, MAX_BRIGHTNESS ) );
    renderer->Draw2DTile( -1, -1, 2, 2 );

    renderer->SetBlendFunc( Renderer::BlendSrcAlpha, Renderer::BlendOne );

    renderer->SetColor( 1, 1, 1, 0.9f * brightnessFactor );
    renderer->GetTextureStage(0)->SetTexture( *mFlare0 );
    renderer->Draw2DTile( vFlare.x - 0.2f, vFlare.y - 0.2f, 0.4f, 0.4f );
    
    renderer->SetColor( 0.9f, 0.9f, 0.95f, 0.8f * brightnessFactor );
    renderer->GetTextureStage(0)->SetTexture( *mFlare2 );
    renderer->Draw2DTile( vFlare.x - 0.2f, vFlare.y - 0.2f, 0.4f, 0.4f );

    vFlare.x /= 2;
    vFlare.y /= 2;
    renderer->SetColor( 49.0f/255.0f, 68/255.0f, 70/255.0f, 0.5f * brightnessFactor );
    renderer->GetTextureStage(0)->SetTexture( *mFlare3 );
    renderer->Draw2DTile( vFlare.x - 0.3f, vFlare.y - 0.3f, 0.6f, 0.6f );

    vFlare.x /= 2;
    vFlare.y /= 2;
    renderer->SetColor( 15/255.0f, 46/255.0f, 34/255.0f, 0.5f * brightnessFactor );
    renderer->GetTextureStage(0)->SetTexture( *mFlare1 );
    renderer->Draw2DTile( vFlare.x - 0.1f, vFlare.y - 0.1f, 0.2f, 0.2f );

    vFlare.x = -vFlare.x*4;
    vFlare.y = -vFlare.y*4;
    renderer->SetColor( 17/255.0f, 81/255.0f, 125/255.0f, 0.5f * brightnessFactor );
    renderer->GetTextureStage(0)->SetTexture( *mFlare3 );
    renderer->Draw2DTile( vFlare.x - 0.13f, vFlare.y - 0.13f, 0.26f, 0.26f );
    
    vFlare.x /= 2;
    vFlare.y /= 2;
    renderer->SetColor( 49/255.0f, 68/255.0f, 70/255.0f, 0.5f * brightnessFactor );
    renderer->GetTextureStage(0)->SetTexture( *mFlare1 );
    renderer->Draw2DTile( vFlare.x - 0.2f, vFlare.y - 0.2f, 0.4f, 0.4f );

    vFlare.x /= 2;
    vFlare.y /= 2;
    renderer->SetColor( 135/255.0f, 116/255.0f, 66/255.0f, 0.5f * brightnessFactor );
    renderer->GetTextureStage(0)->SetTexture( *mFlare4 );
    renderer->Draw2DTile( vFlare.x - 0.15f, vFlare.y - 0.15f, 0.3f, 0.3f );
    
    vFlare.x /= 2;
    vFlare.y /= 2;
    renderer->SetColor( 135/255.0f, 116/255.0f, 66/255.0f, 0.5f * brightnessFactor );
    renderer->GetTextureStage(0)->SetTexture( *mFlare4 );
    renderer->Draw2DTile( vFlare.x - 0.075f, vFlare.y - 0.075f, 0.15f, 0.15f );
        
    renderer->End2DProjection();
    
    renderer->SetColor( 1, 1, 1, 1 );
    renderer->GetTextureStage(0)->ResetTexture();

    renderer->SetRenderState( Renderer::Blend, false );
    renderer->SetRenderState( Renderer::Lighting, true );
}


} // namespace Gamedesk
