/**
 *  @file       PSPGraphic.h
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
#ifndef     _PSPGRAPHIC_H_
#define     _PSPGRAPHIC_H_


#ifndef PSPGRAPHIC_API
	#define PSPGRAPHIC_API
#endif


#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>

#include "Engine.h"


namespace Gamedesk 
{
    void StaticInitPSPGraphicClasses();
}


#endif  //  _PSPGRAPHIC_H_
