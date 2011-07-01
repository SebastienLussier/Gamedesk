/**
 *  @file       PSPRenderWindow.cpp
 *  @brief      PSP Render Window.
 *  @author     Sébastien Lussier.
 *  @date       18/09/2008.
 */
/*
 *  Copyright (C) 2008 Gamedesk
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
#include "PSPRenderWindow.h"
#include "Module/ModuleManager.h"
#include "Graphic/GraphicSubsystem.h"
#include "Graphic/Renderer.h"


#include <pspdisplay.h>
#include <pspgu.h>


namespace Gamedesk {


IMPLEMENT_CLASS(PSPRenderWindow);


PSPRenderWindow::PSPRenderWindow()
{
}

PSPRenderWindow::~PSPRenderWindow()
{
    Destroy();
}

void PSPRenderWindow::Create( const WindowInfo& pWindowInfo, const RenderTarget::Format& pFormat )
{
    Super::Create( pWindowInfo, pFormat );
}

void PSPRenderWindow::Destroy()
{
    Super::Destroy();
}

void PSPRenderWindow::MakeCurrent()
{
}

void PSPRenderWindow::SwapBuffers()
{
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}


} // namespace Gamedesk
