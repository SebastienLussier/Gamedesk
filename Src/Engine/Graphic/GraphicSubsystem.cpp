/**
 *  @file       GraphicSubsystem.cpp
 *  @brief
 *  @author     Sébastien Lussier.
 *  @date       15/01/03.
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
#include "GraphicSubsystem.h"

#include "Application/Application.h"
#include "Application/Window.h"
#include "RenderTarget/RenderWindow.h"
#include "Graphic/Renderer.h"


namespace Gamedesk {
	
	
IMPLEMENT_ABSTRACT_CLASS(GraphicSubsystem);
IMPLEMENT_ABSTRACT_SINGLETON(GraphicSubsystem);


GraphicSubsystem::GraphicSubsystem() :
    mRenderer(NULL)
{
    mInstance = this;
}

GraphicSubsystem::~GraphicSubsystem()
{
    mInstance = NULL;
}

void GraphicSubsystem::Init()
{
    Super::Init();

    // At this point, the application must have created a render window.
    RenderWindow* renderWindow = Application::Instance()->GetRenderWindow();
    GD_ASSERT( renderWindow );
    
    // Initialize the renderer using this render window.
    GD_ASSERT(mRenderer);
    mRenderer->Init( renderWindow );
}

void GraphicSubsystem::Kill()
{
    Super::Kill();
}

Renderer* GraphicSubsystem::GetRenderer() const
{
    return mRenderer;
}

} // namespace Gamedesk
