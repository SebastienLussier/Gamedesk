/**
 *  @file       DX9Input.cpp
 *  @brief      DX9 Input module for gamedesk.
 *  @author     Sébastien Lussier.
 *  @date       05/01/04.
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
#include "DX9Input.h"
#include "Module/ModuleManager.h"


namespace Gamedesk {
	
	
IMPLEMENT_MODULE(DX9Input);


// DLL Instance is needed to initialize DirectInput
HINSTANCE gDLLInstance;
extern "C" int APIENTRY DllMain( HINSTANCE hInstance, DWORD /*fdwReason*/, LPVOID /*pvReserved*/ )
{
    gDLLInstance = hInstance;
    return 1;
}


} // namespace Gamedesk
