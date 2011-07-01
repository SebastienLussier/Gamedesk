/**
 *  @file       PSPGraphic.cpp
 *  @brief      PSP renderer for gamedesk.
 *  @author     Sébastien Lussier.
 *  @date       09/09/2008.
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
#include "PSPGraphic.h"
#include "Module/ModuleManager.h"


namespace Gamedesk {
	
	
IMPLEMENT_MODULE(PSPGraphic);


#include "PSPGraphicSubsystem.h"
#include "PSPRenderer.h"
#include "PSPRenderWindow.h"
#include "PSPTexture.h"


void StaticInitPSPGraphicClasses()
{
	PSPGraphicSubsystem::StaticClass();
	PSPRenderer::StaticClass();
	PSPRenderWindow::StaticClass();
	PSPTexture2D::StaticClass();
}
